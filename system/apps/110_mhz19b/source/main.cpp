#ifdef __APPLE__
#include <math.h>
#endif
#include <library.h>
#include <algorithm>

#include "../../os_host/source/framework/BufferedIo.h"

constexpr int gConfigMaxSensors = 1;

/*
//https://github.com/AlexGyver/MeteoClock/blob/master/libraries/mhz19_uart/src/MHZ19_uart.h
class CCo2Sensor
{
  CAsyncSerial mSerial;
  void Update()
  {
    mSerial
      .Send(CBuffer(0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79))
      .Wait(100)
      .OnData( [](const CBuffer& buf)
        {
          if (buf.GetLength() != 9 || buf[0] != 0xff || buf[1] != 0x86)
            return;
          int temperatureDeg = buf[4]-40;
          int co2ppm = buf[2]*256 + buf[3];
          int co2 = buf[2]*256 + buf[3];
        });
      .OnError( []()
        {
        });
  }
*/

#include "graph/palette.h"
#include "graph/draw.h"
#include "graph/graph.h"
#include "graph/series.h"
#include "graph/axis.h"

#include "logger.h"

#define EVERY(ms) static long dwTick##__LINE__ = 0; bool bDo##__LINE__ = BIOS::SYS::GetTick() - dwTick##__LINE__ > ms; if (bDo##__LINE__) dwTick##__LINE__ = BIOS::SYS::GetTick(); if (bDo##__LINE__)

class CDeviceManager
{
    long mLastConversion{0};
    bool mWorking{false};
    uint8_t mBuffer[32];
    int mBufferI{0};
    int mPpm{0};
    int mTemp{0};
    int mStatus{0};
    
public:
    CDeviceManager()
    {
    }
    
    void Init()
    {
        using namespace BIOS;

        GPIO::PinMode(GPIO::P1, GPIO::Uart);
        GPIO::PinMode(GPIO::P2, GPIO::Uart);
        GPIO::UART::Setup(9600, GPIO::UART::EConfig(GPIO::UART::length8 | GPIO::UART::parityNone | GPIO::UART::stopBits1));
    }
    
    void Deinit()
    {
        using namespace BIOS;

        BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Input);
        BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Input);
    }
    
    bool GetData(CArray<int>& meas)
    {
        BIOS::LCD::Print(BIOS::LCD::Width-8, BIOS::LCD::Height-14, RGB565(ffffff), RGB565(101010), " ");
        int32_t now = BIOS::SYS::GetTick();
        
        EVERY(2000)
        {
            static constexpr uint8_t buf[] = {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
            for (int i=0; i<COUNT(buf); i++)
                BIOS::GPIO::UART::Write(buf[i]);
            mBufferI = 0;
            BIOS::LCD::Print(BIOS::LCD::Width-8, BIOS::LCD::Height-14, RGB565(55ff55), RGB565(101010), "S");
        }
        
        while (BIOS::GPIO::UART::Available())
        {
            uint8_t data = BIOS::GPIO::UART::Read();
            if (mBufferI < COUNT(mBuffer))
            {
                mBuffer[mBufferI++] = data;
                if (mBufferI == 9)
                {
                    if (mBuffer[0] == 0xff && mBuffer[1] == 0x86)
                    {
                        mLastConversion = now;
                        mPpm = mBuffer[2]*256 + mBuffer[3];
                        mTemp = mBuffer[4] - 40;
                        mStatus = mBuffer[5];
                        BIOS::LCD::Print(BIOS::LCD::Width-8, BIOS::LCD::Height-14, RGB565(5555ff), RGB565(101010), "R");
                    } else
                    {
                        //BIOS::DBG::Print("Header not matching");
                        BIOS::LCD::Print(BIOS::LCD::Width-8, BIOS::LCD::Height-14, RGB565(ff5555), RGB565(101010), "E");
                    }
                }
                for (int i=0; i<9; i++)
                    BIOS::LCD::Printf(20 + i*24, BIOS::LCD::Height-14-14, RGB565(b0b0b0), RGB565(101010), "%02x", mBuffer[i]);
            }
        }

        for (int i=0; i<meas.GetSize(); i++)
            meas[i] = CSeriesBase::Invalid;
        
        if (mLastConversion != 0 && now - mLastConversion < 5000)
        {
            meas[0] = mPpm;
            return true;
        }
        return false;
    }
    
    int GetTemperature()
    {
        return mTemp;
    }
    int GetStatus()
    {
        return mStatus;
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
        return i;
    }
    
    virtual int backward(int i)
    {
        return i;
    }
    
    virtual void FormatNumber(char* buf, int i)
    {
        if (i>=1000)
            sprintf(buf, "%d k", i/1000);
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
        mDevice.Init();
    }
	
	void Destroy()
	{
		mLog.Close();
		CWnd::Destroy();
        mDevice.Deinit();
	}

    void OnPaint()
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
        
        CRect rc1(m_rcClient);
        rc1.bottom = 14;
        GUI::Background(rc1, RGB565(4040b0), RGB565(404040));

        BIOS::LCD::Print(8, 0, RGB565(ffffff), RGBTRANS, "MH-Z19B co2 meter");

        CRect rc2(10, 16+20-8, 310, 220-8);
        MyGui::Window(rc2, MyGui::TitleColor);
        
		// Initial range
        mWndGraph.SetRange(Range{0*16, 20*16});
        mYAxis.Update(mWndGraph.GetRange());
        mXAxis.Update(5000);
        
        ShowMeasurement(-1, 0, 0, 0);
		
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

    void ShowMeasurement(int index, int value, int temp, int status)
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
            BIOS::LCD::Printf(10+16+index*64, 16+20+2-8, color, MyGui::TitleColor, "               ");
        else
        {
            BIOS::LCD::Printf(10+16+index*64, 16+20+2-8, color, MyGui::TitleColor, "%d ppm, %d" "\xf8" "C, %d", value, temp, status);
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
            
            for (int i=0; i<data.GetSize(); i++)
            {
                ShowMeasurement(i, data[i], mDevice.GetTemperature(), mDevice.GetStatus());
            }
			
			if (mLog.IsOpened())
				mLog.Append(data);
        }
    }
	
    void OnTimer()
    {
        static int counter = 0;
        
        EVERY(500)
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

