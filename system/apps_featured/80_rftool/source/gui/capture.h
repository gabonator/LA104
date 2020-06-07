class CCapture : public CWnd
{
    int mFocus{0};
    int mScroll{0};
    int mLastCount{0};
    
public:
    void Create( const char* pszId, int dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        SetTimer(500);
    }

    virtual void OnTimer() override
    {
        int nNewCount = appData.GetCaptureRecords();
        if (mLastCount != nNewCount)
        {
            Invalidate();
        }
    }
    
    virtual void OnPaint() override
    {
        int nElementSpacing = 32;
        int nElementHeight = nElementSpacing+1;
        CRect rcElement(m_rcClient.left, m_rcClient.top, m_rcClient.right-8, m_rcClient.top + nElementHeight);

        mLastCount = appData.GetCaptureRecords();
        int nTopIndex = mScroll;
        int nBottomIndex = min(nTopIndex + 6, mLastCount);
        CRect rcScrollbar(m_rcClient.right-2, m_rcClient.top, m_rcClient.right, m_rcClient.top + 6*nElementSpacing+1);
        int nScrollTop = mLastCount ? rcScrollbar.top + nTopIndex*rcScrollbar.Height()/mLastCount : rcScrollbar.top;
        int nScrollBottom = mLastCount ? rcScrollbar.top + nBottomIndex*rcScrollbar.Height()/mLastCount : rcScrollbar.top;
        BIOS::LCD::Bar(rcScrollbar, RGB565(606060));
        rcScrollbar.top = nScrollTop;
        rcScrollbar.bottom = nScrollBottom;
        BIOS::LCD::Bar(rcScrollbar, RGB565(b0b0b0));

        using namespace Layout;
        
        for (int i=nTopIndex; i<nBottomIndex; i++)
        {
            int ts;
            char name[64];
            char desc[64];
            char time[8];
            appData.GetCaptureRecord(i, ts, name, desc);
            
            ts /= 1000;
            sprintf(time, "%02d:%02d", ts/60, ts%60);
                      
            bool focus = HasFocus() && (mFocus + mScroll) == i;
            Color def(focus ? RGB565(000000) : RGB565(b0b0b0));
            Color hig(RGB565(ffffff));

            Render r(rcElement);
            r   << Rectangle(focus ? RGB565(808080) : RGB565(101010))
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
    }
    
    virtual void OnKey(int key) override
    {
        if (key == BIOS::KEY::Up)
        {
            if (mFocus > 0)
            {
                mFocus--;
                Invalidate();
                return;
            } else
            if (mScroll > 0)
            {
                mScroll--;
                Invalidate();
                return;
            }
        }
        if (key == BIOS::KEY::Down)
        {
            if (mFocus + mScroll < appData.GetCaptureRecords()-1)
            {
                if (mFocus >= 5)
                    mScroll++;
                else
                    mFocus++;
                Invalidate();
                return;
            }
        }
        CWnd::OnKey(key);
    }

};
