#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "../../os_host/source/framework/BufferedIo.h"
#include "gpio.h"

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];
CBufferedWriter mWriter;
CBufferedReader mReader;

volatile bool flag1 = false;
uint8_t chunk[1024];

void PrintStats(int ms, int size)
{
  float MBps = (size/1024.0f/1024.0f)/(ms/1000.0f);
  if (size >= 1024*1024)
    CONSOLE::Print("%d MB in %d.%d s, %f MBps", size/1024/1024, ms/1000, ms%1000, MBps);
  else
    CONSOLE::Print("%d KB in %d.%d s, %f MBps", size/1024, ms/1000, ms%1000, MBps);
  CONSOLE::Print("\n");
}

#define __IO volatile
typedef struct
{
  __IO uint16_t CR1;
  uint16_t  RESERVED0;
  __IO uint16_t CR2;
  uint16_t  RESERVED1;
  __IO uint16_t SR;
  uint16_t  RESERVED2;
  __IO uint16_t DR;
  uint16_t  RESERVED3;
  __IO uint16_t CRCPR;
  uint16_t  RESERVED4;
  __IO uint16_t RXCRCR;
  uint16_t  RESERVED5;
  __IO uint16_t TXCRCR;
  uint16_t  RESERVED6;
  __IO uint16_t I2SCFGR;
  uint16_t  RESERVED7;
  __IO uint16_t I2SPR;
  uint16_t  RESERVED8;  
} SPI_TypeDef;

#define PERIPH_BASE           ((uint32_t)0x40000000)
#define PERIPH_BASE_APB2		(PERIPH_BASE + 0x10000)
#define SPI1_BASE			(PERIPH_BASE_APB2 + 0x3000)
#define SPI1                ((SPI_TypeDef *) SPI1_BASE)
#define SPI_I2S_FLAG_RXNE               ((uint16_t)0x0001)
#define SPI_I2S_FLAG_TXE                ((uint16_t)0x0002)

uint8_t spiTransfer(uint8_t Byte)
{
    while (!(SPI1->SR & SPI_I2S_FLAG_TXE));
    SPI1->DR = Byte;
    while (!(SPI1->SR & SPI_I2S_FLAG_RXNE));
    return SPI1->DR;
}

void spiconf()
{
  CONSOLE::Print("JEDEC request\n");
  uint8_t jedec[3];
  uint8_t manufacturerId;
  uint8_t deviceId;
  uint8_t uid[8];

  Gpio::SetLevel(Gpio::BASEB, 1, 0);
  spiTransfer(0x9f);
  jedec[0] = spiTransfer(0xff);
  jedec[1] = spiTransfer(0xff);
  jedec[2] = spiTransfer(0xff);
  Gpio::SetLevel(Gpio::BASEB, 1, 1);

  BIOS::SYS::DelayMs(2);

  Gpio::SetLevel(Gpio::BASEB, 1, 0);
  spiTransfer(0x90);
  spiTransfer(0xff);
  spiTransfer(0xff);
  spiTransfer(0xff);
  manufacturerId = spiTransfer(0xff);
  deviceId = spiTransfer(0xff);
  Gpio::SetLevel(Gpio::BASEB, 1, 1);

  BIOS::SYS::DelayMs(2);

  Gpio::SetLevel(Gpio::BASEB, 1, 0);
  spiTransfer(0x4b);
  spiTransfer(0xff);
  spiTransfer(0xff);
  spiTransfer(0xff);
  spiTransfer(0xff);
  for (int i=0; i<8; i++)
    uid[i] = spiTransfer(0xff);
  Gpio::SetLevel(Gpio::BASEB, 1, 1);

  CONSOLE::Print("JEDEC: %02x %02x %02x ", jedec[0], jedec[1], jedec[2]);
  if (jedec[0] == 0xef && jedec[1] == 0x40 && jedec[2] == 0x17)
    CONSOLE::Print("Winbond W25Q64BV");
  CONSOLE::Print("\n");
  CONSOLE::Print("Manufacturer ID: %02x\n", manufacturerId);
  CONSOLE::Print("Device ID: %02x\n", deviceId);
  CONSOLE::Print("UID: %02x %02x %02x %02x %02x %02x %02x %02x\n", 
    uid[0], uid[1], uid[2], uid[3], uid[4], uid[5], uid[6], uid[7]);

/*
SPI_BaudRatePrescaler_4:
16.143, 14.831, 14.383, 4.345, 6.271, 2.378
SPI_BaudRatePrescaler_2:
15.711, 14.456, 13.961, 3.877, 5.341, 2.271
*/
/*
#define SPI_BaudRatePrescaler_2         ((uint16_t)0x0000)
#define SPI_BaudRatePrescaler_4         ((uint16_t)0x0008)
#define SPI_BaudRatePrescaler_8         ((uint16_t)0x0010)
#define SPI_BaudRatePrescaler_16        ((uint16_t)0x0018)
#define SPI_BaudRatePrescaler_32        ((uint16_t)0x0020)
#define SPI_BaudRatePrescaler_64        ((uint16_t)0x0028)
#define SPI_BaudRatePrescaler_128       ((uint16_t)0x0030)
#define SPI_BaudRatePrescaler_256       ((uint16_t)0x0038)
uint32_t baudmask = SPI_BaudRatePrescaler_2 | SPI_BaudRatePrescaler_4 | SPI_BaudRatePrescaler_8 | SPI_BaudRatePrescaler_16 | SPI_BaudRatePrescaler_32 | SPI_BaudRatePrescaler_64 | SPI_BaudRatePrescaler_128 | SPI_BaudRatePrescaler_256;
uint32_t tmpreg = SPI1->CR1;
CONSOLE::Print("CR1=%04x, baud=%04x\n", tmpreg, tmpreg & baudmask);
tmpreg &= ~baudmask;
tmpreg |= SPI_BaudRatePrescaler_2;
SPI1->CR1 = tmpreg;
*/  
#if 0
//        uint32_t tmpreg = SPI2->CR1;
//        #define CR1_CLEAR_Mask       ((uint16_t)0x3040)

//        tmpreg &= CR1_CLEAR_Mask;
  uint32_t tmpreg = 0;
  /* Configure SPIx: direction, NSS management, first transmitted bit, BaudRate prescaler
     master/salve mode, CPOL and CPHA */
  /* Set BIDImode, BIDIOE and RxONLY bits according to SPI_Direction value */
  /* Set SSM, SSI and MSTR bits according to SPI_Mode and SPI_NSS values */
  /* Set LSBFirst bit according to SPI_FirstBit value */
  /* Set BR bits according to SPI_BaudRatePrescaler value */
  /* Set CPOL bit according to SPI_CPOL value */
  /* Set CPHA bit according to SPI_CPHA value */
  uint32_t SPI_Direction = 0; //SPI_Direction_2Lines_FullDuplex
  uint32_t SPI_Mode = 0x0104; //SPI_Mode_Master;
  uint32_t SPI_DataSize = 0;
  uint32_t SPI_CPOL = 0; // low
  uint32_t SPI_CPHA = 0; // SPI_CPHA_1Edge
  uint32_t SPI_NSS = 0x200; // SPI_NSS_Hard, SPI_NSS_Soft
  uint32_t SPI_BaudRatePrescaler = 0; //0x0030; // SPI_BaudRatePrescaler_128
  uint32_t SPI_FirstBit = 0x00; // SPI_FirstBit_MSB;
        tmpreg |= (uint16_t)((uint32_t)SPI_Direction | SPI_Mode |
                  SPI_DataSize | SPI_CPOL |  
                  SPI_CPHA | SPI_NSS |  
                  SPI_BaudRatePrescaler | SPI_FirstBit);
  /* Write to SPIx CR1 */
  SPI1->CR1 = tmpreg;
#endif
}

__attribute__((__section__(".entry")))
int main(void)
{
    APP::Init("Disk speed test");
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);

    spiconf();

    for (int i=0; i<(int)sizeof(chunk); i++)
      chunk[i] = i/2;

    //
    {
      CONSOLE::Print("Writing bytes...\n");
      mWriter.Open((char*)"speedtst.bin");
      int32_t t0 = BIOS::SYS::GetTick();
      for (int i=0; i<1024*1024; i++)
        mWriter << (uint8_t)i;
      int32_t t1 = BIOS::SYS::GetTick();
      mWriter.Close();
      PrintStats(t1-t0, 1024*1024);
    }
    //
    {
      CONSOLE::Print("Writing 1k chunks...\n");
      CStream streamChunk(chunk, sizeof(chunk));
      mWriter.Open((char*)"speedtst.bin");
      int32_t t0 = BIOS::SYS::GetTick();
      for (int i=0; i<1024; i++)
        mWriter << streamChunk;
      int32_t t1 = BIOS::SYS::GetTick();
      mWriter.Close();
      PrintStats(t1-t0, 1024*1024);
    }
    //
    {
      for (int i=0; i<(int)sizeof(gFatSharedBuffer); i++)
        gFatSharedBuffer[i] = i/3;
      int sectors = 1024*1024/BIOS::FAT::SectorSize;
      CONSOLE::Print("Writing sectors...\n");
      BIOS::FAT::Open((char*)"speedtst.bin", BIOS::FAT::IoWrite);
      int32_t t0 = BIOS::SYS::GetTick();
      for (int i=0; i<sectors; i++)
        BIOS::FAT::Write(gFatSharedBuffer);
      BIOS::FAT::Close(1024*1024);
      int32_t t1 = BIOS::SYS::GetTick();
      PrintStats(t1-t0, 1024*1024);
    }
    //
    {
      uint8_t temp;
      CONSOLE::Print("Reading bytes...\n");
      mReader.Open((char*)"speedtst.bin");
      int32_t t0 = BIOS::SYS::GetTick();
      for (int i=0; i<1024*1024; i++)
        mReader >> temp;
      int32_t t1 = BIOS::SYS::GetTick();
      mReader.Close();
      PrintStats(t1-t0, 1024*1024);
    }
    //
    {
      CStream streamChunk(chunk, sizeof(chunk));
      CONSOLE::Print("Reading 1k chunks...\n");
      mReader.Open((char*)"speedtst.bin");
      int32_t t0 = BIOS::SYS::GetTick();
      for (int i=0; i<1024; i++)
        mReader >> streamChunk;
      int32_t t1 = BIOS::SYS::GetTick();
      mReader.Close();
      PrintStats(t1-t0, 1024*1024);
    }
    //
    {
      int sectors = 1024*1024/BIOS::FAT::SectorSize;

      CONSOLE::Print("Skipping sectors...\n");
      mReader.Open((char*)"speedtst.bin");
      int32_t t0 = BIOS::SYS::GetTick();
      for (int i=0; i<sectors; i++)
      {
        BIOS::FAT::EResult eResult = BIOS::FAT::Read(gFatSharedBuffer);
        _ASSERT( eResult == BIOS::FAT::EOk );
      }
      int32_t t1 = BIOS::SYS::GetTick();
      mReader.Close();
      PrintStats(t1-t0, 1024*1024);
    }
    BIOS::FAT::SetSharedBuffer(nullptr);

    CONSOLE::Print("Done. Press any key to exit.");
    while (!BIOS::KEY::GetKey());

    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}
