class CTokenizer
{
    char* mString;
    
public:
    CTokenizer(char* str)
    {
        mString = str;
    }
    
    bool contains(char* key)
    {
        return strstr(mString, key) != 0;
    }
    
    bool GetToken(char* out, char delim, int maxLength)
    {
        if (mString[0] == 0)
            return false;
        
        for (int i=0; i<maxLength; i++)
        {
            out[i] = mString[i];
            if (mString[i] == 0)
            {
                mString += i;
                return true;
            }
            if (mString[i] == delim)
            {
                out[i] = 0;
                mString += i;
                mString++;
                return true;
            }
        }
        mString += maxLength;
        out[maxLength] = 0;
        return true;
    }
};
