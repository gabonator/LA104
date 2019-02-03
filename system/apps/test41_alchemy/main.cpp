#include <library.h>
#include <algorithm>

#include "utils/shapes.h"
#include "utils/Menu.h"
#include "utils/json.h"
#include "devices/cc1101.h"

class CStorage
{
public:
    int mCurrentModem{0};
    bool mConnected{true};
    CCc1101Config mModemConfig;
    
    int mSignalLength{0};
    int mSignalData[256];
    int mHistogramScaleX{5};
    int mHistogramScaleY{2};
    int mSignalScaleX{20};
    int mSignalOffset{0};
} mStorage;

#include "layouts/modem.h"
#include "layouts/meas.h"
#include "layouts/filter.h"
#include "layouts/modify.h"
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
            case 3: return TItem{"Filter", TItem::Default};
            case 4: return TItem{"Modify", TItem::Default};
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
    CLayoutModify mLayoutModify;
    CLayoutPlay mLayoutPlay;
    CLayoutAnalyse mLayoutAnalyse;

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
        mLayoutModify.Create("Modify", CWnd::WsHidden, rcContent, this);
        mLayoutPlay.Create("Play", CWnd::WsHidden, rcContent, this);
        mLayoutAnalyse.Create("Analyse", CWnd::WsHidden, rcContent, this);
        
        mMenu.SetFocus();
        SetTimer(100);
    }

    virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data) override
    {
        if (pSender == &mMenu)
        {
            mLayoutModem.ShowWindow(data == 1);
            mLayoutMeas.ShowWindow(data == 2);
            mLayoutFilter.ShowWindow(data == 3);
            mLayoutModify.ShowWindow(data == 4);
            mLayoutPlay.ShowWindow(data == 5);
            mLayoutAnalyse.ShowWindow(data == 6);
            Invalidate();
        }
    }
    
    virtual void WindowMessage(int nMsg, int nParam = 0) override
    {
        if (nMsg == CWnd::WmTick)
        {
            if (!mLayoutModem.IsVisible() && mStorage.mCurrentModem == 0 && mStorage.mConnected)
            {
                if (mStorage.mModemConfig.Read())
                {
                    if (mStorage.mModemConfig.Receive(mStorage.mSignalData, COUNT(mStorage.mSignalData), mStorage.mSignalLength))
                    {
                        // new signal was received
                        if (mLayoutMeas.IsVisible())
                        {
                            SendMessage(&mLayoutMeas, ToWord('D', 'A'), 0);
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

