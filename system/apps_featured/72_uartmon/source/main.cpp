#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "framework/shapes.h"
#include "framework/layout.h"

using namespace BIOS;

#include "appdata.h"

class CApplication : public CApplicationData, public CWnd
{
    enum {
        PauseInterval = 50
    };
    
    friend class CPageView;
    friend class CPagePort;
    friend class CPageLog;
    friend class CPageSend;
    
    uint32_t* mGpioStatus{nullptr};
    int mRxBytes{0};
    int mTxBytes{0};
    int32_t mLastReceive{0};

public:
    void Create(const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent)
    {
        mGpioStatus = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioStatus);
        _ASSERT(mGpioStatus);
        CONSOLE::colorBack = RGB565(0000AA);
        CONSOLE::window = CRect(0, 14, BIOS::LCD::Width, 14+14*10);
        CONSOLE::cursor = CONSOLE::window.TopLeft();
        CONSOLE::lineFeed = false;

        GPIO::PinMode(GPIO::P1, GPIO::Uart);
        GPIO::PinMode(GPIO::P2, GPIO::Uart);
        ConfigureUart();

        CWnd::Create(pszId, dwFlags, rc, pParent);
        SetTimer(1000);
    }
    
    void OnPaint() override
    {
        LCD::Bar(CONSOLE::window, RGB565(0000aa));
        
        CRect rc1(m_rcClient);
        rc1.bottom = 14;
        GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
        LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "UART monitor");

        DrawStatus();
        DrawLayout();
    }
    
    void OnTimer() override
    {
        DrawStatus();
    }
    
    void DrawStatus()
    {
        CRect rc2(m_rcClient);
        rc2.top = rc2.bottom-14;
        GUI::Background(rc2, RGB565(404040), RGB565(202020));
        LCD::Printf(8, rc2.top, RGB565(808080), RGBTRANS, "%d bauds %d-%s-%s, RX: %d, TX: %d", GetBaudRate(), GetDataBits(), GetParityAsString(), GetStopBitsAsString(), mRxBytes, mTxBytes);
    }
    
    
    void HandleError(int e)
    {
        const char* errors[] = {
        "Ok",
        "I2cErrorBegin",
        "I2cBusy",
        "I2cErrorStart",
        "I2cErrorStop",
        "I2cErrorAddressAck",
        "I2cErrorReceiveTimeout",
        "I2cErrorTransmitTimeout",
        "UartWrongParity",
        "UartWrongDataBits",
        "UartWrongStopBits",
        "UartErrorInternal",
        "UartErrorOverrun",
        "UartErrorNoise",
        "UartErrorFraming",
        "UartErrorParity",
        "UartErrorOverflow",
        "NotImplemented"};

        _ASSERT(e<COUNT(errors));
        MarkError(errors[e]);
    }
    
    void MarkError(const char* msg)
    {
        CONSOLE::Color(RGB565(ff5555));
        CONSOLE::Print("<%s>", msg);
        CONSOLE::Color(RGB565(ffffff));
    }
    
    void MarkInfo(const char* msg)
    {
        CONSOLE::Color(RGB565(55ff55));
        CONSOLE::Print("<%s>", msg);
        CONSOLE::Color(RGB565(ffffff));
    }

    virtual void OnTick()
    {
        int32_t now = BIOS::SYS::GetTick();
        
#ifdef __APPLE__
        EVERY(500)
        {
            OnRecv('a');
            OnRecv('h');
            OnRecv('o');
            OnRecv('j');
            mLastReceive = now;
        }
#endif
        
        if (*mGpioStatus)
        {
            HandleError(*mGpioStatus);
            *mGpioStatus = 0;
        }

        if (GPIO::UART::Available())
        {
            mLastReceive = now;
        } else {
            if (mLastReceive != 0 && now - mLastReceive > PauseInterval)
            {
                OnPause();
                mLastReceive = 0;
            }
        }
        
        while (GPIO::UART::Available())
        {
            uint8_t data = GPIO::UART::Read();
            OnRecv(data);
        }
    }
    
    virtual void OnPause()
    {
        if (mMarkPause)
        {
            MarkInfo("");
        }
    }
    
    virtual void OnRecv(char data)
    {
        mRxBytes++;
        switch (mRxMode)
        {
            case Ascii:
                if (data == 0)
                    data = ' ';
                CONSOLE::Print("%c", data);
                if (mNewLineRx && data == '\n')
                    CONSOLE::LineFeed();
                break;
            case Hex:
                CONSOLE::Print("%02x ", data);
                break;
            case Dec:
                CONSOLE::Print("%d ", data);
                break;
        }
    }

    void Send(char data)
    {
        mTxBytes++;
        switch (mTxMode)
        {
            case Ascii:
                CONSOLE::Print("%c", data);
                if (mNewLineTx && data == '\n')
                    CONSOLE::LineFeed();
                break;
            case Hex:
                CONSOLE::Print("%02x ", data);
                break;
            case Dec:
                CONSOLE::Print("%d ", data);
                break;
        }
    }
    
    void Send(const char* str)
    {
        CONSOLE::Color(RGB565(55ff55));
        char c;
        while ((c = *str++) != 0)
        {
            GPIO::UART::Write(c);
            Send(c);
        }
        CONSOLE::Color(RGB565(ffffff));
    }
    
    void ClearLog()
    {
        LCD::Bar(CONSOLE::window, CONSOLE::colorBack);
        CONSOLE::cursor = CONSOLE::window.TopLeft();
        mRxBytes = 0;
        mTxBytes = 0;
    }
    
    void Destroy()
    {
        GPIO::PinMode(GPIO::P1, GPIO::Input);
        GPIO::PinMode(GPIO::P2, GPIO::Input);
    }
    
    virtual void ConfigureUart() override
    {
        *mGpioStatus = 0;
        GPIO::UART::Setup(mBaudrate, mFlags);
        _ASSERT(*mGpioStatus == 0);
    }
    
    virtual void WindowMessage(int nMsg, int nParam = 0) override
    {
        if (nMsg == CWnd::WmTick)
            OnTick();
        
        CWnd::WindowMessage(nMsg, nParam);
    }
    
    // gui
    virtual void DrawLayout() = 0;
    
    // logger
    virtual bool StartLogging() = 0;
    virtual void StopLogging() = 0;
    virtual char* GetLogName() = 0;
};

#include "page.h"
#include "pageport.h"
#include "pageview.h"
#include "pagesend.h"
#include "pagelog.h"
#include "logger.h"

class CApplicationGui : public CApplicationLogger
{
    int mFocus{0};
    CPagePort mPagePort{*this};
    CPageView mPageView{*this};
    CPageSend mPageSend{*this};
    CPageLog mPageLog{*this};
    CPage* mPages[4] = {&mPagePort, &mPageView, &mPageSend, &mPageLog};
    int mnPage{0};
    
public:
    void DrawMenu()
    {
        CRect rcControls(0, 14+14*10, BIOS::LCD::Width, 14+14*10+14+4);
        
        LCD::Bar(rcControls, RGB565(0000AA));
        rcControls.top += 4;
        int x = 8;
        for (int i=0; i<COUNT(mPages); i++)
        {
            if (mnPage != i)
            {
                x += BIOS::LCD::Draw(x, rcControls.top, RGB565(4040b0), RGBTRANS, CShapes_tab_left);
                x += BIOS::LCD::Print(x, rcControls.top, RGB565(b0b0b0), RGB565(4040b0), mPages[i]->Name());
                x += BIOS::LCD::Draw(x, rcControls.top, RGB565(4040b0), RGBTRANS, CShapes_tab_right);
            } else if (mY != -1) {
                x += BIOS::LCD::Draw(x, rcControls.top, RGB565(404040), RGBTRANS, CShapes_tab_left);
                x += BIOS::LCD::Print(x, rcControls.top, RGB565(ffffff), RGB565(404040), mPages[i]->Name());
                x += BIOS::LCD::Draw(x, rcControls.top, RGB565(404040), RGBTRANS, CShapes_tab_right);
            } else {
                x += BIOS::LCD::Draw(x, rcControls.top, RGB565(ffffff), RGBTRANS, CShapes_tab_left);
                x += BIOS::LCD::Print(x, rcControls.top, RGB565(000000), RGB565(ffffff), mPages[i]->Name());
                x += BIOS::LCD::Draw(x, rcControls.top, RGB565(ffffff), RGBTRANS, CShapes_tab_right);
            }
            x += 8;
        }
    }
    
    void DrawLayout() override
    {
        DrawMenu();
        DrawPage();
    }
    
    void DrawPage()
    {
        CRect rcControls(0, 14+14*10+14+4, BIOS::LCD::Width, BIOS::LCD::Height-14);
        Layout::Render r(rcControls);
        mPages[mnPage]->Draw(r);
    }
    
    virtual void OnTick() override
    {
        CApplication::OnTick();
        mPages[mnPage]->OnTick();        
    }

    virtual void OnKey(int key) override
    {
        if (mY == -1)
        {
            if (key == BIOS::KEY::Right && mnPage < COUNT(mPages)-1)
            {
                mnPage++;
                DrawLayout();
            }
            if (key == BIOS::KEY::Left && mnPage > 0)
            {
                mnPage--;
                DrawLayout();
            }
            if (key == BIOS::KEY::Down)
            {
                mY++;
                mPages[mnPage]->OnKey(0);
                DrawLayout();
            }
        } else {
            if (key == BIOS::KEY::Up)
                mY--;
            if (key == BIOS::KEY::Down)
                mY++;
            if (key == BIOS::KEY::Left)
                mX--;
            if (key == BIOS::KEY::Right)
                mX++;

            mPages[mnPage]->OnKey(key);
            if (mY == -1)
                DrawLayout();
            else
                DrawPage();
        }
    }
};

CApplicationGui app;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    app.AttachFs();
    app.Create("Application", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
	app.SetFocus();
    app.WindowMessage( CWnd::WmPaint );

    BIOS::KEY::EKey key = BIOS::KEY::None;
    while (key != BIOS::KEY::Escape)
    {
        key = BIOS::KEY::GetKey();

        if (key != BIOS::KEY::None)
            app.WindowMessage(CWnd::WmKey, key);
		
        app.WindowMessage(CWnd::WmTick);
    }
    
    app.Destroy();
    app.DetachFs();
    return 0;
}

extern "C" void __cxa_pure_virtual()
{
    _ASSERT(!"Pure virtual call");
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

