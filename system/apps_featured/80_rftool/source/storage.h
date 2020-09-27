class CConfiguration
{
public:
    char* _formatInt(char* buf, int n)
    {
        sprintf(buf, "%d", n);
        return buf;
    }
    
    char* _formatHex(char* buf, int n)
    {
        sprintf(buf, "0x%x", n);
        return buf;
    }
    
    void ConfigurationSave(const char* name)
    {
        bool b = mCommonWriter.Open((char*)name);
        if (!b)
        {
            _ASSERT(0);
            return;
        }
        char temp[16];

        //{frequency:868280000,bandwidth:135000,datarate:4096,gain:-12}
        mCommonWriter << "{frequency:" << _formatInt(temp, CC1101::GetFrequency())
        << ",bandwidth:" << _formatInt(temp, CC1101::GetBandwidth())
        << ",datarate:" << _formatInt(temp, CC1101::GetDataRate())
        << ",gain:" << _formatInt(temp, CC1101::GetGain())
        << "}";
        mCommonWriter.Close();

    }
    
    bool ConfigurationLoad(char* name)
    {
        bool b = mCommonReader.Open(name);
        if (!b)
            return false;

        _ASSERT(mCommonReader.GetFileSize() < BIOS::FAT::SectorSize);
        mCommonReader.Close();
        CJson json((char*)mCommonReader.GetBuffer());
        json.ForEach([](const CSubstring& key, const CSubstring& value)
        {
            if (key == "frequency")
            {
                int numeric = CConversion(value).ToInt();
                CC1101::SetFrequency(numeric);
            }
            else if (key == "bandwidth")
            {
                int numeric = CConversion(value).ToInt();
                CC1101::SetBandwidth(numeric);
            }
            else if (key == "datarate")
            {
                int numeric = CConversion(value).ToInt();
                CC1101::SetDataRate(numeric);
            }
            else if (key == "gain")
            {
                int numeric = CConversion(value).ToInt();
                CC1101::SetGain(numeric);
            }
        });
        return true;
    }
    
    void GetCurrentPath(char* path) // TODO: duplicty
    {
        // Full application path
        strcpy(path, BIOS::OS::GetArgument());

        // Strip app name
        char* last = strrchr(path, '/');
        if (last)
            *last = 0;
        else
            strcpy(path, "");
    }

    void GetUniqueName(char* name, const char* suffix)
    {
        static int counter = 0;
        GetCurrentPath(name);
        sprintf(name + strlen(name), "/dump%03d.%s", counter++, suffix);
    }

    void SaveSignal(char* name, CArray<uint16_t>& data)
    {
        bool b = mCommonWriter.Open((char*)name);
        if (!b)
        {
            _ASSERT(0);
            return;
        }
        
        mCommonWriter << "[";
        char temp[16];
        for (int i=0; i<data.GetSize(); i++)
        {
            if (i>0)
                mCommonWriter << ",";
            mCommonWriter << _formatInt(temp, data[i]);
        }
        mCommonWriter << "]";
        mCommonWriter.Close();
    }
    
    void LoadSignal(char* name)
    {
        bool b = mCommonReader.Open(name);
        if (!b)
        {
            _ASSERT(0);
            return;
        }
        int length = mCommonReader.GetFileSize();
        _ASSERT(length < BIOS::FAT::SectorSize);
        mCommonReader.Close();

        LoadSignalJson((char*)mCommonReader.GetBuffer(), length);
    }
    
    void LoadSignalJson(char* buf, int length = -1)
    {
        PULSE::arrSignal.RemoveAll();
        if (length == -1)
            length = strlen(buf);
        CJson json(CSubstring(buf, length));
        json.ForEach([](const CSubstring& value)
        {
            int pulse = CConversion(value).ToInt();
            PULSE::arrSignal.Add(pulse);
        });
        
        bool old = appData.GetLogging();
        appData.SetLogging(false);
        // temporarily disable logging
        analyse(PULSE::arrSignal);
        appData.SetLogging(old);
    }
    
    // list
    
    void SaveList(char* name)
    {
        bool b = mCommonWriter.Open((char*)name);
        if (!b)
        {
            _ASSERT(0);
            return;
        }
        char temp[16];

        mCommonWriter << "[";
        for (int i=0; i<appData.GetCaptureRecords(); i++)
        {
            int ts, uid;
            char name[64];
            char temp[16];
            appData.GetCaptureRecord(i, ts, uid, name, nullptr);

            if (i>0)
                mCommonWriter << ",";
            
            mCommonWriter << "{_protocol:\"" << name << "\",";
            
            const CAttributes& attrs = appData.GetAttributes(i);
            
            for (int j=0; j<attrs.GetSize(); j++)
            {
                if (j>0)
                    mCommonWriter << ",";
                if (strstr(attrs[j].key, "data"))
                    mCommonWriter << "\"" << attrs[j].key << "\":" << _formatHex(temp, attrs[j].value);
                else
                    mCommonWriter << "\"" << attrs[j].key << "\":" << _formatInt(temp, attrs[j].value);
            }
            mCommonWriter << "}";
        }
        mCommonWriter << "]";

        mCommonWriter.Close();
    }
    
    void LoadList(char* name)
    {
        bool b = mCommonReader.Open(name);
        if (!b)
        {
            _ASSERT(0);
            return;
        }
        int length = mCommonReader.GetFileSize();
        _ASSERT(length < BIOS::FAT::SectorSize);
        mCommonReader.Close();
        
        LoadListJson((char*)mCommonReader.GetBuffer(), length);
    }
    
    void LoadListJson(char* buf, int length = -1)
    {
        PULSE::arrSignal.RemoveAll();
        
        if (length == -1)
            length = strlen(buf);
        
        CJson json(CSubstring(buf, length));
        json.ForEach([](const CSubstring& value)
        {
            attributes.RemoveAll();
            
            CJson record(value);
            static CProtocol* protocol;
            protocol = nullptr;
            
            record.ForEach([](const CSubstring& key, const CSubstring& value)
            {
                if (key == "_protocol")
                {
                    char protocolName[32];
                    CConversion(value).ToString(protocolName, sizeof(protocolName));
                    for (int i=0; i<COUNT(protocols); i++)
                    {
                        CProtocol* _protocol = protocols[i];
                        
                        char curName[32];
                        _protocol->GetName(curName);
                        
                        if (strcmp(curName, protocolName) == 0)
                            protocol = _protocol;
                    }
                    _ASSERT(protocol);
                }
                else
                {
                    char keyName[16];
                    CConversion(key).ToString(keyName, sizeof(keyName));
                    const char* fixName = FixedString(keyName); // we cant use strdup

                    attributes[fixName] = CConversion(value).ToInt();
                }
            });
            
            _ASSERT(protocol);
            if (protocol)
            {
                attributes["_uid"] = appData.GetRecordUid();
                attributes["_timestamp"] = BIOS::SYS::GetTick();
                appData.AddCaptureRecord(protocol, attributes);
            }
        });
    }
    
    static const char* FixedString(char* s)
    {
        const char* fixed[] = {"temperature10", "humidity", "id", "battery_low", "channel", "junk", "valid", "_timestamp", "_uid",
            "length", "data_0", "data_1", "data_2", "$model", "$opcode", "key", "counter"};
        
        for (int i=0; i<COUNT(fixed); i++)
            if (strcmp(fixed[i], s) == 0)
                return fixed[i];

        _ASSERT(0);
        return s;
    }
};
