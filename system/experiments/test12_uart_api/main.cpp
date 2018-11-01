#include <library.h>

void serialPrint(const char* buf)
{
  while (*buf)
    BIOS::GPIO::UART::Write(*buf++);
}

__attribute__((__section__(".entry"))) 
int main(void)
{ 
  BIOS::LCD::Clear(RGB565(202020));
  BIOS::DBG::Print("start\n");

  BIOS::GPIO::UART::Setup(9600, BIOS::GPIO::UART::length8);
  BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
  BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);

  long last = BIOS::SYS::GetTick();
  while (BIOS::KEY::GetKey() != BIOS::KEY::Escape)
  {
    long l = BIOS::SYS::GetTick();
    if (l > last+1000)
    {
      last = l;
      BIOS::DBG::Print("[send]");
      serialPrint("Ahoj!\n");
      BIOS::DBG::Print("[ok]");
    }

    while (BIOS::GPIO::UART::Available())
      BIOS::DBG::Print("%c", BIOS::GPIO::UART::Read());
  }

  return 0;
}

