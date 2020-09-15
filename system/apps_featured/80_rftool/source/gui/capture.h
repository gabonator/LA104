
class CCapture : public CWnd
{
    CScroller mScroller{6, &mRedrawMask};
    int mLastCount{0};
    int mRedrawMask{0};
    int mCurrentUid{0};

public:
    void Create( const char* pszId, int dwFlags, const CRect& rc, CWnd* pParent )
    {
        mScroller.mExtraTop = 5;

        CWnd::Create(pszId, dwFlags, rc, pParent);
        SetTimer(200);
    }

    virtual void OnTimer() override 
    {
        int nNewCount = appData.GetCaptureRecords();

        if (mLastCount != nNewCount)
            mRedrawMask = -1;

        if (mRedrawMask)
            Invalidate();
    }
    
    virtual void WindowMessage(int nMsg, int nParam /*=0*/) override
    {
        if (nMsg == WmWillShow || nMsg == WmWillLoseFocus || nMsg == WmWillGetFocus)
        {
            mRedrawMask = -1;
        }
        CWnd::WindowMessage(nMsg, nParam);
    }
    
    virtual void OnPaint() override
    {
        int nElementSpacing = 32;
        int nElementHeight = nElementSpacing+1;
        
        CRect rcMenu(m_rcClient.left, m_rcClient.top+2, m_rcClient.right-8, m_rcClient.top + 16);
        Layout::Render r(rcMenu);
        
        if (mRedrawMask == -1)
            r << Layout::Background();
            
        r << Layout::Goto(appData.GetPaused() ? 40 - 8 : 40)
            << Layout::Select(HasFocus() && mScroller.mFocus == 0) << Layout::Button(appData.GetPaused() ? "Resume" : "Pause")
            << Layout::Select(HasFocus() && mScroller.mFocus == 1) << Layout::Button("Clear")
            << Layout::Select(HasFocus() && mScroller.mFocus == 2) << Layout::Button("Import")
            << Layout::Select(HasFocus() && mScroller.mFocus == 3) << Layout::Button("Load")
            << Layout::Select(HasFocus() && mScroller.mFocus == 4) << Layout::Button("Save")
            << Layout::Select(false);
        
        CRect rcElement(m_rcClient.left, m_rcClient.top+18, m_rcClient.right-8, m_rcClient.top + 18 + nElementHeight);

        mLastCount = appData.GetCaptureRecords();
        if (mLastCount == 0)
        {
            mRedrawMask = 0;
            rcElement.bottom = m_rcClient.top + 6*nElementSpacing+1;
            GUI::Background(rcElement, RGB565(404040), RGB565(101010));
            
            CRect rcScrollbar(m_rcClient.right-2, m_rcClient.top + 18, m_rcClient.right, m_rcClient.top + 6*nElementSpacing+1 + 18);
            BIOS::LCD::Bar(rcScrollbar, RGB565(606060));
            return;
        }

        int nTopIndex, nBottomIndex;
        mScroller.SetCount(mLastCount, 0);
        mScroller.GetRange(nTopIndex, nBottomIndex);
        
        CRect rcScrollbar(m_rcClient.right-2, m_rcClient.top+18, m_rcClient.right, m_rcClient.top + 18 + 6*nElementSpacing+1);
        int nScrollTop = rcScrollbar.top + nTopIndex*rcScrollbar.Height()/mLastCount;
        int nScrollBottom = rcScrollbar.top + nBottomIndex*rcScrollbar.Height()/mLastCount;
        BIOS::LCD::Bar(rcScrollbar, RGB565(606060));
        rcScrollbar.top = nScrollTop;
        rcScrollbar.bottom = nScrollBottom;
        BIOS::LCD::Bar(rcScrollbar, RGB565(b0b0b0));

        using namespace Layout;
        
        int mask = mRedrawMask;
        mRedrawMask = 0;
        long now = BIOS::SYS::GetTick();
        int index = 0;
        for (int i=nTopIndex; i<nBottomIndex; i++, index++)
        {
            if (!(mask & (1<<index)))
            {
                rcElement.Offset(0, nElementSpacing);
                continue;
            }
                
            int ts;
            int uid;
            char name[64];
            char desc[64];
            char time[8];
            appData.GetCaptureRecord(mLastCount-1-i, ts, uid, name, desc);
            
            int alpha = 0;
            if (now - ts < 5000)
            {
                alpha = 255-(now - ts)*256/5000;
                mRedrawMask |= 1<<index;
            }
                
            ts /= 1000;
            sprintf(time, "%02d:%02d", ts/60, ts%60);
                      
            bool focus = HasFocus() && (mScroller.mFocus - mScroller.mExtraTop) == index;
            
            if (focus)
                mCurrentUid = uid;
            
            Color def(focus ? RGB565(000000) : RGB565(b0b0b0));
            Color hig(RGB565(ffffff));

            int y = (focus ? 0x80 : 0x10) + alpha/2;
            y = min(y, 255);
            Render r(rcElement);
            r   << Rectangle(focus ? RGB565RGB(0x80, 0x80, y) : RGB565RGB(0x10, 0x10, y))
                << Padding(8, 2, 8, 2)
                << Color(focus ? RGB565(404040) : RGB565(808080)) << time << " " << def << name;
            
            if (focus)
                r << Goto(rcElement.right-52) << Select(focus) << Button((char*)"Edit");
                
            r << NewLine() << def;

            // simple formatted text "normal <highlighted> normal <highlighted>"
            char* delim = nullptr;
            char* ptext = desc;
            while ((delim = strstr(ptext, "<")) != nullptr)
            {
                *delim++ = 0;
                r << ptext;
                ptext = delim;

                delim = strstr(ptext, ">");
                _ASSERT(delim);

                *delim++ = 0;
                r << hig << ptext << def;
                ptext = delim;
            }
            r << ptext;
            rcElement.Offset(0, nElementSpacing);
        }
        
        rcElement.top++;
        rcElement.bottom = m_rcClient.top + 6*nElementSpacing+1;
        GUI::Background(rcElement, RGB565(404040), RGB565(101010));
    }
    
    virtual void OnKey(int key) override
    {
        if (key == BIOS::KEY::Right && mScroller.mFocus < mScroller.mExtraTop)
        {
            if (mScroller.Down())
            {
                Invalidate();
                return;
            }
        }
        if (key == BIOS::KEY::Left && mScroller.mFocus < mScroller.mExtraTop)
        {
            if (mScroller.Up())
            {
                Invalidate();
                return;
            }
        }
        if (key == BIOS::KEY::Up)
        {
            if (mScroller.mFocus < mScroller.mExtraTop)
                mScroller.mFocus = 0;

            if (mScroller.Up())
            {
                Invalidate();
                return;
            }
        }
        if (key == BIOS::KEY::Down)
        {
            if (mScroller.mFocus < mScroller.mExtraTop)
            {
                mScroller.mFocus = mScroller.mExtraTop-1;
                mScroller.Down();
                Invalidate();
                return;
            }
            
            if (mScroller.Down())
            {
                Invalidate();
                return;
            }
        }
        if (key == BIOS::KEY::Enter)
        {
            if (mScroller.mFocus < mScroller.mExtraTop)
            {
                switch (mScroller.mFocus)
                {
                    case 0:
                        appData.SetPaused(!appData.GetPaused());
                        mScroller.OnScrollChange();
                        break; // pause, resume
                    case 1:
                        SendMessage(GetParent(), 0, (uintptr_t)"clearlist");
                        mScroller.OnScrollChange();
                        break; // clear
                    case 2:
                        SendMessage(GetParent(), 0, (uintptr_t)"import");
                        mScroller.OnScrollChange();
                        break; // import
                    case 3:
                        SendMessage(GetParent(), 0, (uintptr_t)"loadlist");
                        mScroller.OnScrollChange();
                        break; // load
                    case 4:
                        SendMessage(GetParent(), 0, (uintptr_t)"savelist");
                        mScroller.OnScrollChange();
                        break; // save
                }
            } else
            {
                SendMessage(GetParent(), 0xabbb, mCurrentUid);
            }
        }
        CWnd::OnKey(key);
    }
};
