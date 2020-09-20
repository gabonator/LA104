#include "signalview.h"

class CDetails : public CWnd
{
    CSignalView mSignalView;
    CScroller mScroller{5, nullptr};
    int mCaptureUid{0};
    bool mRedraw{false};
    uint8_t mAttrIndices[16];
    int mAttrCount{0};
    
public:
    void Create( const char* pszId, int dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        mSignalView.Create("signalview", CWnd::WsNoActivate, CRect(m_rcClient.left+4, m_rcClient.bottom-32-6, m_rcClient.right-4, m_rcClient.bottom-6), this);
        mAttrCount = 0;
    }

    CArray<uint16_t>& GetWave()
    {
        return mSignalView.GetWave();
    }
    
    CProtocol* GetProtocol()
    {
        int index = appData.GetCaptureIndex(mCaptureUid);
        _ASSERT(index >= 0);
        CProtocol* p = appData.GetRecordProtocol(index);
        _ASSERT(p);
        return p;
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
        using namespace Layout;
        Color def(RGB565(b0b0b0), RGBTRANS);
        Color hig(RGB565(ffffff), RGBTRANS);
        
        int captureIndex = appData.GetCaptureIndex(mCaptureUid);

        if (captureIndex < 0)
            return;
        
        mScroller.SetCount(mAttrCount, 4);
        
        char name[64];
        char value[32];
        char units[8];

        Render r(m_rcClient);
        if (mRedraw)
        {
            mRedraw = false;
            int ts, uid;
            appData.GetCaptureRecord(captureIndex, ts, uid, name, nullptr);
            r << Window(name);
        } else
        {
            r << Padding(8, 20, 8, 40) << Bar(RGB565(404040));
        }
        
        r << def;
        //ShowRawData(r, appData.GetAttributes(captureIndex));
        //r << def << NewLine();
        
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
            appData.GetCaptureAttribute(captureIndex, mAttrIndices[i], name, value, units);
            r << name << ":" << Select(HasFocus() && mScroller.mFocus + mScroller.mScroll == i) << Units(value, units) << Select(false) << def << NewLine();
        }
        //r << Goto(m_rcClient.CenterX()) << Select(HasFocus() && mScroller.mFocus == mScroller.mRows) << Button((char*)"Transmit");
        
        int last = min(mScroller.mRows, mAttrCount);
        
        r << Goto(m_rcClient.left + 15)
          << Select(HasFocus() && mScroller.mFocus == last) << Button((char*)"Delete")
          << Select(HasFocus() && mScroller.mFocus == last+1) << Button((char*)"Export")
          << Select(HasFocus() && mScroller.mFocus == last+2) << Button((char*)"Transmit");
        
        DrawScrollbar(HasFocus() && mScroller.mFocus == last+3);
    }
    
    void ShowRawData(Layout::Render& r, const CAttributes& attr)
    {
        int bits = attr["length"];
        int ofs = 0;
        char temp[16];
        sprintf(temp, "%d", bits);
        r << temp << " bits: " << Layout::Color(RGB565(ffffff), RGBTRANS);;
        int word = 0;
        uint32_t d = 0;
        do
        {
            switch(word++)
            {
                case 0: d = attr["data_0"]; break;
                case 1: d = attr["data_1"]; break;
                case 2: d = attr["data_2"]; break;
            }
            int shownibs = (min(bits-ofs, 32)+3)/4;
            char fmt[8];
            sprintf(fmt, "%%0%dx", shownibs); // %0?x
            sprintf(temp, fmt, d >> ((8-shownibs)*4));
            r << temp << Layout::Spacer(2);
            ofs += shownibs*4;
        } while (ofs<bits);
    }
    
    void DrawScrollbar(bool focus)
    {
        //bool focus = HasFocus() && mScroller.mFocus == mScroller.mRows+3;
                   
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
    
    void SetUid(int uid)
    {
        mCaptureUid = uid;
        int index = appData.GetCaptureIndex(uid);
        mAttrCount = 0;

        const CAttributes& attr = appData.GetAttributes(index);
        for (int j = 0; j<attr.GetSize(); j++)
        {
            const char* key = attr[j].key;
            if (key[0] != '_' /* && strstr(key, "data_") == nullptr && strcmp(key, "length") != 0*/)
                mAttrIndices[mAttrCount++] = j;
        }
        mSignalView.ShowWindow(true);
    }

    virtual void OnKey(int key) override
    {
        int last = min(mScroller.mRows, mAttrCount);

        if (key == BIOS::KEY::Escape)
        {
            SendMessage(GetParent(), 0, (uintptr_t)"close");
            return;
        }
        if (key == BIOS::KEY::Enter)
        {
            if (mScroller.mFocus == last)
            {
                SendMessage(GetParent(), 0, (uintptr_t)"delete");
            }
            else if (mScroller.mFocus == last+1)
            {
                SendMessage(GetParent(), 0, (uintptr_t)"export");
            }
            else if (mScroller.mFocus == last+2)
            {
                SendMessage(GetParent(), 0, (uintptr_t)"transmit");
            }
        }
        if (mScroller.mFocus == last+3)
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
                DrawScrollbar(true);
            }
            if (key == BIOS::KEY::Left)
            {
                mSignalView.mSettings_mSignalOffset -= mSignalView.mSettings_mSignalScaleX*10;
                mSignalView.mSettings_mSignalOffset = max(0, mSignalView.mSettings_mSignalOffset);
                DrawScrollbar(true);
            }
            if (key == BIOS::KEY::Right)
            {
                mSignalView.mSettings_mSignalOffset += mSignalView.mSettings_mSignalScaleX*10;
                mSignalView.mSettings_mSignalOffset = max(0, mSignalView.mSettings_mSignalOffset);
                DrawScrollbar(true);
            }
        } else
        if (mScroller.mFocus < last)
        {
            if (key == BIOS::KEY::Left)
            {
                int captureIndex = appData.GetCaptureIndex(mCaptureUid);
                if (captureIndex != -1)
                    appData.DeltaCaptureAttribute(captureIndex, mAttrIndices[mScroller.mFocus + mScroller.mScroll], -1);
                Invalidate();
            }
            if (key == BIOS::KEY::Right)
            {
                int captureIndex = appData.GetCaptureIndex(mCaptureUid);
                if (captureIndex != -1)
                    appData.DeltaCaptureAttribute(captureIndex, mAttrIndices[mScroller.mFocus + mScroller.mScroll], +1);
                Invalidate();
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
