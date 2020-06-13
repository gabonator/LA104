#include <library.h>
using namespace BIOS;

#include "framework/shapes.h"
#include "framework/layout.h"
#include "framework/block.h"
#include "framework/Menu.h"
#include "framework/scroller.h"

#include "protocol/protocol.h"
#include "protocol/weather.h"
#include "protocol/oregon.h"

#include "modem/cc1101.h"
#include "streamer/streamer.h"
#include "framer/framer.h"

CAttributes::TAttribute attributesData[10];
CWeather weather;
COregon oregon;
CAttributes attributes(attributesData, COUNT(attributesData));

class CApplicationData
{
    bool mConnected = false;
    int nFrequency = 433100000;
    int nBandwidth = 225000;
    int nGain = -10;
    int nDataRate = 4300;
    CArray<CProtocol*> mProtocols;
    CProtocol* mProtocolsData[16] {0};
    TKeyValue mAttributesBuffer[16*10];
    
    //typedef uint8_t TProtocolBuffer[64];
    //TProtocolBuffer mProtocolsBuffer[16];
    
public:
    CApplicationData()
    {
        mProtocols.Init(mProtocolsData, COUNT(mProtocolsData));
    }
    bool GetConnected()
    {
        return mConnected;
    }
    int GetFrequency()
    {
        return nFrequency;
    }
    int GetBandwidth()
    {
        return nBandwidth;
    }
    int GetGain()
    {
        return nGain;
    }
    int GetDataRate()
    {
        return nDataRate;
    }
    void DeltaFrequency(int d)
    {
        nFrequency += d*100000;
    }
    void DeltaBandwidth(int d)
    {
        if (d < 0)
            nBandwidth /= 2;
        else
            nBandwidth *= 2;
    }
    void DeltaGain(int d)
    {
        nGain += d;
    }
    void DeltaDataRate(int d)
    {
        nDataRate += d*100;
    }
    int GetSample()
    {
        return ((BIOS::SYS::GetTick()>>7)&1)?255:0;
    }
    
    // Capture
    int GetCaptureRecords()
    {
        return mProtocols.GetSize();
    }
    
    void GetCaptureRecord(int i, int& ts, char* name, char* desc)
    {
        CAttributes local(&mAttributesBuffer[i*10], 10);
        for (int j=0; j<10; j++)
            if (mAttributesBuffer[i*10+j].key == nullptr)
            {
                local.SetSize(j);
                break;
            }

        ts = local["timestamp"];
        if (name)
            mProtocols[i]->GetName(name);
        if (desc)
            mProtocols[i]->GetDescription(local, desc);
    }

    void AddCaptureRecord(CProtocol* pProtocol, const CAttributes& attr)
    {
        int index = mProtocols.GetSize();
        if (index == mProtocols.GetMaxSize())
            return;
        
        CAttributes local(&mAttributesBuffer[index*10], 10);
        local.Copy(attr);
            
        mProtocols.Add(pProtocol);
    }

    // Capture data
    int GetCaptureAttributesCount(int i)
    {
        CAttributes local(&mAttributesBuffer[i*10], 10);
        for (int j=0; j<10; j++)
            if (mAttributesBuffer[i*10+j].key == nullptr)
            {
                local.SetSize(j);
                break;
            }

        return local.GetSize();
    }

    void GetCaptureAttribute(int i, int j, char* name, char* value, char* units)
    {
        CAttributes local(&mAttributesBuffer[i*10], 10);
        for (int j=0; j<10; j++)
            if (mAttributesBuffer[i*10+j].key == nullptr)
            {
                local.SetSize(j);
                break;
            }

        const CAttributes::TAttribute& attr = local[j];
        if (strcmp(attr.key, "temperature10") == 0)
        {
            strcpy(name, "Temperature");
            sprintf(value, "%d.%d", attr.value/10, attr.value%10);
            strcpy(units, "'C");
            return;

        }
        if (strcmp(attr.key, "humidity") == 0)
        {
            strcpy(name, "Humidity");
            sprintf(value, "%d", attr.value);
            strcpy(units, "%");
            return;

        }
        if (strcmp(attr.key, "id") == 0 || strcmp(attr.key, "channel") == 0 || strcmp(attr.key, "junk") == 0 || strcmp(attr.key, "length") == 0)
        {
            strcpy(name, attr.key);
            sprintf(value, "%d", attr.value);
            strcpy(units, "");
            return;
        }
        if (strcmp(attr.key, "battery_low") == 0)
        {
            strcpy(name, attr.key);
            if (attr.value)
                strcpy(value, "true");
            else
                strcpy(value, "false");
            strcpy(units, "");
            return;
        }

        if (strncmp(attr.key, "data64", 6) == 0)
        {
            strcpy(name, attr.key);
            sprintf(value, "%08x", attr.value);
            strcpy(units, "");
            return;
        }


        strcpy(name, attr.key);
        sprintf(value, "%d", attr.value);
        strcpy(units, "?");
    }
    
    void GetWaveform(int i, CArray<uint16_t>& pulse)
    {
        CProtocol* protocol = mProtocols[i];
        CAttributes local(&mAttributesBuffer[i*10], 10);
        for (int j=0; j<10; j++)
            if (mAttributesBuffer[i*10+j].key == nullptr)
            {
                local.SetSize(j);
                break;
            }
        
        pulse.SetSize(0);
        protocol->Modulate(local, pulse);
    }
    
    
    //
    void SetConnected(bool b)
    {
        mConnected = b;
    }
};

CApplicationData appData;

#include "gui/modem.h"
#include "gui/capture.h"
#include "gui/details.h"

class CMenuMain : public CTopMenu
{
public:
    virtual TItem GetItem(int i)
    {
        switch (i)
        {
            case 0: return TItem{"RF tool", TItem::Static};
            case 1: return TItem{"Modem", TItem::Default};
            case 2: return TItem{"Capture", TItem::Default};
            case 3: return TItem{"Details", TItem::Default};
            default: return TItem{nullptr, TItem::None};
        }
    }
};

class CTimeWnd : public CWnd
{
public:
    void Create( const char* pszId, int dwFlags, const CRect& rc, CWnd* pParent )
    {
        CWnd::Create(pszId, dwFlags, rc, pParent);
        SetTimer(1000);
    }

    void OnTimer()
    {
        Invalidate();
    }
    
    virtual void OnPaint()
    {
        char time[16];
        int seconds = BIOS::SYS::GetTick() / 1000;
        sprintf(time, "%02d:%02d", seconds / 60, seconds % 60);
        BIOS::LCD::Print(BIOS::LCD::Width - strlen(time)*8-4, 0, RGB565(ffffff), RGB565(4040B0), time);
    }
};

class CApplication : public CWnd
{
    CMenuMain mMenu;
    CTimeWnd mTime;

    CControlLayout mControl;
	CModemLayout mModem;
	CPreview mPreview;
    
    CCapture mCapture;
    CDetails mDetails;

public:
	void Create()
	{
        appData.SetConnected(framerStart());
        
		CWnd::Create("Application", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
		
		const int padding = 20;
		
        mMenu.Create("MainMenu", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width-60, 14), this);
        mTime.Create("Time", CWnd::WsVisible, CRect(BIOS::LCD::Width-60, 0, BIOS::LCD::Width, 14), this);
		mControl.Create(" Control ", CWnd::WsHidden, CRect(10, 14+padding, BIOS::LCD::Width-10, 14+padding + mControl.Height), this);
        mModem.Create(" Modem ", CWnd::WsHidden, CRect(10, mControl.m_rcClient.bottom + padding, BIOS::LCD::Width-10, mControl.m_rcClient.bottom + padding + mModem.Height), this);

        mPreview.Create(" Preview ", CWnd::WsHidden | CWnd::WsNoActivate, CRect(10, mModem.m_rcClient.bottom + padding, BIOS::LCD::Width-10, mModem.m_rcClient.bottom + padding + mPreview.Height), this);
        mModem.SetFocus();

        
        mCapture.Create("Capture", CWnd::WsHidden, CRect(10, 14+10, BIOS::LCD::Width-10, BIOS::LCD::Height-10), this);
        mDetails.Create("Details", CWnd::WsHidden, CRect(30, 40, BIOS::LCD::Width-30, BIOS::LCD::Height-40), this);
		SetTimer(50);
        OnMessage(&mMenu, 0, 1);
	}
	
	void Destroy()
	{
        framerStop();
	}
    
	virtual void OnTimer() override
	{
		EVERY(5000)
		{
            /*
            CArray<uint16_t> arrPulses;
            uint16_t arrPulsesData[256];
            arrPulses.Init(arrPulsesData, COUNT(arrPulsesData));
            
            weather.Example(attributes);
            weather.Modulate(attributes, arrPulses);
            BIOS::DBG::Print("dump: {");
            for (int i=0; i<arrPulses.GetSize(); i++)
                BIOS::DBG::Print("%d,", arrPulses[i]);
            BIOS::DBG::Print("}\n");
             */
            
            /*
            uint16_t bufferConrad[] =            {500,9000,500,2000,500,4000,500,4000,500,2000,500,2000,500,4000,500,2000,500,4000,500,2000,500,2000,500,4000,500,2000,500,4000,500,4000,500,2000,500,4000,500,2000,500,4000,500,2000,500,4000,500,2000,500,2000,500,2000,500,2000,500,4000,500,4000,500,4000,500,4000,500,4000,500,4000,500,4000,500,4000,500,4000,500,4000,500,4000,500,2000,500};
             CArray<uint16_t> pulse;
             pulse.Init(bufferConrad, COUNT(bufferConrad));
             pulse.SetSize(COUNT(bufferConrad));
             */

            uint16_t buffer[] =  {380,360,320,420,300,400,300,400,340,420,280,400,340,400,320,400,300,420,300,420,300,420,300,3820,700,380,740,360,720,400,720,380,700,380,720,400,700,400,680,400,700,420,680,420,700,360,720,400,340,760,700,400,700,380,360,740,720,380,340,780,700,400,700,380,720,380,340,760,720,380,320,780,720,380,720,380,320,760,720,380,720,400,700,380,720,400,700,400,700,380,340,760,340,780,300,800,300,780,720,360,340,780,320,780,300,800,720,360,340,780,340,740,360,760,300,780,340,760,700,400,340,780,680,400,700,400,340,780,320,760,320,760,340,760,720,380,320,780,720,380,340,760,720,400,300,780,700,400,720,400,320,760,340,760,300};

            CArray<uint16_t> pulse;
            pulse.Init(buffer, COUNT(buffer));
            pulse.SetSize(COUNT(buffer));

            AnalyseBuffer(pulse);
            
            /*
            CArray<uint16_t> pulses;
            uint16_t pulsesData[256];
            pulses.Init(pulsesData, COUNT(pulsesData));

            weather.Example(attributes);
            static int temp = 10;
            attributes["temperature10"] = (temp++)*10;
            weather.Modulate(attributes, pulses);
            AnalyseBuffer(pulses);
            */
		}
	}
	
    void AnalyseBuffer(CArray<uint16_t>& pulse)
    {
        static int nRecordId = 1000;
        
        if (weather.Demodulate(pulse, attributes))
        {
            //weather.Example(attributes);
            attributes["timestamp"] = BIOS::SYS::GetTick();
            attributes["uid"] = nRecordId++;
            appData.AddCaptureRecord(&weather, attributes);
            appData.GetWaveform(appData.GetCaptureRecords()-1, mDetails.GetWave());
            return;
        }
        if (oregon.Demodulate(pulse, attributes))
        {
            //weather.Example(attributes);
            attributes["timestamp"] = BIOS::SYS::GetTick();
            attributes["uid"] = nRecordId++;
            appData.AddCaptureRecord(&oregon, attributes);
            appData.GetWaveform(appData.GetCaptureRecords()-1, mDetails.GetWave());
            return;
        }
    }
    
	virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
	{
        if (pSender == &mMenu)
        {
            mControl.ShowWindow(data == 1);
            mModem.ShowWindow(data == 1);
            mPreview.ShowWindow(data == 1);
            mCapture.ShowWindow(data == 2);
            mDetails.ShowWindow(data == 3);
            Invalidate();
        }

//		if (pSender == &mColor)
		{
//			mPreview.SetColor(data);
		}
	}
	
private:
	virtual void OnPaint() override
	{
		GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
		
		CRect rc1(m_rcClient);
		rc1.bottom = 14;
		GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
		BIOS::LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "RF tool");
	}
};

CApplication app;

void mainInit()
{
    app.Create();
    app.WindowMessage( CWnd::WmPaint );
}

bool mainLoop()
{
    BIOS::KEY::EKey key;
    key = KEY::GetKey();
    if (key != BIOS::KEY::None)
        app.WindowMessage(CWnd::WmKey, key);
    app.WindowMessage(CWnd::WmTick);

    return key != KEY::EKey::Escape;
}

void mainFinish()
{
    app.Destroy();
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
	mainInit();
	while (mainLoop());
	mainFinish();
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
#ifdef __APPLE__
    //kill(getpid(), SIGSTOP);
#endif
    while (1);
}
