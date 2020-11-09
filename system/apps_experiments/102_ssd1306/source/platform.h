#include <library.h>

typedef uint8_t byte;
typedef bool boolean;

enum {LOW = 0, HIGH = 1};
enum {INPUT = BIOS::GPIO::EMode::Input | BIOS::GPIO::EMode::PullUp, OUTPUT = BIOS::GPIO::EMode::Output};

class TwoWire
{
public:
  void begin();
  void end();
  bool beginTransmission(int addr);
  int read();
  void write(int d);
  bool endTransmission(bool stop = false);
  void requestFrom(int addr, int count);
};

void delay(int d);
void digitalWrite(int, int);
void pinMode(int, int);
long millis();
extern TwoWire Wire;

class Adafruit_GFX {
public:
  Adafruit_GFX(int16_t w, int16_t h);
  int HEIGHT;
  int WIDTH;
  int _width;
  int _height;
  uint8_t rotation;     ///< Display rotation (0 thru 3)
  virtual void setRotation(uint8_t r);
  uint8_t getRotation(void) const;
  int16_t width(void) const;
  int16_t height(void) const;
  virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                             uint16_t color);
  virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
  void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
                              int16_t w, int16_t h, uint16_t color);
};

#define PROGMEM 