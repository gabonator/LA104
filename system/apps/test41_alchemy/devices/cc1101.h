#include "spi.h"

class CCc1101Modem
{
    CSpi mSpi;
public:
    void Init()
    {
        mSpi.begin();
    }
    
    void Write(uint_fast8_t reg)
    {
        mSpi.select();
        mSpi.wait();
        mSpi.transfer(reg);
        mSpi.deselect();
    }

    void Write(uint_fast8_t reg, uint_fast8_t value)
    {
        mSpi.select();
        mSpi.wait();
        mSpi.transfer(reg);
        mSpi.transfer(value);
        mSpi.deselect();
    }

    uint_fast8_t Read(uint_fast8_t reg)
    {
        enum {READ_SINGLE = 0x80};
        
        mSpi.select();
        mSpi.wait();
        mSpi.transfer(reg | READ_SINGLE);
        uint_fast8_t aux = mSpi.transfer(0);
        mSpi.deselect();
        return aux;
    }
    
    void Reset()
    {
        mSpi.deselect();                    // Deselect CC1101
        delayMicroseconds(5);
        mSpi.select();                      // Select CC1101
        delayMicroseconds(10);
        mSpi.deselect();                    // Deselect CC1101
        delayMicroseconds(41);
        mSpi.select();                      // Select CC1101
        mSpi.wait();                          // Wait until MISO goes low
        enum {CC1101_SRES = 0x30};
        Write(CC1101_SRES);                // Send reset command strobe
        mSpi.wait();                          // Wait until MISO goes low

        mSpi.deselect();                    // Deselect CC1101
    }
    
    void delayMicroseconds(int us)
    {
        us = us*12;
        while (us--)
        {
            __asm__("");
        }
    }

} mDeviceModem;

class CCc1101Config
{
public:
    enum TRegister {
        IOCFG2, IOCFG1, IOCFG0, FIFOTHR, SYNC1, SYNC0, PKTLEN,
        PKTCTRL1, PKTCTRL0, ADDR, CHANNR, FSCTRL1, FSCTRL0,
        FREQ2, FREQ1, FREQ0, MDMCFG4, MDMCFG3, MDMCFG2, MDMCFG1, MDMCFG0,
        DEVIATN, MCSM2, MCSM1, MCSM0, FOCCFG, BSCFG, AGCCTRL2, AGCCTRL1, AGCCTRL0,
        WOREVT1, WOREVT0, WORCTRL, FREND1, FREND0, FSCAL3, FSCAL2, FSCAL1, FSCAL0,
        RCCTRL1, RCCTRL0, FSTEST, PTEST, AGCTEST, TEST2, TEST1, TEST0,
        ALL
    };
    
    uint8_t mRegisters[ALL];
    bool mOnline{false};
    
    const char* mConfig =
    /*
    "{IOCFG2:0x0D,FIFOTHR:0x47,PKTCTRL0:0x32,FSCTRL1:0x06,"
    "FSCTRL0:0x00,FREQ2:0x10,FREQ1:0xB0,FREQ0:0x71,MDMCFG4:0xA7,MDMCFG3:0x32,"
    "MDMCFG2:0x30,MDMCFG1:0x22,MDMCFG0:0xF8,MCSM2:0x07,MCSM1:0x30,MCSM0:0x18,"
    "AGCCTRL2:0x04,AGCCTRL1:0x00,AGCCTRL0:0x92,FREND1:0xB6,FREND0:0x11,"
    "FSCAL3:0xE9,FSCAL2:0x2A,FSCAL1:0x00,FSCAL0:0x1F,TEST2:0x81,TEST1:0x35,"
    "TEST0:0x09}";
    */
    
    "{IOCFG2:0x0D,FIFOTHR:0x47,PKTCTRL0:0x32,FSCTRL1:0x06,FSCTRL0:0x00,FREQ2:0x10,"
    "FREQ1:0xB0,FREQ0:0x71,MDMCFG4:0xA7,MDMCFG3:0x32,MDMCFG2:0x30,MDMCFG1:0x22,"
    "MDMCFG0:0xF8,MCSM2:0x07,MCSM1:0x30,MCSM0:0x18,AGCCTRL2:0x04,AGCCTRL1:0x00,"
    "AGCCTRL0:0x92,FREND1:0xB6,FREND0:0x11,FSCAL3:0xE9,FSCAL2:0x2A,FSCAL1:0x00,"
    "FSCAL0:0x1F,TEST2:0x81,TEST1:0x35,TEST0:0x09"
    ",AGCCTRL2:0x07}";
    //"}";

    TRegister GetRegister(char* reg)
    {
        #define REG(r) if (strcmp(reg, #r)==0) return r
        REG(IOCFG2); REG(IOCFG1); REG(IOCFG0); REG(FIFOTHR); REG(SYNC1); REG(SYNC0); REG(PKTLEN);
        REG(PKTCTRL1); REG(PKTCTRL0); REG(ADDR); REG(CHANNR); REG(FSCTRL1); REG(FSCTRL0);
        REG(FREQ2); REG(FREQ1); REG(FREQ0); REG(MDMCFG4); REG(MDMCFG3); REG(MDMCFG2); REG(MDMCFG1); REG(MDMCFG0);
        REG(DEVIATN); REG(MCSM2); REG(MCSM1); REG(MCSM0); REG(FOCCFG); REG(BSCFG); REG(AGCCTRL2); REG(AGCCTRL1); REG(AGCCTRL0);
        REG(WOREVT1); REG(WOREVT0); REG(WOREVT0); REG(FREND1); REG(FREND1); REG(FREND0); REG(FSCAL3); REG(FSCAL2);
        REG(FSCAL1); REG(FSCAL0);
        REG(RCCTRL1); REG(RCCTRL0); REG(FSTEST); REG(PTEST); REG(AGCTEST); REG(TEST2); REG(TEST1); REG(TEST0);
        #undef REG
        _ASSERT(0);
        return (TRegister)0;
    }
    
    bool Load(char* filename)
    {
//        static CCc1101Config* _this;
        static long _lTimeout;
        static bool _check;
        _lTimeout = BIOS::SYS::GetTick() + 1500;
        _check = true;
//        _this = this;
        
        BIOS::DBG::Print("CONFIG START");
        mDeviceModem.Reset();
  /*      
        CJson(mConfig).ForEach([](const CSubstring& key, const CSubstring& value)
        {
            char strKey[32];
            key.ToString(strKey, 32);
            int reg = _this->GetRegister(strKey);
            int val = CConversion(value).ToInt();
            _this->mRegisters[reg] = val;
            do {
                mDeviceModem.Write(reg, val);
                if ((long)BIOS::SYS::GetTick() > _lTimeout)
                {
                    _check = false;
                    return;
                }
            }
            while ((int)mDeviceModem.Read(reg) != val);
        });*/

  char str[512];
  strcpy(str, mConfig+1);
  str[strlen(str)-1] = 0;
  char* pch = strtok (str,":,");
  while (pch != NULL)
  {
    int reg = GetRegister(pch);
    pch = strtok (NULL, ":,");
    int val = CConversion(pch).ToInt();
    pch = strtok (NULL, ":,");
//    BIOS::DBG::Print("%02x->%02x,", reg, val);

            mRegisters[reg] = val;
            do {
                mDeviceModem.Write(reg, val);
                if ((long)BIOS::SYS::GetTick() > _lTimeout)
                {
                    _check = false;
                    break;
                }
            }
            while ((int)mDeviceModem.Read(reg) != val);

  }
        BIOS::DBG::Print("CONFIG DONE=%d", _check);

        if (!_check)
            return false;
        
        enum {CC1101_SRX = 0x34};
        mDeviceModem.Write(CC1101_SRX);
        mDeviceModem.Write(PKTCTRL0, 0x30);
        mDeviceModem.Write(IOCFG0, 0x0d);

        mOnline = mDeviceModem.Read(TEST1) == mRegisters[TEST1];
        return mOnline;
    }
    
    bool Apply()
    {
        return false;
    }
    
    int GetOscFrequency()
    {
        return 26e6; // 26 MHz;
    }
    
    int GetFrequency()
    {
        //return 433e6;
        // TODO: problem!
        uint32_t freqReg = ((uint32_t)mRegisters[FREQ2] << 16) | (((uint32_t)mRegisters[FREQ1]) << 8) | mRegisters[FREQ0];
        _ASSERT(mRegisters[CHANNR] == 0);
        uint32_t freqOffset = 0; //mRegisters[CHANNR]*((256 + mRegisters[MDMCFG0])<<(_modem.channel_space_exp & 0x03));
        uint32_t freqHz = ((uint64_t)(freqReg + freqOffset) * (uint32_t)GetOscFrequency()) >> 16;
        return freqHz;
    }

    int GetBandwidth()
    {
        int CHANBW_M = (mRegisters[MDMCFG4] >> 4) & 3;
        int CHANBW_E = mRegisters[MDMCFG4] >> 6;
        return (GetOscFrequency() / 8 / (4 + CHANBW_M)) >> CHANBW_E;
    }

    int GetDataRate()
    {
        int DRATE_M = mRegisters[MDMCFG3];
        int DRATE_E = mRegisters[MDMCFG4] & 0x0f;
        uint64_t rate = ((uint64_t)((256 + DRATE_M) << (DRATE_E)) * GetOscFrequency()) >> 28ULL;
        return rate;
    }

    int GetGain()
    {
        int MAX_LNA_GAIN = (mRegisters[AGCCTRL2]>>3) & 7;
        return -17*MAX_LNA_GAIN/7;
    }
    
    const char* GetModulation()
    {
        int MOD_FORMAT = (mRegisters[MDMCFG2]>>4) & 7;
        switch (MOD_FORMAT)
        {
            case 0: return "2-FSK";
            case 1: return "GFSK";
            case 3: return "ASK/OOK";
            case 4: return "4-FSK";
            case 7: return "MSK";
            default:
                return "???";
        }
    }
    
    void DeltaFreq(int d)
    {
        uint32_t freqReg = ((uint32_t)mRegisters[FREQ2] << 16) | (((uint32_t)mRegisters[FREQ1]) << 8) | mRegisters[FREQ0];
        freqReg += d;
        mRegisters[FREQ2] = freqReg >> 16;
        mRegisters[FREQ1] = freqReg >> 8;
        mRegisters[FREQ0] = freqReg;

        mDeviceModem.Write(FREQ2, mRegisters[FREQ2]);
        mDeviceModem.Write(FREQ1, mRegisters[FREQ1]);
        mDeviceModem.Write(FREQ0, mRegisters[FREQ0]);
    }

    void DeltaGain(int d)
    {
        int MAX_LNA_GAIN = (mRegisters[AGCCTRL2]>>3) & 7;
        MAX_LNA_GAIN += d;
        MAX_LNA_GAIN = min(max(0, MAX_LNA_GAIN), 7);
        mRegisters[AGCCTRL2] &= ~0b00111000;
        mRegisters[AGCCTRL2] |= MAX_LNA_GAIN << 3;
        mDeviceModem.Write(AGCCTRL2, mRegisters[AGCCTRL2]);
    }
   
    void DeltaBandwidth(int d)
    {
        int CHANBW = mRegisters[MDMCFG4] >> 4;
        CHANBW += d;
        CHANBW = min(max(0, CHANBW), 15);
        mRegisters[MDMCFG4] &= 0x0f;
        mRegisters[MDMCFG4] |= CHANBW << 4;
        mDeviceModem.Write(MDMCFG4, mRegisters[MDMCFG4]);
    }

private:
    constexpr static int ticks20us = 140;
    
    int On()
    {
        int i;
        for (i=0; i<8000; i+=20)
        {
            volatile int j=ticks20us;
            while (j--);
            
            if (!Read())
                return i;
        }
        return i;
    }
    
    int Off()
    {
        int i;
        for (i=0; i<8000; i+=20)
        {
            volatile int j=ticks20us;
            while (j--);
            
            if (Read())
                return i;
        }
        return i;
    }
    
    int tempBuffer[10];
    int tempReceived{0};
    
public:
    bool Read()
    {
#ifdef __APPLE__
        static int counter = 0;
        counter++;
        if (counter < 20)
            return false;
        counter = 0;
        return true;
#endif
        return BIOS::GPIO::DigitalRead(BIOS::GPIO::P4);
    }

    bool Receive(int* pBuffer, int nBufferSize, int& nReceived)
    {
#ifdef __APPLE__
        for (nReceived=0; nReceived<30; nReceived+=2)
        {
            pBuffer[nReceived] = 250-30+rand()%60;
            if ((rand() & 1) == 0)
                pBuffer[nReceived+1] = 2000 - 50 + rand()%100;
            else
                pBuffer[nReceived+1] = 4000 - 100 + rand()%200;
        }
        return true;
#endif
        nReceived = 0;
        
        while (1)
        {
            int nOn = On();
            
            if (nOn < 8000) // TODO: CONSTANT
            {
                pBuffer[nReceived++] = nOn;
                if (nReceived >= nBufferSize)
                    break;
            }
            else
                break;
            
            int nOff = Off();
            
            if (nOff < 8000)
            {
                pBuffer[nReceived++] = nOff;
                if (nReceived >= nBufferSize)
                    break;
            }
            else
                break;
        }

        if (nReceived < COUNT(tempBuffer))
        {
            memcpy(tempBuffer, pBuffer, sizeof(tempBuffer));
            nReceived = tempReceived; // TODO: when switching btwn different modems invalidate temp buf
            return false;
        }
        
        memcpy(tempBuffer, pBuffer, sizeof(tempBuffer));
        tempReceived = nBufferSize;
        return true;
    }

};
