#include <library.h>
using namespace BIOS;

#include "framework/shapes.h"
#include "framework/layout.h"
#include "framework/block.h"
#include "framework/Menu.h"
#include "framework/scroller.h"

#include "protocol/protocol.h"
#include "protocol/weather.h"
#include "protocol/oregon2.h"
#include "protocol/key360.h"
#include "protocol/vw.h"
#include "protocol/nexus.h"
#include "protocol/gate.h"

#include "modem/json.h" // TODO: framework
#include "modem/cc1101.h"
#include "streamer/streamer.h"
#include "framer/framer.h"

CAttributes::TAttribute attributesData[14];
CWeather weather;
COregon2 oregon;
CKey360 key360;
CVw vw;
CNexus nexus;
CGate gate;

CBufferedReader mCommonReader;
CBufferedWriter mCommonWriter;


CAttributes attributes(attributesData, COUNT(attributesData));

static CProtocol* protocols[] = {&weather, &oregon, &key360, &vw, &nexus, &gate};

void updateWave(int i);

class CApplicationData
{
    const static int KVSlots{16};
    bool mConnected = false;
//    int nFrequency = 433100000;
//    int nBandwidth = 225000;
//    int nGain = -10;
//    int nDataRate = 4300;
    CArray<CProtocol*> mProtocols;
    CProtocol* mProtocolsData[16] {0};
    TKeyValue mAttributesBuffer[16*KVSlots];
    bool mLogging{false};
    
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
        return CC1101::GetFrequency();
    }
    int GetBandwidth()
    {
        return CC1101::GetBandwidth();
    }
    int GetGain()
    {
        return CC1101::GetGain();
    }
    int GetDataRate()
    {
        return CC1101::GetDataRate();
    }
    void DeltaFrequency(int d)
    {
        CC1101::SetFrequency(CC1101::GetFrequency()+d*50000);
    }
    void DeltaBandwidth(int d)
    {
        CC1101::DeltaBandwidth(d);
    }
    void DeltaGain(int d)
    {
        CC1101::DeltaGain(d);
    }
    void DeltaDataRate(int d)
    {
        CC1101::SetDataRate(CC1101::GetDataRate() + d * 200);
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
    
    void GetCaptureRecord(int i, int& ts, int& uid, char* name, char* desc)
    {
        CAttributes local(&mAttributesBuffer[i*KVSlots], KVSlots);
        for (int j=0; j<KVSlots; j++)
            if (mAttributesBuffer[i*KVSlots+j].key == nullptr)
            {
                local.SetSize(j);
                break;
            }

        ts = local["_timestamp"];
        uid = local["_uid"];
        if (name)
            mProtocols[i]->GetName(name);
        if (desc)
            mProtocols[i]->GetDescription(local, desc);
    }

    bool AddCaptureRecord(CProtocol* pProtocol, const CAttributes& attr)
    {
        int index = mProtocols.GetSize();
        if (index == mProtocols.GetMaxSize())
        {
            // TODO: notify full buffer
            return false;
        }
        
        CAttributes local(&mAttributesBuffer[index*KVSlots], KVSlots);
        local.Copy(attr);
            
        mProtocols.Add(pProtocol);
        return true;
    }

    CAttributes GetAttributes(int i)
    {
        CAttributes local(&mAttributesBuffer[i*KVSlots], KVSlots);
        for (int j=0; j<KVSlots; j++)
            if (mAttributesBuffer[i*KVSlots+j].key == nullptr)
            {
                local.SetSize(j);
                return local;
            }
        
        _ASSERT(0);
        return local;
    }

    // Capture data
    int GetCaptureIndex(int uid)
    {
        for (int i=0; i<GetCaptureRecords(); i++)
        {
            CAttributes attr = GetAttributes(i);
            if (attr["_uid"] == uid)
                return i;
        }
        return -1;
    }
    
    int GetCaptureAttributesCount(int i)
    {
        CAttributes local(&mAttributesBuffer[i*KVSlots], KVSlots);
        for (int j=0; j<KVSlots; j++)
            if (mAttributesBuffer[i*KVSlots+j].key == nullptr)
            {
                local.SetSize(j);
                break;
            }

        return local.GetSize();
    }

    void DeltaCaptureAttribute(int i, int j, int d)
    {
        CAttributes local(&mAttributesBuffer[i*KVSlots], KVSlots);
        for (int j=0; j<KVSlots; j++)
            if (mAttributesBuffer[i*KVSlots+j].key == nullptr)
            {
                local.SetSize(j);
                break;
            }
        
        local.GetData()[j].value += d;
        updateWave(i);
    }

    void GetCaptureAttribute(int i, int j, char* name, char* value, char* units)
    {
        CAttributes local(&mAttributesBuffer[i*KVSlots], KVSlots);
        for (int j=0; j<KVSlots; j++)
            if (mAttributesBuffer[i*KVSlots+j].key == nullptr)
            {
                local.SetSize(j);
                break;
            }

        const CAttributes::TAttribute& attr = local[j];
        if (attr.key[0] == '$')
        {
            strcpy(name, attr.key+1);
            strcpy(value, (char*)attr.value);
            return;
        }
        
        if (strcmp(attr.key, "temperature10") == 0)
        {
            strcpy(name, "Temperature");
            int t = (int)attr.value;
            if (t<0)
            {
                t = -t;
                sprintf(value, "-%d.%d", t/10, t%10);
            }
            else
                sprintf(value, "%d.%d", t/10, t%10);
            strcpy(units, "'C");
            return;

        }
        if (strcmp(attr.key, "humidity") == 0)
        {
            strcpy(name, "Humidity");
            sprintf(value, "%d", (int)attr.value);
            strcpy(units, "%");
            return;

        }
        if (strcmp(attr.key, "id") == 0 || strcmp(attr.key, "channel") == 0 || strcmp(attr.key, "junk") == 0 || strcmp(attr.key, "length") == 0)
        {
            strcpy(name, attr.key);
            sprintf(value, "%d", (int)attr.value);
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

        if (strncmp(attr.key, "data64", 6) == 0) // ??? 64???
        {
            strcpy(name, attr.key);
            sprintf(value, "%08x", (int)attr.value);
            strcpy(units, "");
            return;
        }

        strcpy(name, attr.key);
        sprintf(value, "%d", (int)attr.value);
        strcpy(units, "?");
    }
    
    void GetWaveform(int i, CArray<uint16_t>& pulse)
    {
        CProtocol* protocol = mProtocols[i];
        CAttributes local(&mAttributesBuffer[i*KVSlots], KVSlots);
        for (int j=0; j<KVSlots; j++)
            if (mAttributesBuffer[i*KVSlots+j].key == nullptr)
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
    
    //
    bool GetLogging()
    {
        return mLogging;
    }
    
    void SetLogging(bool logging)
    {
        mLogging = logging;
    }
    
    void ToggleFrequency()
    {
        int f = CC1101::GetFrequency();
        if (f < 500000000) // 500 MHz
            f = 868280000; // 868.28 MHz
        else
            f = 433940000; // 433.94 MHz
        CC1101::SetFrequency(f);
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

class CConfiguration
{
public:
    void ConfigurationSave(const char* name)
    {
        bool b = mCommonWriter.Open((char*)name);
        if (!b)
        {
            _ASSERT(0);
            return;
        }
        //{frequency:868280000,bandwidth:135000,datarate:4096,gain:-12}
        mCommonWriter << "{frequency:" << CC1101::GetFrequency()
        << ",bandwidth:" << CC1101::GetBandwidth()
        << ",datarate:" << CC1101::GetDataRate()
        << ",gain:" << CC1101::GetGain()
        << "}";
        mCommonWriter.Close();

    }
    void ConfigurationLoad(char* name)
    {
        bool b = mCommonReader.Open(name);
        if (!b)
        {
            _ASSERT(0);
            return;
        }
        _ASSERT(mCommonReader.GetFileSize() < BIOS::FAT::SectorSize);
        mCommonReader.Close();
        CJson json((char*)mCommonReader.GetBuffer());
        json.ForEach([](const CSubstring& key, const CSubstring& value)
        {
            if (key == "frequency")
            {
                int numeric = CConversion(value).ToInt();
                CC1101::SetFrequency(numeric);
            }
            else if (key == "bandwidth")
            {
                int numeric = CConversion(value).ToInt();
                CC1101::SetBandwidth(numeric);
            }
            else if (key == "datarate")
            {
                int numeric = CConversion(value).ToInt();
                CC1101::SetDataRate(numeric);
            }
            else if (key == "gain")
            {
                int numeric = CConversion(value).ToInt();
                CC1101::SetGain(numeric);
            }
        });
    }
};

class CFileFilter
{
public:
    virtual bool ShowFile(BIOS::FAT::TFindFile* file) { return true; }
};

class CFileFilterSuffix : public CFileFilter
{
    const char* mSuffix;

public:
    CFileFilterSuffix(const char* suffix) : mSuffix(suffix)
    {
    }
    
    virtual bool ShowFile(BIOS::FAT::TFindFile* file)
    {
        return strstr(file->strName, mSuffix) != nullptr;
    }
};

class CFileDialog : public CWnd
{
    constexpr static int Width = 200;
    constexpr static int Height = 140;
    
    bool mRunning{false};
    char mPath[64];
    char mFilename[64];
    int mScroll{0};
    int mIndex{0};
    int mTotalFiles{0};
    int mMaxLines{0};
    CFileFilter* mFilter{nullptr};
    
public:
    bool ModalShow(CWnd* pParent, const char* caption, CFileFilter* pFilter)
    {
        Create("file", CWnd::WsVisible, CRect(LCD::Width/2 - Width/2, LCD::Height/2 - Height/2, LCD::Width/2 + Width/2, LCD::Height/2 + Height/2), pParent);

        Layout::Render(m_rcClient) << Layout::Window(caption);

        mFilter = pFilter;
        mRunning = true;
        GetCurrentPath(mPath);
        strcpy(mFilename, "");
        mIndex = 0;
        mScroll = 0;

        SetFocus();
        WindowMessage(CWnd::WmPaint);
        while (mRunning)
        {
            BIOS::KEY::EKey key = BIOS::KEY::GetKey();
            if (key != BIOS::KEY::EKey::None)
                WindowMessage(CWnd::WmKey, key);
            WindowMessage(CWnd::WmTick);
        }
        
        Destroy();
        return strlen(mFilename) > 0;
    }
    
    void GetCurrentPath(char* path)
    {
        // Full application path
        strcpy(path, BIOS::OS::GetArgument());

        // Strip app name
        char* last = strrchr(path, '/');
        if (last)
            *last = 0;
        else
            strcpy(path, "");
    }
    
    char* GetFilename()
    {
        return mFilename;
    }
    
    void AppendFile(char* path, int index)
    {
        if (BIOS::FAT::OpenDir(path) != BIOS::FAT::EResult::EOk)
            return;
        
        BIOS::FAT::TFindFile file;
        int i = 0;
        while (BIOS::FAT::FindNext(&file) == BIOS::FAT::EResult::EOk)
        {
            if (mFilter->ShowFile(&file))
            {
                if (i++==index)
                {
                    if (strlen(path) > 0 && path[strlen(path)-1] != '/')
                        strcat(path, "/");
                    strcat(path, file.strName);
                    return;
                }
            }
        }
    }

    int ListFiles(char* path, const CRect& rcWindow, int index, int scroll)
    {
        if (BIOS::FAT::OpenDir(path) != BIOS::FAT::EResult::EOk)
            return 0;
        
        int y = rcWindow.top;
        int i = 0;
        int first = scroll;
        int last = first;
        
        BIOS::FAT::TFindFile file;
        while (BIOS::FAT::FindNext(&file) == BIOS::FAT::EResult::EOk)
        {
            if (mFilter->ShowFile(&file))
            {
                if (scroll > 0)
                {
                    scroll--;
                    i++;
                    continue;
                }
                if (y+16 <= rcWindow.bottom)
                {
                    last++;
                    int x = rcWindow.left;
                    if (file.nAtrib & BIOS::FAT::EAttribute::EDirectory)
                    {
                        if (i==index)
                            x += BIOS::LCD::Printf(x, y, RGB565(000000), RGB565(ffffff), "<%s>", file.strName);
                        else
                            x += BIOS::LCD::Printf(x, y, RGB565(ffffff), RGB565(404040), "<%s>", file.strName);
                    } else
                    {
                        if (i==index)
                            x += BIOS::LCD::Printf(x, y, RGB565(404040), RGB565(ffffff), " %s ", file.strName);
                        else
                            x += BIOS::LCD::Printf(x, y, RGB565(b0b0b0), RGB565(404040), " %s ", file.strName);
                    }
                    BIOS::LCD::Bar(x, y, rcWindow.right-4, y+14, RGB565(404040));
                }
                y += 16;
                i++;
            }
        }
        
        int total = i;
        
        int top = rcWindow.top;
        int bottom = rcWindow.bottom - 2;
        BIOS::LCD::Bar(rcWindow.right-4, top, rcWindow.right, bottom, RGB565(606060));
        if (total > 0)
        {
            BIOS::LCD::Bar(rcWindow.right-4, top + first * (bottom - top) / total, rcWindow.right, top + last * (bottom - top) / total, RGB565(d0d0d0));
        }
        return i;
    }

    
    //
    virtual void OnPaint() override
    {
        using namespace Layout;
        CRect rcFiles(m_rcClient);
        rcFiles.Deflate(8, 20, 8, 8);
        //BIOS::LCD::Bar(rcFiles, RGB565(4040b0));
        
        mMaxLines = (rcFiles.Height())/16;
        mTotalFiles = ListFiles(mPath, rcFiles, mIndex, mScroll);
    }
    
    virtual void OnKey(int key) override
    {
        if (key == BIOS::KEY::Up && mIndex > 0)
        {
            mIndex--;
            if (mIndex < mScroll)
                mScroll = mIndex;
            Invalidate();
        }
        if (key == BIOS::KEY::Down && mIndex < mTotalFiles-1)
        {
            mIndex++;
            if (mIndex >= mScroll+mMaxLines)
                mScroll = mIndex - mMaxLines +1;
            Invalidate();
        }
        if (key == BIOS::KEY::Enter)
        {
            strcpy(mFilename, mPath);
            AppendFile(mFilename, mIndex);
            mRunning = false;
        }

        //if (key == BIOS::KEY::Enter)
//            mRunning = false;
        if (key == BIOS::KEY::Escape)
            mRunning = false;
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
	}
	
	void Destroy()
	{
        framerStop();
	}
    
	virtual void OnTimer() override
	{
        framerLoop();
#ifdef __APPLE__
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
/*
            uint16_t buffer[] =  {380,360,320,420,300,400,300,400,340,420,280,400,340,400,320,400,300,420,300,420,300,420,300,3820,700,380,740,360,720,400,720,380,700,380,720,400,700,400,680,400,700,420,680,420,700,360,720,400,340,760,700,400,700,380,360,740,720,380,340,780,700,400,700,380,720,380,340,760,720,380,320,780,720,380,720,380,320,760,720,380,720,400,700,380,720,400,700,400,700,380,340,760,340,780,300,800,300,780,720,360,340,780,320,780,300,800,720,360,340,780,340,740,360,760,300,780,340,760,700,400,340,780,680,400,700,400,340,780,320,760,320,760,340,760,720,380,320,780,720,380,340,760,720,400,300,780,700,400,720,400,320,760,340,760,300};

            CArray<uint16_t> pulse;
            pulse.Init(buffer, COUNT(buffer));
            pulse.SetSize(COUNT(buffer));

            AnalyseBuffer(pulse);
*/            
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
            

            /*
            //key 360
            uint16_t buffer[] = {360, 440, 240, 440, 280, 460, 280, 420, 280, 440, 320, 380, 320, 420, 300, 420, 300, 420, 300, 420, 300, 400, 320, 3800, 700, 400, 700, 400, 720, 400, 700, 380, 700, 400, 700, 380, 720, 400, 700, 380, 720, 380, 740, 380, 700, 380, 720, 400, 300, 780, 740, 360, 720, 400, 320, 760, 340, 760, 720, 380, 320, 780, 340, 740, 340, 760, 340, 780, 320, 760, 340, 760, 720, 360, 740, 380, 340, 760, 720, 380, 720, 380, 700, 420, 700, 360, 720, 380, 740, 360, 360, 740, 340, 780, 320, 760, 340, 780, 700, 380, 340, 740, 380, 740, 340, 740, 740, 380, 340, 760, 360, 740, 340, 740, 340, 760, 340, 760, 720, 400, 320, 760, 720, 380, 720, 400, 340, 740, 340, 760, 340, 740, 340, 780, 720, 360, 360, 740, 720, 400, 340, 760, 700, 400, 320, 760, 740, 360, 720, 360, 360, 760, 340, };
*/
            /*
            //key360
            uint16_t buffer[] = {340, 400, 320, 420, 320, 380, 340, 400, 300, 400, 320, 420, 300, 420, 300, 400, 300, 440, 300, 400, 320, 400, 320, 3800, 720, 400, 680, 400, 700, 400, 700, 380, 720, 380, 720, 400, 720, 380, 700, 380, 720, 400, 680, 400, 720, 400, 700, 360, 360, 760, 720, 380, 720, 380, 340, 760, 680, 400, 700, 420, 700, 400, 700, 380, 720, 380, 700, 440, 680, 380, 700, 420, 320, 760, 720, 380, 340, 780, 680, 400, 720, 400, 680, 400, 700, 400, 720, 360, 720, 400, 320, 780, 320, 780, 300, 780, 340, 780, 700, 400, 320, 780, 300, 780, 320, 800, 700, 400, 320, 760, 320, 760, 320, 780, 340, 780, 320, 780, 700, 400, 320, 780, 680, 440, 660, 420, 300, 800, 300, 780, 300, 800, 320, 780, 720, 360, 320, 780, 700, 420, 320, 760, 700, 400, 320, 800, 700, 400, 680, 400, 320, 780, 340,};
            */
            /*
            uint16_t buffer[] = {460, 8820, 440, 1940, 440, 1940, 420, 4000, 440, 3960, 460, 1920, 420, 4000, 460, 3940, 460, 3980, 460, 1920, 420, 1940, 420, 1960, 420, 1920, 460, 1920, 440, 4000, 440, 3980, 440, 1900, 480, 3960, 460, 1920, 420, 3980, 440, 4000, 460, 1900, 440, 1940, 460, 1920, 440, 1920, 460, 3960, 440, 3980, 460, 3940, 480, 3960, 460, 3960, 460, 3960, 440, 4000, 420, 4000, 420, 4000, 420, 4000, 440, 1920, 460, 4100, 420, 8800, 460, 1900, 460, 1940, 440, 3980, 420, 3980, 440, 1940, 440, 3980, 440, 3980, 440, 3980, 440, 1940, 440, 1940, 420, 1940, 440, 1920, 460, 1920, 460, 3940, 480, 3960, 460, 1920, 440, 3980, 420, 1940, 440, 3980, 440, 3980, 460, 1920, 420, 1940, 420, 1980, 420, 1920, 440, 3980, 440, 4000, 440, 3980, 440, 3980, 420, 4000, 420, 4000, 420, 4000, 420, 4000, 420, 4000, 440, 3980, 440, 1920, 460, 4100, 420, 8820, 440, 1920, 440, 1940, 440, 3980, 440, 3980, 440, 1920, 440, 4000, 420, 3980, 460, 3980, 420, 1960, 420, 1920, 460, 1920, 440, 1920, 440, 1940, 440, 3980, 460, 3980, 420, 1960, 420, 3980, 440, 1920, 440, 4000, 440, 3960, 460, 1920, 420, 1940, 440, 1940, 420, 1960, 420, 4000, 440, 3980, 420, 4020, 400, 3980, 460, 3980, 440, 3980, 440, 4000, 420, 4000, 420, 4000, 420, 3980, 440, 1940, 440, 4100, 420, 8800, 440, 1940, 440, 1940, 420, 4000, 440, 3980, 440, 1920, 440, 3980, 440, 3980, 460, 3980, 420, 1960, 420, 1940, 440, 1940, 420, 1940, 440, 1920, 440, 3980, 460, 3980, 420, 1920, 480, 3980, 420, 1940, 420, 3980, 440, 3980, 460, 1920, 440, 1940, 420, 1960, 420, 1940, 440, 3980, 440, 4000, 420, 3980, 420, 4000, 420, 4000, 440, 3980, 440, 3980, 440, 3980, 440, 4000, 420, 3980, 440, 1960, 420};
*/
            
            uint16_t buffer[] = {540, 440, 460, 460, 460, 440, 500, 440, 480, 440, 460, 480, 460, 460, 480, 440, 460, 460, 460, 460, 500, 440, 460, 460, 500, 420, 460, 480, 460, 460, 460, 460, 460, 460, 460, 460, 460, 480, 460, 460, 460, 460, 460, 460, 460, 480, 460, 920, 980, 920, 960, 460, 480, 420, 500, 420, 500, 420, 500, 920, 500, 420, 480, 440, 980, 920, 980, 900, 500, 420, 500, 440, 480, 460, 940, 940, 980, 900, 480, 440, 480, 460, 460, 440, 500, 440, 500, 420, 500, 440, 940, 940, 480, 420, 500, 440, 480, 460, 460, 460, 460, 460, 500, 440, 480, 440, 960, 920, 480, 440, 480, 440, 480, 440, 980, 920, 480, 440, 980, 920, 500, 420, 480, 440, 480, 440, 480, 440, 480, 480, 940, 920, 500, 440, 940, 440, 500, 420, 500, 940, 460, 460, 480, 440, 480, 440, 480, 440, 980, 420, 480, 440, 480, 940, 980, 440, 480, 420, 500, 920, 480, 440, 480, 460, 960, 440, 480, 920, 500, 420, 500, 420, 480,
            };
            /*
            //conrad
            uint16_t buffer[] = {440, 8820, 440, 1940, 420, 1960, 420, 3980, 440, 3980, 440, 1940, 440, 3960, 460, 3960, 460, 3960, 480, 1920, 420, 1940, 460, 3960, 440, 1940, 420, 1940, 460, 3940, 480, 1940, 420, 1920, 440, 4000, 420, 4000, 440, 3980, 440, 3960, 460, 1920, 440, 1940, 420, 1960, 420, 1920, 460, 3980, 440, 3980, 460, 3940, 480, 3940, 480, 3940, 480, 3940, 480, 3940, 480, 3940, 480, 3960, 460, 1920, 440, 3980, 440, 4100, 440, 8780, 460, 1940, 420, 1960, 420, 3980, 420, 4020, 440, 1940, 420, 3980, 440, 3980, 460, 3960, 460, 1900, 440, 1940, 420, 3980, 460, 1940, 440, 1940, 420, 4000, 420, 1940, 420, 1940, 440, 3980, 460, 3960, 460, 3960, 460, 3960, 460, 1920, 420, 1940, 440, 1940, 440, 1920, 460, 3980, 420, 4000, 420, 3980, 440, 3980, 440, 3980, 440, 4000, 440, 3980, 420, 4000, 420, 4000, 420, 1960, 440, 3980, 440, 4100, 440, 8800, 440, 1920, 440, 1940, 440, 3960, 440, 3980, 440, 1940, 420, 4000, 440, 3980, 460, 3960, 460, 1900, 480, 1900, 440, 3980, 460, 1940, 420, 1960, 400, 3980, 460, 1920, 420, 1960, 440, 3960, 480, 3960, 420, 4000, 420, 4000, 440, 1940, 440, 1940, 420, 1940, 440, 1940, 420, 3980, 440, 3980, 440, 4000, 440, 3960, 440, 4000, 420, 4000, 440, 3980, 420, 4000, 420, 3980, 440, 1960, 420, 3980, 420, 4140, 440, 8780, 440, 1940, 420, 1940, 420, 4020, 440, 3980, 440, 1920, 440, 3980, 440, 3980, 460, 3960, 460, 1920, 420, 1940, 440, 3980, 440, 1920, 440, 1940, 420, 4000, 440, 1940, 420, 1960, 440, 3960, 460, 3960, 460, 3980, 440, 3960, 460, 1920, 440, 1940, 440, 1940, 420, 1940, 440, 4000, 420, 3980, 420, 4000, 440, 3980, 460, 3960, 440, 4000, 420, 3980, 440, 3980, 440, 3980, 440, 1940, 440, 3980, 460, };
            */
            /*
            uint16_t buffer[] = {460, 500, 420, 520, 420, 480, 460, 480, 440, 480, 440, 500, 420, 500, 440, 460, 460, 480, 460, 460, 460, 460, 460, 460, 460, 460, 460, 480, 460, 460, 460, 460, 460, 460, 460, 460, 500, 440, 460, 460, 460, 460, 460, 460, 460, 480, 460, 920, 980, 920, 960, 440, 480, 460, 460, 460, 460, 460, 500, 920, 460, 460, 480, 440, 980, 920, 940, 920, 500, 420, 480, 460, 500, 420, 960, 920, 980, 920, 500, 420, 500, 420, 500, 420, 1000, 420, 500, 920, 940, 440, 480, 480, 480, 440, 480, 440, 480, 920, 480, 440, 480, 440, 480, 460, 960, 920, 480, 440, 980, 920, 960, 940, 480, 420, 980, 920, 480, 440, 500, 440, 480, 440, 480, 440, 460, 460, 960, 920, 500, 420, 500, 460, 460, 460, 460, 460, 940, 940, 480, 420, 500, 460, 460, 460, 460, 460, 460, 460, 960, 920, 500, 420, 500, 420, 500, 460, 940, 940, 480, 420, 1000, 920, 940, 440, 500, 420, 480, 480, 440};*/
            CArray<uint16_t> pulse;
            pulse.Init(buffer, COUNT(buffer));
            pulse.SetSize(COUNT(buffer));

            analyse(pulse);

		}
#endif
	}
	
    int AnalyseBuffer(CArray<uint16_t>& pulse)
    {
        static int nRecordId = 1000;
//        BIOS::DBG::Print("<%d>", pulse.GetSize());
        int uid = -1;
        
        for (int i=0; i<COUNT(protocols); i++)
        {
if (appData.GetLogging())
{
            char msg[2] = {(char)('A' + i), 0};
            BIOS::DBG::Print(msg);
}

            if (protocols[i]->Demodulate(pulse, attributes))
            {
//                BIOS::DBG::Print("x");
                //weather.Example(attributes);
                attributes["_timestamp"] = BIOS::SYS::GetTick();
                
                attributes["_uid"] = uid = nRecordId++;
//                BIOS::DBG::Print("y");
                if (appData.AddCaptureRecord(protocols[i], attributes))
                    appData.GetWaveform(appData.GetCaptureRecords()-1, mDetails.GetWave());
                else
                    uid = -2;
//                BIOS::DBG::Print("z");

//                BIOS::DBG::Print("w");

                return uid;
            }
        }
        return -1;
//        BIOS::DBG::Print(".");
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
                ConfigurationSave("startup.cfg");
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
        
        // User has selected any record in list, lets show details
        if (pSender == &mCapture)
        {
            mDetails.SetUid(data);
            mMenu.Select(3);
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
};

CApplication app;

bool analyse(CArray<uint16_t>& pulse)
{
    // show in status bar
    BIOS::SYS::Beep(50);
    if (appData.GetLogging())
    {
        char fname[16];
        PULSE::uniqueName(fname);
        char message[64];
        sprintf(message, "%d pulses saved as '%s'", pulse.GetSize(), fname);
        app.ShowStatus(message);
        PULSE::dump(fname, pulse);
    }

    int uid = app.AnalyseBuffer(pulse);
    int i = uid < 0 ? uid : appData.GetCaptureIndex(uid);
    if (i == -2)
    {
        char message[64];
        sprintf(message, "%d pulses: full buffer", pulse.GetSize());
        app.ShowStatus(message);
    } else
    if (i == -1)
    {
        char message[64];
        sprintf(message, "%d pulses: unknown protocol", pulse.GetSize());
        app.ShowStatus(message);
    } else
    {
        int ts, uid;
        char name[64];
        appData.GetCaptureRecord(i, ts, uid, name, nullptr);
        
        char message[64];
        sprintf(message, "%d pulses: %s", pulse.GetSize(), name);
        app.ShowStatus(message);
    }
}

void updateWave(int i)
{
    app.updateWave(i);
}

void mainInit()
{
#ifdef __APPLE__
    BIOS::FAT::Init();
    BIOS::OS::SetArgument("RFTOOL/RFTOOL.ELF");
#endif
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
