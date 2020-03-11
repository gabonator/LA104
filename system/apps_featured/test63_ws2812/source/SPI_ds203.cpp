#if defined(DS203) || defined(DS213)

#include <library.h>
#include "SPI.h"

CSPI SPI;

void CSPI::begin()
{
  BIOS::DAC::SetMode(BIOS::DAC::EMode::LogicLow, nullptr, 0);
}

void CSPI::end()
{
}

void CSPI::setClockDivider(int clockDivider)
{
}

void CSPI::dmaSendAsync(uint8_t* stream, int len)
{
  // MSB first (SPI) -> LSB first (UART)
  for (int i=0; i<len; i++)
  {
    int v = stream[i];
    int n = 0;
    if (v&1) n |= 128;
    if (v&2) n |= 64;
    if (v&4) n |= 32;
    if (v&8) n |= 16;
    if (v&16) n |= 8;
    if (v&32) n |= 4;
    if (v&64) n |= 2;
    if (v&128) n |= 1;
    stream[i] = n;
  }

  // determined empirically
  #define DELAY()  __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); \
     __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); \
     __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop");
  #define DELAYS() __asm__("nop"); __asm__("nop"); __asm__("nop");

  BIOS::OS::DisableInterrupts();
#ifdef DS203
  uint32_t destptr[2] = {0x40010C14, 0x40010C10};
  //constexpr int pin = 6;
  #define pin 6
#endif
#ifdef DS213
  uint32_t destptr[2] = {0x40010814, 0x40010810};
//  constexpr int pin = 2;
  #define pin 2
#endif
  while (len--)
  {
    int v = *stream++;
    *((volatile uint32_t*)destptr[v&1]) = 1<<pin;
    v>>=1;
    DELAY();
    *((volatile uint32_t*)destptr[v&1]) = 1<<pin;
    v>>=1;
    DELAY();
    *((volatile uint32_t*)destptr[v&1]) = 1<<pin;
    v>>=1;
    DELAY();
    *((volatile uint32_t*)destptr[v&1]) = 1<<pin;
    v>>=1;
    DELAY();
    *((volatile uint32_t*)destptr[v&1]) = 1<<pin;
    v>>=1;
    DELAY();
    *((volatile uint32_t*)destptr[v&1]) = 1<<pin;
    v>>=1;
    DELAY();
    *((volatile uint32_t*)destptr[v&1]) = 1<<pin;
    v>>=1;
    DELAY();
    *((volatile uint32_t*)destptr[v&1]) = 1<<pin;
    DELAYS();
  }
  BIOS::OS::EnableInterrupts(0);
}

#endif