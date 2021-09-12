#include <stdint.h>
extern void isrinit(void);

#include "../../../../os_host/source/stm32f10x_conf.h"
void assert_failed(uint8_t* file, uint32_t line);

#include "STM32F10x_StdPeriph_Driver/inc/misc.h"
#include "STM32F10x_StdPeriph_Driver/inc/stm32f10x_exti.h"
#include "STM32F10x_StdPeriph_Driver/inc/stm32f10x_gpio.h"

#include "STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c"
#include "STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c"
#include "STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c"
#include "STM32F10x_StdPeriph_Driver/src/misc.c"

volatile uint32_t buf = 0;
volatile uint32_t bitn = 0;
volatile uint32_t ready = 0;

uint32_t isrget()
{
  // disable isr during readout!
  uint32_t temp = ready;
  ready = 0;
  return temp;
}

void isrhandle(void)
{   
    if(EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
        //Handle the interrupt
        bool bit = GPIOB->IDR & GPIO_Pin_11;
        if (bitn == 0) // start bit
        {
          if (bit == 0)
          {
            buf = 0;
            bitn++;
          }
        }
        else if (bitn >= 1 && bitn < 9)
        {
          buf >>= 1;
          if (bit)
            buf |= 128;
          bitn++;
        } else
        if (bitn == 9)
        {
          bitn++; // parity
        }
        else if (bitn == 10)
        {
          ready <<= 8;
          ready |= buf;
          bitn = 0;// stop
        }
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
}

void isrinit(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10); // 10;

  EXTI_StructInit(&EXTI_InitStructure);
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_Line = EXTI_Line10;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Clear SC EXTI Line Pending Bit */
  EXTI_ClearITPendingBit(EXTI_Line10);

  /* Configure the NVIC Preemption Priority Bits */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  /* Clear the SC_EXTI IRQ Pendinpbg Bit */
  NVIC_ClearPendingIRQ(EXTI15_10_IRQn);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}
