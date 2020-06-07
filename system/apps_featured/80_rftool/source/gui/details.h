#include "signalview.h"

class CDetails : public CWnd
{
    CSignalView mSignalView;
    int mFocus{0};
    int mScroll{0};
    int mAttributes{0};
    int mCaptureIndex{0};
    
public:
    void Create( const char* pszId, int dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        mSignalView.Create("signalview", CWnd::WsVisible | CWnd::WsNoActivate, CRect(m_rcClient.left+4, m_rcClient.bottom-32-6, m_rcClient.right-4, m_rcClient.bottom-6), this);
        mAttributes = appData.GetCaptureAttributes(mCaptureIndex);
    }

    virtual void OnPaint() override
    {
        using namespace Layout;
        Color def(RGB565(b0b0b0), RGBTRANS);
        Color hig(RGB565(ffffff), RGBTRANS);

        int attributes = appData.GetCaptureAttributes(mCaptureIndex);
        
        char name[64];
        char value[32];
        char units[8];
        int ts;
        appData.GetCaptureRecord(mCaptureIndex, ts, name, nullptr);

        Render r(m_rcClient);
        r << Window(name) << def;
        for (int i=0; i<attributes; i++)
        {
            appData.GetCaptureAttribute(mCaptureIndex, i, name, value, units);
            r << name << ":" << Select(HasFocus() && mFocus == i) << Units(value, units) << Select(false) << def << NewLine();
        }
        r << Goto(m_rcClient.CenterX()) << Select(HasFocus() && mFocus == mAttributes) << Button((char*)"Transmit");
        
        DrawScrollbar();
    }
    
    void DrawScrollbar()
    {
        bool focus = HasFocus() && mFocus == mAttributes+1;
                   
        CRect rcScrollbar(mSignalView.m_rcClient.left+4, mSignalView.m_rcClient.bottom-1, mSignalView.m_rcClient.right-4, mSignalView.m_rcClient.bottom+1);
        BIOS::LCD::Bar(rcScrollbar, focus ? RGB565(808080) : RGB565(202020));
        
        int left, right, total;
        mSignalView.Redraw();
        mSignalView.GetRange(left, right, total);
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
        if (key == BIOS::KEY::Enter && mFocus + mScroll == mAttributes+1)
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
        if (key == BIOS::KEY::Left && mFocus + mScroll == mAttributes+1)
        {
            mSignalView.mSettings_mSignalOffset -= mSignalView.mSettings_mSignalScaleX*10;
            mSignalView.mSettings_mSignalOffset = max(0, mSignalView.mSettings_mSignalOffset);
            DrawScrollbar();
        }
        if (key == BIOS::KEY::Right && mFocus + mScroll == mAttributes+1)
        {
            mSignalView.mSettings_mSignalOffset += mSignalView.mSettings_mSignalScaleX*10;
            mSignalView.mSettings_mSignalOffset = max(0, mSignalView.mSettings_mSignalOffset);
            DrawScrollbar();
        }
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
            if (mFocus + mScroll < mAttributes-1+2)
            {
                if (mFocus >= 5 && mFocus + mScroll < mAttributes-1)
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
