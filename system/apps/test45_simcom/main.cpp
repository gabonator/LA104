#include <library.h>
using namespace BIOS;

#include "simcom.h"

void Log(char c, bool read)
{
    return;
    
    static int x = 0;
    static int y = 14;

    LCD::Bar(x, y, x+8, y+16, RGB565(404040));
    LCD::Printf(x, y, read ? RGB565(ffffff) : RGB565(40ff40), RGB565(404040), "%c", c);

    x += 8;
    if (x >= LCD::Width)
    {
        x = 0;
        y += 14;
        if (y > LCD::Height-14)
            y = 0;
    }
}

class CMyHttpReceiver : public CHttpResponse
{
public:
    virtual void OnHttpCode(int code) override
    {
        if (code != 200)
            BIOS::DBG::Print("HTTP Error: %d\n", code);
    }
    
    virtual void OnBody(char* body, int length) override
    {
        BIOS::DBG::Print("Resp: ");
        BIOS::DBG::Print(body);
        BIOS::DBG::Print("\n");
    }
};

class CClientApp
{
    static CClientApp* pInstance;

    CGprs gprs;
    CHttpRequestGet request{"api.valky.eu", "/test/"};
    CMyHttpReceiver response;

    bool shouldProcess{false};
public:

    void Init()
    {
        BIOS::GPIO::PinMode(BIOS::GPIO::P3, BIOS::GPIO::Output);
        // io pins before special io
        BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
        BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);
        GPIO::UART::Setup(115200, GPIO::UART::EConfig::length8);
        

        //gprs.SetApn("o2internet"); //O2
        gprs.SetApn("internet"); // 4ka
        
        gprs.SetReceiver(&response);
        gprs.AttachPrint([](char c){ Log(c, false); GPIO::UART::Write(c); });
        gprs.AttachPower([](bool value){
            BIOS::GPIO::DigitalWrite(BIOS::GPIO::P3, 1-value); });
    }

    void comm_yield()
    {
        while (GPIO::UART::Available())
        {
            char c = GPIO::UART::Read();
            Log(c, true);
            gprs << c;
            if (c == '\n')
                shouldProcess = true;
        }
    }

    void gprs_yield()
    {
      static long last = 0;
      long now = millis();
      if (shouldProcess || now > last + 1000)
      {
        shouldProcess = false;
        last = now;
        gprs();
      }
    }

    void Do()
    {
      comm_yield();
      gprs_yield();
      
      if (gprs.isReady())
      {
        response.Reset();
        request.SetArguments((char*)"value=17&something=nice");
        gprs.request(request);
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
    
    BIOS::LCD::Print(8, 0, RGB565(ffffff), RGBTRANS, "SIMCOM web client");
    DBG::Print("$\n");
    
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
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(") [%d]: %s\n", line, cond);
#ifdef __APPLE__
    //kill(getpid(), SIGSTOP);
#endif
    while (1);
}

