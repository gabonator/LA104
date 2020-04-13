#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "assert.h"

using namespace BIOS;

#include "vfd/debug.h"
#include "vfd/packet.h"
#include "vfd/serial.h"
#include "vfd/invt.h"
#include "vfd/invtgd10.h"

#include "vfdAttributes.h"
#include "vfdManager.h"

CVfdManager gVfd;

void setup()
{
  gVfd.setup();
}

void loop()
{
  gVfd.randomize();
  gVfd.loop();
}

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
    LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "Invt GD10 VFD simulator");

    CRect rc2(rcClient);
    rc2.top = rc2.bottom-14;
    GUI::Background(rc2, RGB565(404040), RGB565(202020));
    LCD::Print(8, rc2.top, RGB565(808080), RGBTRANS, "RS485: 19200 bauds, 8-E-1");

    GPIO::PinMode(GPIO::P1, GPIO::Uart);
    GPIO::PinMode(GPIO::P2, GPIO::Uart);
    GPIO::UART::Setup(19200, GPIO::UART::EConfig(GPIO::UART::length8 | GPIO::UART::parityEven | GPIO::UART::stopBits1));

    setup();
    KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
      loop();
    }
    
    GPIO::PinMode(GPIO::P1, GPIO::Output);
    GPIO::PinMode(GPIO::P2, GPIO::Input);
    return 0;
}
