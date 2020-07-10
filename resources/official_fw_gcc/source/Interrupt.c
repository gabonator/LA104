/**********************(C) COPYRIGHT 2017 e-Design Co.,Ltd.*********************
 * FileName    : Interrupt.c
 * Description : 
 * Author      : SNAKE 
 * History     :
*******************************************************************************/

#include "Interrupt.h"
#include "USB_istr.h"
#include "MyDefine.h"

u16  BeepCnt, LastEnc, LastKey, KeyHold, gKeyActv, Counter, gmS;
u32  VbtySum = 4200 * 64;
vu16 Dly_mS;

void NMIException(void)
{ }

void HardFaultException(void)
{
    while(1)
    {}
}

void MemManageException(void)
{
    while(1)
    {}
}

void BusFaultException(void)
{
    while(1)
    {}
}
void UsageFaultException(void)
{
    while(1)
    {}
}

void DebugMonitor(void)
{ }

void SVCHandler(void)
{ }

void PendSVC(void)
{ }

/*******************************************************************************
 * FunctionName : SysTick_Handler
 * Description  : 
*******************************************************************************/
void SysTick_Handler(void)
{
    u16 InputSt, EncdInp, EncActP, EncActN;
    if(gFullmS > 0) gFullmS--;
    if(Dly_mS > 0) Dly_mS--;
    gmS++;
    Counter++;
    if(Counter % 2 == 0)                                  // 
    {
        InputSt = __Bios(KEYnDEV, BITMAP);                // 
        EncdInp = InputSt & ENCDBIT;
        EncActP = EncdInp & ~LastEnc;                     // 
        EncActN = ~EncdInp & LastEnc;                     // 
        LastEnc = EncdInp;
        if(EncActN & ENC1a) gKeyActv |= (EncdInp & ENC1b) ? ENCD_1p : ENCD_1n;
        if(EncActP & ENC1b) gKeyActv |= (EncdInp & ENC1a) ? ENCD_1p : ENCD_1n;
        if(EncActN & ENC2a) gKeyActv |= (EncdInp & ENC2b) ? ENCD_2n : ENCD_2p;
        if(EncActP & ENC2b) gKeyActv |= (EncdInp & ENC2a) ? ENCD_2n : ENCD_2p;
        if(Counter % 32 == 0)                              // 
        {
            KeyHold  = InputSt & KEYSBIT;                  // ʶ
            gKeyActv |= (~KeyHold & LastKey) << 4;         // ʶ
            gKeyActv |= (KeyHold & ~LastKey) << 8;         // ʶ
            LastKey  = KeyHold;

            VbtySum += __Bios(PWRCTRL, VBTYmV) - VbtySum / 64; // VBAT ƽ
            gBatVol = GetBatteryVol();
        }
        if(BeepCnt > 1) BeepCnt -= 2;
        else  __Bios(BUZZDEV, DSBL);                       // 
    }
    if(Counter > 5000) Counter = 0;
    if(gmS >= 1000)
    {
        gmS = 0;
        if((gItemParam[STANDBY] != 0) && (gStandbyCnt > 0)) gStandbyCnt--;   // 
        if((gItemParam[POWEROFF] != 0) && (gAutoPwrCnt > 0)) gAutoPwrCnt--;  // 
        gUSB_Vol = Get_USB_Vol();
        gBatFlag = 1;
    }

}

/*******************************************************************************
 * FunctionName : Beep_mS
 * Description  : 
 * Param        : u32 mS 
*******************************************************************************/
void Beep_mS(u32 mS)
{
    BeepCnt = mS;
    __Bios(BUZZDEV, ENBL);                              // 
    __Bios(BUZZDEV, gItemParam[VOLUME] * 10);           // 
}

void WWDG_IRQHandler(void)
{ }

void PVD_IRQHandler(void)
{ }

void TAMPER_IRQHandler(void)
{ }

void RTC_IRQHandler(void)
{ }

void FLASH_IRQHandler(void)
{ }

void RCC_IRQHandler(void)
{ }

void EXTI0_IRQHandler(void)
{ }

void EXTI1_IRQHandler(void)
{ }

void EXTI2_IRQHandler(void)
{ }

void EXTI3_IRQHandler(void)
{ }

void EXTI4_IRQHandler(void)
{ }

void DMA1_Channel1_IRQHandler(void)
{ }

void DMA1_Channel2_IRQHandler(void)
{ }

void DMA1_Channel3_IRQHandler(void)
{ }

void DMA1_Channel4_IRQHandler(void)
{ }

void DMA1_Channel5_IRQHandler(void)
{ }

void DMA1_Channel6_IRQHandler(void)
{ }

void DMA1_Channel7_IRQHandler(void)
{ }

void ADC1_2_IRQHandler(void)
{ }

void USB_HP_CAN1_TX_IRQHandler(void)
{ }

void USB_LP_CAN1_RX0_IRQHandler(void)
{
    USB_Istr();
}

void CAN_RX1_IRQHandler(void)
{ }

void CAN_SCE_IRQHandler(void)
{ }

void EXTI9_5_IRQHandler(void)
{ }

void TIM1_BRK_IRQHandler(void)
{ }

void TIM1_UP_IRQHandler(void)
{ }

void TIM1_TRG_COM_IRQHandler(void)
{ }

void TIM1_CC_IRQHandler(void)
{ }

void TIM2_IRQHandler(void)
{ }

void TIM3_IRQHandler(void)
{ }

void TIM4_IRQHandler(void)
{ }

void I2C1_EV_IRQHandler(void)
{ }

void I2C1_ER_IRQHandler(void)
{ }

void I2C2_EV_IRQHandler(void)
{ }

void I2C2_ER_IRQHandler(void)
{ }

void SPI1_IRQHandler(void)
{ }

void SPI2_IRQHandler(void)
{ }

void USART1_IRQHandler(void)
{ }

void USART2_IRQHandler(void)
{ }

void USART3_IRQHandler(void)
{ }

void EXTI15_10_IRQHandler(void)
{ }

void RTCAlarm_IRQHandler(void)
{ }

void USBWakeUp_IRQHandler(void)
{ }

void TIM8_BRK_IRQHandler(void)
{ }

void TIM8_UP_IRQHandler(void)
{ }

void TIM8_TRG_COM_IRQHandler(void)
{ }

void TIM8_CC_IRQHandler(void)
{ }

void ADC3_IRQHandler(void)
{ }

void FSMC_IRQHandler(void)
{ }

void SDIO_IRQHandler(void)
{ }

void TIM5_IRQHandler(void)
{ }

void SPI3_IRQHandler(void)
{ }

void UART4_IRQHandler(void)
{ }

void UART5_IRQHandler(void)
{ }

void TIM6_IRQHandler(void)
{ }

void TIM7_IRQHandler(void)
{ }

void DMA2_Channel1_IRQHandler(void)
{ }

void DMA2_Channel2_IRQHandler(void)
{ }

void DMA2_Channel3_IRQHandler(void)
{ }

void DMA2_Channel4_5_IRQHandler(void)
{ }

/********************************* END OF FILE ********************************/
