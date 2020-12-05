#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "shapes.h"
#include "layout.h"

using namespace BIOS;
    
class CApplication : public CWnd
{
    int mBaudrate{9600};
    GPIO::UART::EConfig mFlags{GPIO::UART::EConfig(GPIO::UART::length8 | GPIO::UART::parityNone | GPIO::UART::stopBits1)};
    uint32_t* mGpioStatus{nullptr};
    int mRxBytes{0};
    int mTxBytes{0};
    enum EFormatMode { Ascii, Hex, Dec };
    EFormatMode mRxMode{Ascii};
    EFormatMode mTxMode{Ascii};
    int mFocus{0};
    bool mNewLineRx{true};
    bool mNewLineTx{true};

public:
    void Create(const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent)
    {
        mGpioStatus = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioStatus);
        _ASSERT(mGpioStatus);
        CONSOLE::window = CRect(0, 14, BIOS::LCD::Width, 14+14*10);
        CONSOLE::cursor = CONSOLE::window.TopLeft();

        GPIO::PinMode(GPIO::P1, GPIO::Uart);
        GPIO::PinMode(GPIO::P2, GPIO::Uart);
        ConfigureUart();

        CWnd::Create(pszId, dwFlags, rc, pParent);
        SetTimer(1000);
    }
    
    void OnPaint() override
    {
        LCD::Bar(CONSOLE::window, RGB565(0000b0));
        
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
        CONSOLE::Color(RGB565(ff0000));
        CONSOLE::Print("<%s>", errors[e]);
        CONSOLE::Color(RGB565(ffffff));
    }
    
    void OnTick()
    {
#ifdef __APPLE__
        EVERY(500)
        {
            Recv('a');
            Recv('h');
            Recv('o');
            Recv('j');
        }
#endif
        if (*mGpioStatus)
        {
            HandleError(*mGpioStatus);
            *mGpioStatus = 0;
        }
        while (GPIO::UART::Available())
        {
            uint8_t data = GPIO::UART::Read();
            Recv(data);
        }
/*
        EVERY(1000)
        {
            static int counter = 1000;
            char message[64];
            sprintf(message, "Ahoj, toto je riadok %d.\n", counter++);
            Send(message);
        }
*/
    }
    void Recv(char data)
    {
        mRxBytes++;
        switch (mRxMode)
        {
            case Ascii:
                if (data == '\r')
                  break;
                CONSOLE::Print("%c", data);
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
                break;
            case Hex:
                CONSOLE::Print("%02x ", data);
                break;
            case Dec:
                CONSOLE::Print("%d ", data);
                break;
        }
    }
    
    void Send(char* str)
    {
        CONSOLE::Color(RGB565(00ff00));
        char c;
        while ((c = *str++) != 0)
        {
            GPIO::UART::Write(c);
            Send(c);
        }
        CONSOLE::Color(RGB565(ffffff));
    }

    const char* Format(int n)
    {
        static char temp[16];
        sprintf(temp, "%d", n);
        return temp;
    }
    
    void DrawLayout()
    {
        //DrawBaudrateLayout();
        //return;
        using namespace Layout;
        Color def(RGB565(b0b0b0));
        Color hig(RGB565(ffffff));
        
        #define S(i) Select(mFocus == i)

        CRect rcControls(0, 14+14*10, BIOS::LCD::Width, BIOS::LCD::Height-14);
        GUI::Background(rcControls, RGB565(404040), RGB565(404040));
        rcControls.Deflate(4, 4, 4, 2);

        Render r(rcControls);
        // window prilis blika, iba vnutro
        r //<< Window(heading, mRedraw)
            << def  << S(0) << Button("Clear log")
                    << S(1) << Radio(mNewLineRx, "new line on CR/LF") << "  "
                    << S(2) << Button("Open") << NewLine()
            << hig << "TX" << def << " (P1):"
                    << S(3) << Radio(mTxMode == Ascii, "ascii") << ","
                    << S(4) << Radio(mTxMode == Hex, "hex") << ","
                    << S(5) << Radio(mTxMode == Dec, "dec") << "   "
                    << S(6) << Button("Send") << NewLine()
            << hig << "RX" << def << " (P2):"
                    << S(7) << Radio(mRxMode == Ascii, "ascii") << ","
                    << S(8) << Radio(mRxMode == Hex, "hex") << ","
                    << S(9) << Radio(mRxMode == Dec, "dec") << NewLine()
        
                    << S(10) << Button(Format(mBaudrate)) << "bps"
                    << S(11) << Button(Format(GetDataBits())) << "bits"
                    << S(12) << Button(GetParityAsString()) << "parity"
                    << S(13) << Button(GetStopBitsAsString()) << "stop";
    }
    
    void DrawBaudrateLayout()
    {
        using namespace Layout;

        CRect rcControls(0, 14+14*10, BIOS::LCD::Width, BIOS::LCD::Height-14);
        GUI::Background(rcControls, RGB565(404040), RGB565(404040));
        rcControls.Deflate(4, 4, 4, 2);

        const static int baudrates[] = {110, 150, 300, 1200, 2400, 4800, 9600, 19200, 31250, 38400, 57600, 115200, 230400};
        Render r(rcControls);
        r << "Select baudrate: " << NewLine(); //<< Button("Clear log");
        for (int i=0; i<COUNT(baudrates); i++)
        {
            if (i == 6 || i == 11)
                r << NewLine();
            r << Button(Format(baudrates[i]));
        }
    }
    
    virtual void OnKey(int key) override
    {
        if (key == BIOS::KEY::Right && mFocus < 13)
        {
            mFocus++;
            DrawLayout();
        }
        if (key == BIOS::KEY::Left && mFocus > 0)
        {
            mFocus--;
            DrawLayout();
        }
        if (key == BIOS::KEY::Down)
        {
            const static int map[] = {
                3, 3, 6,
                7, 8, 9, 9,
                11, 12, 13,
                11, 11, 12, 13
            };
            mFocus = map[mFocus];
            DrawLayout();
        }
        if (key == BIOS::KEY::Up)
        {
            const static int map[] = {
                0, 1, 2,
                0, 1, 1, 2,
                3, 4, 5,
                7, 7, 8, 9
            };
            mFocus = map[mFocus];
            DrawLayout();
        }
        if (key == BIOS::KEY::Enter)
        {
            switch (mFocus)
            {
                case 0: break; // clear log
                case 1: mNewLineRx = !mNewLineRx; mNewLineTx = mNewLineRx; break; // clear log
                case 2: break; // open
                case 3: mTxMode = Ascii; break; // tx ascii
                case 4: mTxMode = Hex; break; // tx hex
                case 5: mTxMode = Dec; break; // tx dec
                case 6: Send((char*)"Ahoj!\n"); break; // send
                case 7: mRxMode = Ascii; break; // rx ascii
                case 8: mRxMode = Hex; break; // rx hex
                case 9: mRxMode = Dec; break; // rx dec
                case 10:
                    mBaudrate = CycleBaudRate(mBaudrate);
                    ConfigureUart();
                    break;
                case 11:
                    if (GetDataBits() == 8)
                        SetDataBits(9);
                    else
                        SetDataBits(8);
                    break;
                case 12:
                    if (strcmp(GetParityAsString(), "N") == 0)
                        SetPartiy("E");
                    else if (strcmp(GetParityAsString(), "E") == 0)
                        SetPartiy("O");
                    else
                        SetPartiy("N");
                    break;
                case 13:
                    if (strcmp(GetStopBitsAsString(), "1") == 0)
                        SetStopBits("1.5");
                    else if (strcmp(GetStopBitsAsString(), "1.5") == 0)
                        SetStopBits("2");
                    else
                        SetStopBits("1");
                    break;
            }
            DrawLayout();
        }

    }
    
    void Destroy()
    {
        GPIO::PinMode(GPIO::P1, GPIO::Input);
        GPIO::PinMode(GPIO::P2, GPIO::Input);
    }
    
    
    void ConfigureUart()
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
    
    int CycleBaudRate(int b)
    {
        const static int baudrates[] = {110, 150, 300, 1200, 2400, 4800, 9600, 19200, 31250, 38400, 57600, 115200, 230400};
        int newBaudrate = -1;
        for (int i=0; i<COUNT(baudrates)-1; i++)
            if (baudrates[i] == b)
                return baudrates[i+1];
        return baudrates[0];
    }
    
    void SetPartiy(const char* p)
    {
        mFlags = GPIO::UART::EConfig(mFlags & ~GPIO::UART::EConfigMask::parity);
        if (strcmp(p, "E") == 0)
            mFlags = GPIO::UART::EConfig(mFlags | GPIO::UART::EConfig::parityEven);
        else if (strcmp(p, "O") == 0)
            mFlags = GPIO::UART::EConfig(mFlags | GPIO::UART::EConfig::parityOdd);
        else if (strcmp(p, "N") == 0)
            mFlags = GPIO::UART::EConfig(mFlags | GPIO::UART::EConfig::parityNone);
        else
            _ASSERT(0);

        ConfigureUart();
    }

    void SetStopBits(const char* p)
    {
        mFlags = GPIO::UART::EConfig(mFlags & ~GPIO::UART::EConfigMask::stopBits);
        if (strcmp(p, "1") == 0)
            mFlags = GPIO::UART::EConfig(mFlags | GPIO::UART::EConfig::stopBits1);
        else if (strcmp(p, "1.5") == 0)
            mFlags = GPIO::UART::EConfig(mFlags | GPIO::UART::EConfig::stopBits15);
        else if (strcmp(p, "2") == 0)
            mFlags = GPIO::UART::EConfig(mFlags | GPIO::UART::EConfig::stopBits2);
        else
            _ASSERT(0);

        ConfigureUart();
    }

    void SetDataBits(int n)
    {
        mFlags = GPIO::UART::EConfig(mFlags & ~GPIO::UART::EConfigMask::length);
        if (n == 8)
            mFlags = GPIO::UART::EConfig(mFlags | GPIO::UART::EConfig::length8);
        else
        if (n == 9)
            mFlags = GPIO::UART::EConfig(mFlags | GPIO::UART::EConfig::length9);
        else
            _ASSERT(0);

        ConfigureUart();
    }
    
    int GetBaudRate()
    {
        return mBaudrate;
    }
    
    int GetDataBits()
    {
        if (mFlags & GPIO::UART::length9)
            return 9;
        return 8;
    }
    
    const char* GetParityAsString()
    {
        if (mFlags & GPIO::UART::parityEven)
            return "E";
        if (mFlags & GPIO::UART::parityOdd)
            return "O";
        return "N";
    }
    
    const char* GetStopBitsAsString()
    {
        if (mFlags & GPIO::UART::stopBits15)
            return "1.5";
        if (mFlags & GPIO::UART::stopBits2)
            return "2";
        return "1";
    }
};


CApplication app;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{    
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
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

