#include <library.h>
#include "../../os_host/source/framework/Console.h"

using namespace BIOS;

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
    LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "UART monitor");

    CRect rc2(rcClient);
    rc2.top = rc2.bottom-14;
    GUI::Background(rc2, RGB565(404040), RGB565(202020));
    LCD::Print(8, rc2.top, RGB565(808080), RGBTRANS, "19200 bauds, 8-E-1, hex");

    GPIO::PinMode(GPIO::P1, GPIO::Uart);
    GPIO::PinMode(GPIO::P2, GPIO::Uart);
    GPIO::UART::Setup(19200, GPIO::UART::EConfig(GPIO::UART::length8 | GPIO::UART::parityEven | GPIO::UART::stopBits1));

    KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
      if (GPIO::UART::Available())
      {
        uint8_t data = GPIO::UART::Read();
        CONSOLE::Print("%02x ", data);
      }
    }

    GPIO::PinMode(GPIO::P1, GPIO::Input);
    GPIO::PinMode(GPIO::P2, GPIO::Input);
    
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

