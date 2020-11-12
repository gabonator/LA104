#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include "Adafruit_SSD1306.h"

void putpixel(int, int, int);
#include "starfields.h"

Adafruit_SSD1306 display(128, 64, &Wire);
uint8_t buffer[1024];

int appMode = 0;

bool setup()
{
  uint32_t *gpioI2cSpeed = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioI2cSpeed);
  if (gpioI2cSpeed)
    *gpioI2cSpeed = 400000;

  display.setBuffer(buffer);

  _ASSERT(sizeof(buffer) >= display.WIDTH * ((display.HEIGHT + 7) / 8));

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    CONSOLE::Print("SSD1306 initialization failed\n");
    BIOS::SYS::DelayMs(2000);
    return false;
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  BIOS::SYS::DelayMs(500);
  Init();
  return true;
}

void drawPatternSlow(int i)
{
  for (int x=0; x<128; x++)
    for (int y=0; y<64; y++)
      display.drawPixel(x, y, ((x+i)/8 + (y+i/2)/8)&1);
}

inline float xabs(float x)
{
  if (x>=0)
    return x;
  return -x;
}

float xsin(float x)
{
  const float tp = 1.f/(2.f*3.14159265f);
  x *= tp;
  x -= .25f + (int)(x + .25f);
  x *= 16.f * (xabs(x) - .5f);
  return x;
}

float xcos(float x)
{
  return xsin(x + 3.14159265f/2.f);
}

void drawPatternFast(int shift)
{
  uint16_t pattern[16]; // 16x16 px pattern
  uint8_t* buffer = display.getBuffer();

  int shiftX = xsin(shift/300.0f)*200+1000; // shift
  int shiftY = xcos(shift/230.0f)*200+1000; // shift/2

  constexpr uint32_t basePattern[2] = {0xff00ff00, 0x00ff00ff};

  for (int i=0; i<16; i++)
  {
    uint32_t base = basePattern[((i + shiftX)/8) & 1];
    base >>= (shiftY)&15; // max 16 bit shift, repeating pattern
    pattern[i] = (uint16_t)base;
  }

  for (int y=0; y<64/8; y+=2)
  {
    for (int x=0; x<128; x++)
      *buffer++ = (uint8_t)pattern[x&15];
    for (int x=0; x<128; x++)
      *buffer++ = (uint8_t)(pattern[x&15] >> 8);
  }
}

void putpixel(int x, int y, int c)
{
  display.drawPixel(x, y, c);
}

#include "i2c.h"

/*
namespace i2c 
{
  void start(int addr) { BIOS::GPIO::I2C::BeginTransmission(addr); }
  void write(int data) { BIOS::GPIO::I2C::Write(data); }
  void stop() { BIOS::GPIO::I2C::EndTransmission(); }
}
*/

void displayFast()
{
  static bool once = true;
  if (once) 
  {
    i2c::init();

    i2c::start(display.i2caddr);
    i2c::write(0);
    i2c::write(SSD1306_PAGEADDR);
    i2c::write(0);
    i2c::write(0xff);
    i2c::write(SSD1306_COLUMNADDR);
    i2c::write(0);
    i2c::stop();

    i2c::start(display.i2caddr);
    i2c::write(0);
    i2c::write(display.WIDTH-1);
    i2c::stop();

    once = false;
  }

  uint8_t *ptr = display.getBuffer();
  int count = display.WIDTH * ((display.HEIGHT + 7) / 8);
  i2c::start(display.i2caddr);
  i2c::write((uint8_t)0x40);
  while (count--)
    i2c::write(*ptr++);
  i2c::stop();
}

void loop(BIOS::KEY::EKey key)
{
  static int i = 0;

  if (key != BIOS::KEY::None)
  {
    appMode = (appMode + 1) % 3;
  }

  long t0 = BIOS::SYS::GetTick();

  switch (appMode)
  {
    case 0:
      drawPatternFast(i);
      break;
    case 1:
      display.clearDisplay();
      Draw();
      break;
    case 2:
      drawPatternSlow(i);
      break;
  }
 
  i++;
  long t1 = BIOS::SYS::GetTick();

  displayFast();

  //display.display();
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

    if (setup())
    {
        BIOS::KEY::EKey key;
        while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
        {
          loop(key);
        }
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
