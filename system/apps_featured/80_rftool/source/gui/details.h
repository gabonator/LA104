#include "signalview.h"

class CDetails : public CWnd
{
    CSignalView mSignalView;
    CScroller mScroller{6, nullptr};
    int mAttributes{0};
    int mCaptureIndex{0};
    bool mRedraw{false};
    
public:
    void Create( const char* pszId, int dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        mSignalView.Create("signalview", CWnd::WsVisible | CWnd::WsNoActivate, CRect(m_rcClient.left+4, m_rcClient.bottom-32-6, m_rcClient.right-4, m_rcClient.bottom-6), this);
        mAttributes = 0; //appData.GetCaptureAttributesCount(mCaptureIndex);
    }

    CArray<uint16_t>& GetWave()
    {
        return mSignalView.GetWave();
    }
    
    virtual void WindowMessage(int nMsg, int nParam /*=0*/) override
    {
        if (nMsg == WmWillShow)
        {
            mRedraw = true;
        }
        CWnd::WindowMessage(nMsg, nParam);
    }

    virtual void OnPaint() override
    {
        // TODO: Capture Index zmenit za uid
        using namespace Layout;
        Color def(RGB565(b0b0b0), RGBTRANS);
        Color hig(RGB565(ffffff), RGBTRANS);

        int attributes = appData.GetCaptureAttributesCount(mCaptureIndex);
        mScroller.SetCount(attributes, 2);
        mAttributes = attributes;// TODO: !
        
        char name[64];
        char value[32];
        char units[8];
        //int ts;
        //appData.GetCaptureRecord(mCaptureIndex, ts, nullptr, nullptr);

        Render r(m_rcClient);
        if (mRedraw)
        {
            mRedraw = false;
            int ts;
            appData.GetCaptureRecord(mCaptureIndex, ts, name, nullptr);
            r << Window(name);
        } else
        {
            r << Padding(8, 20, 8, 40) << Bar(RGB565(404040));
        }

        r << def;
        int top, bottom;
        mScroller.GetRange(top, bottom);
        
        if (mScroller.mMax > mScroller.mRows && mScroller.mRows != 0)
        {
            CRect rcScrollbar(m_rcClient.right-12, m_rcClient.top + 20, m_rcClient.right-10, m_rcClient.top + 20 + 16*mScroller.mRows);
            int nScrollTop = rcScrollbar.top + top*rcScrollbar.Height()/mScroller.mMax;
            int nScrollBottom = rcScrollbar.top + bottom*rcScrollbar.Height()/mScroller.mMax;
            BIOS::LCD::Bar(rcScrollbar, RGB565(606060));
            rcScrollbar.top = nScrollTop;
            rcScrollbar.bottom = nScrollBottom;
            BIOS::LCD::Bar(rcScrollbar, RGB565(b0b0b0));
        }

        for (int i=top; i<bottom; i++)
        {
            appData.GetCaptureAttribute(mCaptureIndex, i, name, value, units);
            if (name[0] != '_')
                r << name << ":" << Select(HasFocus() && mScroller.mFocus + mScroller.mScroll == i) << Units(value, units) << Select(false) << def << NewLine();
        }
        r << Goto(m_rcClient.CenterX()) << Select(HasFocus() && mScroller.mFocus == mScroller.mRows) << Button((char*)"Transmit");
        
        DrawScrollbar();
    }
    
    void DrawScrollbar()
    {
        bool focus = HasFocus() && mScroller.mFocus == mScroller.mRows+1;
                   
        CRect rcScrollbar(mSignalView.m_rcClient.left+4, mSignalView.m_rcClient.bottom-1, mSignalView.m_rcClient.right-4, mSignalView.m_rcClient.bottom+1);
        BIOS::LCD::Bar(rcScrollbar, focus ? RGB565(808080) : RGB565(202020));
        
        int left, right, total;
        mSignalView.Redraw();
        mSignalView.GetRange(left, right, total);
        if (total == 0)
            return;
        CRect rcHighlight(rcScrollbar);
        rcHighlight.left = rcScrollbar.left + left*rcScrollbar.Width()/total;
        rcHighlight.right = rcScrollbar.left + right*rcScrollbar.Width()/total;
        rcHighlight.right = min(rcHighlight.right, rcScrollbar.right);
        BIOS::LCD::Bar(rcHighlight, focus ? RGB565(ffffff) : RGB565(b0b0b0));
    }
    
    void DrawWave(const CRect& rc)
    {
        //BIOS::LCD::Bar(rc, RGB565(000000));
    }

    virtual void OnKey(int key) override
    {
        if (mScroller.mFocus == mScroller.mRows+1)
        {
            if (key == BIOS::KEY::Enter)
            {
                switch (mSignalView.mSettings_mSignalScaleX)
                {
                    case 200: mSignalView.mSettings_mSignalScaleX = 500; break;
                    case 500: mSignalView.mSettings_mSignalScaleX = 20; break;
                    case 20: mSignalView.mSettings_mSignalScaleX = 50; break;
                    case 50: mSignalView.mSettings_mSignalScaleX = 100; break;
                    case 100: mSignalView.mSettings_mSignalScaleX = 200; break;
                }
                DrawScrollbar();
            }
            if (key == BIOS::KEY::Left)
            {
                mSignalView.mSettings_mSignalOffset -= mSignalView.mSettings_mSignalScaleX*10;
                mSignalView.mSettings_mSignalOffset = max(0, mSignalView.mSettings_mSignalOffset);
                DrawScrollbar();
            }
            if (key == BIOS::KEY::Right)
            {
                mSignalView.mSettings_mSignalOffset += mSignalView.mSettings_mSignalScaleX*10;
                mSignalView.mSettings_mSignalOffset = max(0, mSignalView.mSettings_mSignalOffset);
                DrawScrollbar();
            }
        }
        if (key == BIOS::KEY::Up)
        {
            if (mScroller.Up())
            {
                Invalidate();
                return;
            }
        }
        if (key == BIOS::KEY::Down)
        {
            if (mScroller.Down())
            {
                Invalidate();
                return;
            }
        }
        CWnd::OnKey(key);
    }

};
