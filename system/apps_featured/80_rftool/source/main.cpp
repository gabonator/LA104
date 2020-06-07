#include <library.h>
using namespace BIOS;

#include "framework/shapes.h"
#include "framework/layout.h"
#include "framework/block.h"
#include "framework/Menu.h"

class CApplicationData
{
    int nFrequency = 433100000;
    int nBandwidth = 225000;
    int nGain = -10;
    int nDataRate = 4300;

public:
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
        return (BIOS::SYS::GetTick() >> 12) % 10;
    }
    
    void GetCaptureRecord(int i, int& ts, char* name, char* desc)
    {
        switch (i)
        {
            case 0:
                ts = 34*1000;
                strcpy(name, "TFA-Twin-Plus-30.3049");
                if (desc)
                    strcpy(desc, "Ch: <74> Temp: <22.3 'C> Humidity: <99%>");
                break;
            case 1:
                ts = 39*1000;
                strcpy(name, "OS - THGR810");
                if (desc)
                    strcpy(desc, "Ch: <19> Temp: <12.7 'C> Humidity: <34%>");
                break;
            case 2:
                ts = 72*1000;
                strcpy(name, "Keyfob-1");
                if (desc)
                    strcpy(desc, "<40 09 0b ca 2d 84 30 f3 4c> open");
                break;
            default:
                ts = (75+i*5)*1000;
                strcpy(name, "Keyless entry");
                if (desc)
                    sprintf(desc, "Data: <%c%c %c%c %c%c %c%c>", 'A' + i, 'A' + i, 'A' + i, 'A' + i, 'A' + i, 'A' + i, 'a' + i, 'a' + i);
                break;
        }
    }
    int GetCaptureAttributes(int i)
    {
        return 6;
    }
    void GetCaptureAttribute(int i, int j, char* name, char* value, char* units)
    {
        switch (j)
        {
            case 0: strcpy(name, "Temperature"); strcpy(value, "19.2"); strcpy(units, "'C"); break;
            case 1: strcpy(name, "Humidity"); strcpy(value, "49"); strcpy(units, "%"); break;
            case 2: strcpy(name, "Id"); strcpy(value, "38"); strcpy(units, ""); break;
            case 3: strcpy(name, "Low battery"); strcpy(value, "false"); strcpy(units, ""); break;
            case 4: strcpy(name, "Channel"); strcpy(value, "7"); strcpy(units, ""); break;
            case 5: strcpy(name, "Junk"); strcpy(value, "2"); strcpy(units, ""); break;
        }
    }
    CArray<uint16_t>& GetWaveform()
    {
        static uint16_t arrData[800];
        static CArray<uint16_t> arr(arrData, COUNT(arrData));
        uint16_t sample[] =
        {1980,980,980,1980,1000,1000,1980,980,1000,940,1020,980,1000,980,980,1980,2000,1980,980,1000,2000,980,1000,1960,2000,2000,1960,1000,1000,1960,2000,1980,1020,960,2000,1980,2000,2000,1980,1980,1000,980,1000,960,1000,960,2000,1000,980,980,1000,980,980,1980,1020,960,2000,1980,2000,1980,1000,960,2000,1000,980,980,1000,980,980,10020,2000,980,980,1960,1020,980,1980,1000,980,980,1000,980,980,980,980,1980,2020,1980,1000,960,2000,1000,980,1980,1980,1980,2000,980,1020,1980,1980,2000,1000,960,2000,1960,2020,1980,1980,1980,1020,960,1020,940,1020,960,2000,1000,980,960,980,1000,980,2000,980,980,2000,1980,1980,2020,980,980,2000,980,980,960,1000,980,980,10040,1980,980,1000,1980,1000,960,2000,1000,980,980,1000,980,980,980,1000,1960,2020,1980,980,1000,2000,980,980,1960,2020,1980,2000,980,980,1980,2000,1960,1020,980,2000,1980,2000,1980,1980,2000,980,980,1020,940,1020,980,1980,980,980,1000,980,980,1000,1980,980,1000,1960,2020,2000,1960,1020,960,2000,960,1000,980,1000,960,1000};
        for (int i=0; i<COUNT(sample); i++)
            arrData[i] = sample[i];
        arr.SetSize(COUNT(sample));
        return arr;
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
		CWnd::Create("Application", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
		
		const int padding = 20;
		
        mMenu.Create("MainMenu", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width-60, 14), this);
        mTime.Create("Time", CWnd::WsVisible, CRect(BIOS::LCD::Width-60, 0, BIOS::LCD::Width, 14), this);
		mControl.Create(" Control ", CWnd::WsVisible, CRect(10, 14+padding, BIOS::LCD::Width-10, 14+padding + mControl.Height), this);
        mModem.Create(" Modem ", CWnd::WsVisible, CRect(10, mControl.m_rcClient.bottom + padding, BIOS::LCD::Width-10, mControl.m_rcClient.bottom + padding + mModem.Height), this);

        mPreview.Create(" Preview ", CWnd::WsVisible | CWnd::WsNoActivate, CRect(10, mModem.m_rcClient.bottom + padding, BIOS::LCD::Width-10, mModem.m_rcClient.bottom + padding + mPreview.Height), this);
        mModem.SetFocus();

        
        mCapture.Create("Capture", CWnd::WsHidden, CRect(10, 14+10, BIOS::LCD::Width-10, BIOS::LCD::Height-10), this);
        mDetails.Create("Details", CWnd::WsHidden, CRect(30, 40, BIOS::LCD::Width-30, BIOS::LCD::Height-40), this);
		SetTimer(50);
	}
	
	void Destroy()
	{
	}
	
    
	virtual void OnTimer() override
	{
		EVERY(1000)
		{
//            ShowTime();
			//mColor.SetColor(mPreview.GetColor());
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
