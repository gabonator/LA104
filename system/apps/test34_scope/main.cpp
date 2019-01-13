#ifdef __APPLE__
#include <math.h>
#endif
#include <library.h>
#include <algorithm>

constexpr int gConfigMaxSensors = 8;
constexpr int gDecimationLimit = 2; // 2x oversampling max

#include "graph/palette.h"
#include "graph/draw.h"
#include "graph/graph.h"
#include "graph/series.h"
#include "graph/axis.h"

#include "sensor/pcf8591t.h"

#define EVERY(ms) static long dwTick##__LINE__ = 0; bool bDo##__LINE__ = BIOS::SYS::GetTick() - dwTick##__LINE__ > ms; if (bDo##__LINE__) dwTick##__LINE__ = BIOS::SYS::GetTick(); if (bDo##__LINE__)

class CDeviceManager
{
//    bool mWorking{false};
    
public:
#ifdef __APPLE__
    bool Demo(CArray<int>& meas)
    {
        for (int i=0; i<meas.GetSize(); i++)
            meas[i] = CSeriesBase::Invalid;

        long now = BIOS::SYS::GetTick();

        meas[0] = (int)(sin(now*0.0002)*128+128);
        meas[3] = (now % 20000 > 6000) ? 0 : 255;
        meas[7] = ((now/2)%1000)*128/1000;
        return true;
    }
#endif
    
    bool GetData(CArray<int>& meas)
    {
#ifdef __APPLE__
        return Demo(meas);
#endif
        CPCF8591 sensor;
        sensor.begin(0);
        
        for (int i=0; i<8; i++)
            meas[i] = CSeriesBase::Invalid;

        for (int i=0; i<4; i++)
            meas[i] = sensor.analogRead(i);
                        
        return true;
    }
};


class CXAxisTime : public CXAxis
{
    const char* units{""};
    int multiply{1};
    
protected:
    virtual void FormatNumber(char* buf, int i)
    {
        sprintf(buf, "%d%s", i, units);
    }
    
    virtual int reconstruct(int i)
    {
        return i*multiply;
    }

public:
    void SelectUnits(int& value)
    {
        multiply = 1000;
        value /= 1000;
        
        // seconds;
        if (value < 3)
            return;
        
        units = "s";
        if (value < 2*60)
            return;
        
        // minutes
        units = "m";
        multiply *= 60;
        value /= 60;
        if (value < 2*60)
            return;

        // hours
        units = "h";
        multiply *= 60;
        value /= 60;
    }

    virtual void Update(int value)
    {
        int simplifiedValue = value;
        SelectUnits(simplifiedValue);
        Redraw(simplifiedValue, value);
    }
};

class CYAxisVoltage : public CYAxis
{
protected:
    virtual int forward(int i)
    {
        return i*30/255;
    }
    
    virtual int backward(int i)
    {
        return i*255/30;
    }
    
    virtual void FormatNumber(char* buf, int i)
    {
        sprintf(buf, "%d.%dV", i/10, i%10);
    }
};

class CApplication : public CWnd
{
    CGraph mWndGraph;
    CXAxisTime mXAxis;
    CYAxisVoltage mYAxis;
    
    CSimpleSeries mSeriesData[gConfigMaxSensors];
    CArray<CSimpleSeries> mSeries;
    
    CDeviceManager mDevice;
    
public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        mSeries.Init(mSeriesData, COUNT(mSeriesData));
        mSeries.SetSize(COUNT(mSeriesData));
        
        CWnd::Create(pszId, dwFlags, rc, pParent);
        CRect rcGraph(10+4+40, 16+20+20, BIOS::LCD::Width-10-4, BIOS::LCD::Height-20-8-16+4);

        mWndGraph.Create("graph", CWnd::WsVisible, rcGraph, this);
        for (int i=0; i<mSeries.GetSize(); i++)
            mWndGraph.AddSeries(&mSeries[i]);

        CRect rcXAxis(10+4+40, BIOS::LCD::Height-20-8-16+4, BIOS::LCD::Width-10-4, BIOS::LCD::Height-20-8+4);
        mXAxis.Create("xaxis", CWnd::WsVisible, rcXAxis, this);

        CRect rcYAxis(10+4, 16+20+20, 10+4+40, BIOS::LCD::Height-20-8-16+4);
        mYAxis.Create("yaxis", CWnd::WsVisible, rcYAxis, this);

        SetTimer(20);
    }

    void OnPaint()
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
        
        CRect rc1(m_rcClient);
        rc1.bottom = 14;
        GUI::Background(rc1, RGB565(4040b0), RGB565(404040));

        BIOS::LCD::Print(8, 0, RGB565(ffffff), RGBTRANS, "4 Channel Oscilloscope (PCF8591)");

        CRect rc2(10, 16+20, 310, 220);
        GUI::Window(rc2, RGB565(ffffff));
        
        mWndGraph.SetRange(Range{0, 20*16});
        mYAxis.Update(mWndGraph.GetRange());
        mXAxis.Update(5000);
        
        ShowMeasurement(-1, 0);
    }
    
    uint_fast16_t InterpolateColor( uint_fast16_t clrA, uint_fast16_t clrB, uint_fast8_t nLevel )
    {
        int br = Get565R(clrB)*nLevel;
        int bg = Get565G(clrB)*nLevel;
        int bb = Get565B(clrB)*nLevel;
        nLevel = 255-nLevel;
        int ar = Get565R(clrA)*nLevel;
        int ag = Get565G(clrA)*nLevel;
        int ab = Get565B(clrA)*nLevel;
        ar = (ar+br) / 256;
        ag = (ag+bg) / 256;
        ab = (ab+bb) / 256;
        return RGB565RGB(ar, ag, ab);
    }

    void ShowMeasurement(int index, int value)
    {
        static bool nosignal = false;
        if (index == -1)
        {
            nosignal = true;
            BIOS::LCD::Print(10+16+50, 16+20+2, RGB565(000000), RGB565(ffffff), "No sensor detected...");
            return;
        }
        
        if (nosignal)
        {
            nosignal = false;
            BIOS::LCD::Bar(10+8, 16+20+2, 310-8, 16+20+2+14, RGB565(ffffff));
        }
        
        if (index>=4)
            return;
        
        int color = CPalette::GetColor(index);
        int colorY = (Get565R(color) + Get565G(color)*2 + Get565B(color))/4;
        if (colorY > 150)
            color = InterpolateColor(color, RGB565(000000), 128);
        
        if (value == CSeriesBase::Invalid)
            BIOS::LCD::Printf(10+16+index*64, 16+20+2, color, RGB565(ffffff), "        ");
        else
        {
            char buf[16];
            int volt = value*30/255;
            if (value > 0)
                sprintf(buf, "%d.%dV ", volt/10, volt%10);
            else
            {
                volt = -volt;
                sprintf(buf, "%d.%dV ", volt/10, volt%10);
            }
            BIOS::LCD::Print(10+16+index*64, 16+20+2, color, RGB565(ffffff), buf);
        }
    }
    
    void PushData()
    {
        int dataPlacement[gConfigMaxSensors];
        CArray<int> data;
        data.Init(dataPlacement, COUNT(dataPlacement));
        data.SetSize(COUNT(dataPlacement));

        if (mDevice.GetData(data))
        {
            _ASSERT(data.GetSize() == mSeries.GetSize());
            for (int i=0; i<data.GetSize(); i++)
                mSeries[i] << data[i];
            EVERY(500)
            {
                for (int i=0; i<data.GetSize(); i++)
                    ShowMeasurement(i, data[i]);
            }
        }
    }
    
    void OnTimer()
    {
        static int counter = 0;
        
        PushData();
        counter++;

        bool needRedraw = mWndGraph.Redraw();

        if (needRedraw || (counter > 5 && mSeries[0].GetLength() > 5))
        {
            mYAxis.Update(mWndGraph.GetRange());
            
            static int lastestimated = 0;

            counter = 0;
            int duration = mSeries[0].GetTotalDuration();
            int estimated = 0;
            if (mSeries[0].GetLength() > 5)
                estimated = duration * mSeries[0].GetMaxLength() / mSeries[0].GetLength();
            
            if (estimated < 5000)
                estimated = 5000;
            
            if (abs(estimated - lastestimated) < estimated / 50)
                return;
            
            lastestimated = estimated;
            mXAxis.Update(estimated);
        }
    }
};

CApplication app;

#ifndef __APPLE__
__attribute__((__section__(".entry")))
#endif
int _main(void)
{ 
    app.Create("", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
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

