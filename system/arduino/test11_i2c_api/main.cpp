#include <library.h>

#include "arduino.h"
using namespace Arduino;
#include "bmp085.h"

__attribute__((__section__(".entry"))) 
int main(void)
{ 
  BIOS::LCD::Clear(RGB565(202020));
  BIOS::DBG::Print("start\n");

  Adafruit_BMP085 bmp;

  while (BIOS::KEY::GetKey() != BIOS::KEY::Escape)
  {
    if (!bmp.begin())
    {
      BIOS::DBG::Print("not found\n");
      continue;
    }

    BIOS::DBG::Print("T=%f P=%d\n", bmp.readTemperature(), bmp.readPressure());

    BIOS::SYS::DelayMs(1000);
  }

  return 0;
}

