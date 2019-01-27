#include <library.h>
#include "oregon.h"

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print("[%d]: %s\n", line, cond);
#ifdef __APPLE__
    kill(getpid(), SIGSTOP);
#endif
    while (1);
}


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
  CH4  -> SO
  P4  -> /CS during startup and then switch to GDO0 (demodulated OOK output)
  3V+ -> VDD



            case CPinIoBase::SCK: return BIOS::GPIO::EPin::P1;
            case CPinIoBase::MOSI: return BIOS::GPIO::EPin::P2;
            case CPinIoBase::MISO: return BIOS::GPIO::EPin::P3; //BIOS::GPIO::EPin::CH4;
            case CPinIoBase::CS: return BIOS::GPIO::EPin::P4;


            case CPinIoBase::SCK: return BIOS::GPIO::EPin::P1;
            case CPinIoBase::MOSI: return BIOS::GPIO::EPin::P2;
            case CPinIoBase::MISO: return BIOS::GPIO::EPin::CH4;
            case CPinIoBase::CS: return BIOS::GPIO::EPin::P4;
            case CPinIoBase::D: return BIOS::GPIO::EPin::P3;

*/

class CPinIoBase
{
public:
    enum EPin {SCK, MOSI, MISO, CS, D};
    enum EDir {Input, Output};
    
    virtual void Init() = 0;
    virtual void PinMode(EPin p, EDir d) = 0;
    virtual void Set(EPin p, bool b) = 0;
    virtual bool Get(EPin p) = 0;
};

class CPinIoGpio : public CPinIoBase
{
public:
    virtual void Init() override
    {
    }

    BIOS::GPIO::EPin Pin(EPin p)
    {
        switch (p)
        {
            case CPinIoBase::SCK: return BIOS::GPIO::EPin::P1;
            case CPinIoBase::MOSI: return BIOS::GPIO::EPin::P2;
            case CPinIoBase::MISO: return BIOS::GPIO::EPin::CH4; // P3
            case CPinIoBase::CS: return BIOS::GPIO::EPin::P4;
            case CPinIoBase::D: return BIOS::GPIO::EPin::P3; //BIOS::GPIO::EPin::P3;
            default:
                _ASSERT(0);
                return BIOS::GPIO::EPin::P1;
        }
    }
    
    virtual void PinMode(EPin p, EDir d) override
    {
        if (d == EDir::Input)
            BIOS::GPIO::PinMode(Pin(p), BIOS::GPIO::EMode::Input);
        else
            BIOS::GPIO::PinMode(Pin(p), BIOS::GPIO::EMode::Output);
    }
    
    virtual void Set(EPin p, bool b) override
    {
        BIOS::GPIO::DigitalWrite(Pin(p), b);
    }
    
    virtual bool Get(EPin p) override
    {

if (Pin(p) >= BIOS::GPIO::EPin::CH1)
{
BIOS::GPIO::DigitalRead(Pin(p));
delayMicroseconds(20);
//BIOS::SYS::DelayMs(1);
}
        return BIOS::GPIO::DigitalRead(Pin(p));
    }
};
/*
class CPinIoPcf : public CPinIoBase
{
    PCF8574 port{0x38};
    uint8_t dataWr{0xff};
    uint8_t dataRd{0xff};

public:
    int Pin(EPin p)
    {
        // 0:SCK, 1:MOSI, 2:MISO, 3:CD
        return (int)p;
    }
    
    virtual void Init() override
    {
        Wire.begin();
        port.begin();
    }
    
    void Sync()
    {
        dataRd = port.read8();
        port.write8(dataWr);
    }
    
    virtual void PinMode(EPin p, EDir d) override
    {
        if (d == EDir::Input)
            Set(p, true); // disable NPN, 100uA weak pull up
        else
            Set(p, false); // short to ground
    }
    
    virtual void Set(EPin p, bool b) override
    {
        if (b)
            dataWr |= 1<<Pin(p); // pull up
        else
            dataWr &= ~(1<<Pin(p)); // strong
        
        Sync();
    }
    
    virtual bool Get(EPin p) override
    {
        Sync();

        return !!(dataRd & (1<<Pin(p)));
    }
};
*/
class CSpi
{
  // https://github.com/Flickerstrip/SoftwareSPI/blob/master/SoftwareSPI.cpp
public:
    CPinIoBase* _io;
    
public:

  void begin() 
  {
      _io->PinMode(CPinIoBase::CS, CPinIoBase::Output);
      _io->PinMode(CPinIoBase::SCK, CPinIoBase::Output);
      _io->PinMode(CPinIoBase::MOSI, CPinIoBase::Output);
      _io->PinMode(CPinIoBase::MISO, CPinIoBase::Input); // TODO:
      _io->PinMode(CPinIoBase::D, CPinIoBase::Input); 
//      select();
//      SYS::DelayMs(100);
      deselect();
  }

  uint8_t transfer(uint8_t b) 
  {
    uint8_t rec = 0;

    for (int i = 0; i < 8; i++) 
    {
        _io->Set(CPinIoBase::MOSI, b & (1<<(7-i))); //MSB first
        _io->Set(CPinIoBase::SCK, true);
        if (_io->Get(CPinIoBase::MISO))
          rec |= 1<<(7-i);
        _io->Set(CPinIoBase::SCK, false);
    }
      
    return rec;
  }

  void select() 
  {
      _io->Set(CPinIoBase::CS, false);
  }

  void deselect() 
  {
      _io->Set(CPinIoBase::CS, true);
  }

  void wait()
  {
    int32_t tick = BIOS::SYS::GetTick();
    int iter = 0;
    while (_io->Get(CPinIoBase::MISO))
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
};

void delayMicroseconds(int us)
{
  us = us*12;
  while (us--)
  {
    __asm__("");
  }
}

COregon Oregon;
CPinIoGpio GlobalPort;
//CPinIoPcf GlobalPort;
CSpi Spi;

byte OregonRead()
{
  return GlobalPort.Get(CPinIoBase::D);
}

class CX
{
public:
  bool InitOregon()
  {
    while (!Oregon.Receiver.Init())
    {
      BIOS::DBG::Print("Cannot initialize receiver!\n");
      if (BIOS::KEY::GetKey() != BIOS::KEY::None) 
        return false;
      BIOS::SYS::DelayMs(1000);
    }
    BIOS::DBG::Print("Oregon scientific ready.\n");
    //BIOS::GPIO::PinMode(BIOS::GPIO::P4, BIOS::GPIO::Input);
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

//CPinIoGpio CX::GlobalPort;
//CSpi* CX::pGlobalSpi = nullptr;

CX cx;

void CSPI::init()
{
//    Spi.Init();
}

uint8_t CSPI::send(uint8_t data)
{
    return Spi.transfer(data);
}

  void CSPI::select() { Spi.select(); }
  void CSPI::deselect() { Spi.deselect(); }
  void CSPI::wait() { Spi.wait(); }

void Urob()
{
    GlobalPort.Init();
    Spi._io = &GlobalPort;
    Spi.begin();
/*
while (1)
{
Spi.select();
BIOS::SYS::DelayMs(1);
Spi.deselect();
BIOS::SYS::DelayMs(1);
} */

    if (cx.InitOregon())
    {
        while (BIOS::KEY::GetKey() != BIOS::KEY::Escape)
            cx.DoOregon();
    }
}

#ifndef __APPLE__
__attribute__((__section__(".entry")))
#endif
int _main(void)
{ 
  BIOS::LCD::Clear(RGB565(10b010));
  BIOS::DBG::Print("Built " __DATE__ " " __TIME__ "\n");

    Urob();

  return 0;
}

