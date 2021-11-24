#ifdef _ARM
#include <library.h>

class CFpgaTestBench
{
    uint16_t mLastSpiCr;
    int32_t mStarted;

public:
    CFpgaTestBench()
    {
        mStarted = BIOS::SYS::GetTick();
    }

    bool Timeout()
    {
         int32_t now = BIOS::SYS::GetTick();
         return now-mStarted > 5000;
    }

    void SpiBegin()
    {
        #define SPI_BaudRatePrescaler_16        ((uint16_t)0x0018)
        #define SPI_BaudRatePrescaler_32        ((uint16_t)0x0020)
        #define SPI_BaudRatePrescaler_64        ((uint16_t)0x0028)
        #define SPI_BaudRatePrescaler_128       ((uint16_t)0x0030)
        #define SPI_BaudRatePrescaler_256       ((uint16_t)0x0038)
        #define SPI_FirstBit_MSB                ((uint16_t)0x0000)
        #define SPI_FirstBit_LSB                ((uint16_t)0x0080)

        volatile uint16_t* ptrSpiCR1 = (uint16_t*)0x40003C00;
  	int clockDivider = SPI_BaudRatePrescaler_16;
  	const uint32_t SPI_CR1_BR = 0x38;
  	mLastSpiCr = *ptrSpiCR1;
  	uint16_t cr1 = *ptrSpiCR1 & ~(SPI_CR1_BR);
  	*ptrSpiCR1 = cr1 | (clockDivider & SPI_CR1_BR); 
    }

    void SpiEnd()
    {
        volatile uint16_t* ptrSpiCR1 = (uint16_t*)0x40003C00;
  	*ptrSpiCR1 = mLastSpiCr;
    }

    uint8_t SpiTransfer(uint8_t data)
    {
        volatile uint16_t* ptrSpiDr = (uint16_t*)0x40003C0C; // SPI3_DR_ADDRESS
        volatile uint16_t* ptrSpiSr = (uint16_t*)0x40003C08; // SPI3 Status reg
        #define SPI_I2S_FLAG_RXNE               ((uint16_t)0x0001)
        #define SPI_I2S_FLAG_TXE                ((uint16_t)0x0002)
        #define SPI_I2S_FLAG_BSY                ((uint16_t)0x0080)

        // Write data to be transmitted to the SPI data register
        *ptrSpiDr = data;
        // Wait until transmit complete
        while (!(*ptrSpiSr & (SPI_I2S_FLAG_TXE)));
        // Wait until receive complete
        while (!(*ptrSpiSr & (SPI_I2S_FLAG_RXNE)));
        // Wait until SPI is not busy anymore
        while (*ptrSpiSr & (SPI_I2S_FLAG_BSY));
        // Return received data from SPI data register
        return *ptrSpiDr;
    }

    void SpiCs(bool level) // SPI3 CS
    {
        #define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */
        #define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
        #define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)
        enum {
            StateInput = 0,
            StateOutput10Mhz = 1,     // ..01
            StateOutput2Mhz = 2,      // ..10
            StateOutput50Mhz = 3,     // ..11

            StateInputAnalog = 0<<2,
            StateInputFloating = 1<<2,
            StateInputPull = 2<<2,

            StateOutputPushPull = 0<<2,
            StateOutputOpenDrain = 1<<2,
            StateOutputFunctionPushPull = 2<<2,
            StateOutputFunctionOpenDrain = 3<<2,
            
            // macros for easy use
            StateSimpleInput = StateInput | StateInputFloating,
            StateSimpleOutput = StateOutput2Mhz | StateOutputPushPull
        };

        SetState(GPIOB_BASE, 7, StateSimpleOutput);
        SetPin(GPIOB_BASE, 7, !level);
    }

    void SetState(uint32_t nPortBase, int nPin, int nState)
    {
        uint32_t dwAddr;

        if ( nPin < 8 )
          dwAddr = nPortBase + 0;	// CRL
        else
        {
          dwAddr = nPortBase + 4;	// CRH
          nPin &= 7;
        }

        uint32_t dwMask = ~(0xf << (nPin*4));
        uint32_t dwBits = nState << (nPin*4);

        uint32_t* pCR = ((uint32_t*)dwAddr);
        *pCR = (*pCR & dwMask) | dwBits;
    }

    void SetPin(uint32_t nPortBase, int nPin, bool bValue)
    {
        uint32_t dwAddr = nPortBase;
        if ( bValue )
        {
          uint32_t* pBSRR = &((uint32_t*)dwAddr)[4];
          *pBSRR = 1<<nPin;
        } else
        {
          uint32_t* pBRR = &((uint32_t*)dwAddr)[5];
          *pBRR = 1<<nPin;
        }
    }

    void SpiTransfer(const CArray<uint8_t>& arrIn, CArray<uint8_t>& arrOut)
    {
        SpiBegin();

        arrOut.SetSize(arrIn.GetSize());
        SpiCs(true);
        for (int i=0; i<arrIn.GetSize(); i++)
          arrOut[i] = SpiTransfer(arrIn[i]);
        SpiCs(false);

        SpiEnd();
    }

    bool GetPin(uint32_t nPortBase, int nPin)
    {
        uint32_t dwAddr = nPortBase;
        uint32_t dwIDR = ((uint32_t*)dwAddr)[2];
        return (dwIDR & (1<<nPin)) ? 1 : 0;
    }                   

    bool GetMiso() // SPI3 MISO - PB4
    {
        #define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */
        #define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
        #define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)

        return GetPin(GPIOB_BASE, 4);
    }

    void DelayMs(int ms)
    {
        BIOS::SYS::DelayMs(ms);
    }
};

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed!");
    while (1);
}

__attribute__((__section__(".entry"))) bool verify();
#endif

#ifdef _VERILATOR
#include <unistd.h>
#include <time.h>
#include "../../../../../system/os_host/source/framework/Classes.h"

class CFpgaTestBench
{
public:
    enum {
      TimeoutLimit = 5000,
      TicksPerMs = 8000*4 // 8MHz?
    };

    static Vapp* mTop;
    int mStarted;
    static int mTicks;

public:
    CFpgaTestBench()
    {
        mStarted = SysTick();
        mTicks = 0;
    }

    uint8_t& CLK()
    {
        return mTop->clk;
    }
    uint8_t& SCK()
    {
        return mTop->SCK;
    }
    uint8_t& SS()
    {
        return mTop->SSEL;
    }
    uint8_t& MOSI()
    {
        return mTop->MOSI;
    }
    uint8_t& MISO()
    {
        return mTop->MISO;
    }
    void eval()
    {
        mTop->eval();
    }

    int SysTick()
    {
        struct timespec ts;
        unsigned theTick = 0U;
        clock_gettime( CLOCK_REALTIME, &ts );
        theTick  = ts.tv_nsec / 1000000;
        theTick += ts.tv_sec * 1000;
        return theTick;
    }

    bool Timeout()
    {
         int32_t now = SysTick();
         return now-mStarted > TimeoutLimit;
    }

#ifndef _NOSPI
    void SpiCs(bool cs)
    {
        mTop->clk = !mTop->clk;
        mTop->eval();
        mTop->clk = !mTop->clk;
        mTop->eval();

        mTop->SSEL = !cs;

        mTop->clk = !mTop->clk;
        mTop->eval();
        mTop->clk = !mTop->clk;
        mTop->eval();

        mTicks+=4;
    }

    uint8_t SpiTick(bool b)
    {
        mTop->MOSI = b;

        mTop->clk = !mTop->clk;
        mTop->eval();
        mTop->clk = !mTop->clk;
        mTop->eval();
        mTop->clk = !mTop->clk;
        mTop->eval();
        mTop->clk = !mTop->clk;
        mTop->eval();

        mTop->SCK = 1;

        mTop->clk = !mTop->clk;
        mTop->eval();
        mTop->clk = !mTop->clk;
        mTop->eval();
        mTop->clk = !mTop->clk;
        mTop->eval();
        mTop->clk = !mTop->clk;
        mTop->eval();

        mTop->SCK = 0;

        mTop->clk = !mTop->clk;
        mTop->eval();
        mTop->clk = !mTop->clk;
        mTop->eval();
        mTop->clk = !mTop->clk;
        mTop->eval();
        mTop->clk = !mTop->clk;
        mTop->eval();

        printf("%d", mTop->MISO);
        mTicks+=6;
        return mTop->MISO;
    }

    uint8_t SpiTransfer(uint8_t byte)
    {
        uint8_t aux = 0;
        for (int i=0; i<8; i++)
        {
            aux <<= 1;
            aux |= SpiTick(byte & 128);
            byte <<= 1;
        }
        return aux;
    }

    void SpiTransfer(const CArray<uint8_t>& arrIn, CArray<uint8_t>& arrOut)
    {
        arrOut.SetSize(arrIn.GetSize());
        SpiCs(true);
        for (int i=0; i<arrIn.GetSize(); i++)
          arrOut[i] = SpiTransfer(arrIn[i]);
        SpiCs(false);
    }

    void SpiBegin()
    {
    }

    void SpiEnd()
    {
    }
#endif

    bool GetMiso() // SPI3 MISO - PB4
    {
        return mTop->MISO;
    }
};

int CFpgaTestBench::mTicks{0};

void _HandleAssertion(const char* file, int line, const char* cond)
{
    printf("Assertion failed in %s [%d]: %s\n", file, line, cond);
}

namespace BIOS {
  namespace DBG {
    int Print(const char *format, ...)
    {
      va_list args;
      va_start(args, format);
      vprintf(format, args);
      va_end(args);
      return 0;
    }
  }
  namespace SYS {
    int32_t GetTick()
    {
      return CFpgaTestBench::mTicks/CFpgaTestBench::TicksPerMs;
    }

    void DelayMs(int ms)
    {
        int ticks = CFpgaTestBench::TicksPerMs*ms;
        CFpgaTestBench::mTicks += ticks;
        while (ticks--)
        {
          CFpgaTestBench::mTop->clk = !CFpgaTestBench::mTop->clk;
          CFpgaTestBench::mTop->eval();
        }
    }
  }
}

#endif
