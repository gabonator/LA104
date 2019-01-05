#include <stm32f10x.h>
#include "gpio.h"
#include "delay.h"

uint8_t gpio_buffer[GPIO_BUFFER_SIZE];

#define readPort (GPIOB->IDR >> 8)

int realign_port(uint32_t nibbles)
{
  // P1: 10  2
  // P2: 11  3
  // P3: 8   0
  // P4: 9   1

  return ((nibbles & 3) << 2) | ((nibbles >> 2) & 3);
}

void realign_buffer(uint8_t* data, int len)
{
  while (len--)
  {
    *data = realign_port(*data);
    data++;
  }
}

void reverse(uint8_t* data, int len)
{
  // wtf??? all data is received in reversed order!?!?!
  int i=0;
  int j=len-1;
  for (; i<len/2; i++, j--)
  {
    uint8_t temp = data[i];
    data[i] = data[j];
    data[j] = temp;    
  }
}


static inline void wait_for_trigger(uint8_t trigger_mask, uint8_t trigger_values) {
  if(trigger_mask) {
    while((realign_port(readPort) ^ trigger_values) & trigger_mask); // wait until trigger
  }
}

void setDivider(int divider) 
{
// 243 - 200khz
// 231 - 100khz
// 159 - 1khz
  // TODO: dma transfer not implemented !
/*
  static int lastDivider = 0;
  if (lastDivider == divider)
    return;
  lastDivider = divider;

  if(divider >= 11 && divider < 65536) {
    HAL_TIM_Base_Stop(&htim1);
    htim1.Init.Period = divider;
    if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
      Error_Handler();
    }
    HAL_TIM_Base_Start(&htim1);
  }
*/
}

void do_gpio_dma(uint32_t length, uint8_t trigger_mask, uint8_t trigger_values, int divider) {
  setDivider(divider);

  if(length > sizeof(gpio_buffer)) {
    length = sizeof(gpio_buffer);
  }

  wait_for_trigger(trigger_mask, trigger_values); // TODO: switch this to watching the DMA values
  uint8_t* buffer = gpio_buffer;
  int count = length;
  int delayTicks = 140;
  // 132 = 8333 bps
  // 140 = 9434 bps
  // 145 = 10000 bps
  while (count--)
  {
    *buffer++ = readPort;
    {
      volatile int us = delayTicks;
      while (us--);
    }
  }

  realign_buffer(gpio_buffer, length);
  reverse(gpio_buffer, length);

/*
  HAL_DMA_Start(htim1.hdma[TIM_DMA_ID_UPDATE], (uint32_t)&GPIOA->IDR, (uint32_t)&gpio_buffer, length);
  __HAL_TIM_ENABLE_DMA(&htim1, TIM_DMA_UPDATE);
  HAL_DMA_PollForTransfer(htim1.hdma[TIM_DMA_ID_UPDATE], HAL_DMA_FULL_TRANSFER, 100);
  __HAL_TIM_DISABLE_DMA(&htim1, TIM_DMA_UPDATE);
*/
}

/*
void do_gpio_loop(uint32_t length, uint8_t trigger_mask, uint8_t trigger_values) {
  uint8_t *buffer = gpio_buffer;
  uint8_t *ending;

  if(length > sizeof(gpio_buffer)) {
    length = sizeof(gpio_buffer);
  }
  ending = gpio_buffer+length;

  wait_for_trigger(trigger_mask, trigger_values);

  __disable_irq();

// good for 7.2MHz
// check Src/read256.c for a higher speed version (~12MHz) that uses more flash
    asm(
      "ldr     r1, [%1]\n"
      "poll:"
      "strb    r1, [%0, #0]\n"
      "adds    %0, #1\n"
      "cmp     %2, %0\n"
      "ldr     r1, [%1]\n" // moving this before the cmp slows the loop down to 6MHz
      "bhi.n   poll\n"
    : // no outputs
    : "r" (buffer), "r" (&GPIOA->IDR), "r" (ending)
    : "r1"
    );

  __enable_irq();
}
*/