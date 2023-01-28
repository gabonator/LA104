#include "../../../os_host/source/framework/BufferedIo.h"
#include "../../../os_host/source/framework/Serialize.h"
#ifndef __APPLE__
#include "../../../os_host/source/framework/Serialize.cpp"
#endif
uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

using namespace BIOS;
#include "file/shapes.h"
#include "file/layout.h"
#include "file/file.h"
#include "screenshot.h"

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
    enum { Label, Error, GroupBegin, GroupEnd } MsgType;
    
private:
    bool mLsbFirst{false};
    void (*mHandler)(Level*){nullptr};
    void (*mHandlerMsg)(int, const char*){nullptr};

public:
    void setHandler(void (*handler)(Level*), void (*handlerMsg)(int msgType, const char*))
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
//        Scan();
//        Scan();
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
            mHandlerMsg(Error, msg);
    }
    void printLabel(const char* msg)
    {
        if (mHandlerMsg)
            mHandlerMsg(Label, msg);
    }
    void groupBegin(const char* msg)
    {
        if (mHandlerMsg)
            mHandlerMsg(GroupBegin, msg);
    }
    void groupEnd(const char* msg = nullptr)
    {
        if (mHandlerMsg)
            mHandlerMsg(GroupEnd, msg);
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
    
    char temp[32];
    int read;
    
    test.groupBegin("write 0xf0");
    read = test.spiTransfer(0xf0);
    sprintf(temp, "read 0x%02x", read);
    test.groupEnd(temp);
    
    test.groupBegin("write 0xf1");
    read = test.spiTransfer(0xf1);
    sprintf(temp, "read 0x%02x", read);
    test.groupEnd(temp);
    
    test.groupBegin("write 0xf2");
    read = test.spiTransfer(0xf2);
    sprintf(temp, "read 0x%02x", read);
    test.groupEnd(temp);

    test.groupBegin("write 0x73");
    read = test.spiTransfer(0x73);
    sprintf(temp, "read 0x%02x", read);
    test.groupEnd(temp);

    test.groupBegin("write 0x48");
    read = test.spiTransfer(0x48);
    sprintf(temp, "read 0x%02x", read);
    test.groupEnd(temp);

    test.groupBegin("write 0xa9");
    read = test.spiTransfer(0xa9);
    sprintf(temp, "read 0x%02x", read);
    test.groupEnd(temp);

    test.groupBegin("write 0xf5");
    read = test.spiTransfer(0x48);
    sprintf(temp, "read 0x%02x", read);
    test.groupEnd(temp);
    
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
    char mFpgaName[16];
    char mVerifyName[16];
    
public:
    DiskNotify()
    {
#ifndef __APPLE__
        mpFlashWriteRange = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::FlashWriteRange);
        _ASSERT(mpFlashWriteRange)
        mpFlashWriteRange[0] = -1;
        mpFlashWriteRange[1] = -1;
#endif
        strcpy(mFpgaName, "bench.fpg");
        strcpy(mVerifyName, "bench.vlf");
    }
    
    void SetFpgaName(char* name)
    {
        strcpy(mFpgaName, name);
        // force reload
        if (mpFlashWriteRange)
            mpFlashWriteRange[0] = 0;
    }

    void SetVerifyName(char* name)
    {
        strcpy(mVerifyName, name);
        // force reload
        if (mpFlashWriteRange)
            mpFlashWriteRange[0] = 0;
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
//        BIOS::DBG::Print("p");
        BIOS::FAT::EResult result = BIOS::FAT::OpenDir((char*)""); // freezes!
        _ASSERT(result == BIOS::FAT::EOk);
        BIOS::FAT::TFindFile file;
//        BIOS::DBG::Print("f");
        while (BIOS::FAT::FindNext(&file) == BIOS::FAT::EOk)
        {
            if (compare(file.strName, name) == 0)
            {
                memcpy(&found, &file, sizeof(BIOS::FAT::TFindFile));
                return true;
            }
        }
//        BIOS::DBG::Print(".");
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
                mpFlashWriteRange[0] = -1;
                mpFlashWriteRange[1] = 0;
                BIOS::SYS::DelayMs(50);
            }
//            BIOS::SYS::DelayMs(300); // 300 ok
        }

        // TODO: Deadlock here
        BIOS::FAT::TFindFile fileTemp1;
        if (FindFile(mFpgaName, fileTemp1))
            mFpgaNewer = CheckNewer(mFileFpga, fileTemp1);

        BIOS::FAT::TFindFile fileTemp2;
        if (FindFile(mVerifyName, fileTemp2))
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

class CTools : public CWnd
{
    constexpr static int Width = 180;
    constexpr static int Height = 128;
    
    bool mSelected{false};
    bool mQuit{false};
    int mIndex{0};
    
public:
    int ModalShow(CWnd* pParent)
    {
        Create("tools", CWnd::WsVisible, CRect(LCD::Width/2 - Width/2, LCD::Height/2 - Height/2, LCD::Width/2 + Width/2, LCD::Height/2 + Height/2), pParent);

        Layout::Render(m_rcClient) << Layout::Window("Tools");

        mSelected = false;
        mQuit = false;

        SetFocus();
        WindowMessage(CWnd::WmPaint);
        while (!mSelected && !mQuit)
        {
            BIOS::KEY::EKey key = BIOS::KEY::GetKey();
            if (key != BIOS::KEY::EKey::None)
                WindowMessage(CWnd::WmKey, key);
            WindowMessage(CWnd::WmTick);
        }
        
        Destroy();
        pParent->SetFocus();
        return mSelected ? mIndex : -1;
    }

    virtual void OnPaint() override
    {
        using namespace Layout;
        CRect rcInner(m_rcClient);
        rcInner.Deflate(8, 20, 8, 8);
        Layout::Render(rcInner)
            << Select(mIndex == 0) << Layout::MenuItem("Run (F4)") << Layout::NewLine()
            << Select(mIndex == 1) << Layout::MenuItem("Loop") << Layout::NewLine()
            << Select(mIndex == 2) << Layout::MenuItem("Load FPGA image") << Layout::NewLine()
            << Select(mIndex == 3) << Layout::MenuItem("Load verification") << Layout::NewLine()
            << Select(mIndex == 4) << Layout::MenuItem("Load waveform") << Layout::NewLine()
            << Select(mIndex == 5) << Layout::MenuItem("Save waveform") << Layout::NewLine()
            << Select(mIndex == 6) << Layout::MenuItem("Save screenshot") << Layout::NewLine();
    }
    
    virtual void OnKey(int key) override
    {
        if (key == BIOS::KEY::Up && mIndex > 0)
        {
            mIndex--;
            Invalidate();
        }
        if (key == BIOS::KEY::Down && mIndex < 6)
        {
            mIndex++;
            Invalidate();
        }
        if (key == BIOS::KEY::Enter)
            mSelected = true;
        if (key == BIOS::KEY::Escape)
            mQuit = true;
    }
};

class CFreeFile
{
    const char* mPattern;
public:
    CFreeFile(const char* pattern) : mPattern{pattern}
    {
    }
    
    void Next(char* name)
    {
        // TODO!
        sprintf(name, mPattern, 0);
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
        bool focus{false};
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
    int mWaveformLength{0};
    uint16_t mWaveform[1024+512*2];
    int mWaveformScale{0};
    int mWaveformShift{0};
    bool mMeasure{false};
    bool mAutosave{true};
    CTestBench mTestBench;
    
    struct TLabel {
        char msg[16];
        int index;
        int row;
        int type;
    };
    struct TGroup {
        int begin;
        int end;
        uint16_t color;
    };
    
    TLabel mLabelsData[20];
    CArray<TLabel> mLabels;

    TGroup mGroupsData[10];
    CArray<TGroup> mGroups;
    int mFocus{0};
    
    CFileDialog mFile;
    CFileFilterSuffix filterFpga{".FPG"};
    CFileFilterSuffix filterVerification{".GEN"};
    bool mAutoRun{false};
    CTools mTools;
    CBufferedWriter mWriter;
    CBufferedReader mReader;
    CFreeFile mFreeWave{"fpga%03d.htm"};

public:
    
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
        BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);

        mModuleFpga.focus = true;
        mLabels.Init(mLabelsData, COUNT(mLabelsData));
        mGroups.Init(mGroupsData, COUNT(mGroupsData));
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
        strcpy(mModuleFpga.timestamp, "FPGA not loaded");
        strcpy(mModuleVerify.timestamp, "Default wave");
    }

	void OnKey(int key) override
	{
        if (mAutoRun)
        {
            mAutoRun = false;
            return;
        }
        
        int move = mFocus == 0 ? 4 : 40;
        if (key == BIOS::KEY::Left)
        {
            switch (mFocus)
            {
                case 0:
                case 1:
                    if (mWaveformShift > 0)
                    {
                        mWaveformShift = max(0, mWaveformShift-move);
                        DrawWave();
                        DrawScrollbar();
                    }
                    break;
                case 2:
                    if (mWaveformScale > 0)
                    {
                        ZoomOut();
                        DrawWave();
                        DrawUi();
                    }
                    break;
                case 3:
                    mModuleFpga.focus = true;
                    mModuleVerify.focus = false;
                    DrawFooter(mModuleFpga, mModuleVerify);
                    break;
                    
            }
        }
        if (key == BIOS::KEY::Right)
        {
            switch (mFocus)
            {
                case 0:
                case 1:
                    if (mWaveformShift < mWaveformLength-256+128)
                    {
                        mWaveformShift = min(mWaveformShift + move, mWaveformLength-256+128);
                        DrawWave();
                        DrawScrollbar();
                    }
                    break;
                case 2:
                    ZoomIn();
                    DrawWave();
                    DrawUi();
                    break;
                case 3:
                    mModuleFpga.focus = false;
                    mModuleVerify.focus = true;
                    DrawFooter(mModuleFpga, mModuleVerify);
                    break;
            }
        }
        if (key == BIOS::KEY::Enter)
        {
            switch (mFocus)
            {
                case 0:
                case 1:
                case 2:
                    switch (mTools.ModalShow(this))
                    {
                        case 0:
                            mMeasure = true;
                            break;
                        case 1:
                            mAutoRun = true;
                            break;
                        case 2:
                            LoadFile("Load FPGA image", mModuleFpga, filterFpga);
                            break;
                        case 3:
                            LoadFile("Verification module", mModuleVerify, filterVerification);
                            break;
                        case 5:
                            SaveWaveform();
                            break;
                        case 4:
                            LoadWaveform();
                            break;
                        case 6:
                            Screenshot();
                            break;

                    }
                    Invalidate();
                    break;
                case 3:
                    if (mModuleFpga.focus)
                        LoadFile("Load FPGA image", mModuleFpga, filterFpga);
                    if (mModuleVerify.focus)
                        LoadFile("Verification module", mModuleVerify, filterVerification);
                    break;
            }
        }
        if (key == BIOS::KEY::Up)
        {
            if (mFocus > 0)
            {
                mFocus--;
                DrawUi();
                DrawFooter(mModuleFpga, mModuleVerify);
            }
        }
        if (key == BIOS::KEY::Down)
        {
            if (mFocus < 3)
            {
                mFocus++;
                DrawUi();
                DrawFooter(mModuleFpga, mModuleVerify);
            }
        }
        if (key == BIOS::KEY::F4)
            mMeasure = true;
	}

    void SaveWaveform()
    {
        char temp[16];
        mFreeWave.Next(temp);
        SaveWaveform(temp);
    }
    
    void SaveWaveform(char* name)
    {
        // fpgbench.htm
        // fpga001.js
        /*
         testbenchrecord = {
            fpga: {
              file: "bench.fpg",
              timestamp:
              crc
            },
            verify:
            fpgacrc: 0x12312312,


         testbenchrecord = {
            fpga: "bench.fpg",
            fpgacrc: 0x12312312,
            verify: nullptr,
            verifycrc: 0,
            waveforms: {
                clk:  "010101010101",
                sck:  "101010101010",
                cs:   "1123129031302",
                mosi: "11010101010101",
                miso: "zzzzzzzzzz1000111010"
            }
         };
         */
        mWriter.Open(name);
        mWriter << "<script>\n";
        mWriter << "testbench_target = {\n";
        mWriter << "  title: \"LA104 - AG1KLPQ48\",\n";
        mWriter << "  fpga: {\n";
        if (mModuleFpga.name[0])
            mWriter << "    file: \"" << mModuleFpga.name << "\",\n";
        else
            mWriter << "    file: null,\n";
        mWriter << "    timestamp: \"" << mModuleFpga.timestamp << "\",\n";
        mWriter << "    crc: " << "0" << "\n";
        mWriter << "  },\n";
        mWriter << "  verify: {\n";
        if (mModuleVerify.name[0])
            mWriter << "    file: \"" << mModuleVerify.name << "\",\n";
        else
            mWriter << "    file: null,\n";
        mWriter << "    timestamp: \"" << mModuleVerify.timestamp << "\",\n";
        mWriter << "    crc: " << "0" << "\n";
        mWriter << "  },\n";
        mWriter << "  waveforms: {\n";
        for (int j=0; j<5; j++)
        {
            static const char* waveNames[] = {" clk", " sck", "  cs", "mosi", "miso"};
            static const char waveTypes[] = {'?', '0', '1', 'z', 'x', 'E', 'E'};
            mWriter << "    " << waveNames[j] << ": \"";
            for (int i=0; i<mWaveformLength; i++)
                mWriter << waveTypes[(mWaveform[i] >> j*3) & 7];
            mWriter << '\"';
            if (j < 5-1)
                mWriter << ',';
            mWriter << '\n';
        }
        mWriter << "  },\n";
        mWriter << "  labels: [\n";
        for (int i=0; i<mLabels.GetSize(); i++)
        {
            TLabel& l = mLabels[i];
            char temp[8];
            sprintf(temp, "%d", l.index);
            mWriter << "    {";
            mWriter << "pos: " << temp << ", ";
            sprintf(temp, "%d", l.type);
            mWriter << "type: " << temp << ", ";
            mWriter << "text: \"" << l.msg << "\"";
            mWriter << "}";
            if (i < mLabels.GetSize()-1)
                mWriter << ',';
            mWriter << '\n';
        }
        mWriter << "  ],\n";
        mWriter << "  groups: [\n";
        for (int i=0; i<mGroups.GetSize(); i++)
        {
            TGroup& g = mGroups[i];
            char temp[8];
            sprintf(temp, "%d", g.begin);
            mWriter << "    {";
            mWriter << "begin: " << temp << ", ";
            sprintf(temp, "%d", g.end);
            mWriter << "end: " << temp << ", ";
            sprintf(temp, "#%02x%02x%02x", Get565R(g.color), Get565G(g.color), Get565B(g.color));
            mWriter << "color: \"" << temp << "\"";
            mWriter << "}";
            if (i < mGroups.GetSize()-1)
                mWriter << ',';
            mWriter << '\n';
        }
        mWriter << "  ]\n";
        mWriter << "};\n";
        mWriter << "</script>\n";
        mWriter << "<script src=\"https://l.valky.eu/la104fpgaenv1\"></script>\n";
        mWriter.Close();
    }
        
    void LoadWaveform()
    {
        // TODO: file dialog
        char temp[16];
        mFreeWave.Next(temp);
        if (!mReader.Open(temp))
        {
            _ASSERT(0);
            return;
        }
        bool continuous = false;
        int currentWave = -1;
        int currentGroup = 0;
        
        memset(mWaveform, 0, sizeof(mWaveform));
        mWaveformLength = 0;
        mGroups.RemoveAll();
        mLabels.RemoveAll();
        
        while (!mReader.Eof())
        {
            bool wrap = true;
            mReader.ReadLine(temp, sizeof(temp), &wrap);
            char* wave = (currentWave != -1) ? temp : nullptr;
//            switch (currentGroup)
//            {
//            }
            if (!continuous)
            {
                if ((wave = strstr(temp, " clk: \"")) != nullptr)
                {
                    wave += 7;
                    currentWave = 0;
                    mWaveformLength = 0;
                } else
                if ((wave = strstr(temp, " sck: \"")) != nullptr)
                {
                    wave += 7;
                    currentWave = 1;
                    mWaveformLength = 0;
                } else
                if ((wave = strstr(temp, " cs: \"")) != nullptr)
                {
                    wave += 6;
                    currentWave = 2;
                    mWaveformLength = 0;
                } else
                if ((wave = strstr(temp, " miso: \"")) != nullptr)
                {
                    wave += 8;
                    currentWave = 3;
                    mWaveformLength = 0;
                } else
                if ((wave = strstr(temp, " mosi: \"")) != nullptr)
                {
                    wave += 8;
                    currentWave = 4;
                    mWaveformLength = 0;
                }
                if (strstr(temp, " labels: [") != nullptr)
                    currentGroup = 1;
                if (strstr(temp, " groups: [") != nullptr)
                    currentGroup = 2;
                if (strstr(temp, " ]") != nullptr)
                    currentGroup = 0;
            }
            if (currentWave != -1 && wave)
            {
                for (int i=0; wave[i]; i++)
                {
                    if (wave[i] == '\"')
                    {
                        currentWave = -1;
                        break;
                    }
                    int value = 0;
                    switch (wave[i])
                    {
                        case '?':
                            value = Undefined;
                            break;
                        case '0':
                            value = Low;
                            break;
                        case '1':
                            value = High;
                            break;
                        case 'z':
                            value = Z;
                            break;
                        case 'x':
                            value = Transition;
                            break;
                        case '\"':
                            currentWave = -1;
                            wave = nullptr;
                            break;
                        default:
                            _ASSERT(0);
                    }
                    if (!wave)
                        break;
                    mWaveform[mWaveformLength++] |= value << (currentWave*3);
                }
            }
            continuous = wrap;
        }
        mReader.Close();
    }
    
    void Screenshot()
    {
        Invalidate();
        _ASSERT(SaveScreenshot16((char*)"fpgawave.bmp"));
    }
    
    void LoadFile(const char* title, ModuleInfo& mod, CFileFilterSuffix& filter)
    {
        if (mFile.ModalShow(nullptr, title, &filter))
        {
            if (&mod == &mModuleFpga)
                mNotify.SetFpgaName(mFile.GetFilename());//gabo
            if (&mod == &mModuleVerify)
                mNotify.SetVerifyName(mFile.GetFilename());
        }
        SetFocus();
        Invalidate();
    }
    void ZoomIn()
    {
        mWaveformScale++;
        mWaveformShift += (512>>mWaveformScale)/4;
    }
    void ZoomOut()
    {
        mWaveformShift -= (512>>mWaveformScale)/4;
        mWaveformShift = max(mWaveformShift, 0);
        mWaveformScale--;
    }

    void OnPaint() override
    {
        BIOS::LCD::Bar(m_rcClient, RGB565(404040));
        
        CRect rc1(m_rcClient);
        rc1.bottom = 14;
        GUI::Background(rc1, RGB565(4040b0), RGB565(404040));

        BIOS::LCD::Print(8, 0, RGB565(ffffff), RGBTRANS, "FPGA Test bench");

        const int labelsY = 20;
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

        DrawWave();
        DrawPreview();
        DrawUi();
        DrawFooter(mModuleFpga, mModuleVerify);
    }
    
    void DrawWave()
    {
        const int labelsY = 20;
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
        int curGroup = 0;
        uint16_t activeGroup = 0;
        
        for (int i=0; i<BIOS::LCD::Width-labelWidth; i++)
        {
            int ii = i >> mWaveformScale;
            if (mGroups.GetSize() > curGroup)
            {
                if (!activeGroup && mWaveformShift+ii >= mGroups[curGroup].begin)
                {
                    activeGroup = mGroups[curGroup].color;
                }
                if (activeGroup && mWaveformShift+ii >= mGroups[curGroup].end)
                {
                    activeGroup = 0;
                    curGroup++;
                }
            }
            uint16_t vals = mWaveformShift+ii < COUNT(mWaveform) ? mWaveform[mWaveformShift+ii] : Invalid;
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

            DrawLevel(i, o1, l1, activeGroup);
            BIOS::LCD::BufferWrite(lineSkip, labelSpacing);
            DrawLevel(i, o2, l2, activeGroup);
            BIOS::LCD::BufferWrite(lineSkip, labelSpacing);
            DrawLevel(i, o3, l3, activeGroup);
            BIOS::LCD::BufferWrite(lineSkip, labelSpacing);
            DrawLevel(i, o4, l4, activeGroup);
            BIOS::LCD::BufferWrite(lineSkip, labelSpacing);
            DrawLevel(i, o5, l5, activeGroup);
            
            o1 = l1;
            o2 = l2;
            o3 = l3;
            o4 = l4;
            o5 = l5;
        }
        BIOS::LCD::BufferEnd();
        
        // Annotations
        const int annotationY = labelsY+4*labelSpacing+5*labelHeight+4;
        BIOS::LCD::Bar(CRect(8, annotationY, BIOS::LCD::Width-8, annotationY+28), RGB565(404040));
        for (int i=0; i<mLabels.GetSize(); i++)
        {
            const TLabel& label = mLabels[i];
            int labelSize = strlen(label.msg)*8+12;
            int x = 64 + ((label.index - mWaveformShift)<<mWaveformScale);
            int y = annotationY + label.row*14;
            if (x+labelSize >= 8 && x < BIOS::LCD::Width-16)
            {
                if (label.type == CTestBench::Label)
                    if (x-3 >= 8 && x-3+8 < BIOS::LCD::Width)
                        BIOS::LCD::Print(x-3, y, RGB565(b0b0b0), RGB565(404040), '\x18');
                if (label.type == CTestBench::GroupBegin)
                    if (x-3 >= 8 && x-3+8 < BIOS::LCD::Width)
                        BIOS::LCD::Print(x-3, y, RGB565(b0b0b0), RGB565(404040), '\xc0');
                x += 12;
                for (int i=0; label.msg[i]; i++)
                {
                    if (x >= 8 && x+16 < BIOS::LCD::Width)
                        BIOS::LCD::Print(x, y, RGB565(b0b0b0), RGB565(404040), label.msg[i]);
                    x += 8;
                }
            }
        }
    }
    
    void DrawUi()
    {
        BIOS::LCD::Print(0, 122, mFocus == 0 ? RGB565(404040) : RGB565(b0b0b0),  mFocus == 0 ? RGB565(ffffff) : RGB565(404040), "\x11");
        BIOS::LCD::Print(BIOS::LCD::Width-8, 122, mFocus == 0 ? RGB565(404040) : RGB565(b0b0b0), mFocus == 0 ? RGB565(ffffff) : RGB565(404040), "\x10");
        BIOS::LCD::Printf(8, 186, mFocus == 2 ? RGB565(b0b0b0) : RGB565(b0b0b0), mFocus == 2 ? RGB565(ffffff) : RGB565(404040), "Zoom: %dx ", 1<<mWaveformScale);
        DrawScrollbar();
    }
    
    void DrawScrollbar()
    {
        if (mWaveformLength == 0)
            return;
        int viewRangeMin = mWaveformShift;
        int viewRangeMax = mWaveformShift + (256 >> mWaveformScale);

        CRect rcScroll(viewRangeMin*BIOS::LCD::Width/mWaveformLength, 155-4,
                       viewRangeMax*BIOS::LCD::Width/mWaveformLength, 155-1);
        rcScroll.right = min(rcScroll.right, BIOS::LCD::Width);
        if (rcScroll.left > 0)
            BIOS::LCD::Bar(CRect(0, rcScroll.top, rcScroll.left, rcScroll.bottom), RGB565(404040));
        if (rcScroll.right < BIOS::LCD::Width)
            BIOS::LCD::Bar(CRect(rcScroll.right, rcScroll.top, BIOS::LCD::Width, rcScroll.bottom), RGB565(404040));
        BIOS::LCD::Bar(rcScroll, mFocus == 1 ? RGB565(ffffff) : RGB565(b0b0b0));
        rcScroll.Offset(0, 25);
        if (rcScroll.left > 0)
            BIOS::LCD::Bar(CRect(0, rcScroll.top, rcScroll.left, rcScroll.bottom), RGB565(404040));
        if (rcScroll.right < BIOS::LCD::Width)
            BIOS::LCD::Bar(CRect(rcScroll.right, rcScroll.top, BIOS::LCD::Width, rcScroll.bottom), RGB565(404040));
        BIOS::LCD::Bar(rcScroll, mFocus == 1 ? RGB565(ffffff) : RGB565(b0b0b0));
    }
    
    void DrawPreview()
    {
        BIOS::LCD::BufferBegin(CRect(0, 155, BIOS::LCD::Width, 155+5*4));
        const int increment[] = {0, 0, 2, 1, 1};

        int activeGroup = 0;
        int curGroup = 0;

        for (int i=0; i<BIOS::LCD::Width; i++)
        {
            int j1 = i*mWaveformLength/BIOS::LCD::Width;
            int j2 = (i+1)*mWaveformLength/BIOS::LCD::Width;
            
            if (mGroups.GetSize() > curGroup)
            {
                if (!activeGroup && j1 >= mGroups[curGroup].begin)
                {
                    activeGroup = mGroups[curGroup].color;
                }
                if (activeGroup && j1 >= mGroups[curGroup].end)
                {
                    activeGroup = 0;
                    curGroup++;
                }
            }

            int sum[5] = {0};
            for (int j=j1; j<j2; j++)
            {
                uint16_t vals = mWaveform[j];
                Level o1 = (Level)((vals >> 12) & 7);
                Level o2 = (Level)((vals >> 9) & 7);
                Level o3 = (Level)((vals >> 6) & 7);
                Level o4 = (Level)((vals >> 3) & 7);
                Level o5 = (Level)((vals >> 0) & 7);
                sum[0] += increment[o1];
                sum[1] += increment[o2];
                sum[2] += increment[o3];
                sum[3] += increment[o4];
                sum[4] += increment[o5];
            }
            
            for (int l=0; l<5; l++)
            {
                if (!activeGroup)
                {
                    int lvl = (sum[l]*0xb0)/(j2-j1)/2;
                    if (lvl < 0x40)
                        lvl = 0x40;
                    uint16_t color = RGB565RGB(lvl, lvl, lvl);
                    for (int k=0; k<4; k++)
                        BIOS::LCD::BufferWrite(color);
                } else {
                    int lvl = (sum[l]*0x100)/(j2-j1)/2;
                    int clrr = (Get565R(activeGroup)*lvl)>>8;
                    int clrg = (Get565G(activeGroup)*lvl)>>8;
                    int clrb = (Get565B(activeGroup)*lvl)>>8;
                    clrr = max(clrr, 0x40);
                    clrg = max(clrg, 0x40);
                    clrb = max(clrb, 0x40);
                    uint16_t color = RGB565RGB(clrr, clrg, clrb);
                    for (int k=0; k<4; k++)
                        BIOS::LCD::BufferWrite(color);
                }
            }
        }
        BIOS::LCD::BufferEnd();
        

    }
    void DrawLevel(int x, Level o, Level l, uint16_t ovrColor)
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
        
        if (!ovrColor)
        {
            BIOS::LCD::BufferWrite(lines[l], 16);
            return;
        }

        uint16_t temp[16];
        memcpy(temp, lines[l], sizeof(temp));
        for (int i=0; i<16; i++)
            if (temp[i] == G)
                temp[i] = ovrColor;
        BIOS::LCD::BufferWrite(temp, 16);
    }
    
    void DrawFooter(const ModuleInfo& mod1, const ModuleInfo& mod2)
    {
        static const char floppy0[] =
        "\x10"
        "................"
        ".              ."
        ".              ."
        ".......        ."
        ".     .    ....."
        ".     .    ....."
        ".     .    ....."
        ".     .    ....."
        ".     .    ....."
        ".     .    .   ."
        ".     .    ....."
        ".     .        ."
        ".......        ."
        ".              ."
        ".             . "
        "..............  ";

        static const char floppy1[] =
        "\x10"
        "................"
        "................"
        "................"
        ".      ........."
        ".      ....    ."
        ".      ....    ."
        ".      ....    ."
        ".      ....    ."
        ".      ....    ."
        ".      .... .. ."
        ".      ....    ."
        ".      ........."
        ".      ........."
        "................"
        "............... "
        "..............  ";

        
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
        
        GUI::Background(CRect(rcFooter.left, rcFooter.top, rcFooter.CenterX(), rcFooter.bottom), grad1[0], grad1[1]);

        GUI::Background(CRect(rcFooter.CenterX(), rcFooter.top, rcFooter.right, rcFooter.bottom), grad2[0], grad2[1]);

        bool focus1 = mod1.focus && mFocus == 3;
        bool focus2 = mod2.focus && mFocus == 3;
        BIOS::LCD::Draw(rcFooter.CenterX()-18, rcFooter.top+2, focus1 ? RGB565(ffffff) : RGB565(b0b0b0), RGBTRANS, focus1 ? floppy1 : floppy0);
        BIOS::LCD::Draw(rcFooter.right-18, rcFooter.top+2, focus2 ? RGB565(ffffff) : RGB565(b0b0b0), RGBTRANS, focus2 ? floppy1 : floppy0);
        BIOS::LCD::Print(8, rcFooter.top+2, RGB565(b0b0b0), RGBTRANS, mod1.timestamp);
        BIOS::LCD::Print(8, rcFooter.top+2+16, RGB565(b0b0b0), RGBTRANS, mod1.name);
        BIOS::LCD::Print(rcFooter.CenterX()+8, rcFooter.top+2, RGB565(b0b0b0), RGBTRANS, mod2.timestamp);
        BIOS::LCD::Print(rcFooter.CenterX()+8, rcFooter.top+2+14, RGB565(b0b0b0), RGBTRANS, mod2.name);
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
                         inOut ? RGB565(b0ffb0) : RGB565(b0b0ff), RGBTRANS, inOut ? "\x1a" : "\x1b");
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
        if (mAutoRun)
        {
            EVERY(300)
            {
                mMeasure = true;
            }
        }
        if (mMeasure)
        {
            mModuleVerify.status = Loading1;
            DrawFooter(mModuleFpga, mModuleVerify);
            mMeasure = false;
            Measure();
            if (mAutosave && !mAutoRun)
            {
                if (strcmp(mModuleFpga.name, "BENCH.FPG") == 0)
                {
                    BIOS::SYS::DelayMs(1000); //1500ok 1000ok 500bad
                    SaveWaveform((char*)"bench.htm");
                }
            }
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
        static CApplication* pThis;
        pThis = this;
        static int firstRowMaxX;
        firstRowMaxX = -1;
        static TGroup group;
        
        mWaveformLength = 0;
        mLabels.RemoveAll();
        mGroups.RemoveAll();
        
        mTestBench.Init();
        BIOS::SYS::DelayMs(20);
        
        mTestBench.setHandler([](CTestBench::Level* levels){
            uint16_t vals = (levels[0] << 0) | (levels[1] << 3) | (levels[2] << 6) | (levels[3] << 9) | (levels[4] << 12);
            _ASSERT(pThis->mWaveformLength < COUNT(pThis->mWaveform));
            pThis->mWaveform[pThis->mWaveformLength++] = vals;
        }, [](int msgType, const char* msg) {
            int index = pThis->mWaveformLength;
            if (msgType == CTestBench::GroupEnd)
                index = group.begin;
            
            if (msg && pThis->mLabels.GetSize() < pThis->mLabels.GetMaxSize())
            {
                TLabel label;
                label.type = msgType;
                label.index = index;
                int maxX = label.index + strlen(msg)*8;
                if (firstRowMaxX < label.index)
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
            if (msgType == CTestBench::GroupBegin)
                group.begin = index;
            if (msgType == CTestBench::GroupEnd)
            {
                const uint16_t colors[] = {RGB565(ffb0b0), RGB565(b0ffb0), RGB565(b0b0ff)};
                group.end = pThis->mWaveformLength;
                group.color = colors[pThis->mGroups.GetSize()%COUNT(colors)];
                pThis->mGroups.Add(group);
            }

        });

        main_verify(mTestBench);
        mTestBench.setHandler(nullptr, nullptr);
        DrawWave();
        DrawPreview();
        DrawScrollbar();
        
        BIOS::SYS::DelayMs(20);
        mTestBench.Deinit();
    }
};
