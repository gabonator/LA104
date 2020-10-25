#include <library.h>
//#include "assert.h"
#include "../../os_host/source/framework/Console.h"
#include "Adafruit_MLX90614.h"

uint32_t* gpioStatus = nullptr;

using namespace BIOS;

//TwoWire Wire;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup()
{
  gpioStatus = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioStatus);
  mlx.begin();  
}

void loop()
{
  CONSOLE::Print("Ambient = %.2f \xf8""C, Object = %.2f \xf8""C\n", (float)mlx.readAmbientTempC(), (float)mlx.readObjectTempC());
  if (*gpioStatus != 0)
  {
    CONSOLE::Print("[i2c error:%d]", *gpioStatus);
    *gpioStatus = 0;
  }

  BIOS::SYS::DelayMs(500);
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
    LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "MLX90614");

    CRect rc2(rcClient);
    rc2.top = rc2.bottom-14;
    GUI::Background(rc2, RGB565(404040), RGB565(202020));
    LCD::Print(8, rc2.top, RGB565(808080), RGBTRANS, "P1: SCL, P2: SDA");

    setup();
    KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
      loop();
    }
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
    while (1);
}
