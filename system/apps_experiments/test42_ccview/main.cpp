#include <library.h>
#include "oregon.h"
#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/framework/Serialize.h"

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

  case CPinIoBase::MOSI: return BIOS::GPIO::EPin::P1;
  case CPinIoBase::SCK: return BIOS::GPIO::EPin::P2;
  case CPinIoBase::CS: return BIOS::GPIO::EPin::P3;
  case CPinIoBase::D: return BIOS::GPIO::EPin::P4;
  case CPinIoBase::MISO: return BIOS::GPIO::EPin::CH4;

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
            case CPinIoBase::MOSI: return BIOS::GPIO::EPin::P1;
            case CPinIoBase::SCK: return BIOS::GPIO::EPin::P2;
            case CPinIoBase::CS: return BIOS::GPIO::EPin::P3;
            case CPinIoBase::D: return BIOS::GPIO::EPin::P4;
            case CPinIoBase::MISO: return BIOS::GPIO::EPin::CH4;
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
        return BIOS::GPIO::DigitalRead(Pin(p));
    }
};

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

int x = 0;
uint16_t blkOn[]    = {RGB565(ffff00), RGB565(ffff00), RGB565(ffff00), RGB565(ffff00), RGB565(ffff00), RGB565(ffff00), RGB565(ffff00), RGB565(ffff00)};
uint16_t blkOff[]   = {RGB565(808080), RGB565(808080), RGB565(808080), RGB565(808080), RGB565(808080), RGB565(808080), RGB565(808080), RGB565(808080)};
uint16_t blkClear[] = {RGB565(ff0000), RGB565(ff0000), RGB565(ff0000), RGB565(ff0000), RGB565(ff0000), RGB565(ff0000), RGB565(ff0000), RGB565(ff0000)};

byte OregonRead()
{
  return GlobalPort.Get(CPinIoBase::D);
}

void CSPI::init()
{
}

uint8_t CSPI::send(uint8_t data)
{
    return Spi.transfer(data);
}

  void CSPI::select() { Spi.select(); }
  void CSPI::deselect() { Spi.deselect(); }
  void CSPI::wait() { Spi.wait(); }

//constexpr int ticks20us = 200;
constexpr int ticks20us = 140;
// 200 -> 300 1420 2920
// 150 -> 420 1840 3780
// 140 -> 440 1940 4020 // 42 .. 46
// 130 -> 480 2080 4260
//            210  432
// 120 -> 520 2240 4600

int On()
{
  int i;
  for (i=0; i<8000; i+=20)
  {
    volatile int j=ticks20us; 
    while (j--);

    if (!OregonRead())
      return i;
  }
  return i;
}

int Off()
{
  int i;
  for (i=0; i<8000; i+=20)
  {
    volatile int j=ticks20us; 
    while (j--);

    if (OregonRead())
      return i;
  }
  return i;
}

  int buffer[256];
  int bufferi = 0;
  int scroll = 0;
  int step = 20;
  int tempBuffer[10];
  int tempi = 0;

constexpr int w = RGB565(ffffff);
constexpr int k = RGB565(303030);

uint16_t blkSwap[10] = {w, w, w, w, w, w, w, w, w, w};
uint16_t blk1[10]    = {w, k, k, k, k, k, k, k, k, k};
uint16_t blk0[10]    = {k, k, k, k, k, k, k, k, k, w};

void Graph()
{
  int index = scroll;
  int remain = 0;
  int lastlevel = 1;
  int x;
  for (x=0; x<320 && index < bufferi; x++, remain -= step)
  {
    if (remain <= 0)
      remain = buffer[index++];

    CRect rcBuf(x, 168, x+1, 168+10);
    BIOS::LCD::BufferBegin(rcBuf);

    int curlevel = 1-(index & 1);
    if (curlevel != lastlevel)
    {
      lastlevel = curlevel;
      BIOS::LCD::BufferWrite(blkSwap, 10);
    } else
    if (curlevel)
    {
      BIOS::LCD::BufferWrite(blk1, 10);
    } else
    if (!curlevel)
    {
      BIOS::LCD::BufferWrite(blk0, 10);
    }
  }
  if ( x<320)
  {
    CRect rcClear(x, 168, BIOS::LCD::Width, 168+10);
    BIOS::LCD::Bar(rcClear, k);
  }
}

void Decode()
{
  bufferi = 0;

  while (1)
  {
    int nOn = On();
    
    if (nOn < 8000)
    {
      buffer[bufferi++] = nOn;
      if (bufferi >= COUNT(buffer))
        break;
    }
    else 
      break;

    int nOff = Off();
    
    if (nOff < 8000)
    {
      buffer[bufferi++] = nOff;
      if (bufferi >= COUNT(buffer))
        break;
    }
    else 
      break;
  }

  if (bufferi < 10)
  {
    memcpy(tempBuffer, buffer, 10*sizeof(tempBuffer[0]));
    bufferi = tempi;
    return;
  }

  memcpy(tempBuffer, buffer, 10*sizeof(tempBuffer[0]));
  tempi = bufferi;
/*
  500 - 500us
  1000
  4000   - 4ms

  100us/div -> 40 slots
*/

  uint8_t histoDuration[60];
  uint8_t histoDurationHigh[60];
  uint8_t histoDurationLow[60];
  uint8_t histoDurationRising[60];
  uint8_t histoDurationFalling[60];

  memset(histoDuration, 0, sizeof(histoDuration));
  memset(histoDurationHigh, 0, sizeof(histoDurationHigh));
  memset(histoDurationLow, 0, sizeof(histoDurationLow));
  memset(histoDurationRising, 0, sizeof(histoDurationRising));
  memset(histoDurationFalling, 0, sizeof(histoDurationFalling));

  for (int i=0; i<bufferi; i++)
  {
    int slot = buffer[i] / 100; // 0..60
    if (slot >= COUNT(histoDuration))
    {
      slot = COUNT(histoDuration)-1;
    }

    histoDuration[slot]++;
    if ((i%2) == 0)    
      histoDurationLow[slot]++;
    else
      histoDurationHigh[slot]++;
    if (i>2)
    {
      int slot2 = (buffer[i] + buffer[i-1])/100;
      if (slot2 < COUNT(histoDurationRising))
      {
        if ((i%2)==0)
          histoDurationRising[slot2]++;
        else
          histoDurationFalling[slot2]++;
      }
    }
  }

  // 320px 4
  for (int i=0; i<COUNT(histoDuration); i++)
  {
    CRect rc(8+i*5, 8, 8+i*5+3, 160);
    CRect rcCurrent = rc;
    CRect rcOverflow = rc;
    rcOverflow.bottom = rcOverflow.top + 4;
    BIOS::LCD::Bar(rcOverflow, RGB565(ff0000));

    rcCurrent.top = rcCurrent.bottom - histoDurationLow[i]*2;
    if (rcCurrent.top < rc.top)
      continue;
    BIOS::LCD::Bar(rcCurrent, RGB565(0000b0));
    rcCurrent.bottom = rcCurrent.top;
    rcCurrent.top = rcCurrent.bottom - histoDurationHigh[i]*2;
    if (rcCurrent.top < rc.top)
      continue;
    BIOS::LCD::Bar(rcCurrent, RGB565(00b000));
    rcCurrent.bottom = rcCurrent.top;
    rcCurrent.top = rcCurrent.bottom - histoDurationRising[i]*2;
    if (rcCurrent.top < rc.top)
      continue;
    BIOS::LCD::Bar(rcCurrent, RGB565(ffff00));
    rcCurrent.bottom = rcCurrent.top;
    rcCurrent.top = rcCurrent.bottom - histoDurationFalling[i]*2;
    if (rcCurrent.top < rc.top)
      continue;
    BIOS::LCD::Bar(rcCurrent, RGB565(ff00ff));
    rcCurrent.bottom = rcCurrent.top;
    rcCurrent.top = rc.top;
    if (rcCurrent.top < rc.top)
      continue;
    if (i%10 == 0)
      BIOS::LCD::Bar(rcCurrent, RGB565(383838));
    else
      BIOS::LCD::Bar(rcCurrent, RGB565(303030));
  }
  BIOS::LCD::Printf(20, 240-16, RGB565(ffffff), RGB565(202020), " n=%d    ", bufferi);
}

void Urob()
{
    GlobalPort.Init();
    Spi._io = &GlobalPort;
    Spi.begin();

    while (!Oregon.Receiver.Init())
    {
      BIOS::DBG::Print("Cannot initialize receiver!\n");
      if (BIOS::KEY::GetKey() != BIOS::KEY::None) 
        return;
      BIOS::SYS::DelayMs(1000);
    }
    bool paused = false;
    {
        BIOS::KEY::EKey key;
        while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
        {
          if (key == BIOS::KEY::F1)
          {
            paused = !paused;
            BIOS::LCD::Printf(20, 240-16, RGB565(ffffff), RGB565(202020), paused ? "paused   " : "running   ");
            if (paused)
              Graph();
          }

          if (key == BIOS::KEY::F3)
          {
            static int counter = 0;
            BIOS::LCD::Printf(20, 240-16, RGB565(ffffff), RGB565(202020), "saving rf%03d.sig     ", counter);
            char fileName[16];
            sprintf(fileName, "rf%03d.sig", counter++);
            CBufferedWriter writer;
            if (writer.Open( fileName ))
            {
              char temp[8];
              writer << "[";
              for (int i=0; i<bufferi; i++)
              {
                if (i>0)
                  sprintf(temp, ", %d", buffer[i]);
                else
                  sprintf(temp, "%d", buffer[i]);
                writer << temp;
              }
              writer << "]";
              writer.Close();
              BIOS::LCD::Printf(20, 240-16, RGB565(ffffff), RGB565(202020), "Ok!                ");
            } else
            {
              BIOS::LCD::Printf(20, 240-16, RGB565(ffffff), RGB565(202020), "Failed!            ");
            }
          }

          if (paused && key == BIOS::KEY::Left && scroll > 0)
          {
            scroll--;
            Graph();
          }
          if (paused && key == BIOS::KEY::Right)
          {
            scroll++;
            Graph();
          }
          if (paused && key == BIOS::KEY::Up && step > 0)
          {
            step--;
            Graph();
          }
          if (paused && key == BIOS::KEY::Down)
          {
            step++;
            Graph();
          }
          if (paused)
            continue;

          if (OregonRead())
            Decode();
        }
    }
}


#ifndef __APPLE__
__attribute__((__section__(".entry")))
#endif
int _main(void)
{ 
  BIOS::LCD::Clear(RGB565(404040));
    
  Urob();

  return 0;
}

