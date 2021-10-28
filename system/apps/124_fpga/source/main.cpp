#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include "../../../os_host/source/framework/BufferedIo.h"

#include "file/shapes.h"
#include "file/layout.h"
#include "file/file.h"

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

bool DoFlashing(char* name)
{
    CBufferedReader fw;
    if ( !fw.Open(name) )
        return false;

    int count = 0xb720;
    volatile uint16_t* ptrSpi = (uint16_t*)0x40003C0C; // SPI3_DR_ADDRESS
//    volatile uint32_t* ptrSpiStatus = 0x40003C08; // SPI3 Status reg
    volatile uint32_t* bit_SPI3_txe = (volatile uint32_t *)0x42078104;
    volatile uint32_t* cdone = (volatile uint32_t*)0x4222010c;
    CONSOLE::Print("cdone = %d ", *cdone);
    int counter; // volatile?

    // reset
    *((volatile uint32_t*)0x4221819c) = 0;
    *((volatile uint32_t*)0x42218198) = 0;
    counter = 7;
    while (counter--);

    *((volatile uint32_t*)0x42218198) = 1;
    counter = 0x1baf;
    while (counter--);
    CONSOLE::Print("cdone = %d ", *cdone);

    CStream spiStream((uint8_t*)ptrSpi, 1);
    while (count--)
    {
      fw >> spiStream;
      while (bit_SPI3_txe == 0);
    }

    *((volatile uint32_t*)0x4221819c) = 1;

    fw.Close();
    BIOS::SYS::DelayMs(10);
    CONSOLE::Print("cdone = %d", *cdone);

    CONSOLE::Print("\n");
    return !!*cdone;
}

bool GetPin(uint32_t nPortBase, int nPin)
{
    uint32_t dwAddr = nPortBase;
    uint32_t dwIDR = ((uint32_t*)dwAddr)[2];
    return (dwIDR & (1<<nPin)) ? 1 : 0;
}                   

bool GetPB4() // SPI3 MISO
{
    #define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */
    #define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
    #define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)

    return GetPin(GPIOB_BASE, 4);
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

void SetPB7(bool level) // SPI3 CS
{
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
  SetPin(GPIOB_BASE, 7, level);
}


uint8_t SPI3Transfer(uint8_t data)
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

CFileDialog mFile;
CFileFilterSuffix filter(".FPG");

__attribute__((__section__(".entry")))
int _main(void)
{    
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);
    //mReader.Open((char*)"avr/tinystar.hex");

    APP::Init("FPGA flasher");
    APP::Status("AG1KLPQ48");

    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
        if (mFile.ModalShow(nullptr, "Load firmware", &filter))
        {
            APP::Init("FPGA flasher");
            APP::Status("AG1KLPQ48");
            
            CONSOLE::Print("Flashing '%s'...\n", mFile.GetFilename());

            CONSOLE::Color(RGB565(b0b0b0));
            if (DoFlashing(mFile.GetFilename()))
            {
                CONSOLE::Color(RGB565(ffffff));
                CONSOLE::Print("Flashed successfully!\n");
            } else
            {
                CONSOLE::Color(RGB565(ffffff));
                CONSOLE::Print("Flashing failed!\n");
            }
 
#define SPI_BaudRatePrescaler_16        ((uint16_t)0x0018)
#define SPI_BaudRatePrescaler_32        ((uint16_t)0x0020)
#define SPI_BaudRatePrescaler_64        ((uint16_t)0x0028)
#define SPI_BaudRatePrescaler_128       ((uint16_t)0x0030)
#define SPI_BaudRatePrescaler_256       ((uint16_t)0x0038)
#define SPI_FirstBit_MSB                ((uint16_t)0x0000)
#define SPI_FirstBit_LSB                ((uint16_t)0x0080)

        volatile uint16_t* ptrSpiCR1 = (uint16_t*)0x40003C00; // SPI3 Status reg
  	int clockDivider = SPI_BaudRatePrescaler_16;
  	const uint32_t SPI_CR1_BR = 0x38;
  	uint16_t last = *ptrSpiCR1;
  	uint16_t cr1 = *ptrSpiCR1 & ~(SPI_CR1_BR);
  	*ptrSpiCR1 = cr1 | (clockDivider & SPI_CR1_BR);
  
            // Wait for key press
            while ((key = BIOS::KEY::GetKey()) == BIOS::KEY::None)
            {
                // display PB4_MISO
                bool b = GetPB4();
                BIOS::LCD::Bar(CRect(BIOS::LCD::Width-14, BIOS::LCD::Height-14-14,
                    BIOS::LCD::Width, BIOS::LCD::Height-14), 
                    b ? RGB565(ff0000) : RGB565(00ff00));

                // test spi comm every second
                EVERY(1000) 
                {
                    uint8_t s = 0x30;
                    SetPB7(0);
                    int ret1 = SPI3Transfer(s);
                    int ret2 = SPI3Transfer(s);
                    int ret3 = SPI3Transfer(s);
                    int ret4 = SPI3Transfer(s);
                    SetPB7(1);
                    int ret5 = SPI3Transfer(s);
                    int ret6 = SPI3Transfer(s);

                    BIOS::LCD::Printf(0, BIOS::LCD::Height-14-14, RGB565(000000), RGB565(ffffff), 
                        " spi: %02x %02x %02x %02x (%02x %02x)", ret1, ret2, ret3, ret4, ret5, ret6);
                }
            }

  	*ptrSpiCR1 = last;

        } else
        {
            break;
        }
    }

    BIOS::FAT::SetSharedBuffer(nullptr);
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}
