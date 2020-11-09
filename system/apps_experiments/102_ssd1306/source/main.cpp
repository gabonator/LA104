#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include "Adafruit_SSD1306.h"

Adafruit_SSD1306 display(128, 64, &Wire);
uint8_t buffer[1024];

void setup()
{
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.setBuffer(buffer);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    CONSOLE::Print("SSD1306 allocation failed\n");
    return;
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  BIOS::SYS::DelayMs(500); // Pause for 2 seconds
}

void loop(BIOS::KEY::EKey key)
{
//  buffer[rand() & 1023] = rand();

//  display.drawBitmap(0, 0, buffer, 128, 64, 1);
  static int i = 0;

//  for (int j=0; j<500; j++)
//    display.drawPixel(rand() & 127, rand() & 63, i++ & 1);

  long t0 = BIOS::SYS::GetTick();
  for (int x=0; x<128; x++)
    for (int y=0; y<64; y++)
      display.drawPixel(x, y, ((x+i)/8 + (y+i/2)/8)&1);
  i++;
  long t1 = BIOS::SYS::GetTick();
  display.display();
  long t2 = BIOS::SYS::GetTick();


  EVERY(1000)
  { 
    static int lastFrame = 0;
    int frames = i - lastFrame;
    lastFrame = i;
    char info[64];
    sprintf(info, "%d fps, draw %d ms, transfer %d ms", frames, t1-t0, t2-t1);
    APP::Status(info);
  }
}


#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    APP::Init("SSD1306 128x64 oled");
    APP::Status("P1: SCL, P2: SDA");

    setup();

    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
      loop(key);
    }
    Wire.end();
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
    while (1);
}
