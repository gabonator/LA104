#include <library.h>

#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/framework/Serialize.h"
#include "../../os_host/source/gui/Controls.h"

int mStorage_mMinGapTime = 10000; // TODO: join

#include "utils/shapes.h"
#include "utils/Menu.h"
#include "utils/json.h"
#include "devices/sampler.h"
#include "devices/cc1101.h"
#include "devices/raw.h"
#include "devices/infra.h"

/*
  Chceme nahravat vsetko a appendovat
*/

class CStorage
{
public:
    class CTrigger // filter
    {
    public:
        // Primary
        int nMaxGap{5000};
        int nMaxGapOnce{9000}; // preamble len
        int nMaxTransitions{-1};  // CHCEME BREAKNUT READING PRE ATTACK? alebo nacitat cely buffer   

        // Secondary
        int nMinTransitions{15}; //pozor! prepisuje buffer aj ked nematchuje!
        int nMinFirstPulseDuration{-1};
        int nMaxFirstPulseDuration{-1};
        bool bPaused;//running?

        // Tertiary
        enum TMode
        {
            Single, All
        } eMode;
        enum TSkip
        {
            Every, Second, Third, Fourth, Fifth
        } eSkip;
        enum TSaveMode
        {
            Manual, Append, AutoAppend
        } eSave;
        bool bBeepOnMatch{true};
    };

public:
    CSampler* mDeviceCurrent{nullptr};
    
    bool mConnected{true};
    CDeviceCC1101 mDeviceRadio;
    CDeviceRaw mDeviceRaw;
    CDeviceInfra mDeviceInfra;
    bool mTriggerSingle{true};

    int mSignalLength{0};
    uint16_t mSignalData[256];
    int mHistogramScaleX{3};
    int mHistogramScaleY{2};
    int mSignalScaleX{20};
    int mSignalOffset{0};
    bool mEnabled{true};
    bool mReceived{false};
    //int mMinGapTime{5000};

} mStorage;

#include "layouts/modem.h"
#include "layouts/meas.h"
#include "layouts/filter.h"
#include "layouts/file.h"
#include "layouts/play.h"
#include "layouts/analyse.h"

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
            case 3: return TItem{"Trig", TItem::Default};
            case 4: return TItem{"File", TItem::Default};
            case 5: return TItem{"Play", TItem::Default};
            case 6: return TItem{"Analyse", TItem::Default};
            default: return TItem{nullptr, TItem::None};
        }
    }
};

class CApplication : public CWnd
{
    CMenuMain mMenu;
    CLayoutModem mLayoutModem;
    CLayoutMeas mLayoutMeas;
    CLayoutFilter mLayoutFilter;
    CLayoutFile mLayoutFile;
    CLayoutPlay mLayoutPlay;
    CLayoutAnalyse mLayoutAnalyse;

public:
    CApplication()
    {
        //mStorage.mDeviceCurrent = &mDeviceModem;
    }
    
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        CRect rcMenu(rc);
        rcMenu.bottom = rcMenu.top + 14;
        
        mMenu.Create("MainMenu", CWnd::WsVisible, rcMenu, this);
        
        CRect rcContent(rc);
        rcContent.top = rcMenu.bottom;
        
        mLayoutModem.Create("Modem", CWnd::WsVisible | CWnd::WsTick, rcContent, this);
        mLayoutMeas.Create("Meas", CWnd::WsTick, rcContent, this);
        mLayoutFilter.Create("Filter", CWnd::WsHidden, rcContent, this);
        mLayoutFile.Create("File", CWnd::WsHidden, rcContent, this);
        mLayoutPlay.Create("Play", CWnd::WsHidden, rcContent, this);
        mLayoutAnalyse.Create("Analyse", CWnd::WsHidden, rcContent, this);
        
        mMenu.SetFocus();
        SetTimer(100);
    }

    virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data) override
    {
        if (pSender == &mLayoutModem)
        {/*
            mStorage.mDeviceCurrent->Deinit();
            switch (data)
            {
                case 0: mStorage.mDeviceCurrent = &mDeviceModem; break;
                case 1: mStorage.mDeviceCurrent = &mDeviceRaw; break;
                case 2: mStorage.mDeviceCurrent = &mDeviceInfra; break;
            }
            mStorage.mDeviceCurrent->Init();
          */
        }
        
        if (pSender == &mMenu)
        {
            mLayoutModem.ShowWindow(data == 1);
            mLayoutMeas.ShowWindow(data == 2);
            mLayoutFilter.ShowWindow(data == 3);
            mLayoutFile.ShowWindow(data == 4);
            mLayoutPlay.ShowWindow(data == 5);
            mLayoutAnalyse.ShowWindow(data == 6);
            Invalidate();
        }
    }
    
    virtual void WindowMessage(int nMsg, int nParam = 0) override
    {
        if (nMsg == CWnd::WmKey)
        {
            if (nParam == BIOS::KEY::F1)
            {
                if (mLayoutMeas.IsVisible())
                {
                    mStorage.mEnabled = !mStorage.mEnabled;
                    if (mLayoutMeas.IsVisible())
                        mLayoutMeas.Invalidate();
                    return;
                }
            }
            if (nParam == BIOS::KEY::F3)
            {
                static int counter = 0;
                BIOS::LCD::Printf(4, 240-16, RGB565(ffffff), RGB565(202020), "saving rf%03d.sig     ", counter);
                char fileName[16];
                sprintf(fileName, "rf%03d.sig", counter++);
                CBufferedWriter writer;
                if (writer.Open( fileName ))
                {
                    char temp[8];
                    writer << "[";
                    for (int i=0; i<mStorage.mSignalLength; i++)
                    {
                        if (i>0)
                            sprintf(temp, ", %d", mStorage.mSignalData[i]);
                        else
                            sprintf(temp, "%d", mStorage.mSignalData[i]);
                        writer << temp;
                    }
                    writer << "]";
                    writer.Close();
                    BIOS::LCD::Printf(4, 240-16, RGB565(ffffff), RGB565(202020), "Ok!                ");
                } else
                {
                    BIOS::LCD::Printf(4, 240-16, RGB565(ffffff), RGB565(202020), "Failed!            ");
                }
                return;
            }
        }
        if (nMsg == CWnd::WmTick)
        {
            if (!mLayoutModem.IsVisible() && mStorage.mDeviceCurrent && mStorage.mConnected && mStorage.mEnabled)
            {
                if (mStorage.mDeviceCurrent->Read())
                {
                    if (mStorage.mDeviceCurrent->Receive(mStorage.mSignalData, COUNT(mStorage.mSignalData), mStorage.mSignalLength))
                    {
                        mStorage.mReceived = true;
                        // new signal was received
                        if (mLayoutMeas.IsVisible())
                        {
                            SendMessage(&mLayoutMeas, ToWord('D', 'A'), 0);
                            if ( mStorage.mTriggerSingle /*&& mStorage.mSignalLength >= 60*/ ) // condition to stop?
                            {
                                mStorage.mEnabled = false; // single!
                            }
                        }
                    }
                }
            }
        }
        CWnd::WindowMessage(nMsg, nParam);
    }

    virtual void OnTimer() override
    {
        BIOS::LCD::Draw( 2, 0, RGB565(808080), RGBTRANS, CShapes_dotout);
        if (!mStorage.mDeviceCurrent)
        {
            BIOS::LCD::Draw( 2, 0, RGB565(000000), RGBTRANS, CShapes_dot);
            return;
        }
        if (mStorage.mReceived)
        {
            mStorage.mReceived = false;
            BIOS::LCD::Draw( 2, 0, RGB565(ff4040), RGBTRANS, CShapes_dot);
        } else if (mStorage.mEnabled)
            BIOS::LCD::Draw( 2, 0, RGB565(00b000), RGBTRANS, CShapes_dot);
        else
            BIOS::LCD::Draw( 2, 0, RGB565(b0b0b0), RGBTRANS, CShapes_dot);
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

