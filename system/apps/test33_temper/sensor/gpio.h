#include <library.h>
#define noInterrupts()
#define interrupts()
#define PIN BIOS::GPIO::P1
#define DIRECT_READ(base, pin)          BIOS::GPIO::DigitalRead(PIN)
#define DIRECT_WRITE_LOW(base, pin)     BIOS::GPIO::DigitalWrite(PIN, 0)
#define DIRECT_WRITE_HIGH(base, pin)    BIOS::GPIO::DigitalWrite(PIN, 1)
#define DIRECT_MODE_INPUT(base, pin)    {BIOS::GPIO::DigitalWrite(PIN, 1); BIOS::GPIO::PinMode(PIN, BIOS::GPIO::EMode(BIOS::GPIO::Input | BIOS::GPIO::PullUp));}
#define DIRECT_MODE_OUTPUT(base, pin)   BIOS::GPIO::PinMode(PIN, BIOS::GPIO::Output)

void delayMicroseconds(ui32 us)
{
    us = us*12;
    while (us--)
    {
        __asm__("");
    }
}
