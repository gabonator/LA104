#include <library.h>

using namespace BIOS;

void UartWrite(const uint8_t* data, int len)
{
  DBG::Print("{%d", len);
//  while (len--)
  for (int i=0; i<len; i++)
    BIOS::GPIO::UART::Write(data[i]);  
  DBG::Print("}");
}

void Init()
{
  BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
  BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);
  BIOS::GPIO::UART::Setup(38400, BIOS::GPIO::UART::length8);

//  GPIO::UART::Setup(19200, GPIO::UART::EConfig(GPIO::UART::length8 | GPIO::UART::parityEven | GPIO::UART::stopBits1));
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{   
  LCD::Clear(RGB565(404040));
  Init();
  KEY::EKey key;
  while ((key = KEY::GetKey()) != KEY::EKey::Escape)
  {
    EVERY(200)
    {
      DBG::Print(".");
      uint8_t data[] = {0xaa, 0x55, 0x0f, 0x81, 0x11, 0x00, 0x00};
      UartWrite(data, COUNT(data));
    }
  }

  return 0;
}
