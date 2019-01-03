#include <library.h>
#include "../../os_host/source/framework/BufferedIo.h"

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

uint_fast8_t hex(uint_fast8_t c)
{
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if (c >= 'A' && c <= 'F')
    return c - 'a' + 10;  

  BIOS::DBG::Print("Invalid hex character: '%c'\n", c);
  _ASSERT(0);
  return 0;
}

int _atoi(char* a)
{
  int aux = 0;
  while (*a >= '0' && *a <= '9')
  {
    aux *= 10;
    aux += *a++ - '0';
  }
  return aux;
}

__attribute__((__section__(".entry"))) int main(void);

int main(void)
{ 
  BIOS::LCD::Clear(RGB565(101010));
  BIOS::DBG::Print("Built " __DATE__ " " __TIME__ "\n");

  CBufferedReader reader;
//  if (!reader.Open((char*)"ALLEY.TXT"))
//  if (!reader.Open((char*)"37gabo.txt"))
//  if (!reader.Open((char*)"test.txt"))
//  if (!reader.Open((char*)"gabosong.txt"))
  if (!reader.Open((char*)"alley.txt")) // TODO: opening local file
  {
    BIOS::DBG::Print("Unable to open source file, exiting\n"); 
    BIOS::SYS::DelayMs(1500);
    return 1;
  }

  BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
  BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);

  long current = -1;

  BIOS::KEY::EKey key;
  int timeMul = 0x0050;
  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape) 
  {
    if (key == BIOS::KEY::Left && timeMul > 0x0003)
      timeMul -= 0x0002;
    if (key == BIOS::KEY::Right && timeMul < 0x0200)
      timeMul += 0x0002;

    char line[64];
    reader >> line;

    if (strlen(line) == 0)
      break;

    if (memcmp(line, "baud ", 5) == 0)
    {
      int baudrate = _atoi(line+5);
      BIOS::GPIO::UART::Setup(baudrate, BIOS::GPIO::UART::length8);
      continue;
    }

    if (memcmp(line, "wait ", 5) == 0)
    {
      long wait = _atoi(line+5);

      if (current == -1)
        current = BIOS::SYS::GetTick();
      if (wait > 2000)
        wait = 2000;
      current += (wait*timeMul) >> 4; // alley*4

      while ((long)BIOS::SYS::GetTick() - current < 0);
      continue;
    }

    int len = strlen(line);
    for (int i=0; i<len-2; i+=3)
    {
      uint_fast8_t high = line[i];
      uint_fast8_t low = line[i+1];
      uint_fast8_t data = (hex(high)<<4) | hex(low);

//      BIOS::DBG::Print("%2x,", data);
      BIOS::GPIO::UART::Write(data);
      _ASSERT(line[i+2] == ' ');
    }
//    BIOS::DBG::Print("\n");
  }

  BIOS::DBG::Print("Finished playing\n"); 
  BIOS::SYS::DelayMs(1500);
  return 0;
}
