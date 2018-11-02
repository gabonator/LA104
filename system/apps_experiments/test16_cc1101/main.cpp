#include <library.h>
#include "oregon.h"

//#define TEST

/*
  Oregon scientific weather station sniffer

  https://github.com/matthewbednarski/arduino-projects/blob/master/hardware/cc1101-rf1100se.md
  CC1101 (RF1100se module) pinout:

  VDD   [1] 2    VDD
   SI    3  4    SCK
   SO    5  6    GDO2
  /CS    7  8    GDO0
  GND    9 10    GND
 
  LA104 Connections:

  GND -> GND
  P1  -> SCK
  P2  -> SI
  P3  -> SO
  P4  -> /CS during startup and then switch to GDO0 (demodulated OOK output)
  3V+ -> VDD
*/

class CSpi
{
  // https://github.com/Flickerstrip/SoftwareSPI/blob/master/SoftwareSPI.cpp
  enum {LOW = 0, HIGH = 1};
  BIOS::GPIO::EPin _pin_sck;
  BIOS::GPIO::EPin _pin_mosi;
  BIOS::GPIO::EPin _pin_miso;
  BIOS::GPIO::EPin _pin_cs;
  bool _disableCs;

public:
  CSpi(BIOS::GPIO::EPin pin_sck, BIOS::GPIO::EPin pin_mosi, BIOS::GPIO::EPin pin_miso, BIOS::GPIO::EPin pin_cs) 
  {
      _pin_sck = pin_sck;
      _pin_mosi = pin_mosi;
      _pin_miso = pin_miso;
      _pin_cs = pin_cs;
      _disableCs = false;
  }

  void begin() 
  {
      deselect();
      BIOS::GPIO::PinMode(_pin_cs, BIOS::GPIO::Output);

      BIOS::GPIO::PinMode(_pin_sck, BIOS::GPIO::Output);
      BIOS::GPIO::PinMode(_pin_mosi, BIOS::GPIO::Output);
      BIOS::GPIO::PinMode(_pin_miso, BIOS::GPIO::Input);
  }

  uint8_t transfer(uint8_t b) 
  {
    uint8_t rec = 0;

    for (int i = 0; i < 8; i++) 
    {
        BIOS::GPIO::DigitalWrite(_pin_mosi, b & (1<<(7-i))); //MSB first
        BIOS::GPIO::DigitalWrite(_pin_sck, HIGH);
        if (BIOS::GPIO::DigitalRead(_pin_miso))
          rec |= 1<<(7-i);
        BIOS::GPIO::DigitalWrite(_pin_sck, LOW);
    }

    return rec;
  }

  void select() 
  {
    if (_disableCs)
      return;
    BIOS::GPIO::DigitalWrite(_pin_cs,LOW);
  }

  void deselect() 
  {
    if (_disableCs)
      return;
    BIOS::GPIO::DigitalWrite(_pin_cs,HIGH);
  }

  void wait()
  {
    if (_disableCs)
      return;

    int32_t tick = BIOS::SYS::GetTick();
    int iter = 0;
    while (BIOS::GPIO::DigitalRead(_pin_miso))
    {
      if (iter++ > 1000)
      {
        iter = 0;
        int32_t delta = BIOS::SYS::GetTick() - tick;
        if (delta > 100)
        {
          BIOS::DBG::Print("wait timeout!\n");
          return;
        }
      }
    }
  }
  
  void disableCs()
  {
    _disableCs = true;
  }
};

void delayMicroseconds(int us)
{
  us = us*12;
  while (us--)
  {
    __asm__("");
  }
}

class CX
{
  COregon Oregon;
  CSpi Spi;

public:
  static CSpi* pGlobalSpi;

public:
  CX() : Spi(BIOS::GPIO::P1, BIOS::GPIO::P2, BIOS::GPIO::P3, BIOS::GPIO::P4)
  {
  }

  bool InitOregon()
  {
    pGlobalSpi = &Spi;
    while (!Oregon.Receiver.Init())
    {
      BIOS::DBG::Print("Cannot initialize receiver!\n");
      if (BIOS::KEY::GetKey() != BIOS::KEY::None) 
        return false;
      BIOS::SYS::DelayMs(1000);
    }
    BIOS::DBG::Print("Oregon scientific ready.\n");
    Spi.disableCs();
    BIOS::GPIO::PinMode(BIOS::GPIO::P4, BIOS::GPIO::Input);
    return true;
  }

  void DoOregon()
  {  
    if ( Oregon.WaitHeader() )
    {
      if ( Oregon.GetHeader() )
      {
        byte buffer[COregon::PacketLength] = {0};
        byte received = Oregon.ReceivePacket(buffer);
        if ( received )
        {
          BIOS::DBG::Print("Rcvd: ");
          for( int i=0; i < received; i++)
            BIOS::DBG::Print("%02x ", buffer[i]);
          BIOS::DBG::Print("\n");
          Process(buffer);
        } else
          BIOS::DBG::Print("Receive error\n");
      }
    }  
  }

  void Process(uint8_t* buf)
  {
    byte nibbles[COregon::PacketLength*2] = {0};
    for (int i=0; i<COregon::PacketLength; i++)
    {
      nibbles[i*2] = buf[i]>>4;
      nibbles[i*2+1] = buf[i]&15;
    }

    switch (buf[0])
    {
      case 0xaf: ProcessTemp(nibbles); break;
      case 0xa2: ProcessRain(nibbles); break;
    }
  }
  
  void ProcessTemp(uint8_t* nib)
  {
    if (!CheckCrc(nib, 16))
    {
      BIOS::DBG::Print("CRC error\n");
      return;
    }
    if (digit(nib[11]) && digit(nib[10]) && digit(nib[9]) && digit(nib[14]) && digit(nib[13]))
    {
      float temp = ((nib[11]*100)+(nib[10]*10)+nib[9])/10;
      if (nib[12])
        temp *= -1;
      int hum = nib[14]*10 + nib[13];
      BIOS::DBG::Print("Temperature=%f C, Humidity=%d%%\n", temp, hum);
    } else {
      BIOS::DBG::Print("Corrupted data\n");
    }
  }

  void ProcessRain(uint8_t* nib)
  {
    if (!CheckCrc(nib, 18))
    {
      BIOS::DBG::Print("CRC error\n");
      return;
    }
    if (digit(nib[19]) && digit(nib[18]) && digit(nib[17]) && digit(nib[16]) && digit(nib[15]) &&
        digit(nib[15]) && digit(nib[14]) && digit(nib[13]) && digit(nib[12]) && digit(nib[11]) &&
        digit(nib[10]) && digit(nib[9]) && digit(nib[8]) && digit(nib[7]))
    {
      float rainTotal = ((nib[18]*100000)+(nib[17]*10000)+(nib[16]*1000)+(nib[15]*100)+(nib[14]*10)+nib[13])*24.2/1000;
      float rainRate = ((nib[7]*100000)+(nib[8]*10000)+(nib[9]*1000)+(nib[10]*100)+(nib[11]*10)+nib[12])*24.2/1000;
      BIOS::DBG::Print("Total rain=%f mm, Rain rate=%f mm\n", rainTotal, rainRate);
    } else {
      BIOS::DBG::Print("Corrupted data\n");
    }
  }

  bool CheckCrc(uint8_t* buf, int len)
  {
    uint8_t cs = 0;
    for (int x = 1; x < len; x++)
      cs += buf[x];

    return (cs & 15) == buf[len];
  }

  bool digit(uint8_t d)
  {
    return d>=0 && d<=9;
  }
};

CSpi* CX::pGlobalSpi = nullptr;

void CSPI::init()
{
  CX::pGlobalSpi->begin();
}

uint8_t CSPI::send(uint8_t data)
{
  return CX::pGlobalSpi->transfer(data);
}

#ifndef TEST

__attribute__((__section__(".entry"))) int main(void);

int main(void)
{ 
  BIOS::LCD::Clear(RGB565(101010));
  BIOS::DBG::Print("Built " __DATE__ " " __TIME__ "\n");

  CX cx;
  if (cx.InitOregon())
  {
    while (BIOS::KEY::GetKey() == BIOS::KEY::None) 
      cx.DoOregon();
  }

  return 0;
}

#else

__attribute__((__section__(".entry"))) int main(void);

int main(void)
{ 
  BIOS::LCD::Clear(RGB565(101010));
  BIOS::DBG::Print("Built " __DATE__ " " __TIME__ "\n");

  #define CC1101_TEST2             0x2C        // Various Test Settings
  #define CC1101_TEST1             0x2D        // Various Test Settings
  #define CC1101_TEST0             0x2E        // Various Test Settings
  #define READ_SINGLE              0x80

  CSpi spi(BIOS::GPIO::P1, BIOS::GPIO::P2, BIOS::GPIO::P3, BIOS::GPIO::P4);
  spi.begin();
  while (BIOS::KEY::GetKey() == BIOS::KEY::None) 
  {
    BIOS::DBG::Print("R: ");

    spi.select();
    spi.wait();
    uint8_t rd0 = spi.transfer(CC1101_TEST1 | READ_SINGLE);
    uint8_t rd1 = spi.transfer(0);
    spi.deselect();

    BIOS::DBG::Print("%02x%02x ", rd0, rd1);
    BIOS::SYS::DelayMs(1000);
  }
  return 0;
}
#endif