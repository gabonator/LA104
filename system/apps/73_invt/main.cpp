#include <library.h>
#include "../../os_host/source/framework/Console.h"

using namespace BIOS;

void Serial_print(char* msg)
{
  CONSOLE::Print(msg);
}

void Serial_print(const char* msg)
{
  CONSOLE::Print(msg);
}

void Serial_print(int v)
{
  CONSOLE::Print("%d", v);
}

#include "assert.h"
#include "packet.h"
#include "serial.h"
#include "invt.h"
#include "invtgd10.h"

CInvtGD10 gGD10;
CVfd gVfd;

void setup()
{
  gVfd.setup();
}

void loop()
{
  static long lastRequest = 0;
  static bool processing = false;
  static int seq = 0;
  
  long now = SYS::GetTick();
  if (now - lastRequest > 2000 && !processing)
  {
    lastRequest = now;
    processing = true;

    static uint16_t requestBase;
    
    CVfdPacket packet;
    switch (seq++ % 2)
    {
      case 0: 
        requestBase = 0x3001;
        gVfd.buildReadPacket(packet, requestBase, 0x0009);
        break;
      case 1:
        requestBase = 0x5000;
        gVfd.buildReadPacket(packet, requestBase, 0x0001);
        break;
    }

    gVfd.send(packet);
 
    gVfd.onPacket([](const CVfdPacket& packet)
    {
      // 2100 -> 01 03 08 00 03 00 01 00 00 01 0d 5b 42 
      // 01 adr, 03 read, 8 bytes follow
      
      Serial_print("Local read ");
      gVfd.printPacket(packet);

     // _ASSERT(packet[0] == 0x01 && packet[1] == 0x03);
     // _ASSERT(packet[2] == 2*0x0e);
      for (int i=3; i<packet.getLength()-2; i+=2, requestBase++)
      {
        uint16_t reg = requestBase;
        Serial_print("#");
        char temp[4];
        sprintf(temp, "%02x", reg);
        Serial_print(temp);

        const char* desc = gGD10.GetRegName(reg);
        if (desc)
        {
          Serial_print(" (");
          Serial_print(desc);
          Serial_print(")");
        }
        
        uint16_t value = packet.getWord(i);
        Serial_print(" = ");
        Serial_print(value);

        if (reg == 0x5000)
        {
          const char* fault = gGD10.GetFaultType(value);
          if (fault)
          {
            Serial_print(" (");
            Serial_print(fault);
            Serial_print(")");
          }
        }
        Serial_print("\n");
      }

      processing = false;
    });
    gVfd.onError([]()
    {
      Serial_print("Local packet read error!\n");
      processing = false;
    });
  }
        
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
    LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "Invt GD10 VFD test application");

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
    
    return 0;
}
