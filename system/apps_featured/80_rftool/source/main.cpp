#include <library.h>
using namespace BIOS;

#include "../../os_host/source/framework/BufferedIo.h"

#include "framework/shapes.h"
#include "framework/layout.h"
#include "framework/block.h"
#include "framework/Menu.h"
#include "framework/scroller.h"
#include "framework/file.h"
#include "framework/json.h"
#include "framework/ringbuf.h"

#include "protocol/all.h"

#include "modem/cc1101.h"
#include "streamer/streamer.h"
#include "framer/framer.h"
#include "modem/send.h"

//#define TEST
#ifdef TEST
#include "testwave.h"
#ifndef __APPLE__
#error test should be not run on target
#endif
#endif

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];
CBufferedReader mCommonReader;
CBufferedWriter mCommonWriter;

CAttributes::TAttribute attributesData[14];
CAttributes attributes(attributesData, COUNT(attributesData));

void updateWave(int i);

#include "appdata.h"
#include "storage.h"
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
            case 3: return TItem{"Detail", TItem::Default};
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

class CApplication : public CWnd, public CConfiguration
{
    CMenuMain mMenu;
    CTimeWnd mTime;

    CControlLayout mControl;
	CModemLayout mModem;
	CPreview mPreview;
    
    CCapture mCapture;
    CDetails mDetails;
    
    CFileDialog mFile;

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

        
        mCapture.Create("Capture", CWnd::WsHidden, CRect(10, 14, BIOS::LCD::Width-10, BIOS::LCD::Height-10), this);
        mDetails.Create("Details", CWnd::WsHidden, CRect(30, 40, BIOS::LCD::Width-30, BIOS::LCD::Height-40), this);
		SetTimer(50);
        OnMessage(&mMenu, 0, 1);
        
        char fullPath[64];
        GetCurrentPath(fullPath);
        strcat(fullPath, "/startup.cfg");
        ConfigurationLoad(fullPath);
	}
	
	void Destroy()
	{
        framerStop();
	}
    
	virtual void OnTimer() override
	{
        framerLoop();
	}
	
    int _AnalyseBuffer(CArray<uint16_t>& pulse)
    {
        //static int nRecordId = 1000;
        int uid = -1;
        
        attributes.RemoveAll();
        for (int i=0; i<COUNT(protocols); i++)
        {
            if (protocols[i]->Demodulate(pulse, attributes))
            {
                attributes["_timestamp"] = BIOS::SYS::GetTick();
                attributes["_uid"] = uid = appData.GetRecordUid();

                if (appData.AddCaptureRecord(protocols[i], attributes)) // overwrite
                    appData.GetWaveform(appData.GetCaptureRecords()-1, mDetails.GetWave());
                else
                    uid = -2;

                return uid;
            }
        }
        return -1;
    }
    
    int AnalyseBuffer(CArray<uint16_t>& pulse)
    {
        if (appData.GetPaused())
            return 0;
        
        BIOS::SYS::Beep(5);
        if (appData.GetLogging())
        {
            char fname[32];
            GetUniqueName(fname, "sig");
            char message[64];
            sprintf(message, "%d pulses saved as '%s'", pulse.GetSize(), fname);
            ShowStatus(message);
            SaveSignal(fname, pulse);
            // prepise status nizsie
        }

        int uid = _AnalyseBuffer(pulse);
        int i = uid < 0 ? uid : appData.GetCaptureIndex(uid);
        if (i == -2)
        {
            BIOS::SYS::Beep(50);

            char message[64];
            sprintf(message, "%d pulses: full buffer", pulse.GetSize());
            ShowStatus(message);
        } else
        if (i == -1)
        {
            char message[64];
            sprintf(message, "%d pulses: unknown protocol", pulse.GetSize());
            ShowStatus(message);
        } else
        {
            BIOS::SYS::Beep(50);

            int ts, uid;
            char name[64];
            appData.GetCaptureRecord(i, ts, uid, name, nullptr);
            
            char message[64];
            sprintf(message, "%d pulses: %s", pulse.GetSize(), name);
            ShowStatus(message);
        }
        return uid;
    }
    
    void updateWave(int i)
    {
        appData.GetWaveform(i, mDetails.GetWave());
    }
    
	virtual void OnMessage(CWnd* pSender, int code, uintptr_t data) override
	{
        // Modem configuration notifies attribute change
        if (pSender == &mModem)
        {
            if (strcmp((char*)data, "load") == 0)
            {
                CFileFilterSuffix filter(".CFG");
                if (mFile.ModalShow(this, "Load configuration", &filter))
                {
                    ConfigurationLoad(mFile.GetFilename());
                }
                mModem.SetFocus();
                Invalidate();
            }
            else if (strcmp((char*)data, "save") == 0)
            {
                char fullPath[64];
                GetCurrentPath(fullPath);
                strcat(fullPath, "/startup.cfg");
                ConfigurationSave(fullPath);
                
                ShowStatus("Configuration saved");
            }
            else if (strcmp((char*)data, "update") == 0)
            {
                ShowStatus();
            }
            else
            {
                _ASSERT(0);
            }
        }
        
        if (pSender == &mDetails)
        {
            if (strcmp((char*)data, "close") == 0)
            {
                mMenu.Select(2);
            }
            else if (strcmp((char*)data, "delete") == 0)
            {
                // not supported
            }
            else if (strcmp((char*)data, "export") == 0)
            {
                char fname[32];
                GetUniqueName(fname, "sig");
                SaveSignal(fname, mDetails.GetWave());

                char message[64];
                sprintf(message, "signal saved as %s", fname);
                ShowStatus(message);
            }
            else if (strcmp((char*)data, "transmit") == 0)
            {
                int dataRate = CC1101::GetDataRate();
                int32_t l0 = BIOS::SYS::GetTick();
                SendPulses(mDetails.GetWave(), mDetails.GetProtocol()->PulseDivisor());
                int32_t l1 = BIOS::SYS::GetTick();
                CC1101::SetDataRate(dataRate);
                
                char message[64];
                sprintf(message, "Signal sent in %d ms", l1-l0);
                ShowStatus(message);
            }
        }

        // User has selected any record in list, lets show details
        if (pSender == &mCapture)
        {
            if (code == 0xabbb)
            {
                mDetails.SetUid(data); // TODO: updatnut uid!!!
                int index = appData.GetCaptureIndex(data);
                updateWave(index);
                mMenu.Select(3);
            }
            else if (strcmp((char*)data, "import") == 0)
            {
                CFileFilterSuffix filter(".SIG");
                if (mFile.ModalShow(this, "Load signal", &filter))
                {
                    LoadSignal(mFile.GetFilename());
                }
                mCapture.SetFocus();
                Invalidate();
            }
            else if (strcmp((char*)data, "loadlist") == 0)
            {
                CFileFilterSuffix filter(".LST");
                if (mFile.ModalShow(this, "Load capture list", &filter))
                {
                    LoadList(mFile.GetFilename());
                }
                mCapture.SetFocus();
                Invalidate();
            }
            else if (strcmp((char*)data, "savelist") == 0)
            {
                char fname[32];
                GetUniqueName(fname, "lst");
                SaveList(fname);

                char message[64];
                sprintf(message, "list saved as %s", fname);
                ShowStatus(message);
            }
            else if (strcmp((char*)data, "clearlist") == 0)
            {
                appData.ClearList();
                Invalidate();
            }
        }
        
        // Menu selection has been changed
        if (pSender == &mMenu)
        {
            mControl.ShowWindow(data == 1);
            mModem.ShowWindow(data == 1);
            mPreview.ShowWindow(data == 1);
            mCapture.ShowWindow(data == 2);
            mDetails.ShowWindow(data == 3);
            Invalidate();
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
        
        ShowStatus();
	}

public:
    void ShowStatus(const char* message = nullptr)
    {
        CRect rcClient(m_rcClient);
        rcClient.bottom = BIOS::LCD::Height;
        rcClient.top = BIOS::LCD::Height-14;
        
        BIOS::LCD::Bar(rcClient, RGB565(404040));
        
        using namespace Layout;
        Render r(rcClient);
        r << Padding(8, 0, 4, 0);
        Color h(RGB565(ffffff));
        Color l(RGB565(b0b0b0));
        if (message)
        {
            r << message;
        } else
        {
            auto formatMhz = [](char* buffer, int nFrequency)
            {
                nFrequency /= 100000; // 344 MHZ -> 3440
                int nMhz = nFrequency / 10;
                int nKhz = nFrequency % 10;
                sprintf(buffer, "%d.%d", nMhz, nKhz);
                return buffer;
            };
            auto formatKhz = [](char* buffer, int nFrequency)
            {
                nFrequency /= 1000; // 344 MHZ -> 34400
                sprintf(buffer, "%d", nFrequency);
                return buffer;
            };
            auto formatNumber = [](char* buffer, int n)
            {
                sprintf(buffer, "%d", n);
                return buffer;
            };
            auto formatKNumber = [](char* buffer, int n)
            {
                n /= 100;
                sprintf(buffer, "%d.%d", n/10, n%10);
                return buffer;
            };

            char buffer[16];
            r << h << formatMhz(buffer, appData.GetFrequency()) << l << " MHz" << " / " << h << formatKhz(buffer, appData.GetBandwidth()) << l << " kHz " << h << formatNumber(buffer, appData.GetGain()) << l << " dB " << h << formatKNumber(buffer, appData.GetDataRate()) << l << " kBps";
        }
        
    }
    
#ifdef TEST
    void Test()
    {
        auto TestDump = [this](CArray<uint16_t>& arr) -> CProtocol*
        {
            int uid = _AnalyseBuffer(arr);
            if (uid == -1)
            {
                BIOS::DBG::Print("pulse %d - not identified\n", arr.GetSize());
                return nullptr;
            }
            
            int index = appData.GetCaptureIndex(uid);
            CProtocol* pProtocol = appData.GetRecordProtocol(index);
            char protocolName[32] = "Unknown";
            char desc[128] = "";
            if (pProtocol)
            {
                pProtocol->GetName(protocolName);
                pProtocol->GetDescription(attributes, desc);
            }

            BIOS::DBG::Print("pulse %d - protocol %s\n", arr.GetSize(), protocolName);
            BIOS::DBG::Print("%s\n", desc);

            for (int i=0; i<attributes.GetSize(); i++)
                if (strstr(attributes[i].key, "data") != nullptr)
                    BIOS::DBG::Print("  %s = 0x%x\n", attributes[i].key, attributes[i].value);
                else
                    BIOS::DBG::Print("  %s = %d\n", attributes[i].key, attributes[i].value);
            
            BIOS::DBG::Print("\n");
            return pProtocol;
        };

        CArray<uint16_t> arr(sample15, COUNT(sample15));
        arr.SetSize(arr.GetMaxSize());
        
        //SendPulses(arr, 250);

        CProtocol* proto = TestDump(arr);
        if (!proto)
            return;
        
        PULSE::arrSignal.RemoveAll();
        proto->Modulate(attributes, PULSE::arrSignal);
        char buf[2048] = "";
        for (int i=0; i<PULSE::arrSignal.GetSize(); i++)
            sprintf(buf+strlen(buf), "%d,", PULSE::arrSignal[i]);
        fprintf(stderr, buf);
        
        TestDump(PULSE::arrSignal);
    }
#endif
};

CApplication app;

void analyse(CArray<uint16_t>& pulse)
{
    app.AnalyseBuffer(pulse);
}

void updateWave(int i)
{
    app.updateWave(i);
}

void mainInit()
{
#ifdef __APPLE__
    BIOS::FAT::Init();
    BIOS::OS::SetArgument((char*)"RFTOOL/RFTOOL.ELF");
#endif
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);

    app.Create();
    app.WindowMessage( CWnd::WmPaint );
#ifdef TEST
    app.Test();
#endif
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
    BIOS::FAT::SetSharedBuffer(nullptr);
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
