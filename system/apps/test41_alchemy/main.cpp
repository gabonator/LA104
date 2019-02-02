#include <library.h>
#include <algorithm>

#include "shapes.h"
#include "Menu.h"
#include "json.h"

//#include "lcd.h"
#include "cc1101.h"

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
        static CCc1101Config* _this;
        static long _lTimeout;
        static bool _check;
        _lTimeout = BIOS::SYS::GetTick() + 1500;
        _check = true;
        _this = this;
        
        BIOS::DBG::Print("CONFIG START");
        mDeviceModem.Reset();
        
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
        });
        
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
        /*
        int MAX_LNA_GAIN = (mRegisters[AGCCTRL2]>>3) & 7;
        MAX_LNA_GAIN += d;
        //MAX_LNA_GAIN = min(max(0, MAX_LNA_GAIN), 7);
        mRegisters[AGCCTRL2] &= ~0b00111000;
        mRegisters[AGCCTRL2] |= MAX_LNA_GAIN << 3;
        mDeviceModem.Write(AGCCTRL2, mRegisters[AGCCTRL2]);
         */
    }
   
    void DeltaBandwidth(int d)
    {
        /*
        int CHANBW = mRegisters[MDMCFG4] >> 4;
        CHANBW += d;
        CHANBW = min(max(0, CHANBW), 15);
        mRegisters[MDMCFG4] &= 0x0f;
        mRegisters[MDMCFG4] |= CHANBW << 4;
        mDeviceModem.Write(MDMCFG4, mRegisters[MDMCFG4]);
         */
    }
};

class CStorage
{
public:
    CCc1101Config mModemConfig;
} mStorage;

class CMenuMain : public CTopMenu
{
public:
    virtual TItem GetItem(int i)
    {
        switch (i)
        {
            case 0: return TItem{"Alchemy", TItem::Static};
            case 1: return TItem{"Modem", TItem::Default};
            case 2: return TItem{"Meas", TItem::Default};
            case 3: return TItem{"Filter", TItem::Default};
            case 4: return TItem{"Modify", TItem::Default};
            case 5: return TItem{"Play", TItem::Default};
            case 6: return TItem{"Analyse", TItem::Default};
            default: return TItem{nullptr, TItem::None};
        }
    }
};

class CLayoutPageCc1101 : public CWnd
{
    CRect mRcContent;
    CRect mRcPins;
    CRect mRcSignal;
    int mRow{0};
    
    void DrawStatusPage(const CRect& rcContent)
    {
        GUI::Background(rcContent, RGB565(404040), RGB565(101010));
        
        int x, y;
        int _x = 8, _y = 40;
        x = _x; y = _y;
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "Status: ");
        x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGBTRANS, mStorage.mModemConfig.mOnline ? "Online" : "Offline");
        _y += 16; x = _x; y = _y;
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "Config: ");
        if (HasFocus() && mRow == 0)
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
            x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(ffffff), "KEELOQ01.CFG");
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
        } else {
            //x += BIOS::LCD::Draw(x, y, RGB565(b0b0b0), RGBTRANS, CShapes_sel_left);
            x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGBTRANS, "KEELOQ01.CFG");
            //x += BIOS::LCD::Draw(x, y, RGB565(b0b0b0), RGBTRANS, CShapes_sel_right);
        }
        
        _y += 16; x = _x; y = _y;
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "Modulation: ");
        x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGBTRANS, mStorage.mModemConfig.GetModulation());
        _y += 16; x = _x; y = _y;
        
        int32_t freq = mStorage.mModemConfig.GetFrequency();
        int freqDecimals = freq / 1000000;
        int freqFraction = freq % 1000000;
        
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "Center freq: ");
        if (HasFocus() && mRow == 1)
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
            x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "%d.%02d MHz", freqDecimals, freqFraction / 10000);
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
        } else
        {
            x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGBTRANS, "%d.%02d MHz", freqDecimals, freqFraction / 10000);
        }
        _y += 16; x = _x; y = _y;
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "Bandwidth: ");
        /*if (HasFocus() && mRow == 2)
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
            x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "%d kHz", mStorage.mModemConfig.GetBandwidth() / 1000);
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
        } else*/
        {
            x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGBTRANS, "%d kHz", mStorage.mModemConfig.GetBandwidth() / 1000);
        }
        _y += 16; x = _x; y = _y;
        int rate = mStorage.mModemConfig.GetDataRate();
        int rateDecimals = rate / 1000;
        int rateFraction = rate % 1000;
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "Data rate: ");
        x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGBTRANS, "%d.%d kbps", rateDecimals, rateFraction/100);
        _y += 16; x = _x; y = _y;
        x += BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGBTRANS, "Attenuation: ");
        if (HasFocus() && mRow == 3)
        {
            x -= 8;
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
            x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "%d dB", mStorage.mModemConfig.GetGain());
            x += BIOS::LCD::Draw(x, y, RGB565(ffffff), RGBTRANS, CShapes_sel_right);
        } else
        {
            x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGBTRANS, "%d dB", mStorage.mModemConfig.GetGain());
        }
    }
    
    void DrawPinsPage(const CRect& rcPins)
    {
        int _x, _y;
        _x = rcPins.left + 8;
        _y = 40;

        GUI::Background(rcPins, RGB565(505050), RGB565(202020));
        BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "P1: MOSI");
        _y += 16;
        BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "P2: /CS");
        _y += 16;
        BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "P3: SCK");
        _y += 16;
        BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "P4: GDO0");
        _y += 16;
        BIOS::LCD::Print(_x, _y, RGB565(b0b0b0), RGBTRANS, "CH1: MISO");
        _y += 16;
    }

    void DrawSignalPage(const CRect& rcSignal)
    {
        CRect rcSignalBar = rcSignal;
        rcSignalBar.top -= 14;
        rcSignalBar.bottom = rcSignal.top;
        
        GUI::Background(rcSignalBar, RGB565(404040), RGB565(101010));

        CRect rcSignalDelim = rcSignalBar;
        rcSignalDelim.top = rcSignalDelim.bottom - 1;
        rcSignalDelim.bottom = rcSignalDelim.top + 1;
        BIOS::LCD::Bar(rcSignalDelim, RGB565(b0b0b0));
        
        int x = rcSignalBar.left;
        int y = rcSignalBar.top;
        x += BIOS::LCD::Print(x, y, RGB565(404040), RGB565(b0b0b0), " Received signal");
        x += BIOS::LCD::Draw( x, y, RGB565(b0b0b0), RGBTRANS, CShapes_tab_right);
        
        BIOS::LCD::Bar(rcSignal, RGB565(404040));
    }

    virtual void OnPaint() override
    {
        // TODO: rects into create
        mRcContent = m_rcClient;
        mRcContent.bottom = m_rcClient.bottom-40-14;
        mRcContent.right = mRcContent.right - 100;
        DrawStatusPage(mRcContent);
        
        mRcSignal = CRect(m_rcClient.left, m_rcClient.bottom-40, m_rcClient.right, m_rcClient.bottom);
        DrawSignalPage(mRcSignal);
        
        mRcPins = CRect(mRcContent.right, m_rcClient.top, m_rcClient.right, mRcContent.bottom+14-1);
        DrawPinsPage(mRcPins);
    }
    
    virtual void OnKey(ui16 nKey) override
    {
        if (nKey == BIOS::KEY::Left)
        {
            switch (mRow)
            {
                case 0: break;
                case 1: mStorage.mModemConfig.DeltaFreq(-40); break;
                //case 2: mStorage.mModemConfig.DeltaBandwidth(+1); break;
                case 3: mStorage.mModemConfig.DeltaGain(+1); break;
            }
            DrawStatusPage(mRcContent);
            return;
        }
        
        if (nKey == BIOS::KEY::Right)
        {
            switch (mRow)
            {
                case 0: break;
                case 1: mStorage.mModemConfig.DeltaFreq(+40); break;
                //case 2: mStorage.mModemConfig.DeltaBandwidth(-1); break;
                case 3: mStorage.mModemConfig.DeltaGain(-1); break;
            }
            DrawStatusPage(mRcContent);
            return;
        }
        
        if (nKey == BIOS::KEY::Up)
        {
            if (mRow > 0)
            {
                mRow--;
                DrawStatusPage(mRcContent);
                return;
            }
        }
        if (nKey == BIOS::KEY::Down)
        {
            if (mRow < 3)
            {
                mRow++;
                DrawStatusPage(mRcContent);
                return;
            }
        }
        CWnd::OnKey(nKey);
    }
    
    void DeltaFreq(int d)
    {
        
    }
};

class CSubmenuModem : public CSubMenu
{
public:
    virtual TItem GetItem(int i)
    {
        switch (i)
        {
            case 0: return TItem{"CC1101", TItem::Default};
            case 1: return TItem{"Infra", TItem::Default};
            case 2: return TItem{"Raw", TItem::Default};
            default: return TItem{nullptr, TItem::None};
        }
    }
};

class CLayoutModem : public CWnd
{
    CSubmenuModem mMenu;
    CLayoutPageCc1101 mRadio;

public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
//        SetScrollArea(20, 40);
//        Lcd_Control(0x36);
//        Lcd_Data(16);
        mDeviceModem.Init();
        mStorage.mModemConfig.Load((char*)"");

        BIOS::GPIO::PinMode(BIOS::GPIO::P4, BIOS::GPIO::Input);
        
        CWnd::Create(pszId, dwFlags, rc, pParent);
        
        CRect rcMenu(rc);
        rcMenu.bottom = rcMenu.top + 16;
        
        mMenu.Create("ModemMenu", CWnd::WsVisible, rcMenu, this);
        
        CRect rcContent(rc);
        rcContent.top = rcMenu.bottom;
        
        mRadio.Create("Radio", CWnd::WsVisible | CWnd::WsTick, rcContent, this);
    }
    
    virtual void WindowMessage(int nMsg, int nParam)
    {
        if (nMsg == CWnd::WmTick)
        {
            enum TLast {Z, L, H, LH};
            int current = BIOS::GPIO::DigitalRead(BIOS::GPIO::P4);
            static int x = BIOS::LCD::Width-1;
            static enum TLast last = L;
            static int counter = 0;
            static int highs = 0;
            if (last != LH)
            {
              if (current == 0 && last == H)
                last = LH;
              if (current == 1 && last == L)
                last = LH;
            }
            if (current)
                highs++;
            if (counter++ < 80)
              return;
            counter = 0;
          
            uint16_t pixBuf[40];
            for (int i=0; i<40; i++)
                pixBuf[i] = RGB565(404040);

            if (last == LH)
            {
                int c = highs < 40 ? RGB565(b0b0b0) : RGB565(ffffff);
                for (int i=5;i<35; i++)
                    pixBuf[i] = c;
            } else if (last == L)
                pixBuf[5] = RGB565(ffffff);
            else if (last == H)
                pixBuf[34] = RGB565(ffffff);
            
            BIOS::LCD::BufferBegin(CRect(x, BIOS::LCD::Height-40, x+1, BIOS::LCD::Height));
            BIOS::LCD::BufferWrite(pixBuf, COUNT(pixBuf));
            if (--x <= 0)
                x = BIOS::LCD::Width-1;

            last = current == 0 ? L : H;
            highs = 0;
        }
        CWnd::WindowMessage(nMsg, nParam);
    }

};

class CApplication : public CWnd
{
    CMenuMain mMenu;
    CLayoutModem mLayoutModem;

public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        CRect rcMenu(rc);
        rcMenu.bottom = rcMenu.top + 14;
        
        mMenu.Create("MainMenu", CWnd::WsVisible, rcMenu, this);
        
        CRect rcContent(rc);
        rcContent.top = rcMenu.bottom;
        
        mLayoutModem.Create("Modem", CWnd::WsVisible | CWnd::WsTick, rcContent, this);
        mMenu.SetFocus();
        SetTimer(100);
    }

    virtual void OnPaint() override
    {
    }
    
    
    virtual void OnTimer() override
    {
        BIOS::LCD::Draw( 2, 0, RGB565(808080), RGBTRANS, CShapes_dotout);
        BIOS::LCD::Draw( 2, 0, RGB565(00b000), RGBTRANS, CShapes_dot);
    }
};

CApplication app;

#ifndef __APPLE__
__attribute__((__section__(".entry")))
#endif
int _main(void)
{ 
    app.Create("", CWnd::WsVisible | CWnd::WsTick, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
    app.WindowMessage( CWnd::WmPaint );
    
    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
    {
        if (key != BIOS::KEY::None)
            app.WindowMessage(CWnd::WmKey, key);
        app.WindowMessage(CWnd::WmTick);
    }
    
    app.Destroy();
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

