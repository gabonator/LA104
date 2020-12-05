#ifdef __APPLE__
#include <math.h>
#endif
#include <library.h>
#include <algorithm>

#include "../../os_host/source/framework/BufferedIo.h"

constexpr int gConfigMaxSensors = 8;

#include "graph/palette.h"
#include "graph/draw.h"
#include "graph/graph.h"
#include "graph/series.h"
#include "graph/axis.h"

#include "sensor/ds1820.h"
#include "logger.h"

#define EVERY(ms) static long dwTick##__LINE__ = 0; bool bDo##__LINE__ = BIOS::SYS::GetTick() - dwTick##__LINE__ > ms; if (bDo##__LINE__) dwTick##__LINE__ = BIOS::SYS::GetTick(); if (bDo##__LINE__)

class CDeviceManager
{
    CDS1820 mSensor;
    long mLastConversion{0};
    bool mWorking{false};
    
    uint32_t mSlotsPlacement[gConfigMaxSensors];
    CArray<uint32_t> mSlots;
    
public:
    CDeviceManager()
    {
        mSlots.Init(mSlotsPlacement, COUNT(mSlotsPlacement));
        for (int i=0; i<mSlots.GetSize(); i++)
            mSlots[i] = 0;
    }
    
    uint32_t GetUid(const CDS1820::TAddress& addr)
    {
        uint32_t* pDwords = (uint32_t*)&addr;
        return (pDwords[0] ^ pDwords[1]) | 1;
    }
    
    int GetTemperatureSlot(const CDS1820::TAddress& addr)
    {
        uint32_t uid = GetUid(addr);
        int index = mSlots.Find(uid);
        if (index >= 0)
            return index;
        
        if (mSlots.GetSize() < mSlots.GetMaxSize())
        {
            mSlots.Add(uid);
            return mSlots.GetSize()-1;
        }
        
        return -1;
    }

#ifdef __APPLE__
    bool Demo(CArray<int>& meas)
    {
        for (int i=0; i<meas.GetSize(); i++)
            meas[i] = CSeriesBase::Invalid;

        long now = BIOS::SYS::GetTick();

        meas[0] = (int)((sin(now*0.0002)*20+10)*16);
        meas[3] = (now % 20000 > 5000) ? 10*16 : -5*16;
        meas[7] = (int)((sin(now*0.0001)*5+15)*16);
        return true;
    }
#endif
    
    bool GetData(CArray<int>& meas, CArray<int>& readings)
    {
#ifdef __APPLE__
        return Demo(meas);
#endif
        
        long now = BIOS::SYS::GetTick();

        // waiting 2000 ms to finish measurement
        if (mLastConversion != 0 && now - mLastConversion >= 1000)
        {
            for (int i=0; i<meas.GetSize(); i++)
            {
                meas[i] = CSeriesBase::Invalid;
            }
            
            CDS1820::TAddress addr;
            CDS1820::TScratchpad data;

            mSensor.SearchReset();
            while (mSensor.SearchNext(addr))
            {
                if (!mSensor.ReadScratchpad(addr, data))
                    continue;

                int temperature = 0;
                if (mSensor.GetTemperature(addr, data, temperature))
                {

                    mWorking = true;

                        int slot = GetTemperatureSlot(addr);
                        if (slot != -1)
                        {     
                            // TODO: check sudden temperature change
                            meas[slot] = temperature;
                        }
                }
            }
            

            for (int i=0; i<meas.GetSize(); i++)
            {
                if (meas[i] != CSeriesBase::Invalid)
                {
                    if (readings[i] < 1000)
                        readings[i]++;
                    if (readings[i] < 3)
                        meas[i] = CSeriesBase::Invalid; 
                } else
                {
                    readings[i] = 0;
                }
            }

            mLastConversion = 0;
            return mWorking;
        }
        
        if (mLastConversion == 0)
        {
            // Start temperature conversion
            mSensor.StartConversion();
            mLastConversion = now;
        }
        return false;
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
        if (i>99)
            sprintf(buf, "%d" "\xf8", i);
        else if (i>=0)
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
    
    CSimpleSeries mSeriesData[gConfigMaxSensors];
    CArray<CSimpleSeries> mSeries;
    
    CDeviceManager mDevice;
	CLogger mLog;
    
public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        mSeries.Init(mSeriesData, COUNT(mSeriesData));
        mSeries.SetSize(COUNT(mSeriesData));
        
        CWnd::Create(pszId, dwFlags, rc, pParent);
        CRect rcGraph(10+4+40, 16+20+20-8, BIOS::LCD::Width-10-4, BIOS::LCD::Height-20-8-16+4-8);

        mWndGraph.Create("graph", CWnd::WsVisible, rcGraph, this);
        for (int i=0; i<mSeries.GetSize(); i++)
            mWndGraph.AddSeries(&mSeries[i]);

        CRect rcXAxis(10+4+40, BIOS::LCD::Height-20-8-16+4-8, BIOS::LCD::Width-10-4, BIOS::LCD::Height-20-8+4-8);
        mXAxis.Create("xaxis", CWnd::WsVisible, rcXAxis, this);

        CRect rcYAxis(10+4, 16+20+20-8, 10+4+40, BIOS::LCD::Height-20-8-16+4-8);
        mYAxis.Create("yaxis", CWnd::WsVisible, rcYAxis, this);

		mLog.Open();
        SetTimer(100);
    }
	
	void Destroy()
	{
		mLog.Close();
		CWnd::Destroy();
	}

    void OnPaint()
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
        
        CRect rc1(m_rcClient);
        rc1.bottom = 14;
        GUI::Background(rc1, RGB565(4040b0), RGB565(404040));

        BIOS::LCD::Print(8, 0, RGB565(ffffff), RGBTRANS, "Multi temperature grapher (DS1820)");

        CRect rc2(10, 16+20-8, 310, 220-8);
        MyGui::Window(rc2, MyGui::TitleColor);
        
		// Initial range
        mWndGraph.SetRange(Range{0*16, 20*16});
        mYAxis.Update(mWndGraph.GetRange());
        mXAxis.Update(5000);
        
        ShowMeasurement(-1, 0);
		
		if (mLog.IsOpened())
		{
			BIOS::LCD::Printf(8, BIOS::LCD::Height-16, RGB565(404040), RGBTRANS, "Recording to %s...", mLog.GetLogName());
		}
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
            BIOS::LCD::Print(10+16+50, 16+20+2-8, RGB565(000000), MyGui::TitleColor, "No sensor detected...");
            return;
        }
        
        if (nosignal)
        {
            nosignal = false;
            BIOS::LCD::Bar(10+8, 16+20+2-8, 310-8, 16+20+2+14-8, MyGui::TitleColor);
        }
        
        if (index>=4)
            return;
        
        int color = CPalette::GetColor(index);
        //int colorY = (Get565R(color) + Get565G(color)*2 + Get565B(color))/4;
        //if (colorY > 120)
        //    color = InterpolateColor(color, RGB565(000000), 128);
        
        if (value == CSeriesBase::Invalid)
            BIOS::LCD::Printf(10+16+index*64, 16+20+2-8, color, MyGui::TitleColor, "        ");
        else
        if (value == -9999)
            BIOS::LCD::Printf(10+16+index*64, 16+20+2-8, color, MyGui::TitleColor, "...." "\xf8" "C  ");
        else{
            char buf[16];
            if (value > 0)
                sprintf(buf, "%d.%d", value/16, (value%16)*10/16);
            else
                sprintf(buf, "-%d.%d", (-value)/16, ((-value)%16)*10/16);
            BIOS::LCD::Printf(10+16+index*64, 16+20+2-8, color, MyGui::TitleColor, "%s" "\xf8" "C  ", buf);
        }
    }
    
    void PushData()
    {
        int dataPlacement[gConfigMaxSensors];
        CArray<int> data;
        data.Init(dataPlacement, COUNT(dataPlacement));
        data.SetSize(COUNT(dataPlacement));

        static int readingsPlacement[gConfigMaxSensors];
        CArray<int> readings;
        readings.Init(readingsPlacement, COUNT(readingsPlacement));
        readings.SetSize(COUNT(readingsPlacement));

        if (mDevice.GetData(data, readings))
        {
            _ASSERT(data.GetSize() == mSeries.GetSize());
            for (int i=0; i<data.GetSize(); i++)
                mSeries[i] << data[i];
            
            for (int i=0; i<data.GetSize(); i++)
            {
                if (data[i] == CSeriesBase::Invalid && readings[i] > 0)
                    ShowMeasurement(i, -9999);
                else
                    ShowMeasurement(i, data[i]);
            }
			
			if (mLog.IsOpened())
				mLog.Append(data);
        }
    }
	
    void OnTimer()
    {
        static int counter = 0;
        
        EVERY(100)
        {
            PushData();
            counter++;
        }

        bool needRedraw = mWndGraph.Redraw();

        if (needRedraw || (counter > 2 && mSeries[0].GetLength() > 5))
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

