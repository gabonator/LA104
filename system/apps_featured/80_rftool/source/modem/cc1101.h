#include "spi.h"
//#include <math.h>

class CSampler
{
public:
  virtual bool Init() { _ASSERT(0); return false; }
  virtual void Deinit() { _ASSERT(0); }
  virtual bool Read() { _ASSERT(0); return false; }

    void delayMicroseconds(int us)
    {
        us = us*12;
        while (us--)
        {
            __asm__("");
        }
    }

};

class CCc1101Spi : public CSampler
{
    CSpi mSpi;

public:
    struct TConfig
    {
        bool mInvert{false};
        BIOS::GPIO::EPin mInputPin{BIOS::GPIO::P4};
        BIOS::GPIO::EPin mOutputPin{BIOS::GPIO::P4};
    };

private:
    TConfig mConfig;
    
public:
    virtual bool InitModem() { return false; }
    
    virtual bool Init() override
    {
        mSpi.begin();
        return InitModem();
    }
    
    virtual void Deinit() override
    {
        mSpi.end();
    }
    
    uint_fast8_t Write(uint_fast8_t reg)
    {
        mSpi.select();
        mSpi.wait();
        uint_fast8_t status = mSpi.transfer(reg);
        mSpi.deselect();
        return status;
    }

    void FixWrite(uint_fast8_t reg)
    {
        mSpi.select();
//        mSpi.wait();
        mSpi.fixtransfer(reg);
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

    void Write(uint_fast8_t reg, uint8_t* buffer, int len)
    {
	enum { WRITE_BURST = 0x40 };

        mSpi.select();
        mSpi.wait();
        mSpi.transfer(reg | WRITE_BURST);
        while (len--)
          mSpi.transfer(*buffer++);
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
    
    uint_fast8_t ReadBurst(uint_fast8_t reg)
    {
        enum {READ_BURST = 0xC0};
        
        mSpi.select();
        mSpi.wait();
        mSpi.transfer(reg | READ_BURST);
        uint_fast8_t aux = mSpi.transfer(0);
        mSpi.deselect();
        return aux;
    }

    void Reset()
    {
        mSpi.deselect();                    // Deselect CC1101
        BIOS::SYS::DelayMs(1);
        mSpi.select();                      // Select CC1101
        BIOS::SYS::DelayMs(1);
        mSpi.deselect();                    // Deselect CC1101
        BIOS::SYS::DelayMs(1);
        mSpi.select();                      // Select CC1101
        mSpi.wait();                          // Wait until MISO goes low
        enum {CC1101_SRES = 0x30};
        Write(CC1101_SRES);                // Send reset command strobe
        mSpi.wait();                          // Wait until MISO goes low
        mSpi.deselect();                    // Deselect CC1101
    }

    virtual bool Read() override
    {
#ifdef EMULATED
        int nTick = BIOS::SYS::GetTick();
        int nSecond = nTick / 2000;
        static int nLastSecond = 0;
        if (nSecond == nLastSecond)
            return false;
        nLastSecond = nSecond;
        return true;
#endif
        return BIOS::GPIO::DigitalRead(mConfig.mInputPin) ^ mConfig.mInvert;
    }
    
    TConfig& Configuration()
    {
        return mConfig;
    }
};

class CDeviceCC1101 : public CCc1101Spi
{
//    bool mLoaded{false};
//  char str[600]; // TODO!! STACK!!!
    
public:
    enum TRegister {
        IOCFG2, IOCFG1, IOCFG0, FIFOTHR, SYNC1, SYNC0, PKTLEN,
        PKTCTRL1, PKTCTRL0, ADDR, CHANNR, FSCTRL1, FSCTRL0,
        FREQ2, FREQ1, FREQ0, MDMCFG4, MDMCFG3, MDMCFG2, MDMCFG1, MDMCFG0,
        DEVIATN, MCSM2, MCSM1, MCSM0, FOCCFG, BSCFG, AGCCTRL2, AGCCTRL1, AGCCTRL0,
        WOREVT1, WOREVT0, WORCTRL, FREND1, FREND0, FSCAL3, FSCAL2, FSCAL1, FSCAL0,
        RCCTRL1, RCCTRL0, FSTEST, PTEST, AGCTEST, TEST2, TEST1, TEST0,
        ALL,
        PATABLE=0x3E
    };
    
    uint8_t mRegisters[ALL];
    int mRegisterPower{0x50};

    const char* mConfig =

    "{"
    "IOCFG0:0x06,"
    "PKTCTRL1:0x06,PKTCTRL0:0x00,PKTLEN:0x04,"
    "FSCTRL1:0x06,FSCTRL0:0x00,FREQ2:0x10,FREQ1:0xB0,FREQ0:0x71,"
    "MDMCFG4:0xA4,MDMCFG3:0x32,MDMCFG2:0x30,MDMCFG1:0x22,MDMCFG0:0xF8,"
//    "MCSM2:0x07,MCSM1:0x30,MCSM0:0x18,"
    "MCSM2:0x07,MCSM1:0x00,MCSM0:0x18," // disable CCA! disabled autocal????
    "AGCCTRL2:0x04,AGCCTRL1:0x00,AGCCTRL0:0x92,"
    "FREND1:0xB6,FREND0:0x11,FSCAL3:0xE9,FSCAL2:0x2A,FSCAL1:0x00,"
    "FSCAL0:0x1F,TEST2:0x81,TEST1:0x35,TEST0:0x09"
    ",AGCCTRL2:0x07,PATABLE:0x50}";

/*
    // Oregon
    "{IOCFG2:0x0D,FIFOTHR:0x47,PKTCTRL0:0x32,FSCTRL1:0x06,FSCTRL0:0x00,FREQ2:0x10,"
    "FREQ1:0xB0,FREQ0:0x71,MDMCFG4:0xF7,MDMCFG3:0x32,MDMCFG2:0x30,MDMCFG1:0x22,"
    "MDMCFG0:0xF8,MCSM2:0x07,MCSM1:0x30,MCSM0:0x18,"
    "AGCCTRL2:0x04,AGCCTRL1:0x00,AGCCTRL0:0x92,"
    "FREND1:0xB6,FREND0:0x11,FSCAL3:0xE9,FSCAL2:0x2A,FSCAL1:0x00,"
    "FSCAL0:0x1F,TEST2:0x81,TEST1:0x35,TEST0:0x09"
    ",AGCCTRL2:0x07}";
*/

    TRegister GetRegister(char* reg)
    {
        #define REG(r) if (strcmp(reg, #r)==0) return r
        REG(IOCFG2); REG(IOCFG1); REG(IOCFG0); REG(FIFOTHR); REG(SYNC1); REG(SYNC0); REG(PKTLEN);
        REG(PKTCTRL1); REG(PKTCTRL0); REG(ADDR); REG(CHANNR); REG(FSCTRL1); REG(FSCTRL0);
        REG(FREQ2); REG(FREQ1); REG(FREQ0); REG(MDMCFG4); REG(MDMCFG3); REG(MDMCFG2); REG(MDMCFG1); REG(MDMCFG0);
        REG(DEVIATN); REG(MCSM2); REG(MCSM1); REG(MCSM0); REG(FOCCFG); REG(BSCFG); REG(AGCCTRL2); REG(AGCCTRL1); REG(AGCCTRL0);
        REG(WOREVT1); REG(WOREVT0); REG(WOREVT0); REG(WORCTRL); REG(FREND1); REG(FREND1); REG(FREND0); REG(FSCAL3); REG(FSCAL2);
        REG(FSCAL1); REG(FSCAL0);
        REG(RCCTRL1); REG(RCCTRL0); REG(FSTEST); REG(PTEST); REG(AGCTEST); REG(TEST2); REG(TEST1); REG(TEST0);
        REG(PATABLE);
        #undef REG
        _ASSERT(0);
        return (TRegister)0;
    }

    const char* GetRegister(TRegister reg)
    {
        #define REG(r) if (reg==r) return #r;
        REG(IOCFG2); REG(IOCFG1); REG(IOCFG0); REG(FIFOTHR); REG(SYNC1); REG(SYNC0); REG(PKTLEN);
        REG(PKTCTRL1); REG(PKTCTRL0); REG(ADDR); REG(CHANNR); REG(FSCTRL1); REG(FSCTRL0);
        REG(FREQ2); REG(FREQ1); REG(FREQ0); REG(MDMCFG4); REG(MDMCFG3); REG(MDMCFG2); REG(MDMCFG1); REG(MDMCFG0);
        REG(DEVIATN); REG(MCSM2); REG(MCSM1); REG(MCSM0); REG(FOCCFG); REG(BSCFG); REG(AGCCTRL2); REG(AGCCTRL1); REG(AGCCTRL0);
        REG(WOREVT1); REG(WOREVT0); REG(WOREVT0); REG(WORCTRL); REG(FREND1); REG(FREND1); REG(FREND0); REG(FSCAL3); REG(FSCAL2);
        REG(FSCAL1); REG(FSCAL0);
        REG(RCCTRL1); REG(RCCTRL0); REG(FSTEST); REG(PTEST); REG(AGCTEST); REG(TEST2); REG(TEST1); REG(TEST0);
        REG(PATABLE);
        #undef REG
        _ASSERT(0);
        return "";
    }

    bool Load(char* filename)
    {
        static CDeviceCC1101* _this;
        static long _lTimeout;
        static bool _check;
        _lTimeout = BIOS::SYS::GetTick() + 1500;
        _check = true;
        _this = this;
        
        CCc1101Spi::Reset();

        CJson(mConfig).ForEach([](const CSubstring& key, const CSubstring& value)
        {
            char strKey[32];
            key.ToString(strKey, 32);
            int reg = _this->GetRegister(strKey);
            int val = CConversion(value).ToInt();
            _this->mRegisters[reg] = val;
            do {
                _this->Write(reg, val);
                if ((long)BIOS::SYS::GetTick() > _lTimeout)
                {
                    _check = false;
                    return;
                }
            }
            while ((int)_this->Read(reg) != val);
        });

        if (!_check)
            return false;

        return CCc1101Spi::Read(TEST1) == mRegisters[TEST1];
//        mLoaded = CCc1101Spi::Read(TEST1) == mRegisters[TEST1];
//        return mLoaded;
    }
    
    bool Apply()
    {
        return false;
    }
    
    virtual bool InitModem()
    {
#ifdef __APPLE__
		return true;
#endif
//        if (mLoaded)
//            return true;
        return Load(nullptr);
    }

    int GetOscFrequency()
    {
        return 26e6; // 26 MHz
    }
        
    int GetRssi()
    {
        constexpr int CC1101_RSSI = 0x34;
        int rssi = CCc1101Spi::ReadBurst(CC1101_RSSI);
        if (rssi >= 128)
            rssi = rssi - 256;
        return (int)rssi+128; // 0..255
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

        CCc1101Spi::Write(FREQ2, mRegisters[FREQ2]);
        CCc1101Spi::Write(FREQ1, mRegisters[FREQ1]);
        CCc1101Spi::Write(FREQ0, mRegisters[FREQ0]);
        _ASSERT(CCc1101Spi::Read(FREQ2) == mRegisters[FREQ2]);
        _ASSERT(CCc1101Spi::Read(FREQ1) == mRegisters[FREQ1]);
        _ASSERT(CCc1101Spi::Read(FREQ0) == mRegisters[FREQ0]);
    }
    
    void DeltaGain(int d)
    {
        int MAX_LNA_GAIN = (mRegisters[AGCCTRL2]>>3) & 7;
        MAX_LNA_GAIN += d;
        MAX_LNA_GAIN = min(max(0, MAX_LNA_GAIN), 7);
        mRegisters[AGCCTRL2] &= ~0b00111000;
        mRegisters[AGCCTRL2] |= MAX_LNA_GAIN << 3;
        CCc1101Spi::Write(AGCCTRL2, mRegisters[AGCCTRL2]);
    }
   
    void DeltaBandwidth(int d)
    {
        int CHANBW = mRegisters[MDMCFG4] >> 4;
        CHANBW += d;
        CHANBW = min(max(0, CHANBW), 15);
        mRegisters[MDMCFG4] &= 0x0f;
        mRegisters[MDMCFG4] |= CHANBW << 4;
        CCc1101Spi::Write(MDMCFG4, mRegisters[MDMCFG4]);
    }

    void SetRxState()
    {
//BIOS::DBG::Print("srx ");

        enum {CC1101_SRX = 0x34};
        CCc1101Spi::Write(PKTCTRL0, 0x30); // RX: 0x30 - async serial, data out n GDOx, TX: 0x00 - use TX fifo
        CCc1101Spi::Write(IOCFG0, 0x0d); // RX: GDO0: serial data output, TX: 0x06 - optional transmission sync pulse
        CCc1101Spi::Write(CC1101_SRX);
    }

    void SetTxState()
    {
//BIOS::DBG::Print("stx ");
        enum {CC1101_STX = 0x35};
        CCc1101Spi::Write(CC1101_STX);
    }

    void PrepareTxState()
    {
//BIOS::DBG::Print("ptx ");
        CCc1101Spi::Write(PKTCTRL0, 0x00); // RX: 0x30 - async serial, data out n GDOx, TX: 0x00 - use TX fifo
        CCc1101Spi::Write(IOCFG0, 0x06); // RX: GDO0: serial data output, TX: 0x06 - optional transmission sync pulse
    }

    void LeaveTxState()
    {
//BIOS::DBG::Print("ltx ");
        FlushRxFifo();
        CCc1101Spi::Write(PKTCTRL0, 0x30); // RX: 0x30 - async serial, data out n GDOx, TX: 0x00 - use TX fifo
        CCc1101Spi::Write(IOCFG0, 0x0d); // RX: GDO0: serial data output, TX: 0x06 - optional transmission sync pulse
    }

    void SetIdleState()
    {
//BIOS::DBG::Print("sis ");

        enum {CC1101_SIDLE = 0x36};
        CCc1101Spi::Write(CC1101_SIDLE);
    }

    void FlushTxFifo()
    {
        enum {CC1101_SFTX = 0x3B};
        CCc1101Spi::Write(CC1101_SFTX);
    }

    void FlushRxFifo()
    {
        enum {CC1101_SFRX = 0x3A};
        CCc1101Spi::Write(CC1101_SFRX);
    }

    void Calibrate()
    {
        enum {CC1101_SCAL = 0x33};
        CCc1101Spi::Write(CC1101_SCAL);
    }

    void SetPacketLength(int len)
    {
        _ASSERT(len > 0 && len < 256);
	// todo: save in regs
        CCc1101Spi::Write(PKTLEN, len);         
    }

    void SetFinitePacketLength()
    {
        mRegisters[PKTCTRL0] &= 0b11111100;
        CCc1101Spi::Write(PKTCTRL0, mRegisters[PKTCTRL0]);
    }

    void SetInfinitePacketLength()
    {
        mRegisters[PKTCTRL0] &= 0b11111100;
        mRegisters[PKTCTRL0] |= 0b00000010;
        CCc1101Spi::Write(PKTCTRL0, mRegisters[PKTCTRL0]);
    }
};

class CDeviceCC1101Interface : public CDeviceCC1101
{
public:
    void SetFrequency(int32_t freqHz)
    {
        uint32_t freqReg = (((uint64_t)freqHz) << 16) / GetOscFrequency();

        mRegisters[FREQ2] = freqReg >> 16;
        mRegisters[FREQ1] = freqReg >> 8;
        mRegisters[FREQ0] = freqReg;
        
        CCc1101Spi::Write(FREQ2, mRegisters[FREQ2]);
        CCc1101Spi::Write(FREQ1, mRegisters[FREQ1]);
        CCc1101Spi::Write(FREQ0, mRegisters[FREQ0]);

#ifndef __APPLE__
        _ASSERT(CCc1101Spi::Read(FREQ2) == mRegisters[FREQ2]);
        _ASSERT(CCc1101Spi::Read(FREQ1) == mRegisters[FREQ1]);
        _ASSERT(CCc1101Spi::Read(FREQ0) == mRegisters[FREQ0]);
#endif
    }

    int GetFrequency()
    {
        uint32_t freqReg = ((uint32_t)mRegisters[FREQ2] << 16) | (((uint32_t)mRegisters[FREQ1]) << 8) | mRegisters[FREQ0];
        _ASSERT(mRegisters[CHANNR] == 0);
        uint32_t freqOffset = 0; //mRegisters[CHANNR]*((256 + mRegisters[MDMCFG0])<<(_modem.channel_space_exp & 0x03));
        uint32_t freqHz = ((uint64_t)(freqReg + freqOffset) * (uint32_t)GetOscFrequency()) >> 16;
        return freqHz;
    }

    int GetDataRate()
    {
        int DRATE_M = mRegisters[MDMCFG3];
        int DRATE_E = mRegisters[MDMCFG4] & 0x0f;
        uint64_t rate = ((uint64_t)((256 + DRATE_M) << (DRATE_E)) * GetOscFrequency()) >> 28ULL;
        return rate;
    }

    void SetDataRate(int bps)
    {
        // dataratecalc.js
        uint32_t t = uint32_t( (uint64_t(bps) << 28ULL) / GetOscFrequency() );
        int Ebase = t/511;
        int E = 0, Ev = 1;

        while (Ebase > 0)
        {
          Ebase >>= 1;
          Ev <<= 1;
          E++;
        }

        int M = (t + Ev/2) / Ev - 256;

        _ASSERT(E >= 0 && E <= 15 && M >= 0 && M < 256);
        mRegisters[MDMCFG3] = M;
        mRegisters[MDMCFG4] = (mRegisters[MDMCFG4] & 0xf0) | E;

        CCc1101Spi::Write(MDMCFG3, mRegisters[MDMCFG3]);
        CCc1101Spi::Write(MDMCFG4, mRegisters[MDMCFG4]);
    }

    int GetBandwidth()
    {
        int CHANBW_M = (mRegisters[MDMCFG4] >> 4) & 3;
        int CHANBW_E = mRegisters[MDMCFG4] >> 6;
        return (GetOscFrequency() / 8 / (4 + CHANBW_M)) >> CHANBW_E;
    }

    void SetBandwidth(int bw)
    {
        const int root2[] = {0, 1, 2, 2, 3, 3, 3, 3};
        int t = GetOscFrequency() / 8 / bw;
        int e = root2[min(t/12, 7)];
        e = min(max(0, e), 3);
        int m = (t >> e) - 4;
        m = min(max(0, e), 3);

        _ASSERT(m >= 0 && m <= 3 && e >= 0 && e <= 3);
        mRegisters[MDMCFG4] &= 0x0f;    // eemm....
        mRegisters[MDMCFG4] |= m << 4;
        mRegisters[MDMCFG4] |= e << 6;

        CCc1101Spi::Write(MDMCFG4, mRegisters[MDMCFG4]);
    }

    int GetGain()
    {
        int MAX_LNA_GAIN = (mRegisters[AGCCTRL2]>>3) & 7;
        return -17*MAX_LNA_GAIN/7;
    }

    void SetGain(int gain)
    {
        int MAX_LNA_GAIN = -(gain*7-3)/17;
        MAX_LNA_GAIN = max(0, min(MAX_LNA_GAIN, 7));
        mRegisters[AGCCTRL2] &= 0b11000111;
        mRegisters[AGCCTRL2] |= MAX_LNA_GAIN << 3;
        CCc1101Spi::Write(AGCCTRL2, mRegisters[AGCCTRL2]);
    }

    int GetOutputPower()
    {
      return mRegisterPower;
    }

    void SetOutputPower(int power)
    {
        _ASSERT(power > 0 && power < 256);
        mRegisterPower = power;
        // ASK uses two entries, first power for zero logic level and second power for logic one
        uint8_t table[2] = {0, (uint8_t)power};
        CCc1101Spi::Write(PATABLE, table, 2);
    }

    int UserStrobe(int code)
    {
        return CCc1101Spi::Write(code); // returns status code
    }

    void UserWrite(int address, int value)
    {
        CCc1101Spi::Write(address, value);        
    }

    int UserRead(int address)
    {
        return CCc1101Spi::Read(address);
    }
};
