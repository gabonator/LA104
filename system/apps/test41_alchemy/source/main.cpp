#include <library.h>

#include "../../../os_host/source/framework/BufferedIo.h"
#include "../../../os_host/source/framework/Serialize.h"
#include "../../../os_host/source/gui/Controls.h"

#include "utils/shapes.h"
#include "utils/Menu.h"
#include "utils/json.h"
#include "settings.h"
#include "devices/sampler.h"
#include "devices/cc1101.h"
#include "devices/raw.h"
#include "devices/infra.h"

CDeviceCC1101 mDeviceRadio;
CDeviceRaw mDeviceRaw;
CDeviceInfra mDeviceInfra;

CBufferedWriter mWriter;

#include "codecs/codecs.h"
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
            case 6: return TItem{"Anal", TItem::Default};
            case 5: return TItem{"Play", TItem::Default};
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
    bool mIdle{false};

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
        mLayoutMeas.Create("Meas", CWnd::WsTick, rcContent, this);
        mLayoutFilter.Create("Filter", CWnd::WsHidden, rcContent, this);
        mLayoutFile.Create("File", CWnd::WsHidden, rcContent, this);
        mLayoutPlay.Create("Play", CWnd::WsHidden, rcContent, this);
        mLayoutAnalyse.Create("Analyse", CWnd::WsHidden, rcContent, this);
        
        mMenu.SetFocus();
        SetTimer(100);
        
#ifdef EMULATED
        BIOS::OS::SetArgument((char*)"/Users/gabrielvalky/Documents/git/LA104/system/apps/test41_alchemy/root/41alche.elf");
#endif
        strcat(mRuntime.mPath, BIOS::OS::GetArgument());
        char* last = strrchr(mRuntime.mPath, '/');
        if (last)
            *last = 0;
        else
            strcpy(mRuntime.mPath, "");
    }

    virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
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
                    mRuntime.mEnabled = !mRuntime.mEnabled;
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
            if (!mLayoutModem.IsVisible() && mSettings.mDeviceCurrent && mSettings.mConnected && mRuntime.mEnabled)
            {
                int read = mSettings.mDeviceCurrent->Read();
                if (!mIdle)
                {
                    if (!read)
                        mIdle = true;
                } else
                {
                    if (read)
                    {
                        if (mSettings.mBeepCapture)
                            BIOS::SYS::Beep(10);
                        
                        if (mSettings.mDeviceCurrent->Receive(mStorage.mSignalData, COUNT(mStorage.mSignalData), mStorage.mSignalLength))
                        {
                            mRuntime.mReceived = true;
                            // new signal was received
                            if (mLayoutMeas.IsVisible())
                            {
                                SendMessage(&mLayoutMeas, ToWord('D', 'A'), 0);
                                if ( mSettings.mTriggerSingle /*&& mStorage.mSignalLength >= 60*/ ) // condition to stop?
                                {
                                    mRuntime.mEnabled = false; // single!
                                }
                            }
                        }
                        
                        mIdle = false; // wait for signal fall
                    }
                }
            }
        }
        CWnd::WindowMessage(nMsg, nParam);
    }

    virtual void OnTimer() override
    {
        static int sub = 0;
        if (++sub >= 10)
        {
          mRuntime.mSeconds++;
          sub = 0;
        }
        BIOS::LCD::Draw( 2, 0, RGB565(808080), RGBTRANS, CShapes_dotout);
        if (!mSettings.mDeviceCurrent)
        {
            BIOS::LCD::Draw( 2, 0, RGB565(000000), RGBTRANS, CShapes_dot);
            return;
        }
        if (mRuntime.mTransmitted)
        {
            mRuntime.mTransmitted = false;
            BIOS::LCD::Draw( 2, 0, RGB565(4040ff), RGBTRANS, CShapes_dot);
        }
        else if (mRuntime.mReceived)
        {
            mRuntime.mReceived = false;
            BIOS::LCD::Draw( 2, 0, RGB565(ff4040), RGBTRANS, CShapes_dot);
        } else if (mRuntime.mEnabled)
            BIOS::LCD::Draw( 2, 0, RGB565(00b000), RGBTRANS, CShapes_dot);
        else
            BIOS::LCD::Draw( 2, 0, RGB565(b0b0b0), RGBTRANS, CShapes_dot);
    }
};

CApplication app;

#ifndef EMULATED
__attribute__((__section__(".entry")))
#endif

#ifdef EMSCRIPTEN
void mainInit()
{
    app.Create("WaveAlchemy", CWnd::WsVisible | CWnd::WsTick, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
    app.WindowMessage( CWnd::WmPaint );
}
bool mainLoop()
{
    BIOS::KEY::EKey key;

    key = BIOS::KEY::GetKey();
    if (key == BIOS::KEY::Escape && CWnd::m_arrModals.GetSize() == 0)
        return false;
    
    if (key != BIOS::KEY::None)
        app.WindowMessage(CWnd::WmKey, key);

    app.WindowMessage(CWnd::WmTick);
    return true;
}
void mainFinish()
{
    app.Destroy();
}
#else
int _main(void)
{
    app.Create("WaveAlchemy", CWnd::WsVisible | CWnd::WsTick, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
    app.WindowMessage( CWnd::WmPaint );
    
    BIOS::KEY::EKey key;
    while (true)
    {
        key = BIOS::KEY::GetKey();
        if (key == BIOS::KEY::Escape && CWnd::m_arrModals.GetSize() == 0)
            break;
        
        if (key != BIOS::KEY::None)
            app.WindowMessage(CWnd::WmKey, key);

        app.WindowMessage(CWnd::WmTick);
    }
    
    app.Destroy();
    return 0;
}
#endif

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

