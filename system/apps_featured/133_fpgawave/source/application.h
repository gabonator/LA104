#include "../../../os_host/source/framework/BufferedIo.h"
#include "../../../os_host/source/framework/Serialize.h"
#ifndef __APPLE__
#include "../../../os_host/source/framework/Serialize.cpp"
#endif
uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

#ifdef __APPLE__
class CTestBenchIo
{
    bool _cs{false};
    bool _sck{false};
    bool _miso{false};
    bool _mosi{false};
    bool _clk{false};
    enum {PullUp, PullDown, Float} _misoInput{Float};
    
public:
    void Init()
    {
    }
    void Deinit()
    {
    }
    
    bool cs()
    {
        return _cs;
    }
    void cs(bool l)
    {
        _cs = l;
    }
    bool sck()
    {
        return _sck;
    }
    void sck(bool l)
    {
        _sck = l;
    }
    bool miso()
    {
        if (_cs != 1)
            return _sck;
        if (_misoInput == PullUp)
            return true;
        if (_misoInput == PullDown)
            return false;
        return _miso;
    }
    void miso(bool l)
    {
        _miso = l;
    }
    bool mosi()
    {
//        if ((rand()%100)>60)
//            _mosi = rand() & 1;
//        return _mosi * (1-_cs);
        return _mosi;
    }
    void mosi(bool l)
    {
        _mosi = l;
    }
    bool clk()
    {
        return _clk;
    }
    void clk(bool l)
    {
        _clk = l;
    }
    void misoPullUp()
    {
        _misoInput = PullUp;
    }
    void misoPullDown()
    {
        _misoInput = PullDown;
    }
    void misoFloat()
    {
        _misoInput = Float;
    }
};
#else
class CTestBenchGpio
{
public:
    void WritePin(uint32_t base, int i, bool value)
    {
        uint32_t* pODR = &((uint32_t*)base)[3];
        if (value)
            *pODR |= 1<<i;
        else
            *pODR &= ~(1<<i);
    }
    void PinMode(uint32_t base, int i, uint32_t state)
    {
        int nPinPos = i & 7;
        uint32_t dwMask = ~(0xf << (nPinPos*4));
        uint32_t dwBits = state << (nPinPos*4);

        if (i < 8)
        {
            uint32_t* pCRL = (uint32_t*)base;
            *pCRL = (*pCRL & dwMask) | dwBits;
        } else
        {
            uint32_t* pCRH = (uint32_t*)(base + 4);
            *pCRH = (*pCRH & dwMask) | dwBits;
        }
    }
    uint8_t GetPinMode(uint32_t base, int i)
    {
        int nPinPos = i & 7;
        uint32_t dwMask = ~(0xf << (nPinPos*4));

        if (i < 8)
        {
            uint32_t* pCRL = (uint32_t*)base;
            return ((*pCRL) >> (nPinPos*4)) & 0x0f;
        } else
        {
            uint32_t* pCRH = (uint32_t*)(base + 4);
            return ((*pCRH) >> (nPinPos*4)) & 0x0f;
        }
    }

    bool ReadPin(uint32_t base, int i)
    {
        return (((uint32_t*)base)[2] >> i) & 1;
    }
};

class CTestBenchIo : public CTestBenchGpio
{
    enum {
        StateInput = 0,           // ..00
        StateOutput10Mhz = 1,     // ..01
        StateOutput2Mhz = 2,      // ..10
        StateOutput50Mhz = 3,     // ..11

        StateInputAnalog = 0<<2,
        StateInputFloating = 1<<2,
        StateInputPull = 2<<2,

        StateOutputPushPull = 0<<2,
        StateOutputOpenDrain = 1<<2,
        StateOutputFunctionPushPull = 2<<2,
        StateOutputFunctionOpenDrain = 3<<2,
    };

    enum {
        PERIPH_BASE = ((uint32_t)0x40000000),
        APB2PERIPH_BASE = (PERIPH_BASE + 0x10000),
        PA = (APB2PERIPH_BASE + 0x0800),
        PB = (APB2PERIPH_BASE + 0x0C00),
        
        Output = StateOutput10Mhz | StateOutputPushPull,
        InputFloating = StateInput | StateInputFloating,
        InputPull = StateInput | StateInputPull,
        //InputPullDown = StateInput | StateInputPull
    };
    
    struct SPI_TypeDef {
        volatile uint16_t CR1;
    };
    
    uint32_t mSafeSpi3Cr1;
    uint8_t mPinState[5];
    uint8_t mPinMode[5];
    uint8_t mSafeCfgr4;
public:
    
    void Init()
    {
        #define CFGR_BYTE4_ADDRESS (0x40021007)
        #define RCC_MCO_NoClock ((uint8_t)0x00)

        #define PERIPH_BASE           ((uint32_t)0x40000000)
        #define APB1PERIPH_BASE       PERIPH_BASE
        #define SPI3_BASE             (APB1PERIPH_BASE + 0x3C00)
        #define SPI3                  ((SPI_TypeDef *) SPI3_BASE)
        #define CR1_SPE_Set          ((uint16_t)0x0040)

        mSafeSpi3Cr1 = SPI3->CR1;
        mSafeCfgr4 = *(uint8_t*)CFGR_BYTE4_ADDRESS;
        mPinMode[0] = GetPinMode(PA, 8);
        mPinMode[1] = GetPinMode(PB, 3);
        mPinMode[2] = GetPinMode(PB, 4);
        mPinMode[3] = GetPinMode(PB, 5);
        mPinMode[4] = GetPinMode(PB, 7);
//        mPinState[0] = ReadPin(PA, 8);
//        mPinState[1] = ReadPin(PB, 3);
//        mPinState[2] = ReadPin(PB, 4);
//        mPinState[3] = ReadPin(PB, 5);
//        mPinState[4] = ReadPin(PB, 7);
        
        SPI3->CR1 &= ~CR1_SPE_Set;
        *(uint8_t*)CFGR_BYTE4_ADDRESS = RCC_MCO_NoClock;

        // clk
        PinMode(PA, 8, Output);
        WritePin(PA, 8, false);
        // cs
        PinMode(PB, 7, Output);
        WritePin(PB, 7, true);
        // sck
        PinMode(PB, 3, Output);
        WritePin(PB, 3, false);
        // mosi
        PinMode(PB, 5, Output);
        WritePin(PB, 5, false);
        // miso
        PinMode(PB, 4, InputFloating);
        WritePin(PB, 4, false);
    }
    
    void Deinit()
    {
        PinMode(PA, 8, mPinMode[0]);
        PinMode(PB, 3, mPinMode[1]);
        PinMode(PB, 4, mPinMode[2]);
        PinMode(PB, 5, mPinMode[3]);
        PinMode(PB, 7, mPinMode[4]);
        SPI3->CR1 = mSafeSpi3Cr1;
        *(uint8_t*)CFGR_BYTE4_ADDRESS = mSafeCfgr4;
    }
    
    bool cs()
    {
        return ReadPin(PB, 7);
    }
    void cs(bool l)
    {
        WritePin(PB, 7, l);
    }
    bool sck()
    {
        return ReadPin(PB, 3);
    }
    void sck(bool l)
    {
        WritePin(PB, 3, l);
    }
    bool miso()
    {
        return ReadPin(PB, 4);
    }
    void miso(bool l)
    {
        WritePin(PB, 4, l);
    }
    bool mosi()
    {
        return ReadPin(PB, 5);
    }
    void mosi(bool l)
    {
        WritePin(PB, 5, l);
    }
    bool clk()
    {
        return ReadPin(PA, 8);
    }
    void clk(bool l)
    {
        WritePin(PA, 8, l);
    }
    void misoPullUp()
    {
        PinMode(PB, 4, InputPull);
        WritePin(PB, 4, true);
    }
    void misoPullDown()
    {
        PinMode(PB, 4, InputPull);
        WritePin(PB, 4, false);
    }
    void misoFloat()
    {
        PinMode(PB, 4, InputFloating);
    }
};

#endif

class CTestBench : public CTestBenchIo
{
public:
    enum Level {
        Undefined,
        Low,
        High,
        Z,
        Transition
    };
    
private:
    bool mLsbFirst{false};
    void (*mHandler)(Level*){nullptr};
    void (*mHandlerMsg)(const char*){nullptr};

public:
    void setHandler(void (*handler)(Level*), void (*handlerMsg)(const char*))
    {
        mHandler = handler;
        mHandlerMsg = handlerMsg;
    }
    void spiBegin(bool lsb)
    {
        mLsbFirst = lsb;
        cs(1);
        clk(0);
        sck(0);
        mosi(0);
        misoFloat();
    }

    void spiEnd()
    {
        
    }

    void eval()
    {
        Scan();
        Scan();
        Scan();
        Scan();
    }
    
    void Scan()
    {
        Level lcs{Undefined}, lmiso{Undefined}, lmosi{Undefined}, lsck{Undefined}, lclk{Undefined};
        lmiso = miso() ? High : Low;
        if (lmiso == High)
            misoPullDown();
        else
            misoPullUp();
        for (volatile int temp = 0; temp < 100; temp++);

        for (int i=0; i<20; i++)
        {
            Level bcs = cs() ? High : Low;
            if (lcs == Undefined)
                lcs = bcs;
            else if (lcs != bcs)
                lcs = Transition;

            Level bmosi = mosi() ? High : Low;
            if (lmosi == Undefined)
                lmosi = bmosi;
            else if (lmosi != bmosi)
                lmosi = Transition;

            Level bsck = sck() ? High : Low;
            if (lsck == Undefined)
                lsck = bsck;
            else if (lsck != bsck)
                lsck = Transition;

            Level bclk = clk() ? High : Low;
            if (lclk == Undefined)
                lclk = bclk;
            else if (lclk != bclk)
                lclk = Transition;

            Level bmiso = miso() ? High : Low;
            if (bmiso != lmiso)
                lmiso = Z;
            
            for (volatile int temp = 0; temp < 50; temp++);
        }
        if (mHandler)
        {
            _ASSERT(lmiso != Undefined && lclk != Undefined && lsck != Undefined && lmosi != Undefined);
            Level levels[5] = {lclk, lsck, lcs, lmosi, lmiso};
            mHandler(levels);
        }
    }

    void tick()
    {
        const int oversampling = 4;

        for (int i=0; i<oversampling; i++)
        {
            clk(!clk());
            eval();
        }
    }
    
    uint8_t spiTick(uint8_t b)
    {
        mosi(b);
        sck(0);
        tick();
        sck(1);
        tick();
        return miso();
    }
    
    uint8_t spiTransfer(uint8_t d)
    {
        uint8_t aux = 0;
        if (mLsbFirst)
        {
          for (int i=0; i<8; i++)
          {
              aux >>= 1;
              aux |= spiTick(d & 1) ? 128 : 0;
              d >>= 1;
          }
        } else {
          for (int i=0; i<8; i++)
          {
              aux <<= 1;
              aux |= spiTick(d & 128);
              d <<= 1;
          }
        }
        return aux;
    }
    void printError(const char* msg)
    {
        if (mHandlerMsg)
            mHandlerMsg(msg);
    }
    void printLabel(const char* msg)
    {
        if (mHandlerMsg)
            mHandlerMsg(msg);
    }
};

bool main_verify(CTestBench& test)
{
    test.spiBegin(false);
    test.cs(1);
    test.tick();
    test.tick();
    test.tick();
    test.cs(0);
    test.tick();
    test.tick();
    test.tick();
    test.printLabel("spi write1");
    if (test.spiTransfer(0xf0) != 1)
        test.printError("should be 0x01");
    test.printLabel("spi write2");
    if (test.spiTransfer(0xf1) != 2)
        test.printError("should be 0x02");
    test.printLabel("spi write3");
    if (test.spiTransfer(0xf2) != 8)
        test.printError("should be 0x08"); // 0x03!
    test.eval();
    test.cs(1);
    test.eval();
    test.printLabel("release");
    test.tick();
    test.tick();
    test.tick();
    test.spiEnd();
    return true;
}

class DiskNotify
{
    uint32_t* mpFlashWriteRange{nullptr};
    BIOS::FAT::TFindFile mFileFpga{0};
    BIOS::FAT::TFindFile mFileVerify{0};
    bool mFpgaNewer{false};
    bool mVerifyNewer{false};
    
public:
    DiskNotify()
    {
#ifndef __APPLE__
        mpFlashWriteRange = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::FlashWriteRange);
        _ASSERT(mpFlashWriteRange)
        mpFlashWriteRange[0] = -1;
        mpFlashWriteRange[1] = -1;
#endif
    }
    
    int compare(const char* s1, const char* s2)
    {
        auto lcase = [](char a) -> char{
            if (a >= 'A' && a <= 'Z')
                return a - 'A' + 'a';
            return a;
        };
        while(*s1 && lcase(*s1) == lcase(*s2))
        {
            s1++;
            s2++;
        }
        return lcase(*s1) - lcase(*s2);
    }
    bool FindFile(const char* name, BIOS::FAT::TFindFile& found)
    {
        BIOS::FAT::EResult result = BIOS::FAT::OpenDir((char*)"");
        _ASSERT(result == BIOS::FAT::EOk);
        BIOS::FAT::TFindFile file;
        while (BIOS::FAT::FindNext(&file) == BIOS::FAT::EOk)
        {
            if (compare(file.strName, name) == 0)
            {
                memcpy(&found, &file, sizeof(BIOS::FAT::TFindFile));
                return true;
            }
        }
        return false;
    }
    
    void CheckNew()
    {
        // Did someone write to disk?
        if (mpFlashWriteRange)
        {
            if (mpFlashWriteRange[0] == -1 && mpFlashWriteRange[1] == 0)
                return;
            while (mpFlashWriteRange[0] != -1 || mpFlashWriteRange[1] != 0)
            {
                BIOS::SYS::DelayMs(30);
                mpFlashWriteRange[0] = -1;
                mpFlashWriteRange[1] = 0;
            }
            BIOS::SYS::DelayMs(200);
        }

        BIOS::FAT::TFindFile fileTemp1;
        if (FindFile("bench.fpg", fileTemp1))
            mFpgaNewer = CheckNewer(mFileFpga, fileTemp1);

        BIOS::FAT::TFindFile fileTemp2;
        if (FindFile("bench.vlf", fileTemp2))
            mVerifyNewer = CheckNewer(mFileVerify, fileTemp2);
    }
    
    bool CheckNewer(BIOS::FAT::TFindFile& oldf, const BIOS::FAT::TFindFile& newf)
    {
        if (oldf.nDate == newf.nDate && oldf.nTime == newf.nTime)
            return false;
        memcpy(&oldf, &newf, sizeof(BIOS::FAT::TFindFile));
        return true;
    }
    
    bool IsFpgaNewer()
    {
        if (!mFpgaNewer)
            return false;
        mFpgaNewer = false;
        return true;
    }

    bool IsVerifyNewer()
    {
        if (!mVerifyNewer)
            return false;
        mVerifyNewer = false;
        return true;
    }
    void GetFpgaTimestamp(char* str)
    {
        sprintf(str, "%02d/%02d/%02d %02d:%02d", (mFileFpga.nDate>>9)-20, (mFileFpga.nDate>>5)&15, (mFileFpga.nDate)&31, (mFileFpga.nTime>>11), (mFileFpga.nTime>>5)&63);
    }
    void GetFpgaName(char* str)
    {
        strcpy(str, mFileFpga.strName);
    }
    void GetVerifyTimestamp(char* str)
    {
        sprintf(str, "%02d/%02d/%02d %02d:%02d", (mFileVerify.nDate>>9)-20, (mFileVerify.nDate>>5)&15, (mFileVerify.nDate)&31, (mFileVerify.nTime>>11), (mFileVerify.nTime>>5)&63);
    }
    void GetVerifyName(char* str)
    {
        strcpy(str, mFileVerify.strName);
    }
};


class CFpga
{
public:
    bool Load(char* name)
    {
        return DoFlashing(name);
    }
    
private:
    bool DoFlashing(char* name)
    {
        CBufferedReader fw;
        if ( !fw.Open(name) )
            return false;

        int count = 0xb720;
        volatile uint16_t* ptrSpi = (uint16_t*)0x40003C0C; // SPI3_DR_ADDRESS
        volatile uint32_t* bit_SPI3_txe = (volatile uint32_t *)0x42078104;
        volatile uint32_t* cdone = (volatile uint32_t*)0x4222010c;
        _ASSERT(*cdone == 1);
        int counter; // volatile?

        // reset
        *((volatile uint32_t*)0x4221819c) = 0;
        *((volatile uint32_t*)0x42218198) = 0;
        counter = 7;
        while (counter--);

        *((volatile uint32_t*)0x42218198) = 1;
        counter = 0x1baf;
        while (counter--);
        BIOS::SYS::DelayMs(5);
        _ASSERT(*cdone == 0);
        
        CStream spiStream((uint8_t*)ptrSpi, 1);
        while (count--)
        {
          fw >> spiStream;
          while (bit_SPI3_txe == 0);
        }

        *((volatile uint32_t*)0x4221819c) = 1;

        fw.Close();
        BIOS::SYS::DelayMs(20);
        _ASSERT(*cdone == 1);
        return !!*cdone;
    }

};

class CSimulator
{
public:
    bool Load(char* name)
    {
        return false;
    }
};

class CApplication : public CWnd
{
    DiskNotify mNotify;
    enum ModuleStatus {
        None,
        Loaded,
        Error,
        Loading1,
        Loading2,
        Loading3,
        Loading4,
        Loading5,
        Loading6,
        Loading7,
    };
    struct ModuleInfo {
        char timestamp[20];
        char name[20];
        ModuleStatus status{None};
    };
    ModuleInfo mModuleFpga;
    ModuleInfo mModuleVerify;
    enum Level {
        Undefined, // out of 2 bit range
        Low,
        High,
        Z,
        Transition
    };
    
    CFpga mFpga;
    CSimulator mSimulator;
    uint16_t mWaveform[1024];
    int mWaveformScale{1};
    int mWaveformShift{0};
    bool mMeasure{false};
    CTestBench mTestBench;
    
    struct TLabel {
        char msg[32];
        int index;
        int row;
    };
    TLabel mLabelsData[10];
    CArray<TLabel> mLabels;
    
public:
    
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
        BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);

        mLabels.Init(mLabelsData, COUNT(mLabelsData));
        CWnd::Create(pszId, dwFlags, rc, pParent);
        SetTimer(100);
        for (int i=0; i<COUNT(mWaveform); i++)
        {
            int v1 = ((i/10)&1) ? Level::High : Level::Low;
            int v2 = ((i/17)&1) ? Level::High : Level::Low;
            int v3 = ((i/31)&1) ? Level::Low : Level::High;
            int v4 = ((i/57)&1) ? Level::High : Level::Low;
            int v5 = (i > 80) ? Level::High : Level::Z;
            mWaveform[i] = v1 | (v2<<3) | (v3<<6) | (v4<<9) | (v5<<12);
        }
    }

	void OnKey(int key) override
	{
        if (key == BIOS::KEY::Left)
        {
            if (mWaveformShift > 0)
            {
                mWaveformShift = max(0, mWaveformShift-4);
                DrawWave();
            }
        }
        if (key == BIOS::KEY::Right)
        {
            mWaveformShift += 4;
            DrawWave();
        }
        if (key == BIOS::KEY::Enter)
        {
            mMeasure = true;
        }
	}

    
    void OnPaint() override
    {
		BIOS::LCD::Bar(m_rcClient, RGB565(404040));
        
        CRect rc1(m_rcClient);
        rc1.bottom = 14;
        GUI::Background(rc1, RGB565(4040b0), RGB565(404040));

        BIOS::LCD::Print(8, 0, RGB565(ffffff), RGBTRANS, "FPGA Test bench");

        const int labelsY = 40;
        const int labelWidth = 64;
        const int labelHeight = 16;
        const int labelSpacing = 4;
        int y = labelsY;
        SignalHeading(CRect(0, y, labelWidth, y+labelHeight), "clk", false);
        y += labelHeight + labelSpacing;
        SignalHeading(CRect(0, y, labelWidth, y+labelHeight), "sck", false);
        y += labelHeight + labelSpacing;
        SignalHeading(CRect(0, y, labelWidth, y+labelHeight), "cs", false);
        y += labelHeight + labelSpacing;
        SignalHeading(CRect(0, y, labelWidth, y+labelHeight), "mosi", false);
        y += labelHeight + labelSpacing;
        SignalHeading(CRect(0, y, labelWidth, y+labelHeight), "miso", true);
        y += labelHeight;
        
        
        //DrawFooter(mModuleFpga, mModuleVerify);
        DrawWave();
    }
    
    void DrawWave()
    {
        const int labelsY = 40;
        const int labelWidth = 64;
        const int labelHeight = 16;
        const int labelSpacing = 4;

        BIOS::LCD::BufferBegin(CRect(labelWidth, labelsY, BIOS::LCD::Width, labelsY+4*labelSpacing+5*labelHeight));
        uint16_t lineSkip[16];
        for (int i=0; i<labelSpacing; i++)
            lineSkip[i] = RGB565(404040);
        
        uint16_t Invalid = (Level::Undefined << 0) | (Level::Undefined << 3) | (Level::Undefined << 6) | (Level::Undefined << 9) | (Level::Undefined << 12);
        uint16_t vals = mWaveformShift < COUNT(mWaveform) ? mWaveform[mWaveformShift] : Invalid;
        
        Level o1 = (Level)((vals >> 12) & 7);
        Level o2 = (Level)((vals >> 9) & 7);
        Level o3 = (Level)((vals >> 6) & 7);
        Level o4 = (Level)((vals >> 3) & 7);
        Level o5 = (Level)((vals >> 0) & 7);
        for (int i=0; i<BIOS::LCD::Width-labelWidth; i++)
        {
            uint16_t vals = mWaveformShift+i < COUNT(mWaveform) ? mWaveform[mWaveformShift+i] : Invalid;
            Level l1 = (Level)((vals >> 12) & 7);
            Level l2 = (Level)((vals >> 9) & 7);
            Level l3 = (Level)((vals >> 6) & 7);
            Level l4 = (Level)((vals >> 3) & 7);
            Level l5 = (Level)((vals >> 0) & 7);
            if (i==80)
            {
                int f = 9;
                //l1 = l2 = l3 = l4 = l5 = High;
            }

            DrawLevel(i, o1, l1);
            BIOS::LCD::BufferWrite(lineSkip, labelSpacing);
            DrawLevel(i, o2, l2);
            BIOS::LCD::BufferWrite(lineSkip, labelSpacing);
            DrawLevel(i, o3, l3);
            BIOS::LCD::BufferWrite(lineSkip, labelSpacing);
            DrawLevel(i, o4, l4);
            BIOS::LCD::BufferWrite(lineSkip, labelSpacing);
            DrawLevel(i, o5, l5);
            
            o1 = l1;
            o2 = l2;
            o3 = l3;
            o4 = l4;
            o5 = l5;
        }
        BIOS::LCD::BufferEnd();
        // Annotations
        const int annotationY = labelsY+4*labelSpacing+5*labelHeight+4;
        BIOS::LCD::Bar(CRect(0, annotationY, BIOS::LCD::Width, annotationY+28), RGB565(404040));
        for (int i=0; i<mLabels.GetSize(); i++)
        {
            const TLabel& label = mLabels[i];
            int labelSize = strlen(label.msg)*8+12;
            int x = 64 + label.index - mWaveformShift;
            int y = annotationY + label.row*14;
            if (x+labelSize >= 8 && x < BIOS::LCD::Width-16)
            {
                if (x-3 >= 8 && x-3+8 < BIOS::LCD::Width)
                    BIOS::LCD::Print(x-3, y, RGB565(b0b0b0), RGB565(404040), '\x18');
                x += 12;
                for (int i=0; label.msg[i]; i++)
                {
                    if (x >= 8 && x+8 < BIOS::LCD::Width)
                        BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGB565(404040), label.msg[i]);
                    x += 8;
                }
            }
        }

    }
    void DrawLevel(int x, Level o, Level l)
    {
        uint16_t G = RGB565(b0b0b0), B = RGB565(404040), H = RGB565(ffffff);
        const uint16_t lineHigh[16] = {G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, H};
        const uint16_t lineLow[16] = {H, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B};
        const uint16_t lineTrans[16] = {H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H};
        const uint16_t lineSkip[16] = {B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B};
        const uint16_t lineZ1[16] = {B, B, B, B, B, B, G, B, B, B, B, B, B, B, B, B};
        const uint16_t lineZ2[16] = {B, B, B, B, B, B, B, G, B, B, B, B, B, B, B, B};
        const uint16_t lineZ3[16] = {B, B, B, B, B, B, B, B, G, B, B, B, B, B, B, B};
        const uint16_t lineZ4[16] = {B, B, B, B, B, B, B, G, B, B, B, B, B, B, B, B};
        const uint16_t* lineZall[] = {lineZ1, lineZ2, lineZ3, lineZ4};
        
        if (l == Undefined)
            l = Undefined;
        else if (o != l && l != Z && o != Z)
            l = Transition;
        if (l == Z)
        {
            BIOS::LCD::BufferWrite(lineZall[x & 3], 16);
            return;
        }
        const uint16_t* lines[] = {lineSkip, lineLow, lineHigh, lineZ1, lineTrans};
        BIOS::LCD::BufferWrite(lines[l], 16);
    }
    
    void DrawFooter(const ModuleInfo& mod1, const ModuleInfo& mod2)
    {
        CRect rcFooter(m_rcClient.left, m_rcClient.bottom-32, m_rcClient.right, m_rcClient.bottom);
        //GUI::Background(rcFooter, RGB565(4040b0), RGB565(4040a0));
        
        const uint16_t colorNone[2] = {RGB565(505050), RGB565(505050)};
        const uint16_t colorNormal[2] = {RGB565(4040b0), RGB565(4040a0)};
        const uint16_t colorError[2] = {RGB565(ff4040), RGB565(ff4040)};
        const uint16_t colorHigh1[2] = {RGB565(40b040), RGB565(40b040)};
        const uint16_t colorHigh2[2] = {RGB565(40a050), RGB565(40a050)};
        const uint16_t colorHigh3[2] = {RGB565(409060), RGB565(409060)};
        const uint16_t colorHigh4[2] = {RGB565(408070), RGB565(408070)};
        const uint16_t colorHigh5[2] = {RGB565(407080), RGB565(407080)};
        const uint16_t colorHigh6[2] = {RGB565(406090), RGB565(406090)};
        const uint16_t colorHigh7[2] = {RGB565(4050a0), RGB565(4050a0)};
        const uint16_t* grad[] = {colorNone, colorNormal, colorError, colorHigh1, colorHigh2, colorHigh3, colorHigh4, colorHigh5, colorHigh6, colorHigh7};
        const uint16_t* grad1 = grad[mod1.status];
        const uint16_t* grad2 = grad[mod2.status];
        
        /*
         enum ModuleStatus {
             None,
             Loaded,
             Error,
             Loading1,
             Loading2,
             Loading3,
             Loading4,
             Loading5,
             Loading6,
             Loading7,
         };
         */
        GUI::Background(CRect(rcFooter.left, rcFooter.top, rcFooter.CenterX(), rcFooter.bottom), grad1[0], grad1[1]);

        GUI::Background(CRect(rcFooter.CenterX(), rcFooter.top, rcFooter.right, rcFooter.bottom), grad2[0], grad2[1]);

        BIOS::LCD::Print(8, rcFooter.top+2, RGB565(ffffff), RGBTRANS, mod1.timestamp);
        BIOS::LCD::Print(8, rcFooter.top+2+16, RGB565(ffffff), RGBTRANS, mod1.name);
        BIOS::LCD::Print(rcFooter.CenterX()+8, rcFooter.top+2, RGB565(ffffff), RGBTRANS, mod2.timestamp);
        BIOS::LCD::Print(rcFooter.CenterX()+8, rcFooter.top+2+14, RGB565(ffffff), RGBTRANS, mod2.name);
    }
    
    void SignalHeading(CRect rc, const char* name, bool inOut)
    {
        int labelWidth = strlen(name)*8+16;
        int labelHeight = 14;
        BIOS::LCD::Bar(rc, RGB565(606060));
        BIOS::LCD::Bar(CRect(rc.left, rc.top, rc.right, rc.top+1), RGB565(707070));
        BIOS::LCD::Bar(CRect(rc.left, rc.top, rc.left+1, rc.bottom), RGB565(707070));

//        BIOS::LCD::Bar(CRect(rc.left, rc.bottom, rc.right, rc.bottom+1), RGB565(808080));
//        BIOS::LCD::Bar(CRect(rc.right, rc.top, rc.right+1, rc.bottom+1), RGB565(808080));

        BIOS::LCD::Print(rc.Center().x-labelWidth/2+16, rc.Center().y-labelHeight/2, RGB565(ffffff), RGBTRANS, name);
        BIOS::LCD::Print(rc.left + 8, rc.Center().y-labelHeight/2,
                         inOut ? RGB565(b0ffb0) : RGB565(b0b0ff), RGBTRANS, inOut ? "\x1b" : "\x1a");
    }
    void RedrawButtons()
    {
        int mPos = 7;
        BIOS::LCD::Print(47, 92, mPos == 6 ? RGB565(d0d0d0) : RGB565(404040), mPos == 6 ? RGB565(ffffff) : RGB565(808080), " \x19 Copy ");
        BIOS::LCD::Print(47-1, 92, mPos == 6 ? RGB565(404040) : RGB565(f0f0f0), RGBTRANS, " \x19 Copy ");
        BIOS::LCD::Print(227, 92, mPos == 7 ? RGB565(d0d0d0) : RGB565(404040), mPos == 7 ? RGB565(ffffff) : RGB565(808080), " \x18 Copy ");
        BIOS::LCD::Print(227-1, 92, mPos == 7 ? RGB565(404040) : RGB565(f0f0f0), RGBTRANS, " \x18 Copy ");

        BIOS::LCD::Print(47, 170, mPos == 14 ? RGB565(d0d0d0) : RGB565(404040), mPos == 14 ? RGB565(ffffff) : RGB565(808080), " Run tests ");
        BIOS::LCD::Print(47-1, 170, mPos == 14 ? RGB565(404040) : RGB565(f0f0f0), RGBTRANS, " Run tests ");
    }

    void OnTimer() override
    {
        if (mMeasure)
        {
            mModuleVerify.status = Loading1;
            DrawFooter(mModuleFpga, mModuleVerify);
            mMeasure = false;
            Measure();
            return;
        }
        const ModuleStatus transition[10] = {
            None, Loaded, Error, Loading2, Loading3, Loading4, Loading5, Loading6, Loading7, Loaded
        };

        bool redraw = false;
        redraw |= (transition[mModuleFpga.status] != mModuleFpga.status);
        redraw |= (transition[mModuleVerify.status] != mModuleVerify.status);
        mModuleFpga.status = transition[mModuleFpga.status];
        mModuleVerify.status = transition[mModuleVerify.status];

        mNotify.CheckNew();
        bool fpgaNewer = mNotify.IsFpgaNewer();
        bool verifyNewer = mNotify.IsVerifyNewer();
        if (!fpgaNewer && !verifyNewer)
        {
            if (redraw)
                DrawFooter(mModuleFpga, mModuleVerify);
            return;
        }
        BIOS::SYS::Beep(50);
        if (fpgaNewer)
        {
            mModuleFpga.status = Loading1;
            // try loading!
            mNotify.GetFpgaTimestamp(mModuleFpga.timestamp);
            strcpy(mModuleFpga.name, "Loading...");
            DrawFooter(mModuleFpga, mModuleVerify);
            mNotify.GetFpgaName(mModuleFpga.name);
            if (!mFpga.Load(mModuleFpga.name))
                mModuleFpga.status = Error;
            else
                mMeasure = true;
        }
        if (verifyNewer)
        {
            mModuleVerify.status = Loading1;
            mNotify.GetVerifyTimestamp(mModuleVerify.timestamp);
            strcpy(mModuleFpga.name, "Loading...");
            DrawFooter(mModuleFpga, mModuleVerify);
            mNotify.GetVerifyName(mModuleVerify.name);
            if (!mSimulator.Load(mModuleVerify.name))
                mModuleVerify.status = Error;
            else
                mMeasure = true;
        }
        DrawFooter(mModuleFpga, mModuleVerify);
    }
	
	void Redraw()
	{
	}
    virtual void Destroy()
    {
        CWnd::Destroy();
        BIOS::FAT::SetSharedBuffer(nullptr);
    }
	
    void Measure()
    {
        memset(mWaveform, 0, sizeof(mWaveform));
        static int mWaveIndex;
        static CApplication* pThis;
        mWaveIndex = 0;
        pThis = this;
        static int firstRowMaxX;
        firstRowMaxX = -1;
        
        mTestBench.Init();
        BIOS::SYS::DelayMs(20);

        mTestBench.setHandler([](CTestBench::Level* levels){
            uint16_t vals = (levels[0] << 0) | (levels[1] << 3) | (levels[2] << 6) | (levels[3] << 9) | (levels[4] << 12);
            _ASSERT(mWaveIndex < COUNT(pThis->mWaveform));
            pThis->mWaveform[mWaveIndex++] = vals;
        }, [](const char* msg) {
            if (pThis->mLabels.GetSize() < pThis->mLabels.GetMaxSize())
            {
                TLabel label;
                label.index = mWaveIndex;
                int maxX = mWaveIndex + strlen(msg)*8;
                if (firstRowMaxX < mWaveIndex)
                {
                    label.row = 0;
                    firstRowMaxX = maxX;
                } else {
                    label.row = 1;
                }
                if (strlen(msg) < sizeof(label.msg)-1)
                    strcpy(label.msg, msg);
                else
                {
                    memcpy(label.msg, msg, sizeof(label.msg)-1);
                    label.msg[sizeof(label.msg)-1] = 0;
                }
                pThis->mLabels.Add(label);
            }
        });

        main_verify(mTestBench);
        mTestBench.setHandler(nullptr, nullptr);
        DrawWave();
        
        BIOS::SYS::DelayMs(20);
        mTestBench.Deinit();
    }
};
