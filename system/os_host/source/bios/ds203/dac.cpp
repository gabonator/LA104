//#include "../library/CMSIS/Device/STM32F10x/Include/STM32F10x.h"
typedef volatile uint32_t vu32;
typedef uint32_t u32;
typedef uint8_t u8;

extern "C"
{
extern void __Set(u8, u32);
#define ANALOG_ARR       13     // ģ
#define ANALOG_PTR       14     // ģ
#define ANALOG_CNT       15     // ÿ
#define DIGTAL_PSC       16     // 
#define DIGTAL_ARR       17     // 
#define DIGTAL_CCR       18     // 
}

#define Hz *1.0f
#define KHz *1000.0f
#define MHz *1000000.0f

void ConfigureWave(ui16* pData, ui16 cnt)
{

	#define DMA2_CMAR4  (*((vu32 *)(0x40020400+0x50)))
	#define DMA2_CNDTR4 (*((vu32 *)(0x40020400+0x48)))
	#define DMA2_CCR4   (*((vu32 *)(0x40020400+0x44)))
	#define DMA2_CPAR4  (*((vu32 *)(0x40020400+0x4C)))
    #define DAC_DHR12R1_ADDR ((vu32)(0x40007400+0x08))

    #define CCR_ENABLE_Reset        ((u32)0xFFFFFFFE)
    #define CCR_ENABLE_Set          ((u32)0x00000001)

  DMA2_CCR4 &= CCR_ENABLE_Reset;
//  DMA2_CCR4 |= CCR_ENABLE_Set;

  DMA2_CPAR4 = DAC_DHR12R1_ADDR;   
  DMA2_CCR4  = 0x000035B0;
  DMA2_CNDTR4= cnt;
  DMA2_CMAR4 = (u32)pData;

  DMA2_CCR4 |= CCR_ENABLE_Set;
}

#define CPUCLOCK (72 MHz)
#define MHz *1000000.0f


void SetLogic()
{
  #define Mask32(adr, mask, val) *((volatile uint32_t*)adr) &= ~mask; *((volatile uint32_t*)adr) |= val;
  #define Write32(adr, val) *((volatile uint32_t*)adr) = val;

  // Dac: SetGpioState<GPIOA_BASE, 4, InputFloating>();
  Mask32(0x40010800, 0xf0000, 0x40000);

  // Dac: TIM7_CR1 = 0x0084;
  Write32(0x40001400, 0x84);

  // Dac: DAC_CR = 0;
  Write32(0x40007400, 0x0);

  // Square: TIM4_CR1
  Write32(0x40000800, 0x80);

  // SetGpioState<GPIOB_BASE, 6, Output50MHz | OutputPushPull>();
  Mask32(0x40010C00, 0xf000000, 0x3000000);
}

void SetLogicHigh()
{
  // SetGpioLevel<GPIOB_BASE, 6, true>();
  Write32(0x40010C10, 1<<6);
}

void SetLogicLow()
{
  // SetGpioLevel<GPIOB_BASE, 6, false>();
  Write32(0x40010C14, 1<<6);
}

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

    void SetFrequency(int freqHz) 
    {
		if ( !wave )
		{
			psc = 1;
			if ( freqHz <= 10 Hz )
				psc = 240;
			else if ( freqHz <= 2 KHz )
				psc = 180;
			else if ( freqHz <= 20 KHz )
				psc = 18;
			//Settings.Gen.nPsc = nPsc - 1; ?????

			//arr = (int)((CPUCLOCK) / ( psc * freqHz )) - 1;
			arr = 72000000UL / (psc + 1) / freqHz - 1;
			if (arr<2)
                          arr = 2;
                        ccr = arr * duty / 100;

			__Set(DIGTAL_PSC, psc);
			__Set(DIGTAL_ARR, arr);
			__Set(DIGTAL_CCR, ccr);
			return;
		}

		psc = 20;
//		arr = (int)((CPUCLOCK) / (psc * freqHz)) - 1;
                arr = 72000000 / 20 / freqHz - 1;
                if (arr<1)
                  arr = 1;
		__Set(ANALOG_ARR, arr);
    }

    void SetDuty(int dutyPercent) 
    {
      ccr = arr * (100-dutyPercent) / 100;
      __Set(DIGTAL_CCR, ccr);
    }

    int GetFrequency() 
    {
//TODO: verify calculation arr+1 / arr
      if (!wave) // square
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
              ConfigureWave(buffer, length);

            samples = length;
            wave = buffer;
          }
        break;
        case EMode::LogicHigh:
          SetLogic();
          SetLogicHigh();
        break;
        case EMode::LogicLow:
          SetLogic();
          SetLogicLow();
        break;
        default:
          _ASSERT(!"Not supported");
      }
    }
  }
}