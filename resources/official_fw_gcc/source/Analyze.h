#ifndef __ANALYZE_H
#define __ANALYZE_H

#include "STM32F10x.h"

#define  ASCII_WIDTH   7   // 6
#define  DEX_WIDTH     21  // 18
#define  HEX_WIDTH     28  // 24

#define  ASCII         0
#define  DEX           1
#define  HEX           2

#define  LSB           0   // 低位传输
#define  MSB           1   // 高位传输

void SPI_AnalyzeFrame(void);
void IIC_AnalyzeFrame(void);
void UART_AnalyzeFrame(void);
void Show_UART_AnalyzeData(void);
void Show_SPI_AnalyzeData(void);
void Show_I2C_AnalyzeData(void);
void AnalyzeFrame(u8 Type);
void Show_AnalyzeData(u8 Type);

u16  SPI_AnalyzeDataStartCnt(u64 StartTime);
u16  I2C_AnalyzeStartCnt(u64 StartTime);

#endif