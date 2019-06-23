#include <library.h>
#include "../../os_host/source/framework/Console.h"

using namespace BIOS;

#include "simcom.h"

// https://dweet.io/follow/la104simcom900
// https://dweet.io/dweet/for/la104simcom900?counter=10123
// https://dweet.io/get/latest/dweet/for/la104simcom900

int gReset = 0;

class CMyHttpReceiver : public CHttpResponse
{
    bool mFirstLine{true};

public:
    virtual void OnHttpCode(int code) override
    {
        if (code != 200)
        {
          CONSOLE::Color(RGB565(ff00ff));
          CONSOLE::Print("HTTP Error: %d\n", code);
        }
    }
    
    virtual void OnBody(char* body, int length) override
    {
        if (!mFirstLine)
            return;
        mFirstLine = false;

        if (length > 31)
            strcpy(body+31-3, "...");

        CONSOLE::Color(RGB565(b0b0b0));
        CONSOLE::Print("Resp: ");
        CONSOLE::Color(RGB565(ffffff));
        CONSOLE::Print(body);
        CONSOLE::Print("\n");
    }

    virtual void OnFinished() override
    {
        mFirstLine = true;
    }
};

class CMyHttpRequest : public CHttpRequest
{
    char mArguments[RequestArgumentsLength];

public:
    CMyHttpRequest(const char* host, const char* path)
    {
        mProtocol = "TCP";
        mHost = host;
        mPath = path;
        mPort = 80;
        strcpy(mArguments, "");
    }
    
    virtual CAtStream& Request(CAtStream& s)
    {
        CAtStreamCounter counter;
        GetArguments(counter);
  
        s << "POST " << mPath << " HTTP/1.0\r\n"
        << "Host: " << mHost << "\r\n"
        << "User-Agent: sim800L on LA104 by valky.eu\r\n"
        << "content-type: application/x-www-form-urlencoded\r\n"
        << "content-length: " << counter.Count() << "\r\n"
        << "\r\n";
        GetArguments(s);
        s << "\r\n";

        return s;
    }
    
    void SetArguments(char *args)
    {
        _ASSERT(strlen(args) < sizeof(mArguments)-1);
        strcpy(mArguments, args);
    }
    
    virtual void GetArguments(CAtStream& s)
    {
        s << mArguments;
    }
};

class CClientApp
{
    static CClientApp* pInstance;

    CGprs mGprs;
    CHttpRequestPost mRequest{"dweet.io", "/dweet/for/la104simcom"};
    CMyHttpReceiver mResponse;

    bool shouldProcess{false};
    int mCounter{0};
public:

    void Init()
    {
        BIOS::GPIO::PinMode(BIOS::GPIO::P3, BIOS::GPIO::Output);
        // io pins before special io
        BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
        BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);

        GPIO::UART::Setup(115200, GPIO::UART::EConfig::length8);

        //gprs.SetApn("o2internet"); //O2
        mGprs.SetApn("internet"); // 4ka
        
        mGprs.SetReceiver(&mResponse);
        mGprs.AttachPrint([](char c){ GPIO::UART::Write(c); });
        mGprs.AttachPower([](bool value){ gReset++;
            BIOS::GPIO::DigitalWrite(BIOS::GPIO::P3, 1-value); });
    }

    void comm_yield()
    {
        while (GPIO::UART::Available())
        {
            char c = GPIO::UART::Read();
            mGprs << c;
            if (c == '\n')
                shouldProcess = true;
        }
    }

    void gprs_yield()
    {
      static long last = 0;
      long now = millis();
      if (shouldProcess || now > last + 500)
      {
        shouldProcess = false;
        last = now;
        mGprs();
      }
    }

    void Do()
    {
        comm_yield();
        gprs_yield();

        if (mGprs.isReady())
        {
            mResponse.Reset();
            char request[128];
            sprintf(request, "time=%d&reset=%d&counter=%d", BIOS::SYS::GetTick(), gReset, mCounter++);

            mRequest.SetArguments(request);
            mGprs.request(mRequest);
        }
    }

    CClientApp()
    {
        pInstance = this;
    }
};

CClientApp* CClientApp::pInstance = nullptr;
CClientApp app;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    CRect rcClient(0, 0, LCD::Width, LCD::Height);
    LCD::Bar(rcClient, RGB565(0000b0));
    
    CRect rc1(rcClient);
    rc1.bottom = 14;
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));    
    BIOS::LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "SIMCOM web client & dweet.io");

    CRect rc2(rcClient);
    rc2.top = rc2.bottom-14;
    GUI::Background(rc2, RGB565(404040), RGB565(202020));
    BIOS::LCD::Print(8, rc2.top, RGB565(808080), RGBTRANS, "https://dweet.io/follow/la104simcom");

    app.Init();
    BIOS::KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
        app.Do();
    }
    
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    CONSOLE::Color(RGB565(ffff00));
    CONSOLE::Print("Assertion failed in ");
    CONSOLE::Print(file);
    CONSOLE::Print(" [%d]: %s\n", line, cond);
#ifdef __APPLE__
    //kill(getpid(), SIGSTOP);
#endif
    while (1);
}

