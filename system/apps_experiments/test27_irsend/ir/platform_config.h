#ifndef PLATFORM_CONFIG_H_INCLUDED
#define PLATFORM_CONFIG_H_INCLUDED

// LA104 P4: PB9 

#define IR_TX_CARRIER_TIMER            TIM4
#define IR_TX_CARRIER_TIMER_RCC        RCC_APB1Periph_TIM4
#define IR_TX_CARRIER_TIMER_CH_Init          TIM_OC4Init
#define IR_TX_CARRIER_TIMER_CH_PreloadConfig TIM_OC4PreloadConfig
#define IR_TX_CARRIER_TIMER_CH_SetCompare    TIM_SetCompare4
#define IR_TX_CARRIER_RCC              RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO
#define IR_TX_CARRIER_PORT             GPIOB
#define IR_TX_CARRIER_PIN              GPIO_Pin_9

#define IR_TX_DELAY_TIMER            TIM1
#define IR_TX_DELAY_TIMER_IRQ        TIM1_UP_IRQn
#define IR_TX_DELAY_TIMER_RCC        RCC_APB2Periph_TIM1

#endif // PLATFORM_CONFIG_H_INCLUDED
