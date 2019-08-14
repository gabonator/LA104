#include "file/play.h"
#include "file/load.h"
#include "file/save.h"

class CSubmenuFile : public CSubMenu
{
public:
    virtual TItem GetItem(int i)
    {
        switch (i)
        {
            case 0: return TItem{"Play", TItem::Default};
            case 1: return TItem{"Save", TItem::Default};
            case 2: return TItem{"Load", TItem::Default};
            default: return TItem{nullptr, TItem::None};
        }
    }
};

class CLayoutFile : public CWnd
{
    CSubmenuFile mMenu;
    CLayoutFilePlay mPlay;
    CLayoutFileSave mSave;
    CLayoutFileLoad mLoad;

public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        
        CRect rcMenu(rc);
        rcMenu.bottom = rcMenu.top + 16;
        
        mMenu.Create("FileMenu", CWnd::WsVisible, rcMenu, this);
        
        CRect rcContent(rc);
        rcContent.top = rcMenu.bottom;
        
        mPlay.Create("Play", CWnd::WsVisible | CWnd::WsTick, rcContent, this);
        mSave.Create("Save", CWnd::WsHidden, rcContent, this);
        mLoad.Create("Load", CWnd::WsHidden, rcContent, this);
    }

    virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
    {
        if (pSender == &mMenu)
        {
            mPlay.ShowWindow(data == 0);
            mSave.ShowWindow(data == 1);
            mLoad.ShowWindow(data == 2);
            Invalidate();
        }
    }    
};
