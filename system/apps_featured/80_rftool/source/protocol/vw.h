class CVw : public CProtocol
{
    enum {
        String_Open = 101,
        String_Close = 102,
        String_Trunk = 103
    };
    
public:
    
    virtual int Frequency() override
    {
        return 434420000;
    }

    virtual void Example(CAttributes& attributes) override
    {
        attributes["length"] = 78;
        attributes["data_0"] = 0x40a444d8;
        attributes["data_1"] = 0x27131efc;
        attributes["data_2"] = 0x7c7c0000;
        attributes["$opcode"] = String_Open;
    }

    virtual bool Demodulate(const CArray<uint16_t>& pulse, CAttributes& attributes) override
    {
        uint8_t nibblesData[16];
        CArray<uint8_t> b(nibblesData, COUNT(nibblesData));

        int length = 0;
        if (!PulseToBytes(pulse, b, length))
            return false;

        if (length >= 77)
        {
            // trailing 7 bits is opcode
            if (b[9] == 0x7c)
                attributes["$opcode"] = String_Close;
            if (b[9] == 0x20)
                attributes["$opcode"] = String_Open;
            if (b[9] == 0xc8)
                attributes["$opcode"] = String_Trunk;
        }

        BitstreamToAttributes(b, length, attributes);
        Analyse(attributes);
        return true;
    }

    void Analyse(CAttributes& attributes)
    {
    }

    virtual bool Modulate(const CAttributes& attr, CArray<uint16_t>& pulse) override
    {
        uint8_t nibblesData[16];
        CArray<uint8_t> b(nibblesData, COUNT(nibblesData));

        int length = 0;
        AttributesToBitstream(attr, b, length);
        return BytesToPulse(b, length, pulse);
    }

    virtual int PulseDivisor() override { return 250; }
    
private:
    int PulseLen(int microseconds)
    {
        return (microseconds+250/2)/250;
    }

    int PulseDuration(int ticks)
    {
        return ticks*250;
    }

    bool match(const CArray<uint16_t>& pulse, int& i, const char* pattern)
    {
        int l = strlen(pattern);
            for (int j=0; j<l; j++)
                if (PulseLen(pulse[i+j]) != pattern[j] - '0')
                    return false;

        i += l;
        return true;
    }

    bool PulseToBytes(const CArray<uint16_t>& pulse, CArray<uint8_t>& bytes, int& length)
    {
        int i;
        for (i=0; i<pulse.GetSize()-4; i++)
        {
            int t = PulseLen(pulse[i]);
            if (t == 4)
                break;
            if (t != 2)
                return false;
        }

        if (i < 50) // should be 252
            return false;

        if (!match(pulse, i, "423333332"))
            return false;

        length = 0;
        int bits = 0;
        for (; i<pulse.GetSize(); i++)
        {
            int l = PulseLen(pulse[i]);

            if (l == 2)
            {
                bool last = i==pulse.GetSize()-1;
                if (last)
                    return false;

                int l1 = PulseLen(pulse[++i]);
                if (l1 != 2)
                    return false;

                // append 0
                length++;
            }
            else if (l == 4)
            {
                // append 1
                bits |= 1<<(7-(length&7));
                length++;
            }
            else
            {
                return false;
            }

            if ((length & 7) == 0)
            {
                // swap nibbles
                bytes.Add(bits);
                bits = 0;
            }
        }
        if ((length & 7) != 0)
        {
            bytes.Add(bits);
        }
        return true;
    }

    bool BytesToPulse(const CArray<uint8_t>& bytes, int length, CArray<uint16_t>& pulse)
    {
        const int preambule = 252; // parne cislo?
        for (int i=0; i<preambule; i++)
                pulse.Add(PulseDuration(2));

        const char* preamble = "423333332";
        for (int i=0; i<strlen(preamble); i++)
                pulse.Add(PulseDuration(preamble[i] - '0'));

        for (int i=0; i<length; i++)
        {
            int bit = (bytes[i/8] >> (7-(i&7))) & 1;
            if (bit==0)
            {
                pulse.Add(PulseDuration(2));
                pulse.Add(PulseDuration(2));
            } else
            {
                pulse.Add(PulseDuration(4));
            }
        }
        return true;
    }

    virtual void GetName(char* name) override
    {
        strcpy(name, "VAG AUT64 Keyfob");
    }

    virtual void GetDescription(CAttributes& attributes, char* desc) override
    {
        if (attributes.indexOf("$opcode") != -1)
            sprintf(desc, "%d bits: <%08x %08x %s>", (int)attributes["length"], (int)attributes["data_0"], (int)attributes["data_1"], GetString(attributes["$opcode"]));
        else
            sprintf(desc, "%d bits: <%08x %08x %04x>", (int)attributes["length"], (int)attributes["data_0"], (int)attributes["data_1"], ((uint32_t)attributes["data_2"]) >> 16);
    }
    
    virtual const char* GetString(int i) override
    {
        switch (i)
        {
            case String_Open: return "Unlock";
            case String_Close: return "Lock";
            case String_Trunk: return "Trunk";
        }
        return nullptr;
    }
};
