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
    
    virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data) override
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
    }
    
    void DrawSignalStats(uint16_t* p, int n)
    {
        int total = 0;
        for (int i=0; i<n; i++)
            total += p[i];

        CRect rcStatus(m_rcClient);
        rcStatus.top = rcStatus.bottom - 16;
        GUI::Background(rcStatus, RGB565(404040), RGB565(101010));
        BIOS::LCD::Printf(8, m_rcClient.bottom-16, RGB565(ffffff), RGBTRANS, "%3d    %3d  ", total/1000, n);
        BIOS::LCD::Printf(8, m_rcClient.bottom-16, RGB565(b0b0b0), RGBTRANS, "    ms     t");

    }
    
    virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data) override
    {
        if (code == ToWord('D', 'A')) // new data
        {
            mHistogram.Redraw();
            mSignal.Redraw();
            DrawSignalStats(mStorage.mSignalData, mStorage.mSignalLength);

            if (mStorage.mSignalLength > 10)
            {
                if (!AnalyseManchester())
                    AnalysePulse();
            }
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
    
    
    bool AnalyseManchester()
    {
        int last = -1;

        uint8_t buffer[200];
        int bufferi = 0;
        auto PushManchester = [&](int n)
        {
            if (last == -1)
            {
                last = n;
                return true;
            }
            if (last == 1 && n == 0)
            {
                buffer[bufferi++] = 1;
            }
            else if (last == 0 && n == 1)
            {
                buffer[bufferi++] = 0;
            }
            else
                return false;
            
            if (bufferi >= COUNT(buffer))
            {
                //_ASSERT(!"Buffer too small");
                return false;
            }
            
            last = -1;
            return true;
        };
        
        for (int i=0, b=1; i<mStorage.mSignalLength; i++, b=1-b)
        {
            int v = mStorage.mSignalData[i];
            if (v >= 300 && v < 700)
            {
                if (!PushManchester(b))
                    return false;
            }
            if (v >= 900 && v < 1100)
            {
                if (!PushManchester(b))
                    return false;
                if (!PushManchester(b))
                    return false;
            }
        }
        
        PushManchester(0);
        
        int i = 0;
        for (int i=0; i<bufferi; i++)
            if (buffer[i] == 0)
                break;
        
        for (; i<bufferi; i++)
            if (buffer[i] == 0)
                break;
        
        uint8_t nibbles[64];
        int nibbleCount = 0;
        for (; i<bufferi-3; i+=4, nibbleCount++)
            nibbles[nibbleCount] = buffer[i] + buffer[i+1]*2 + buffer[i+2]*4 + buffer[i+3]*8;


        int x = 8, y = 200;
        x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGB565(101010), "Manch %d: ", bufferi);
        for (int i=0; i<nibbleCount; i++)
        {
            x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGB565(101010), "%x", nibbles[i]);
            if ( x >= BIOS::LCD::Width)
            {
                x = 8;
                y += 14;
            }
        }
/*
        auto checkOregonCrc = [](uint8_t* p, int count)
        {
            int sum = 0;
            for (int i=1; i<count; i++)
                sum += p[i];
            return (sum & 0x0f) == p[count];
        };
  */
        if (nibbleCount == 20 && nibbles[0] == 0xA && nibbles[1] == 0xF) // AF
        {
            // Oregon temperature sensor
            int temperature10 = nibbles[11]*100 + nibbles[10]*10 + nibbles[9]*1;
            if (nibbles[12] != 0)
                temperature10 = -temperature10;
            int humidity = nibbles[14]*10 + nibbles[13];
            
            x = 8;
            y += 14;
            BIOS::LCD::Printf(x, y, RGB565(ffffff), RGB565(101010), "Oregon: temp=%d.%d humidity=%d%%     ", temperature10/10, temperature10%10, humidity/*, checkOregonCrc(nibbles, 19) ? "ok" : "err"*/);
        }
        if (nibbleCount == 23 && nibbles[0] == 0xA && nibbles[1] == 0x2) // AF
        {
            // Oregon temperature sensor
            int rainTotal = (int)(((nibbles[18]*100000)+(nibbles[17]*10000)+(nibbles[16]*1000)+(nibbles[15]*100)+(nibbles[14]*10)+nibbles[13])*24.2f/1000);
            int rainRate = (int)(((nibbles[7]*100000)+(nibbles[8]*10000)+(nibbles[9]*1000)+(nibbles[10]*100)+(nibbles[11]*10)+nibbles[12])*24.2f/1000);
            
            x = 8;
            y += 14;
            BIOS::LCD::Printf(x, y, RGB565(ffffff), RGB565(101010), "Oregon: rain=%dmm rate=%dmm/hr   ", rainTotal, rainRate);
        }
        
        /*
        if ( substr($rec, 0, 2) == "a1" && oregonCrc($nibble, 18) &&
            oregonDigit($nibble[16]) && oregonDigit($nibble[15]) &&
            oregonDigit($nibble[13]) && oregonDigit($nibble[12]) )
        {
            $aux["windspeed"] = (($nibble[16]*10)+ $nibble[15])*3.6/10;
            $aux["units_windspeed"] = "kmh";
            $aux["gusts"] = (($nibble[13]*10)+ $nibble[12])*3.6/10;
            $aux["units_gusts"] = "kmh";
            $aux["heading"] = $nibble[9];
            $aux["units_heading"] = "quadrant16";
        }
        if ( substr($rec, 0, 2) == "a2" && oregonCrc($nibble, 19) &&
            oregonDigit($nibble[18]) && oregonDigit($nibble[17]) && oregonDigit($nibble[16]) && oregonDigit($nibble[15]) &&oregonDigit($nibble[14]) && oregonDigit($nibble[13]) &&
            oregonDigit($nibble[7]) && oregonDigit($nibble[8]) && oregonDigit($nibble[9]) && oregonDigit($nibble[10]) &&oregonDigit($nibble[11]) && oregonDigit($nibble[12]) )
        {
            $aux["raintotal"] = (($nibble[18]*100000)+($nibble[17]*10000)+($nibble[16]*1000)+($nibble[15]*100)+($nibble[14]*10)+$nibble[13])*24.2/1000;
            $aux["units_raintotal"] = "mm";
            $aux["rainrate"] = (($nibble[7]*100000)+($nibble[8]*10000)+($nibble[9]*1000)+($nibble[10]*100)+($nibble[11]*10)+$nibble[12])*24.2/1000;
            $aux["units_rainrate"] = "mm per hr";
        }
    }*/

        return true;
    }

    
    bool AnalysePulse()
    {
        /*
        int last = -1;
        
        uint8_t buffer[200];
        int bufferi = 0;
        auto PushManchester = [&](int n)
        {
            if (last == -1)
            {
                last = n;
                return true;
            }
            if (last == 1 && n == 0)
            {
                buffer[bufferi++] = 1;
            }
            else if (last == 0 && n == 1)
            {
                buffer[bufferi++] = 0;
            }
            else
                return false;
            
            if (bufferi >= COUNT(buffer))
            {
                _ASSERT(!"Buffer too small");
                return false;
            }
            
            last = -1;
            return true;
        };
        
        for (int i=0, b=1; i<mStorage.mSignalLength; i++, b=1-b)
        {
            int v = mStorage.mSignalData[i];
            if (v >= 300 && v < 700)
            {
                if (!PushManchester(b))
                    return false;
            }
            if (v >= 900 && v < 1100)
            {
                if (!PushManchester(b))
                    return false;
                if (!PushManchester(b))
                    return false;
            }
        }
         */
        return false;
    }

};
