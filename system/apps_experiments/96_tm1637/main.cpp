#include <library.h>
#include "../../os_host/source/framework/Console.h"

#include "TM1637Display.cpp"

using namespace BIOS;


// Module connection pins (Digital Pins)
#define CLK BIOS::GPIO::EPin::P2
#define DIO BIOS::GPIO::EPin::P1

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   1000

const uint8_t SEG_DONE[] = {
	SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
	SEG_C | SEG_E | SEG_G,                           // n
	SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
	};

TM1637Display display(CLK, DIO);

bool _Delay(int ms)
{
  for (int i=0; i<ms; i += 10)
  {
    BIOS::SYS::DelayMs(10);
    if (BIOS::KEY::GetKey() != BIOS::KEY::None)
      return false;
  }
  return true;
}

bool loop()
{
  #define Delay(ms) if (!_Delay(ms)) return false;

  int k;
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
  display.setBrightness(0x0f);

  // All segments on
  display.setSegments(data);
  BIOS::SYS::DelayMs(TEST_DELAY);

  // Selectively set different digits
  data[0] = display.encodeDigit(0);
  data[1] = display.encodeDigit(1);
  data[2] = display.encodeDigit(2);
  data[3] = display.encodeDigit(3);
  display.setSegments(data);
  Delay(TEST_DELAY);

  /*
  for(k = 3; k >= 0; k--) {
	display.setSegments(data, 1, k);
	Delay(TEST_DELAY);
	}
  */

  display.clear();
  display.setSegments(data+2, 2, 2);
  Delay(TEST_DELAY);

  display.clear();
  display.setSegments(data+2, 2, 1);
  Delay(TEST_DELAY);

  display.clear();
  display.setSegments(data+1, 3, 1);
  Delay(TEST_DELAY);


  // Show decimal numbers with/without leading zeros
  display.showNumberDec(0, false); // Expect: ___0
  Delay(TEST_DELAY);
  display.showNumberDec(0, true);  // Expect: 0000
  Delay(TEST_DELAY);
	display.showNumberDec(1, false); // Expect: ___1
	Delay(TEST_DELAY);
  display.showNumberDec(1, true);  // Expect: 0001
  Delay(TEST_DELAY);
  display.showNumberDec(301, false); // Expect: _301
  Delay(TEST_DELAY);
  display.showNumberDec(301, true); // Expect: 0301
  Delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(14, false, 2, 1); // Expect: _14_
  Delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(4, true, 2, 2);  // Expect: __04
  Delay(TEST_DELAY);
  display.showNumberDec(-1, false);  // Expect: __-1
  Delay(TEST_DELAY);
  display.showNumberDec(-12);        // Expect: _-12
  Delay(TEST_DELAY);
  display.showNumberDec(-999);       // Expect: -999
  Delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(-5, false, 3, 0); // Expect: _-5_
  Delay(TEST_DELAY);
  display.showNumberHexEx(0xf1af);        // Expect: f1Af
  Delay(TEST_DELAY);
  display.showNumberHexEx(0x2c);          // Expect: __2C
  Delay(TEST_DELAY);
  display.showNumberHexEx(0xd1, 0, true); // Expect: 00d1
  Delay(TEST_DELAY);
  display.clear();
  display.showNumberHexEx(0xd1, 0, true, 2); // Expect: d1__
  Delay(TEST_DELAY);
  
	// Run through all the dots
	for(k=0; k <= 4; k++) {
		display.showNumberDecEx(0, (0x80 >> k), true);
		Delay(TEST_DELAY);
	}

  // Brightness Test
  for(k = 0; k < 4; k++)
	data[k] = 0xff;
  for(k = 0; k < 7; k++) {
    display.setBrightness(k);
    display.setSegments(data);
    Delay(TEST_DELAY);
  }
  
  // On/Off test
  for(k = 0; k < 4; k++) {
    display.setBrightness(7, false);  // Turn off
    display.setSegments(data);
    Delay(TEST_DELAY);
    display.setBrightness(7, true); // Turn on
    display.setSegments(data);
    Delay(TEST_DELAY);  
  }

 
  // Done!
  display.setSegments(SEG_DONE);
  return true;
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    CONSOLE::window.left = 8;
    CONSOLE::cursor = CONSOLE::window.TopLeft();

    CRect rcClient(0, 0, LCD::Width, LCD::Height);
    LCD::Bar(rcClient, RGB565(0000b0));
    
    CRect rc1(rcClient);
    rc1.bottom = 14;
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));    
    LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "TM1637 display test");

    CRect rc2(rcClient);
    rc2.top = rc2.bottom-14;
    GUI::Background(rc2, RGB565(404040), RGB565(202020));
    LCD::Print(8, rc2.top, RGB565(808080), RGBTRANS, "P1: DIO, P2: CLK");

    KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
      if (!loop())
        return 0;
      BIOS::SYS::DelayMs(1000);
    }
    
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    CONSOLE::Color(RGB565(ffff00));
    CONSOLE::Print("Assertion failed in ");
    CONSOLE::Print(file);
    CONSOLE::Print(" [%d]: %s\n", line, cond);
#ifdef __APPLE__
    //kill(getpid(), SIGSTOP);
#endif
    while (1);
}

