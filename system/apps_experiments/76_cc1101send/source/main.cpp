#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "assert.h"
#include "device/cc1101.h"

//#include <iostream>

#include "protocol/protocol.h"
#include "protocol/weather.h"

#include "streamer/streamer.h"
#include "streamer/stm32f10x.h"

#include "graph.h"
CDeviceCC1101 gModem;
#include "send.h"
#include "thermo.h"


// 10ms -> 493 pulses
// 10 000 us = 500 pulses
// 40 pulses per ms
extern int totalSamples;

using namespace BIOS;


bool setup()
{
//	srand(BIOS::SYS::GetTick());
  while (1)
  {
    CONSOLE::Print("Init... ");
    if (gModem.Init())
    {
      CONSOLE::Print("Ok!\n");
      break;
    } else
    {
      CONSOLE::Print("Failed!\n");
      if (KEY::GetKey() == KEY::EKey::Escape)
        return false;
    }
  }

  // 433.91, 135khz, 0dB	
	gModem.SetFrequency(433876000UL);
  gModem.DeltaGain(-100); 
//  gModem.DeltaGain(2); // 0..7
  gModem.DeltaBandwidth(-100);
  gModem.DeltaBandwidth(9); // 8:203khz, 10: 135khz, 12 -> 101khz



  gModem.DeltaGain(+5); // 5:-12db
  gModem.SetDataRate(4000);

//	BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P4, BIOS::GPIO::EMode::Input);


    gModem.SetRxState();
    streamerBufferMaxCounter = 4000;
    streamerBegin();
    return true;
}

void finish()
{
    streamerEnd();
    gModem.SetIdleState();
}

void Noise()
{
  uint8_t buffer[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
  if (!sendPacket(433876000UL, 4000, buffer, COUNT(buffer)))
{
BIOS::SYS::Beep(1000);
BIOS::SYS::DelayMs(1000);
} else
{
BIOS::SYS::Beep(50);
}
//  gModem.SetRxState();
//  gModem.SetDataRate(4000);
}


int signal_[100];
CArray<int> arrsignal(signal_, COUNT(signal_));
CWeather weather;

void _graphPush(int v)
{
  if (v==-1)
  {
    arrsignal.RemoveAll();
  }
  else
  {
    if (arrsignal.GetSize() < arrsignal.GetMaxSize())
      arrsignal.Add(v*25);

/*
    #define Ticks(d) ((d+10)/20)
    #define IsPulse(l) (Ticks(l) == 1)
    #define IsSpacer(l) (Ticks(l) >= 16 && Ticks(l) < 25)
    #define IsShort(l) (Ticks(l) >= 4 && Ticks(l) <= 5)
    #define IsLong(l) (Ticks(l) >= 8 && Ticks(l) <= 10)
    #define IsLongOrShort(l) (IsLong(l) || IsShort(l))
*/
    if (arrsignal.GetSize() >= weather.MinIndentifyCount())
    {
/*
      if (IsPulse(signal[0]) && 
          IsSpacer(signal[1]) && 
          IsPulse(signal[2]) && 
          IsLongOrShort(signal[3]) &&
          IsPulse(signal[4]))
*/
      if (weather.Identify(arrsignal))
      {
        BIOS::LCD::Printf(0, BIOS::LCD::Height-16-16-20, RGB565(ff0000), RGB565(0000d0), "!!!");
        arrsignal.RemoveAll();
        Noise();
      }
    }
  }
  graphPush(v);
}

void pulseMachinePush(int interval)
{
	static int buffer = -1;
	static bool bufferSet = false;
	static bool transmit = false;
	static int index = 0;
	
	if (!transmit)
	{
		if (!bufferSet)
		{
			buffer = interval;
			if (interval != -1)
			{
				bufferSet = true;
				transmit = true;
			}
			return;
		}
	}
	else
	{
		if (interval == -1)
		{
			if (!bufferSet)
				_graphPush(-1);
			bufferSet = false;
			transmit = false;
			return;
		}
		if (bufferSet)
		{
			index = 0;
			_graphPush(buffer);
			BIOS::LCD::Printf(index*32, BIOS::LCD::Height-16-16-20, RGB565(ffffff), RGB565(0000d0), "%4d", buffer);
			buffer = -1;
			bufferSet = false;
		}
		index++;
		if (index < 9)
			BIOS::LCD::Printf(index*32, BIOS::LCD::Height-16-16-20, RGB565(ffffff), RGB565(0000d0), "%4d", interval);
		_graphPush(interval);
	}
}

void pulseMachine(int interval)
{
  // L:1000, H:0, L:1000, H:0, L:300, H:1000, L:0, H:1000, L:0, H:100, L:500, H:500, L:500, H:500
  // -> L:2300, H:2100, L:500, H:500, L:500, H:500

  // 1000,0; 
  static int interval1 = 0;
  static int interval2 = 0;
  static bool leading = true;
	static bool terminated = false;

	if (interval == -1)
	{
// dolozit?
		interval1 = 0;
		interval2 = 0;
		leading = true;
		pulseMachinePush(-1);
		return;
	}
  if (interval1 == 0)
  {
    interval += interval2;
  }

  if (interval > 400)
  {
//    if (!leading)
//      pulseMachinePush(500);
	  if (!terminated)
	  {
		  terminated = true;
			pulseMachinePush(-1);
	  }
    leading = true; // aj tak ho potom pretlaci cez interval2!!
//    interval2 = 0;
//    interval1 = 0;
//    return;
  }

  if (interval1 != 0 && interval2 != 0)
  {
    if (leading)
      leading = false;
    else
	{
      pulseMachinePush(interval2);
		terminated = false;
	}
  }

  interval2 = interval1;
  interval1 = interval;
}

void StreamerPreview()
{
	if (streamerOverrun)
	{
		BIOS::DBG::Print("Overflow!");
		streamerOverrun = 0;
	}
#ifdef __APPLE__
	EVERY(100)
	{
		static int pulse[] = {  1000, 0, 1000, 0, 300, // leading 1300
			/*1000, 0, 1000, 0,*/ 100, 500, 500, 500, 500,  // data
			1000, 0, 1000, 0, 1000, 0, 1000, 0, 1000, 0, 1000, 0, 300, // leading 6300
			//1000, 0, 1000, 0, 100, 500, 500, 500, 500, // data
			0,
			1000, 0, 1000, 0, 1000, 0, 1000, 0, // trailing 4000
			1000, 0, 1000, 0, 1000, 0, 1000, 0, // trailing 4000
			1000, 0, 1000, 0, 1000, 0, 1000, 0, // trailing 4000
		};
		
		static int pos = 0;
		streamerBuffer.push(pulse[pos++]);
		if (pos >= COUNT(pulse))
			pos = 0;
	}
#endif
    static int p = 0;
    static int x=0;
  
	int n = streamerBuffer.size();

    static int lx = 0;
	int by = BIOS::LCD::Height-16-20;
int c = BIOS::GPIO::DigitalRead(BIOS::GPIO::EPin::P4) ? RGB565(ffffff) : RGB565(000000); 
int k = 80;
      BIOS::LCD::Bar(x/k, by+2, x/k+2, by+18, c);
    for (int i=0; i<n; i++, p++)
    {
      int v = streamerBuffer.pull();
      pulseMachine(v);
/*
static bool lastLong = false;

if (v!=0)
{
if (v == 4000)
{
  if (!lastLong)
  {
    lastLong = true;
    BIOS::DBG::Print("-, ");
  }
} else
{
  lastLong = false;
  BIOS::DBG::Print("%d, ", v);
}
}
*/
      if (v > 0 && v<k) v = k;
      BIOS::LCD::Bar(lx/k, by+2, (lx)/k+2, by+18, RGB565(0000b0));
      BIOS::LCD::Bar(x/k, by+2, x/k+2, by+18, c);
      lx = x;
		int x1 = min((x+v)/k, BIOS::LCD::Width);
      BIOS::LCD::Bar(x/k, by, x1, by+2, (p & 1) ? RGB565(ff0000) : RGB565(0000b0));
      BIOS::LCD::Bar(x/k, by+18, x1, by+20, (p & 1) ? RGB565(0000b0) : RGB565(00ff00));
      x += v;
      if (x/k >= BIOS::LCD::Width)
        x -= BIOS::LCD::Width * k;
    }

}


void drop()
{
    int n = streamerBuffer.size();
    for (int i=0; i<n - (n&1); i++)
    {
        int v = streamerBuffer.pull();
    }
}

void loop()
{
  StreamerPreview();

//  {EVERY(1000){ sendThermo();    CONSOLE::Print("."); }}
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
    LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "CC1101 test signal generator");

    if (!setup())
      return 1;

    CRect rc2(rcClient);
    rc2.top = rc2.bottom-14;
    GUI::Background(rc2, RGB565(404040), RGB565(202020));
    LCD::Printf(8, rc2.top, RGB565(808080), RGBTRANS, "F: %.1f MHz, BW: %d kHz, G: %d dB", gModem.GetFrequency() / 1e6f, gModem.GetBandwidth() / 1000, gModem.GetGain());

    KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
      static bool run = true;
      if (key == KEY::EKey::F1)
        run = !run;
      if (run)
        loop();
      else
        drop();
    }
    finish();
    
    return 0;
}
