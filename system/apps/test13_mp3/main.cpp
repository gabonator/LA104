//#include <library.h>
#include "MD_YX5300.h"
#include "shapes.h"
#include "../host/source/bios/Bios.h"
#include "../host/source/framework/Wnd.h"
#include "../host/source/gui/Gui.h"
#include "Menu.h"

//#include "../host/source/framework/BufferedIo.h"
//#include "../host/source/framework/Serialize.h"
//#include "../host/source/framework/Serialize.cpp"
//#include "gif/GifDecoder.h"

// void *__dso_handle = (void *)NULL;
// void *_fini = (void *)NULL;
// TODO: var init problem
//GifDecoder<11> decoder;
//CBufferedReader2 reader;
//MD_YX5300 I;
MD_YX5300* mp3;  // = nullptr

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

class CMenuMain : public CTopMenu
{
public:
    virtual TItem GetItem(int i)
    {
        switch (i)
        {
            case 0: return TItem{"YX5300 player", TItem::Static};
            case 1: return TItem{"Player", TItem::Default};
            case 2: return TItem{"Schematic", TItem::Default};
            default: return TItem{nullptr, TItem::None};
        }
    }
};

class CMenuPlayerControls : public CMenu
{
public:
    CMenuPlayerControls() : CMenu()
    {
        mItem = 2;
    }
    
    virtual TItem GetItem(int i)
    {
        switch (i)
        {
            case 0:
                switch (mItem)
                {
                    case 1: return TItem{"Prev ", TItem::Static};
                    case 2: return TItem{"Play ", TItem::Static};
                    case 3: return TItem{"Pause", TItem::Static};
                    case 4: return TItem{"Stop ", TItem::Static};
                    case 5: return TItem{"Next ", TItem::Static};
                    default: return TItem{"     ", TItem::Static};
                }
                break;
            case 1: return TItem{"<<", TItem::Default, [](){ mp3->playPrev(); }};
            case 2: return TItem{"|>", TItem::Default, [](){ mp3->playStart(); }};
            case 3: return TItem{"||", TItem::Default, [](){ mp3->playPause(); }};
            case 4: return TItem{"[]", TItem::Default, [](){ mp3->playStop(); }};
            case 5: return TItem{">>", TItem::Default, [](){ mp3->playNext(); }};
            default: return TItem{nullptr, TItem::None, nullptr};
        }
    }

    virtual void OnItem(int nItem)
    {
      CMenu::OnItem(nItem);
      DelayedQuery();
    }

    void DelayedQuery()
    {
      KillTimer();
      SetTimer(500);
    }

    void OnTimer()
    {
      KillTimer();
      Query();
    }

    void Query()
    {
      mp3->queryStatus();
      mp3->queryFile();
    }
};

class CMenuRepeatControls : public CMenu
{
    bool mRepeat{false};
    bool mShuffle{false};
    
public:
    virtual TItem GetItem(int i)
    {
        switch (i)
        {
            case 0: return TItem{"Repeat", TItem::Static};
            case 1: return TItem{mRepeat ? "[X]" : "[ ]", TItem::Default};
            case 2: return TItem{"Shuffle", TItem::Static};
            case 3: return TItem{mShuffle ? "[X]" : "[ ]", TItem::Default};
            default: return TItem{nullptr, TItem::None};
        }
    }
    virtual void OnItem(int nItem)
    {
        switch (nItem)
        {
            case 1:
                mRepeat = !mRepeat;
                mp3->repeat(mRepeat);
                break;
            case 3:
                mShuffle = !mShuffle;
                mp3->shuffle(mShuffle);
                break;
        }
        Invalidate();
    }

};

class CMenuVolumeControls : public CMenu
{
    bool mMute{false};
    char mStrVolume[8]{"100%"};
    int mVolume{0x20};

public:
    virtual TItem GetItem(int i)
    {
        static auto _this = this;
        switch (i)
        {
            case 0: return TItem{"Volume ", TItem::Static};
            case 1: return TItem{mMute ? "muted" : mStrVolume, TItem::Static, nullptr};
            case 2: return TItem{" + ", TItem::Default, [](){ _this->AddVolume(+2); /*mp3->volumeInc();*/ }};
            case 3: return TItem{" - ", TItem::Default,  [](){ _this->AddVolume(-2); /*mp3->volumeDec();*/ }};
            case 4: return TItem{mMute ? "Unmute" : "Mute", TItem::Default}; // highlight?
            default: return TItem{nullptr, TItem::None, nullptr};
        }
    }
    virtual void OnItem(int nItem)
    {
        switch (nItem)
        {
            case 4:
                mMute = !mMute;
                mp3->volumeMute(mMute);
                Invalidate();
                break;
        }
        CMenu::OnItem(nItem);
    }

    void AddVolume(int d)
    {
      int nOld = mVolume;
      mVolume = max(0, min(mVolume + d, 0x20));
      if (nOld == mVolume)
        return;

      mp3->volume(mVolume);
      sprintf(mStrVolume, "%d%%", mVolume*100/0x20);
      Invalidate();
    }
/*
    void SetVolume(int n)
    {
      mVolume = n;
      sprintf(mStrVolume, "%d%%", n);
      Invalidate();
    }
*/
};

class CMenuEqualizerControls : public CMenu
{
public:
    virtual TItem GetItem(int i) // Need to press enter to select!
    {
        switch (i)
        {
            case 0: return TItem{"Equalizer", TItem::Static};
            case 1: return TItem{"Normal", TItem::Radio, [](){ mp3->equalizer(0); }};
            case 2: return TItem{"Pop", TItem::Radio, [](){ mp3->equalizer(1); }};
            case 3: return TItem{"Rock", TItem::Radio, [](){ mp3->equalizer(2); }};
            case 4: return TItem{"Jazz", TItem::Radio, [](){ mp3->equalizer(3); }};
            case 5: return TItem{"Classic", TItem::Radio, [](){ mp3->equalizer(4); }};
            case 6: return TItem{"Base", TItem::Radio, [](){ mp3->equalizer(5); }};
            default: return TItem{nullptr, TItem::None, nullptr};
        }
    }
};

class CMenuUtilControls : public CMenu
{
public:
    virtual TItem GetItem(int i)
    {
        switch (i)
        {
            case 0: return TItem{"Reset", TItem::Default, [](){ mp3->reset(); }};
            case 1: return TItem{"Sleep", TItem::Default, [](){ mp3->sleep(); }};
            case 2: return TItem{"WakeUp", TItem::Default, [](){ mp3->wakeUp(); }};
            default: return TItem{nullptr, TItem::None};
        }
    }
};

class CPlayerStatus : public CWnd
{
    int mCurrent{0};
    int mTotal{0};
    int mStatus{-1};

public:
    virtual void OnPaint()
    {
        BIOS::LCD::Bar(m_rcClient, RGB565(b0b0b0));
        char strText[32];
        switch (mStatus)
        {
            case -1: strcpy(strText, "Offline"); break;
            case 0: strcpy(strText, "Stopped"); break;
            case 1: sprintf(strText, "Playing: %d/%d", mCurrent, mTotal); break;
            case 2: sprintf(strText, "Paused: %d/%d", mCurrent, mTotal); break;
            default: sprintf(strText, "Invalid status (%d)", mStatus); break;
        }
        BIOS::LCD::Print((m_rcClient.right + m_rcClient.left - (int)strlen(strText)*8)/2, (m_rcClient.top + m_rcClient.bottom - 14)/2,
                         RGB565(000000), RGBTRANS, strText);
    }

    void SetCurrent(int n)
    {
        if (mCurrent == n)
          return;

        mCurrent = n;
        Invalidate();
    }

    void SetTotal(int n)
    {
        if (mTotal == n)
          return;

        mTotal = n;
        Invalidate();
    }

    void SetStatus(int n)
    {
        if (mStatus == n)
          return;

        mStatus = n;
        Invalidate();
    }
};

class CWindow : public CWnd
{
    virtual void OnPaint()
    {
        GUI::Window(m_rcClient, RGB565(ffffff));
        BIOS::LCD::Print((m_rcClient.right + m_rcClient.left - (int)strlen(m_pszId)*8)/2, m_rcClient.top + 2,
                         RGB565(000000), RGBTRANS, m_pszId);
    }
};

class CPlayer : public CWnd
{
    CWindow mWndContents;
    CPlayerStatus mWndStatus;
    CMenuPlayerControls mWndPlayerControls;
    CMenuRepeatControls mWndRepeatControls;
    CMenuVolumeControls mWndVolumeControls;
    CMenuEqualizerControls mWndEqualizerControls;
    CMenuUtilControls mWndUtilControls;
  
    bool mConnected{false};
    int32_t mLastStatus{-1};
    int32_t mLastValid{-1};
    int32_t mRequestFiles{-1};
    int32_t mRequestStatus{-1};


public:
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        //mp3->setTimeout(200);
        //mp3->setSynchronous(true);
        //mp3->begin();
        //mp3->setTimeout(200);

        pParent->m_dwFlags |= CWnd::WsTick;
        CWnd::Create(pszId, dwFlags | CWnd::WsTick, rc, pParent);
        mWndContents.Create("MP3 Player", CWnd::WsVisible, CRect(20, 16+40, 320-20, 240-40), this);
        
        //SetTimer(100);
        
        CRect rcStatus(mWndContents.m_rcClient);
        rcStatus.Deflate(4, 20, 4, 0);
        rcStatus.bottom = rcStatus.top+30;
        
        mWndStatus.Create("status", CWnd::WsVisible | CWnd::WsNoActivate, rcStatus, &mWndContents);
        mWndPlayerControls.Create("playerCtl", CWnd::WsVisible, &mWndContents);
        mWndRepeatControls.Create("repeatCtl", CWnd::WsVisible, &mWndContents);
        mWndVolumeControls.Create("volumeCtl", CWnd::WsVisible, &mWndContents);
        mWndEqualizerControls.Create("eqCtl", CWnd::WsVisible, &mWndContents);
        mWndUtilControls.Create("utilCtl", CWnd::WsVisible, &mWndContents);
        mWndPlayerControls.SetFocus();
        
        static CPlayer* _this = this;
        
        mp3->setCallback([](const MD_YX5300::cbData *status) {
            const char* strCode = "";
            switch (status->code)
            {
                case MD_YX5300::STS_OK:         strCode = "STS_OK";         break;
                case MD_YX5300::STS_TIMEOUT:    strCode = "STS_TIMEOUT";    break;
                case MD_YX5300::STS_VERSION:    strCode = "STS_VERSION";    break;
                case MD_YX5300::STS_CHECKSUM:   strCode = "STS_CHECKSUM";   break;
                case MD_YX5300::STS_TF_INSERT:  strCode = "STS_TF_INSERT";  break;
                case MD_YX5300::STS_TF_REMOVE:  strCode = "STS_TF_REMOVE";  break;
                case MD_YX5300::STS_ERR_FILE:   strCode = "STS_ERR_FILE";   break;
                case MD_YX5300::STS_ACK_OK:     strCode = "STS_ACK_OK";     break;
                case MD_YX5300::STS_FILE_END:   strCode = "STS_FILE_END";   break;
                case MD_YX5300::STS_INIT:       strCode = "STS_INIT";       break;
                case MD_YX5300::STS_STATUS:     strCode = "STS_STATUS";     break;
                case MD_YX5300::STS_EQUALIZER:  strCode = "STS_EQUALIZER";  break;
                case MD_YX5300::STS_VOLUME:     strCode = "STS_VOLUME";     break;
                case MD_YX5300::STS_TOT_FILES:  strCode = "STS_TOT_FILES";  break;
                case MD_YX5300::STS_PLAYING:    strCode = "STS_PLAYING";    break;
                case MD_YX5300::STS_FLDR_FILES: strCode = "STS_FLDR_FILES"; break;
                case MD_YX5300::STS_TOT_FLDR:   strCode = "STS_TOT_FLDR";   break;
                default: strCode = "STS_???";
                    break;
            }

            char strStatus[64];
            sprintf(strStatus, "%02x %02x: %s", status->code, status->data, strCode);
            _this->ShowStatus(strStatus);

            if (status->code == MD_YX5300::STS_TIMEOUT)
            {
                _this->mConnected = false;
            }
            if (status->code != MD_YX5300::STS_TIMEOUT)
            {
                _this->mLastStatus = BIOS::SYS::GetTick();
                if (!_this->mConnected)
                {
                    _this->mRequestFiles = BIOS::SYS::GetTick() + 1000;
                    _this->mRequestStatus = BIOS::SYS::GetTick() + 2000;
                    _this->mConnected = true;
                }
            }

            if (status->code == MD_YX5300::STS_INIT || status->code == MD_YX5300::STS_TF_INSERT)
                _this->mRequestFiles = BIOS::SYS::GetTick() + 3000;
            
            if (status->code == MD_YX5300::STS_TF_REMOVE)
                _this->mRequestStatus = BIOS::SYS::GetTick() + 1000;
            
            if (status->code == MD_YX5300::STS_FILE_END)
                _this->mRequestStatus = BIOS::SYS::GetTick() + 2000;

            if (status->code == MD_YX5300::STS_PLAYING)
                _this->mWndStatus.SetCurrent(status->data);

            if (status->code == MD_YX5300::STS_STATUS)
            {
                _this->mWndStatus.SetStatus(status->data & 0xff);
                _this->mLastValid = BIOS::SYS::GetTick();
            }

            if (status->code == MD_YX5300::STS_TOT_FILES)
                _this->mWndStatus.SetTotal(status->data);

//            if (status->code == MD_YX5300::STS_VOLUME)
//		_this->mWndVolumeControls.SetVolume(status->data);
        });
    }

    void ShowStatus(char* msg)
    {
        CRect rcStatus(0, BIOS::LCD::Height-14, BIOS::LCD::Width, BIOS::LCD::Height);
        GUI::Background(rcStatus, RGB565(404040), RGB565(101010));
        if (msg)
            BIOS::LCD::Print(rcStatus.left, rcStatus.top, RGB565(404040), RGBTRANS, msg);
    }
    
    virtual void OnTimer()
    {
        int32_t tick = BIOS::SYS::GetTick();
        if (mLastStatus != -1 && tick - mLastStatus > 1500)
        {
            ShowStatus(nullptr);
            mLastStatus = -1;
        }

        if (mLastValid != -1 && (tick - mLastValid > 25000 || !mConnected))
        {
            mWndStatus.SetStatus(-1);
            mLastValid = -1;
        }

        if (mRequestFiles != -1 && mRequestFiles - tick < 0)
        {
            mp3->queryFilesCount();
            mRequestFiles = -1;
        }

        static int n = 0;
        if ((mRequestStatus != -1 && mRequestStatus - tick < 0) || ++n >= 200)
        {
            // every 200 x 100ms, calling this often cause choppy sound
            mRequestStatus = -1;
            n = 0;
            mp3->queryStatus();
        }
    }

    virtual void OnPaint()
    {
        GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
    }

    virtual void WindowMessage(int nMsg, int nParam)
    {
        if (nMsg == CWnd::WmTick)
            mp3->check();

        if (nMsg == CWnd::WmWillShow)
            SetTimer(100);

        if (nMsg == CWnd::WmWillHide)
            KillTimer();

        CWnd::WindowMessage(nMsg, nParam);
    }
};

class CSchematic : public CWnd
{
public:
    virtual void OnPaint()
    {
        BIOS::LCD::Bar(m_rcClient, RGB565(ffffff));
#if 0
        int _x = m_rcClient.left + 8;
        int _y = m_rcClient.top;
        BIOS::LCD::Print(_x, _y+=14, RGB565(000000), RGBTRANS, "Compatible devices:");
        BIOS::LCD::Print(_x, _y+=14, RGB565(000000), RGBTRANS, "  Catalex: Serial MP3 Player, v1.0");
        BIOS::LCD::Print(_x, _y+=14, RGB565(000000), RGBTRANS, "  or any other player based on YX5300");
        BIOS::LCD::Print(_x, _y+=14, RGB565(000000), RGBTRANS, "  chip");
        _y+=14;
        BIOS::LCD::Print(_x, _y+=14, RGB565(000000), RGBTRANS, "Pin connections");
        BIOS::LCD::Print(_x, _y+=14, RGB565(000000), RGBTRANS, "  CH1:               P1:  -> RX");
        BIOS::LCD::Print(_x, _y+=14, RGB565(000000), RGBTRANS, "  CH2:               P2:  -> TX");
        BIOS::LCD::Print(_x, _y+=14, RGB565(000000), RGBTRANS, "  CH3:               P3:");
        BIOS::LCD::Print(_x, _y+=14, RGB565(000000), RGBTRANS, "  CH4:               P4:");
        BIOS::LCD::Print(_x, _y+=14, RGB565(000000), RGBTRANS, "  GND: -> GND        3V+: -> VCC");
#endif
#if 0
        reader.Open((char*)"sch_mp3->gif");
        
        decoder.setFileReadCallback([]() -> uint8_t { uint8_t d; reader >> d; return d; });
        decoder.setFileReadBlockCallback([](void *buffer, int numberOfBytes) { reader >> CStream(buffer, numberOfBytes); });

        decoder.setInfoCallback([](int16_t width, int16_t height) {
            int baseX = (BIOS::LCD::Width - width)/2;
            int baseY = (BIOS::LCD::Height - height)/2;
            BIOS::LCD::BufferBegin(CRect(baseX, baseY, baseX + width, baseY + height), 0);
        });

        decoder.setDrawPixelCallback([](uint16_t c) {
            BIOS::LCD::BufferPush(c);
        });
        
        if (decoder.startDecoding() == ERROR_NONE)
            decoder.decodeFrame();
        
        reader.Close();
#endif
    }
};

class CApplication : public CWnd
{
    CMenuMain mMenu;
    CPlayer mPlayer;
    CSchematic mSchematic;
    MD_YX5300 mp3Instance;

public:
    void Create()
    {
        // TODO: global var not working!
        mp3 = &mp3Instance;
        mp3Instance.begin();
        
        CWnd::Create("Application", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
        mMenu.Create("MainMenu", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, 14), this);
        mPlayer.Create("Player", CWnd::WsHidden, CRect(0, 14, BIOS::LCD::Width, BIOS::LCD::Height), this);
        mSchematic.Create("Schematic", CWnd::WsHidden | CWnd::WsNoActivate, CRect(0, 14, BIOS::LCD::Width, BIOS::LCD::Height), this);

        mPlayer.ShowWindow( true );
    }
    
    void Destroy()
    {
    }
    
    virtual void OnMessage(CWnd* pSender, ui16 code, ui32 data)
    {
        if (code == ToWord('M', 'S'))
        {
            if (pSender == &mMenu && data == 1 && !mPlayer.IsVisible())
            {
                mPlayer.ShowWindow(true);
                mSchematic.ShowWindow(false);
                Invalidate();
            }
            if (pSender == &mMenu && data == 2 && !mSchematic.IsVisible())
            {
                mSchematic.ShowWindow(true);
                mPlayer.ShowWindow(false);
                Invalidate();
            }
        }
    }

};

// todo: move to library?
#ifdef ENTRY
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    CApplication app;

    app.Create();

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
