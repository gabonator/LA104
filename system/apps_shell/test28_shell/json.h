#define _assert(cond)

class CSubstring
{
    const char* mpString;
    int mnBegin;
    int mnLength;

public:
    CSubstring(const char* pString)
    {
        mpString = pString;
        mnBegin = 0;
        mnLength = (int)strlen(mpString);
    }

    CSubstring(const CSubstring& begin, const CSubstring& end)
    {
        mpString = begin.mpString;
        mnBegin = begin.mnBegin;
        mnLength = end.mnBegin - mnBegin;
    }

    CSubstring()
    {
        mpString = nullptr;
        mnLength = 0;
    }

    char operator[](int n) const
    {
        _assert(mpString && n <= mnLength);
        return mpString[mnBegin + n];
    }
    
    int Length() const
    {
        return mnLength;
    }
    
    int Find(char c) const
    {
        for (int i=mnBegin; i<mnBegin+mnLength; i++)
            if (mpString[i] == c)
                return i - mnBegin;
        
        return -1;
    }
    
    bool BeginsWith(const char* str) const
    {
        return strncmp(mpString+mnBegin, str, strlen(str)) == 0;
    }
    
    bool operator ==(const char* str) const
    {
        if ((int)strlen(str) != mnLength)
            return false;
        
        return strncmp(mpString+mnBegin, str, mnLength) == 0;
    }
    
    void operator ++(int)
    {
        mnBegin++;
        mnLength--;
        _assert(mnLength >= 0);
    }
    
    void operator +=(int n)
    {
        mnBegin += n;
        mnLength -= n;
        _assert(mnLength >= 0);
    }
    
    CSubstring TakeFirst(int n)
    {
        if (n<=0)
            return CSubstring();
        
        CSubstring aux = *this;
        aux.mnLength = n;
        *this += n;
        return aux;
    }
    
    const char* GetBuffer() const
    {
        return mpString + mnBegin;
    }
    
    operator bool() const
    {
        return mpString;
    }
    
    void ToString(char* str, int maxLen) const
    {
        if (!mpString || mnLength == 0)
        {
            strcpy(str, "");
        }
        const char* pBeginPtr = mpString + mnBegin;
        int nLen = mnLength;
        
        if (pBeginPtr[0] == '"' && pBeginPtr[nLen-1] == '"')
        {
            pBeginPtr ++;
            nLen -= 2;
        }
        if (nLen > maxLen-1)
            nLen = maxLen-1;
        memcpy(str, pBeginPtr, nLen);
        str[nLen] = 0;
    }
};

class CJson
{
    enum {
        MaxStringLength = 32
    };

    CSubstring mString;

public:
    typedef void (* TKeyValueCallback)(const CSubstring&, const CSubstring&);
    typedef void (* TValueCallback)(const CSubstring&);

public:
    void ToString(char* str, int maxLen) const
    {
        if (mString)
            mString.ToString(str, maxLen);
        else
            strcpy(str, "");
    }

    CJson()
    {
    }
  
    CJson(const CSubstring& str) : mString(str)
    {
    }
    
    char* GetString()
    {
        static char strValue[MaxStringLength];
        strValue[0] = 0;
        
        CSubstring copyString = mString;
        TraverseString(copyString, [](const CSubstring& s)
        {
            if (s.Length() < MaxStringLength-1)
            {
                memcpy(strValue, s.GetBuffer(), s.Length());
                strValue[s.Length()] = 0;
            }
        });

        return strValue;
    }

    int GetNumber()
    {
        static int nResult;
        CSubstring copyString = mString;
        nResult = 0;
        
        TraverseNumber(copyString, [](const CSubstring& s)
        {
            nResult = atoi(s.GetBuffer());
        });
        
        return nResult;
    }

    CJson operator[](const char* pKey)
    {
        static CJson _aux;
        static const char* _pKey;
        _aux = CJson();
        _pKey = pKey;
        
        CSubstring copyString = mString;
        TraverseObject(copyString, [](const CSubstring& key, const CSubstring& value)
        {
            if (key == _pKey)
                _aux = CJson(value);
        });
        return _aux;
    }

    CJson operator[](int nIndex)
    {
        static CJson _aux;
        static int _nIndex;
        _aux = CJson();
        _nIndex = nIndex;
        
        CSubstring copyString = mString;
        TraverseArray(copyString, [](const CSubstring& value)
        {
            if (_nIndex-- == 0)
                _aux = CJson(value);
        });
        return _aux;
    }
    
    operator bool() const
    {
        return mString;
    }
    
    bool Verify()
    {
        CSubstring copyString = mString;
        return TraverseAny(copyString);
    }
    
private:
    bool TraverseString(CSubstring& s, TValueCallback callback)
    {
        if (!s)
            return false;
        
        char term = s[0];
        if (term != '\"' && term != '\'')
            return false;
        
        s++;
        
        for (int i=0; i<s.Length(); i++)
        {
            if (s[i] == '\\')
            {
                i++;
                continue;
            }
            
            if (s[i] == term)
            {
                callback(s.TakeFirst(i));
                s++;
                break;
            }
        }
        
        return true;
    }

    bool TraverseNumber(CSubstring& s, TValueCallback callback)
    {
        if (!s)
            return false;

        if (s[0] == '+' || s[0] == '-' || s[0] == '.' || (s[0] >= '0' && s[0] <= '9') )
        {
            int i;
            for (i=1; i<s.Length(); i++)
                if (s[i] != '.' && (s[i] < '0' || s[i] > '9') )
                    break;
            
            callback(s.TakeFirst(i));
            return true;
        }
        return false;
    }
    
    bool TraverseKeyword(CSubstring& s, TValueCallback callback)
    {
        if (!s)
            return false;

        if (s.BeginsWith("null"))
        {
            callback(s.TakeFirst(4));
            return true;
        }
        if (s.BeginsWith("true"))
        {
            callback(s.TakeFirst(4));
            return true;
        }
        if (s.BeginsWith("false"))
        {
            callback(s.TakeFirst(5));
            return true;
        }
        return false;
    }
    
    bool TraverseKey(CSubstring& s, TValueCallback callback)
    {
        if (!s)
            return false;

        if (TraverseString(s, callback))
            return true;

        int nEnd = s.Find(':');
        if (nEnd < 0)
            return false;
        
        callback(s.TakeFirst(nEnd));
        return true;
    }

    bool TraverseAny(CSubstring& s)
    {
        if (TraverseKeyword(s, [](const CSubstring&){}))
            return true;
        if (TraverseNumber(s, [](const CSubstring&){}))
            return true;
        if (TraverseString(s, [](const CSubstring&){}))
            return true;
        if (TraverseObject(s, [](const CSubstring&, const CSubstring&){}))
            return true;
        if (TraverseArray(s, [](const CSubstring&){}))
            return true;
        return false;
    }
    
    bool TraverseObject(CSubstring& s, TKeyValueCallback fCallback)
    {
        if (!s || s[0] != '{')
            return false;

        s++;
        while (s.Length() && s[0] != '}')
        {
            if (s[0] == ' ' || s[0] == ',')
            {
                s++;
                continue;
            }

            static CSubstring _key;
            TraverseKey(s, [](const CSubstring& sub){ _key = sub; });
            CSubstring key = _key;
            
            if (s[0] != ':')
            {
                //assert(0);
                return false;
            }
            s++;

            CSubstring valueBegin(s);
            if (!TraverseAny(s))
            {
                //assert(0);
                return false;
            }
            CSubstring value(valueBegin, s);

            fCallback(key, value);
        }

        s++;
        return true;
    }

    bool TraverseArray(CSubstring& s, TValueCallback fCallback)
    {
        if (!s || s[0] != '[')
            return false;

        s++;
        while (s.Length() && s[0] != ']')
        {
            if (s[0] == ' ' || s[0] == ',')
            {
                s++;
                continue;
            }

            CSubstring valueBegin(s);
            if (!TraverseAny(s))
            {
                _assert(0);
                return false;
            }
            CSubstring value(valueBegin, s);
            
            fCallback(value);
        }
        s++;

        return true;
    }
};

/*
void jsonTest()
{
    const char* pJson = R"-({"ts":1527979991, "time":"2018-06-03 00:53:11", "ip":"37.9.169.18", "json":{"uid":"891468008914", "reset":"2 - NO_MEAN", "n":3771, "uptime":124779, "delayed":0, "temp":30.3, "hum":46.0, "cashless":{"state":0,"coin":{"full":1,"5":"66","10":"43","20":"47","50":"43","100":"44","200":"38"},"sell":{"9":1}}, "failed":42, "restarts":85, "rssi":15, "rx":1205356, "tx":1849411}, "agent":"iot-endpoint-valky-2018-1 (sim900 on esp8266 by valky.eu built May 24 2018 19:14:00)"})-";

    CJson json(pJson);

    std::cout << json.Verify() << "\n";
    std::cout << "time=" << json["time"].GetString() << "\n";
    std::cout << "50coins=" << json["json"]["cashless"]["coin"]["50"].GetString() << "\n";
    std::cout << "sell=" << json["json"]["cashless"]["sell"]["9"].GetNumber() << "\n";
}
*/
