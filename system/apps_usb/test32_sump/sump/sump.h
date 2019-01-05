#ifndef SUMP_H
#define SUMP_H
#include <stdint.h>

//struct TIM_HandleTypeDef {};
/*
typedef int TIM_HandleTypeDef;
typedef int HAL_StatusTypeDef;

extern TIM_HandleTypeDef htim1;
HAL_StatusTypeDef HAL_TIM_Base_Start(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_HandleTypeDef *htim);
*/


void write_uart_s(const char* msg);
void write_uart_u(int n);
int CDC_Transmit_FS(uint8_t* buffer, int len);

enum { USBD_OK = 0 };
//void HAL_TIM_SetupPeriod(int);

void sump_cmd(char c);
void poll_sump();

#endif
