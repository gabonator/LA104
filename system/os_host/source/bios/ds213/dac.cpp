#include "platform.h"
#include "DS213HwDriver.h"

extern HwDrvDef  Hw;

namespace BIOS
{
  namespace DAC
  {
    void* wave = nullptr;
    int arr = 1;
    int psc = 1;
    int ccr = 1;
    int samples = 1;
    int duty = 50;

    #define Hz *1
    #define KHz *1000
    #define MHz *1000000

    void SetFrequency(int freqHz) 
    {
      if (!wave)
      {
        psc = 1;
        if ( freqHz <= 10 Hz )
          psc = 240;
        else if ( freqHz <= 2 KHz )
          psc = 180;
        else if ( freqHz <= 20 KHz )
          psc = 18;

        arr = 72000000UL / (psc + 1) / freqHz - 1;
        if (arr<2)
          arr = 2;
        ccr = arr * duty / 100;

        Hw.pDevInit(SO_DGTL);
        *Hw.pFout_TIM_PSC = psc;
        *Hw.pFout_TIM_ARR = arr;
        *Hw.pFout_TIM_CCR = ccr;
        return;
      }

      psc = 20;
      arr = 72000000 / 20 / freqHz - 1;
      if (arr<1)
        arr = 1;

      *Hw.pFout_DMA_PSC = psc;
      *Hw.pFout_DMA_ARR = arr;
    }

    void SetDuty(int dutyPercent) 
    {
      ccr = arr * (100-dutyPercent) / 100;
      *Hw.pFout_TIM_CCR = ccr;
    }

    int GetFrequency() 
    {
      if (!wave)
        return 72000000UL / (arr + 1) / (psc + 1);

      return 72000000UL / 20 / ( arr + 1 ); 
    }

    int GetDuty() 
    {
      return ccr * 100 / arr;
    }

    void SetMode(EMode mode, uint16_t* buffer, int length)
    {
      switch (mode)
      {
        case EMode::Square:
          wave = nullptr;
          samples = 0;
          break;
        case EMode::Buffer:
          if (wave != buffer || length != samples)
          {
            if (buffer)
            {
              Hw.pDevInit(SO_ANLG);
              Hw.pFout_DMA(DISABLE);
              *Hw.pFout_DMA_CNT = length;
              *Hw.pFout_DMA_CMA = (uint32_t)buffer;
              *Hw.pFout_DMA_PSC = psc;
              *Hw.pFout_DMA_ARR = arr;
              Hw.pFout_DMA(ENABLE);
            }
            samples = length;
            wave = buffer;
          }
        break;
        case EMode::LogicHigh:
        case EMode::LogicLow:
        default:
          _ASSERT(!"Not supported");
      }
    }
  }
}
