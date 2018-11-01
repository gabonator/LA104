#include <stdint.h>
#include <stm32f10x_conf.h>
#include <stm32f10x.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

/*
  P1: PB15, PB10  remap   TIM2_CH3
  P2: PB14, PB11  remap   TIM2_CH4
  P3: PB13, PB8   default TIM4_CH3
  P4: PB12, PB9   default TIM4_CH4
*/

void assert_failed(u8* file, u32 line)
{
  while (1) ;
}

void RCCInit(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  // TIM2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  // TIM4
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
} 

void TimerInit(uint16_t period) 
{
  TIM_TimeBaseInitTypeDef timerInitStructure;

  // TIM2
  timerInitStructure.TIM_Prescaler = 72 ;
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = period;
  timerInitStructure.TIM_ClockDivision = 0;
  timerInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM2, &timerInitStructure);
  TIM_Cmd(TIM2, ENABLE); 

  // TIM4
  TIM_TimeBaseInit(TIM4, &timerInitStructure);
  TIM_Cmd(TIM4, ENABLE); 
}

void GPIOInit(void)
{
  // TIM2
  {
    GPIO_InitTypeDef gpioStructure; 
    gpioStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz ; 
    GPIO_Init(GPIOB, &gpioStructure); 
  }
  // TIM4
  {
    GPIO_InitTypeDef gpioStructure; 
    gpioStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOB, &gpioStructure); 
  }
}

void PWMInit(void)
{ 
  TIM_OCInitTypeDef outputChannelInit;
  outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
  outputChannelInit.TIM_Pulse = 1;
  outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
  outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

  // TIM2
  TIM_OC3Init(TIM2, &outputChannelInit);
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

  TIM_OC4Init(TIM2, &outputChannelInit);
  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);

  // TIM4
  TIM_OC3Init(TIM4, &outputChannelInit);
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

  TIM_OC4Init(TIM4, &outputChannelInit);
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
}

__attribute__((__section__(".entry"))) 
int main(void)
{ 
  RCCInit() ;
  GPIOInit() ;
  TimerInit(999) ;
  PWMInit() ; 

  for (int k=0; k<10; k++)
  {
    for (int j=0; j<4000; j++)
    {
      int P1pwm = 0;
      int P2pwm = 0;
      int P3pwm = 0;
      int P4pwm = 0;

      if (j<1000)
        P1pwm = j;
      else
      if (j<2000)
        P2pwm = j - 1000;
      else
      if (j<3000)
        P3pwm = j - 2000;
      else
      if (j<4000)
        P4pwm = j - 3000;

      TIM_SetCompare3 (TIM2, P1pwm);
      TIM_SetCompare4 (TIM2, P2pwm);
      TIM_SetCompare3 (TIM4, P3pwm);
      TIM_SetCompare4 (TIM4, P4pwm);

      for (volatile uint32_t i=0; i<10000; i++);
    }
  }
  return 0;
}
