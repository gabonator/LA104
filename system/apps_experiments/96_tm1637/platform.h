#include <library.h>
#include <math.h>

typedef bool boolean;
typedef uint8_t byte;
#define delay(ms) BIOS::SYS::DelayMs(ms)
enum {LOW = 0, HIGH = 1, INPUT = BIOS::GPIO::EMode::Input, OUTPUT = BIOS::GPIO::EMode::Output};


static void delayMicroseconds(ui32 us)
{
	us = us*12;
	while (us--)
	{
		__asm__("");
	}
}

void pinMode(int p, int m) {BIOS::GPIO::PinMode((BIOS::GPIO::EPin)p, (BIOS::GPIO::EMode)m);}
void digitalWrite(int p, int v) {BIOS::GPIO::DigitalWrite((BIOS::GPIO::EPin)p, v);}
int digitalRead(int p) {return BIOS::GPIO::DigitalRead((BIOS::GPIO::EPin)p);}
