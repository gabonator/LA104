class CApplicationData
{
    const static int KVSlots{16};
    bool mConnected = false;
    CArray<CProtocol*> mProtocols;
    CProtocol* mProtocolsData[16] {0};
    TKeyValue mAttributesBuffer[16*KVSlots];
    bool mLogging{false};
    bool mPaused{false};
    int mRecordUid{1000};
        
public:
    CApplicationData()
    {
        mProtocols.Init(mProtocolsData, COUNT(mProtocolsData));
    }
    bool GetConnected()
    {
        return mConnected;
    }
    int GetFrequency()
    {
        return CC1101::GetFrequency();
    }
    int GetBandwidth()
    {
        return CC1101::GetBandwidth();
    }
    int GetGain()
    {
        return CC1101::GetGain();
    }
    int GetDataRate()
    {
        return CC1101::GetDataRate();
    }
    void DeltaFrequency(int d)
    {
        CC1101::SetFrequency(CC1101::GetFrequency()+d*50000);
    }
    void DeltaBandwidth(int d)
    {
        CC1101::DeltaBandwidth(d);
    }
    void DeltaGain(int d)
    {
        CC1101::DeltaGain(d);
    }
    void DeltaDataRate(int d)
    {
        CC1101::SetDataRate(CC1101::GetDataRate() + d * 200);
    }
    int GetSample()
    {
        return ((BIOS::SYS::GetTick()>>7)&1)?255:0;
    }
    
    // Capture
    int GetCaptureRecords()
    {
        return mProtocols.GetSize();
    }
    
    void GetCaptureRecord(int i, int& ts, int& uid, char* name, char* desc)
    {
        CAttributes local(&mAttributesBuffer[i*KVSlots], KVSlots);
        for (int j=0; j<KVSlots; j++)
            if (mAttributesBuffer[i*KVSlots+j].key == nullptr)
            {
                local.SetSize(j);
                break;
            }

        ts = local["_timestamp"];
        uid = local["_uid"];
        if (name)
            mProtocols[i]->GetName(name);
        if (desc)
            mProtocols[i]->GetDescription(local, desc);
    }

    bool AddCaptureRecord(CProtocol* pProtocol, const CAttributes& attr)
    {
        int index = mProtocols.GetSize();
        if (index == mProtocols.GetMaxSize())
        {
            // TODO: notify full buffer
            return false;
        }
        
        CAttributes local(&mAttributesBuffer[index*KVSlots], KVSlots);
        local.Copy(attr);
            
        mProtocols.Add(pProtocol);
        return true;
    }

    CAttributes GetAttributes(int i)
    {
        CAttributes local(&mAttributesBuffer[i*KVSlots], KVSlots);
        for (int j=0; j<KVSlots; j++)
            if (mAttributesBuffer[i*KVSlots+j].key == nullptr)
            {
                local.SetSize(j);
                return local;
            }
        
        _ASSERT(0);
        return local;
    }

    // Capture data
    int GetCaptureIndex(int uid)
    {
        for (int i=0; i<GetCaptureRecords(); i++)
        {
            CAttributes attr = GetAttributes(i);
            if (attr["_uid"] == uid)
                return i;
        }
        return -1;
    }
    
    int GetCaptureAttributesCount(int i)
    {
        CAttributes local(&mAttributesBuffer[i*KVSlots], KVSlots);
        for (int j=0; j<KVSlots; j++)
            if (mAttributesBuffer[i*KVSlots+j].key == nullptr)
            {
                local.SetSize(j);
                break;
            }

        return local.GetSize();
    }

    void DeltaCaptureAttribute(int i, int j, int d)
    {
        CAttributes local(&mAttributesBuffer[i*KVSlots], KVSlots);
        for (int j=0; j<KVSlots; j++)
            if (mAttributesBuffer[i*KVSlots+j].key == nullptr)
            {
                local.SetSize(j);
                break;
            }
        
        local.GetData()[j].value += d;
        updateWave(i);
    }

    void GetCaptureAttribute(int i, int j, char* name, char* value, char* units)
    {
        CAttributes local(&mAttributesBuffer[i*KVSlots], KVSlots);
        for (int j=0; j<KVSlots; j++)
            if (mAttributesBuffer[i*KVSlots+j].key == nullptr)
            {
                local.SetSize(j);
                break;
            }

        const CAttributes::TAttribute& attr = local[j];
        if (attr.key[0] == '$') // POZOR, ukladame pointer!?
        {
            const char* decoded = mProtocols[i]->GetString(attr.value);
            _ASSERT(decoded);
            
            strcpy(name, attr.key+1);
            strcpy(value, decoded);
            strcpy(units, "");
            return;
        }
        
        if (strcmp(attr.key, "temperature10") == 0)
        {
            strcpy(name, "Temperature");
            int t = (int)attr.value;
            if (t<0)
            {
                t = -t;
                sprintf(value, "-%d.%d", t/10, t%10);
            }
            else
                sprintf(value, "%d.%d", t/10, t%10);
            strcpy(units, "\xf8""C");
            return;

        }
        if (strcmp(attr.key, "humidity") == 0)
        {
            strcpy(name, "Humidity");
            sprintf(value, "%d", (int)attr.value);
            strcpy(units, "%");
            return;

        }
        if (strcmp(attr.key, "length") == 0)
        {
            strcpy(name, attr.key);
            sprintf(value, "%d", (int)attr.value);
            strcpy(units, "bits");
            return;
        }
        if (strcmp(attr.key, "id") == 0 || strcmp(attr.key, "channel") == 0 || strcmp(attr.key, "junk") == 0)
        {
            strcpy(name, attr.key);
            sprintf(value, "%d", (int)attr.value);
            strcpy(units, "");
            return;
        }
        if ( strcmp(attr.key, "key") == 0 || strcmp(attr.key, "counter") == 0)
        {
            strcpy(name, attr.key);
            sprintf(value, "%x", (int)attr.value);
            strcpy(units, "hex");
            return;
        }
        if (strcmp(attr.key, "battery_low") == 0 || strcmp(attr.key, "valid") == 0)
        {
            strcpy(name, attr.key);
            if (attr.value)
                strcpy(value, "true");
            else
                strcpy(value, "false");
            strcpy(units, "");
            return;
        }
        if (strncmp(attr.key, "data_", 5) == 0)
        {
            strcpy(name, attr.key);
            sprintf(value, "%08x", (uint32_t)attr.value);
            strcpy(units, "");
            return;
        }

        strcpy(name, attr.key);
        sprintf(value, "%d", (int)attr.value);
        strcpy(units, "?");
    }
    
    void ClearList()
    {
        mProtocols.RemoveAll();
        for (int i=0; i<COUNT(mAttributesBuffer); i++)
        {
            mAttributesBuffer[i].key = nullptr;
            mAttributesBuffer[i].value = 0;
        }
    }
    
    void GetWaveform(int i, CArray<uint16_t>& pulse)
    {
        CProtocol* protocol = mProtocols[i];
        CAttributes local(&mAttributesBuffer[i*KVSlots], KVSlots);
        for (int j=0; j<KVSlots; j++)
            if (mAttributesBuffer[i*KVSlots+j].key == nullptr)
            {
                local.SetSize(j);
                break;
            }
        
        pulse.SetSize(0);
        protocol->Synthesize(local);
        protocol->Modulate(local, pulse);
    }
    
    
    //
    void SetConnected(bool b)
    {
        mConnected = b;
    }
    
    //
    bool GetLogging()
    {
        return mLogging;
    }
    
    void SetLogging(bool logging)
    {
        mLogging = logging;
    }
    
    bool GetPaused()
    {
        return mPaused;
    }

    void SetPaused(bool paused)
    {
        mPaused = paused;
    }

    void ToggleFrequency()
    {
        int f = CC1101::GetFrequency();
        if (f < 500000000) // 500 MHz
            f = 868280000; // 868.28 MHz
        else
            f = 433940000; // 433.94 MHz
        CC1101::SetFrequency(f);
    }
    
    int GetRecordUid()
    {
        return mRecordUid++;
    }
    
    CProtocol* GetRecordProtocol(int i)
    {
        return mProtocols[i];
    }
};

CApplicationData appData;
