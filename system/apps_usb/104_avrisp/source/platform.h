enum {RESET=BIOS::GPIO::P1, PIN_MOSI=BIOS::GPIO::P2, PIN_MISO=BIOS::GPIO::P3, PIN_SCK=BIOS::GPIO::P4};

enum {LOW = 0, HIGH = 1};
enum {INPUT = BIOS::GPIO::EMode::Input, OUTPUT = BIOS::GPIO::EMode::Output};
enum {MSBFIRST};

void pinMode(int pin, int mode);
void digitalWrite(int pin, int level);
int digitalRead(int pin);

class Serial 
{
public:
  int available();
  char read();
  void print(char c);
  void print(char *c);
  void print(const char *c);
};

extern Serial SERIAL;

void delay(int);
void delayMicroseconds(int);
