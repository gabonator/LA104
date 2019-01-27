#include <math.h>
#include <library.h>
#include "analyser.h"

using namespace BIOS;

__attribute__((__section__(".entry"))) int main(void)
{ 
  LCD::Clear(RGB565(000000));

  CAnalyser anal;

  KEY::EKey key;
  while ((key = KEY::GetKey()) != KEY::EKey::Escape)
  {
    if (key == KEY::EKey::F1)
      anal.StartSampling();

    anal.StartSampling();
    int port = anal.Read() >> 28;
    int ch1 = GPIO::DigitalRead(GPIO::EPin::CH1);
    int ch2 = GPIO::DigitalRead(GPIO::EPin::CH2);
    int ch3 = GPIO::DigitalRead(GPIO::EPin::CH3);
    int ch4 = GPIO::DigitalRead(GPIO::EPin::CH4);
    LCD::Printf(0, 0, RGB565(b0b0b0), RGB565(000000), "%d %d%d%d%d", port, ch1, ch2, ch3, ch4);
//    uint32_t finished = anal.Finished();
//    uint32_t state = anal.Read();
//    LCD::Printf(0, 0, RGB565(b0b0b0), RGB565(000000), "%08x - %08x %x", finished, state, state >> 24);
  }

  return 0;
}
