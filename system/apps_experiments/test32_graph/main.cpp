#include <library.h>
#include "shapes.h"
#include <math.h>
#include <algorithm>

#include "palette.h"
#include "draw.h"
#include "graph.h"
#include "series.h"
#include "axis.h"

class CSourceBase
{
public:
    virtual int Get() = 0;
};

class CSourceSquare : public CSourceBase
{
public:
    virtual int Get()
    {
        if (BIOS::SYS::GetTick()%5000 > 2500)
            return 10*16;
        else
            return 5*16;
    }
};

class CSourceHarmonic : public CSourceBase
{
public:
    int k{1};
    virtual int Get()
    {
        return (int)(sin(BIOS::SYS::GetTick()/1000.0f*M_PI/20*k)*128+128.0);
    }
};

class CSourceTwoHarmonic : public CSourceBase
{
public:
    float k1{0.27f*40};
    float k2{0.25f*40};
    
    int Get()
    {
        return (int)(-sin(BIOS::SYS::GetTick()/1000.0f*M_PI/20*k1)*128 + sin(BIOS::SYS::GetTick()/1000.0f*M_PI/20*k2)*64+128.0);
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

class CYAxisTemperature : public CYAxis
{
protected:
    virtual int forward(int i)
    {
        return i/16;
    }
    
    virtual int backward(int i)
    {
        return i*16;
    }
    
    virtual void FormatNumber(char* buf, int i)
    {
        if (i>0)
            sprintf(buf, "%d" "\xf8" "C", i);
        else
            sprintf(buf, "%d", i);
    }

};

class CApplication : public CWnd
{
    CGraph mWndGraph;
    CXAxisTime mXAxis;
    CYAxisTemperature mYAxis;
    CSourceSquare mSource0;
    CSourceHarmonic mSource1;
    CSourceHarmonic mSource2;
    CSourceTwoHarmonic mSource3;

    CSourceBase* mSourcesData[4];
    CArray<CSourceBase*> mSources;
    
    CSimpleSeries mSeriesData[4];
    CArray<CSimpleSeries> mSeries;
    
public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        mSeries.Init(mSeriesData, COUNT(mSeriesData));
        mSeries.SetSize(COUNT(mSeriesData));
        
        CWnd::Create(pszId, dwFlags, rc, pParent);
        CRect rcGraph(10+4+40, 16+20+20, BIOS::LCD::Width-10-4, BIOS::LCD::Height-20-8-16+4);

        mWndGraph.Create("graph", CWnd::WsVisible, rcGraph, this);
        mWndGraph.AddSeries(&mSeries[0]);
        mWndGraph.AddSeries(&mSeries[1]);
        mWndGraph.AddSeries(&mSeries[2]);
        mWndGraph.AddSeries(&mSeries[3]);

        CRect rcXAxis(10+4+40, BIOS::LCD::Height-20-8-16+4, BIOS::LCD::Width-10-4, BIOS::LCD::Height-20-8+4);
        mXAxis.Create("xaxis", CWnd::WsVisible, rcXAxis, this);

        CRect rcYAxis(10+4, 16+20+20, 10+4+40, BIOS::LCD::Height-20-8-16+4);
        mYAxis.Create("yaxis", CWnd::WsVisible, rcYAxis, this);

        SetTimer(5);
        mSource1.k = 2;
        mSource2.k = 3;
        
        mSources.Init(mSourcesData, COUNT(mSourcesData));
        mSources.Add(&mSource0);
        mSources.Add(&mSource1);
        mSources.Add(&mSource2);
        mSources.Add(&mSource3);
    }

    void OnPaint()
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));

        CRect rc1(m_rcClient);
        rc1.bottom = 14;
        GUI::Background(rc1, RGB565(4040b0), RGB565(404040));

        CRect rc2(10, 16+20, 310, 220);
        GUI::Window(rc2, RGB565(ffffff));
    }
    
    void OnTimer()
    {
        static long first = 0;
        static int counter = 0;
        long now = BIOS::SYS::GetTick();
        if (first == 0)
            first = now;
        
        for (int i=0; i<mSeries.GetSize(); i++)
            mSeries[i] << mSources[i]->Get();

        mWndGraph.Redraw();

        if (counter++ > 10)
        {
            mYAxis.Update(mWndGraph.GetRange());
            
            static int lastestimated = 0;

            counter = 0;
            int duration = now - first;
            int estimated = duration * mSeries[0].GetMaxLength() / mSeries[0].GetLength();
            
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

