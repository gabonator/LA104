#pragma once

#include <stdint.h>
typedef uint32_t u32;
typedef uint16_t u16;
typedef const uint32_t uc32;
typedef const uint16_t uc16;
typedef const uint8_t uc8;
typedef uint8_t u8;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;
typedef volatile uint32_t vu32;
typedef volatile uint16_t vu16;
typedef volatile uint8_t vu8;

enum {DISABLE = 0, ENABLE = 1};

#define COL_SIZE 400
#define ROW_SIZE 240

#define __IO

typedef struct
{
  volatile __IO uint16_t CR1;
  volatile uint16_t  RESERVED0;
  volatile __IO uint16_t CR2;
  volatile uint16_t  RESERVED1;
  volatile __IO uint16_t SR;
  volatile uint16_t  RESERVED2;
  volatile __IO uint16_t DR;
  volatile uint16_t  RESERVED3;
  volatile __IO uint16_t CRCPR;
  volatile uint16_t  RESERVED4;
  volatile __IO uint16_t RXCRCR;
  volatile uint16_t  RESERVED5;
  volatile __IO uint16_t TXCRCR;
  volatile uint16_t  RESERVED6;
  volatile __IO uint16_t I2SCFGR;
  volatile uint16_t  RESERVED7;
  volatile __IO uint16_t I2SPR;
  volatile uint16_t  RESERVED8;  
} SPI_TypeDef;

#define SPI_I2S_FLAG_RXNE               ((uint16_t)0x0001)
#define SPI_I2S_FLAG_TXE                ((uint16_t)0x0002)
#define I2S_FLAG_CHSIDE                 ((uint16_t)0x0004)
#define I2S_FLAG_UDR                    ((uint16_t)0x0008)
#define SPI_FLAG_CRCERR                 ((uint16_t)0x0010)
#define SPI_FLAG_MODF                   ((uint16_t)0x0020)
#define SPI_I2S_FLAG_OVR                ((uint16_t)0x0040)
#define SPI_I2S_FLAG_BSY                ((uint16_t)0x0080)

