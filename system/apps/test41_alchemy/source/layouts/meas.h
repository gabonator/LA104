#include "meas/menuitems.h"
#include "../controls/histogram.h"
#include "../controls/signalview.h"

class CLayoutMeasMenu : public CWnd
{
    friend class CLayoutMeas;
    
    CItemMode mWndMode; // Scan/pause/single
    CItemHistogramX mWndHistogramX; // Histo resol
    CItemHistogramY mWndHistogramY; // Histo sensit
    CItemSignalOfs mWndSignalOffset;  // Window
    CItemSignalX mWndSignalZoom; // Zoom
    
public:
    void Create(CWnd* pParent, const CRect& rc)
    {
        CWnd::Create("CLayoutMeasMenu", CWnd::WsVisible, rc , pParent);

        mWndMode.Create(RGB565(b0b0b0), this);
        mWndHistogramX.Create(RGB565(b0b0b0), this);
        mWndHistogramY.Create(RGB565(b0b0b0), this);
        mWndSignalOffset.Create(RGB565(b0b0b0), this);
        mWndSignalZoom.Create(RGB565(b0b0b0), this);
    }
    
    virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
    {
        m_pParent->OnMessage(pSender, code, data);
    }
};

class CLayoutMeas : public CWnd
{
    CLayoutMeasMenu mMenu;
    CHistogramView mHistogram;
    CSignalView mSignal;

public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        constexpr int menuWidth = 80+CWndMenuItem::MarginLeft;
        constexpr int padding = 4;
        constexpr int histogramHeight = 100;
        constexpr int spacing = 32;
        constexpr int signalHeight = 33;
        constexpr int caption = 14;

        CWnd::Create(pszId, dwFlags, rc, pParent);
        mMenu.Create(this, CRect(BIOS::LCD::Width-menuWidth, 20, BIOS::LCD::Width, BIOS::LCD::Height));
        
        CRect rcHistogram(m_rcClient.left + padding, m_rcClient.top + padding + caption,
                          m_rcClient.right - menuWidth - padding, m_rcClient.top + padding + histogramHeight);
        CRect rcSignal(rcHistogram.left, rcHistogram.bottom + spacing + caption,
                       rcHistogram.right, rcHistogram.bottom + spacing + signalHeight + caption);
        
        mHistogram.Create("Histogram", CWnd::WsNoActivate | CWnd::WsVisible, rcHistogram, this);
        mSignal.Create("Signal", CWnd::WsNoActivate | CWnd::WsVisible, rcSignal, this);
    }

    virtual void OnPaint() override
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
        
        int x = 8;
        int y = m_rcClient.top + 4+100+4;
        BIOS::LCD::Draw(x, y, RGB565(b0b0b0), RGBTRANS, sigLTL );
        BIOS::LCD::Draw(x, y, RGB565(00ff00), RGBTRANS, sigXTX );
        x += 20;
        x += BIOS::LCD::Print(x, y, RGB565(00ff00), RGBTRANS, "High ");
        
        x = 8+80;
        BIOS::LCD::Draw(x, y, RGB565(b0b0b0), RGBTRANS, sigTLT );
        BIOS::LCD::Draw(x, y, RGB565(0080ff), RGBTRANS, sigXLX );
        x += 20;
        x += BIOS::LCD::Print(x, y, RGB565(0080ff), RGBTRANS, "Low ");

        x = 8+140;
        x += BIOS::LCD::Print(x, y, RGB565(b00000), RGBTRANS, "\xfe Overflow ");

        x = 4;
        y = mHistogram.m_rcClient.top - 14;
        x += BIOS::LCD::Print(x, y, RGB565(404040), RGB565(b0b0b0), " Histogram ");
        x += BIOS::LCD::Draw( x, y, RGB565(b0b0b0), RGBTRANS, CShapes_tab_right);
        
        x = 4; y = mSignal.m_rcClient.top - 14;
        x += BIOS::LCD::Print(x, y, RGB565(404040), RGB565(b0b0b0), " Signal ");
        x += BIOS::LCD::Draw( x, y, RGB565(b0b0b0), RGBTRANS, CShapes_tab_right);
        
        DrawSignalStats();
        Analyse();
    }
    
    void DrawSignalStats()
    {
        int total = 0;
        for (int i=0; i<mStorage.mSignalLength; i++)
            total += mStorage.mSignalData[i];

        CRect rcStatus(mSignal.m_rcClient.left+100, mSignal.m_rcClient.top - 14, mSignal.m_rcClient.right, mSignal.m_rcClient.top);
        GUI::Background(rcStatus, RGB565(404040), RGB565(101010));
        BIOS::LCD::Printf(rcStatus.left, rcStatus.top, RGB565(b0b0b0), RGBTRANS, "%3d    %3d  ", total/1000, mStorage.mSignalLength);
        BIOS::LCD::Printf(rcStatus.left, rcStatus.top, RGB565(808080), RGBTRANS, "    ms     t");
    }
    
    virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
    {
        if (code == ToWord('D', 'A')) // new data
        {
            mHistogram.Redraw();
            mSignal.Redraw();
            DrawSignalStats();
            Analyse();
        }

        if (pSender == &mMenu.mWndHistogramX || pSender == &mMenu.mWndHistogramY)
        {
            mHistogram.Redraw();
        }
        if (pSender == &mMenu.mWndSignalZoom || pSender == &mMenu.mWndSignalOffset)
        {
            mSignal.Redraw();
        }
    }
    
    void Analyse()
    {
        CRect rcInfo(8, BIOS::LCD::Height-16*2, BIOS::LCD::Width, BIOS::LCD::Height);
        for (int i=0; mCodecs[i]; i++)
        {
            CCodec* pCodec = mCodecs[i];
            if (pCodec->Decode(mStorage.mSignalData, mStorage.mSignalLength))
            {
                if (mRuntime.mRecording)
                {
                    char info[128];
                    pCodec->GetShortInfo(info);
                    char timestamp[16];
                    sprintf(timestamp, "%6d: ", mRuntime.mSeconds);
                    mWriter << timestamp << pCodec->Id() << ": " << info << "\n";
                }

                GUI::Background(rcInfo, RGB565(404040), RGB565(101010));
                BIOS::LCD::Print(rcInfo.left, rcInfo.top, RGB565(ffffff), RGBTRANS, pCodec->Id());
                rcInfo.top += 16;
                pCodec->DisplayShortInfo(rcInfo);
                return;
            }
        }
    }

};
