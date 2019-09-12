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

			arr = (int)((CPUCLOCK) / ( psc * freqHz )) - 1;
                        ccr = arr * duty / 100;

			__Set(DIGTAL_PSC, psc);
			__Set(DIGTAL_ARR, arr);
			__Set(DIGTAL_CCR, ccr);
			return;
		}

		psc = 20;
		arr = (int)((CPUCLOCK) / (psc * freqHz * samples)) - 1;
		__Set(ANALOG_ARR, arr);
    }

    void SetDuty(int dutyPercent) 
    {
      ccr = arr * dutyPercent / 100;
      __Set(DIGTAL_CCR, ccr);
    }

    int GetFrequency() 
    {
      if (!wave) // square
        return CPUCLOCK / (arr + 1) / (psc + 1);

      return CPUCLOCK / 20 / ( arr + 1 ) / samples; 
    }

    int GetDuty() 
    {
      return ccr * 100 / arr;
    }

    void SetWave(uint16_t* _wave, int length)
    {
      if (wave)
        ConfigureWave(_wave, length);
      samples = length;
      wave = _wave;
    }
  }
}