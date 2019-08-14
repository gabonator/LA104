#include "../controls/button.h"
#include "../controls/signal.h"
#include "modem/cc1101.h"
#include "modem/infra.h"
#include "modem/raw.h"

class CSubmenuModem : public CSubMenu
{
public:
    virtual TItem GetItem(int i)
    {
        switch (i)
        {
            case 0: return TItem{"Preset", TItem::Default};
            case 1: return TItem{"CC1101", TItem::Default};
            case 2: return TItem{"Infra", TItem::Default};
            case 3: return TItem{"Raw", TItem::Default};
            default: return TItem{nullptr, TItem::None};
        }
    }
};

class CLayoutPagePreset : public CWnd
{
    CButton mButton433;
    CButton mButton868;
    CButton mButtonKeeLoq;
    CButton mButtonInfra;
    CButton mButtonSave;

public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        
        CRect rcButton(8, 40, 8+180, 40+16);
        mButton433.Create("CC1101 @ 433 MHz", CWnd::WsVisible, rcButton, this);
        rcButton.top += 20;
        rcButton.bottom += 20;
        mButton868.Create("CC1101 @ 868 MHz", CWnd::WsVisible, rcButton, this);
        rcButton.top += 20;
        rcButton.bottom += 20;
        mButtonKeeLoq.Create("CC1101 @ Keeloq", CWnd::WsVisible, rcButton, this);
        rcButton.top += 20;
        rcButton.bottom += 20;
        mButtonInfra.Create("Infra RC", CWnd::WsVisible, rcButton, this);
        rcButton.top += 20;
        rcButton.bottom += 20;
        mButtonSave.Create("Save...", CWnd::WsVisible, rcButton, this);
        rcButton.top += 20;
        rcButton.bottom += 20;
    }
    
    virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
    {
        if (pSender == &mButton433)
        {
        }
        if (pSender == &mButton868)
        {
        }
        if (pSender == &mButtonKeeLoq)
        {
        }
        if (pSender == &mButtonInfra)
        {
        }
        if (pSender == &mButtonSave)
        {
        }
    }
    
private:
    virtual void OnPaint() override
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
    }
};


class CLayoutModem : public CWnd
{
    CSubmenuModem mMenu;
    CLayoutPagePreset mPreset;
    CLayoutPageCc1101 mRadio;
    CLayoutPageInfra mInfra;
    CLayoutPageRaw mRaw;
    
public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        BIOS::GPIO::PinMode(BIOS::GPIO::P4, BIOS::GPIO::Input);
        CWnd::Create(pszId, dwFlags, rc, pParent);
        
        CRect rcMenu(rc);
        rcMenu.bottom = rcMenu.top + 16;
        
        mMenu.Create("ModemMenu", CWnd::WsVisible, rcMenu, this);
        
        CRect rcContent(rc);
        rcContent.top = rcMenu.bottom;
        
        mPreset.Create("Preset", CWnd::WsVisible | CWnd::WsTick, rcContent, this);
        mRadio.Create("Radio", CWnd::WsHidden | CWnd::WsTick, rcContent, this);
        mInfra.Create("Infra", CWnd::WsHidden | CWnd::WsTick, rcContent, this);
        mRaw.Create("Raw", CWnd::WsHidden | CWnd::WsTick, rcContent, this);
    }

    virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
    {
        if (pSender == &mMenu)
        {
            SendMessage(GetParent(), ToWord('M', 'C'), data);
            mPreset.ShowWindow(data == 0);
            mRadio.ShowWindow(data == 1);
            mInfra.ShowWindow(data == 2);
            mRaw.ShowWindow(data == 3);
            Invalidate();
        }
    }
};
