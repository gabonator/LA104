/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : Interrupt.h  
 Version   : DS203_APP Ver 2.3x                                  Author : bure
*******************************************************************************/

#ifndef __Interrupt_H
#define __Interrupt_H

#include "system.h"

//extern vu8  Cursor_Cnt, Key_Wait_Cnt, Key_Repeat_Cnt, Key_Buffer, Cnt_mS, Cnt_20mS;
extern vu8  Cursor_Cnt, Key_Wait_Cnt, Key_Repeat_Cnt, Key_Buffer, Cnt_20mS;
extern vu16  Cnt_mS;
extern vu32 LimitTransfer;

extern vu8  Twink, Blink;
extern u8   Volume, Light, LoBeepLevel;
extern vu16 Delay_Cnt, Beep_mS, Key_Status_Last, Sec_Cnt, PD_Cnt, NotificationTimer,DelayLoopBreak,SpecFrameCount,
            PersHoldTimer,SecondsTick,ConfNameTimer,CursorDisplayTimer, AutoSetTimer;
extern vu8  Tim2Factor;
extern vu8  BurstLimit;
extern vu8  FineAdjust;
extern u8   FineAdjustLimit;
//extern vu8  FineAdjustFactor[9];
extern vu16 FineAdjustFactor[17];
extern vu8  Key_Mid_Speed;
extern vu8  RepeatTimerR;
extern vu8  RepeatTimerL;
extern vu32 Wait_Cnt; 
void NMIException(void);
void HardFaultException(void);
void MemManageException(void);
void BusFaultException(void);
void UsageFaultException(void);
void DebugMonitor(void);
void SVCHandler(void);
void PendSVC(void);
void SysTickHandler(void);
void WWDG_IRQHandler(void);
void PVD_IRQHandler(void);
void TAMPER_IRQHandler(void);
void RTC_IRQHandler(void);
void FLASH_IRQHandler(void);
void RCC_IRQHandler(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void DMA1_Channel1_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
void ADC1_2_IRQHandler(void);
void USB_HP_CAN_TX_IRQHandler(void);
void USB_LP_CAN_RX0_IRQHandler(void);
void CAN_RX1_IRQHandler(void);
void CAN_SCE_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void TIM1_BRK_IRQHandler(void);
void TIM1_UP_IRQHandler(void);
void TIM1_TRG_COM_IRQHandler(void);
void TIM1_CC_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);
void I2C2_EV_IRQHandler(void);
void I2C2_ER_IRQHandler(void);
void SPI1_IRQHandler(void);
void SPI2_IRQHandler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void RTCAlarm_IRQHandler(void);
void USBWakeUp_IRQHandler(void);
void TIM8_BRK_IRQHandler(void);
void TIM8_UP_IRQHandler(void);
void TIM8_TRG_COM_IRQHandler(void);
void TIM8_CC_IRQHandler(void);
void ADC3_IRQHandler(void);
void FSMC_IRQHandler(void);
void SDIO_IRQHandler(void);
void TIM5_IRQHandler(void);
void SPI3_IRQHandler(void);
void UART4_IRQHandler(void);
void UART5_IRQHandler(void);
void TIM6_IRQHandler(void);
void TIM7_IRQHandler(void);
void DMA2_Channel1_IRQHandler(void);
void DMA2_Channel2_IRQHandler(void);
void DMA2_Channel3_IRQHandler(void);
void DMA2_Channel4_5_IRQHandler(void);
void BackLight(s8 shift);
					 

#endif /* __Interrupt_H */

/********************************* END OF FILE ********************************/
