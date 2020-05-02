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
  gModem.DeltaGain(2); // 0..7
//  gModem.DeltaBandwidth(-100);
//  gModem.DeltaBandwidth(15); // 0..15

//	BIOS::GPIO::PinMode(BIOS::GPIO::EPin::P4, BIOS::GPIO::EMode::Input);
    gModem.SetRxState();
    streamerBufferMaxCounter = 200;
    streamerBegin();
    return true;
}

void finish()
{
	streamerEnd();
    gModem.SetIdleState();
//    gModem.Reset();
}

void pulseMachinePush(int interval)
{
static int last=-1;

if (interval==-1)
{
  graphPush(-1);
  last = -1;
  return;
} else
{
  if (last != -1)
    graphPush(last);
  else 
  {
    last = interval;
    return;
  }
}
graphPush(interval);
}

void pulseMachine(int interval)
{
  // L:1000, H:0, L:1000, H:0, L:300, H:1000, L:0, H:1000, L:0, H:100, L:500, H:500, L:500, H:500
  // -> L:2300, H:2100, L:500, H:500, L:500, H:500

  // 1000,0; 
  static int interval1 = 0;
  static int interval2 = 0;
  static bool leading = true;

	if (interval == -1)
	{
		interval1 = 0;
		interval2 = 0;
		leading = true;
		pulseMachinePush(-1);
		return;
	}
  if (interval1 == 0)
  {
    interval += interval2;
    if (interval > 200)
    {
      pulseMachinePush(50);
      pulseMachinePush(-1);
      leading = true;
    }
  }

  if (interval1 != 0 && interval2 != 0)
  {
    if (leading)
      leading = false;
    else
      pulseMachinePush(interval2);
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

    static int p = 0;
    static int x=0;
    int n = streamerBuffer.size();

    static int lx = 0;
	int by = BIOS::LCD::Height-16-20;
int c = BIOS::GPIO::DigitalRead(BIOS::GPIO::EPin::P4) ? RGB565(ffffff) : RGB565(000000); 
int k = 40;
      BIOS::LCD::Bar(x/k, by+2, x/k+2, by+18, c);
    for (int i=0; i<n; i++, p++)
    {
      int v = streamerBuffer.pull();
      pulseMachine(v);
      if (v > 0 && v<k) v = k;
      BIOS::LCD::Bar(lx/k, by+2, (lx)/k+2, by+18, RGB565(0000b0));
      BIOS::LCD::Bar(x/k, by+2, x/k+2, by+18, c);
      lx = x;
      BIOS::LCD::Bar(x/k, by, (x+v)/k, by+2, (p & 1) ? RGB565(ff0000) : RGB565(0000b0));
      BIOS::LCD::Bar(x/k, by+18, (x+v)/k, by+20, (p & 1) ? RGB565(0000b0) : RGB565(00ff00));
      x += v;
      if (x/k >= BIOS::LCD::Width)
        x -= BIOS::LCD::Width * k;
    }

}


void loop()
{
  static long lastRequest = 0;
  static int seq = 0;
  
  long now = SYS::GetTick();
//  if (now - lastRequest > 2500)
EVERY(200)
{
//BIOS::LCD::Printf(0, 0, RGB565(ffffff), RGB565(000000), "sams %d", totalSamples);
//    CONSOLE::Print("cr %d, ", TIM1->CNT);
}
/*
{EVERY(5000){
CONSOLE::Print("Start...");
startDma();
CONSOLE::Print("Data %02x %02x %02x %02x %02x %02x\n", gpio_buffer[0], gpio_buffer[1], gpio_buffer[2], gpio_buffer[3], gpio_buffer[4], gpio_buffer[5]);
}}*/

//  if (now - lastRequest >= 2000)
  {
    lastRequest = now;

//    int n = streamerBuffer.size();
//    for (int i=0; i<n; n++)
////      streamerBuffer.pull();
//    CONSOLE::Print("%d samples, %x%x%x%x, ", n, adc_fifo[0]&15, adc_fifo[1]&15, adc_fifo[2]&15, adc_fifo[3]&15);

/*
streamerBuffer.empty();
CONSOLE::Color(RGB565(ff0000));
      CONSOLE::Print(" %x ", adc_fifo[0]);

CONSOLE::Color(RGB565(b0b0b0));
    for (int i=0; i<16; i++)
      CONSOLE::Print("%x", (uint8_t)(adc_fifo[i]>>8) & 2);
CONSOLE::Color(RGB565(ffffff));
      CONSOLE::Print("\n");
*/

/*
    static int p = 0;
    int n = streamerBuffer.size();
//CONSOLE::Print("s=%d,", n);
    for (int i=0; i<n; i++, p++)
    {
      CONSOLE::Color((!(p & 1)) ? RGB565(b0b0b0) : RGB565(ffffff));
      CONSOLE::Print("%d ", streamerBuffer.pull());
   }
CONSOLE::Color(RGB565(ffffff));

*/
StreamerPreview();

//CONSOLE::Print(" ok ", n);

//TIM1->CNT = 0;

/*
//    CONSOLE::Print("Sending... ");
    if (sendThermo())
      CONSOLE::Print(".");
//      CONSOLE::Print("Ok. ");
    else
      CONSOLE::Print("Failed. \n");
*/
  }
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
      loop();
    }
    finish();
    
    return 0;
}
