extern unsigned long _etext;
extern unsigned long _sidata;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;
extern unsigned long _estack;
extern int main(void);

void __Init_Data(void) {
	unsigned long *src, *dst;
	/* copy the data segment into ram */

/*

  TODO: HUGE NOTICE: WTF!!!! I dont understand what are the 8 bytes at the beginning of _sidata!!!!!!
  It just need to be skipped to work

*/
	src = &_sidata+2; 
	dst = &_sdata;
	if (src != dst)
		while(dst < &_edata)
			*(dst++) = *(src++);

	/* zero the bss segment */
	dst = &_sbss;
	while(dst < &_ebss)
		*(dst++) = 0;
}

void Reset_Handler(void) 
{
	/* Initialize data and bss */
	__Init_Data();
	main();
	while(1) {}
}

void Dummy_Handler(void)
{
}

void Default_Handler(void) 
{
  while (1) {}
}


#define WEAK __attribute__ ((weak))

// TODO: ambigous names!
void WEAK NMI_Handler(void);
void WEAK HardFault_Handler(void);
void WEAK MemManage_Handler(void);
void WEAK BusFault_Handler(void);
void WEAK UsageFault_Handler(void);
void WEAK MemManage_Handler(void);
void WEAK SVC_Handler(void);
void WEAK DebugMon_Handler(void);
void WEAK PendSV_Handler(void);
void WEAK SysTick_Handler(void);

// TODO: ambigous names!
void WEAK DebugMonitor(void);
void WEAK SVCHandler(void);
void WEAK PendSVC(void);
void WEAK NMIException(void);
void WEAK HardFaultException(void);
void WEAK MemManageException(void);
void WEAK BusFaultException(void);
void WEAK UsageFaultException(void);

void WEAK WWDG_IRQHandler(void);
void WEAK PVD_IRQHandler(void);
void WEAK TAMPER_IRQHandler(void);
void WEAK RTC_IRQHandler(void);
void WEAK FLASH_IRQHandler(void);
void WEAK RCC_IRQHandler(void);
void WEAK EXTI0_IRQHandler(void);
void WEAK EXTI1_IRQHandler(void);
void WEAK EXTI2_IRQHandler(void);
void WEAK EXTI3_IRQHandler(void);
void WEAK EXTI4_IRQHandler(void);
void WEAK DMA1_Channel1_IRQHandler(void);
void WEAK DMA1_Channel2_IRQHandler(void);
void WEAK DMA1_Channel3_IRQHandler(void);
void WEAK DMA1_Channel4_IRQHandler(void);
void WEAK DMA1_Channel5_IRQHandler(void);
void WEAK DMA1_Channel6_IRQHandler(void);
void WEAK DMA1_Channel7_IRQHandler(void);
void WEAK ADC1_2_IRQHandler(void);
void WEAK USB_HP_CAN1_TX_IRQHandler(void);
void WEAK USB_LP_CAN1_RX0_IRQHandler(void);  
void WEAK CAN1_RX1_IRQHandler(void);
void WEAK CAN1_SCE_IRQHandler(void);
void WEAK EXTI9_5_IRQHandler(void);
void WEAK TIM1_BRK_IRQHandler(void);
void WEAK TIM1_UP_IRQHandler(void);
void WEAK TIM1_TRG_COM_IRQHandler(void);
void WEAK TIM1_CC_IRQHandler(void);
void WEAK TIM2_IRQHandler(void);
void WEAK TIM3_IRQHandler(void);
void WEAK TIM4_IRQHandler(void);
void WEAK I2C1_EV_IRQHandler(void);
void WEAK I2C1_ER_IRQHandler(void);
void WEAK I2C2_EV_IRQHandler(void);
void WEAK I2C2_ER_IRQHandler(void);
void WEAK SPI1_IRQHandler(void);
void WEAK SPI2_IRQHandler(void);
void WEAK USART1_IRQHandler(void);
void WEAK USART2_IRQHandler(void);
void WEAK USART3_IRQHandler(void);
void WEAK EXTI15_10_IRQHandler(void);
void WEAK RTCAlarm_IRQHandler(void);
void WEAK USBWakeUp_IRQHandler(void);
void WEAK TIM8_BRK_IRQHandler(void);
void WEAK TIM8_UP_IRQHandler(void);
void WEAK TIM8_TRG_COM_IRQHandler(void);
void WEAK TIM8_CC_IRQHandler(void);
void WEAK ADC3_IRQHandler(void);
void WEAK FSMC_IRQHandler(void);
void WEAK SDIO_IRQHandler(void);
void WEAK TIM5_IRQHandler(void);
void WEAK SPI3_IRQHandler(void);
void WEAK UART4_IRQHandler(void);
void WEAK UART5_IRQHandler(void);
void WEAK TIM6_IRQHandler(void);
void WEAK TIM7_IRQHandler(void);
void WEAK DMA2_Channel1_IRQHandler(void);
void WEAK DMA2_Channel2_IRQHandler(void);
void WEAK DMA2_Channel3_IRQHandler(void);
void WEAK DMA2_Channel4_5_IRQHandler(void);


typedef void( *const intfunc )( void );
__attribute__ ((section(".isr_vectors")))
void (* const g_pfnVectors[])(void) = {
    (intfunc)((unsigned long)&_estack), /* The stack pointer after relocation */
Reset_Handler, NMIException, HardFaultException,
MemManageException, BusFaultException, UsageFaultException, 0, 0,
0, 0, SVCHandler, DebugMonitor, 0, PendSVC, SysTick_Handler,
WWDG_IRQHandler, PVD_IRQHandler, TAMPER_IRQHandler, RTC_IRQHandler,
FLASH_IRQHandler, RCC_IRQHandler, EXTI0_IRQHandler,
EXTI1_IRQHandler, EXTI2_IRQHandler, EXTI3_IRQHandler,
EXTI4_IRQHandler, DMA1_Channel1_IRQHandler,
DMA1_Channel2_IRQHandler, DMA1_Channel3_IRQHandler,
DMA1_Channel4_IRQHandler, DMA1_Channel5_IRQHandler,
DMA1_Channel6_IRQHandler, DMA1_Channel7_IRQHandler,
ADC1_2_IRQHandler, USB_HP_CAN1_TX_IRQHandler,
USB_LP_CAN1_RX0_IRQHandler, CAN1_RX1_IRQHandler, CAN1_SCE_IRQHandler,
EXTI9_5_IRQHandler, TIM1_BRK_IRQHandler, TIM1_UP_IRQHandler,
TIM1_TRG_COM_IRQHandler, TIM1_CC_IRQHandler, TIM2_IRQHandler,
TIM3_IRQHandler, TIM4_IRQHandler, I2C1_EV_IRQHandler,
I2C1_ER_IRQHandler, I2C2_EV_IRQHandler, I2C2_ER_IRQHandler,
SPI1_IRQHandler, SPI2_IRQHandler, USART1_IRQHandler,
USART2_IRQHandler, USART3_IRQHandler, EXTI15_10_IRQHandler,
RTCAlarm_IRQHandler, USBWakeUp_IRQHandler, TIM8_BRK_IRQHandler,
TIM8_UP_IRQHandler, TIM8_TRG_COM_IRQHandler, TIM8_CC_IRQHandler,
ADC3_IRQHandler, FSMC_IRQHandler, SDIO_IRQHandler, TIM5_IRQHandler,
SPI3_IRQHandler, UART4_IRQHandler, UART5_IRQHandler,
TIM6_IRQHandler, TIM7_IRQHandler, DMA2_Channel1_IRQHandler,
DMA2_Channel2_IRQHandler, DMA2_Channel3_IRQHandler,
DMA2_Channel4_5_IRQHandler
};

// TODO: ambigous names!
#pragma weak MMI_Handler		= Default_Handler
#pragma weak MemManage_Handler		= Default_Handler
#pragma weak BusFault_Handler		= Default_Handler
#pragma weak UsageFault_Handler		= Default_Handler
#pragma weak SVC_Handler		= Default_Handler
#pragma weak DebugMon_Handler		= Default_Handler
#pragma weak PendSV_Handler		= Default_Handler

#pragma weak SysTick_Handler		= Default_Handler
#pragma weak WWDG_IRQHandler		= Default_Handler
#pragma weak PVD_IRQHandler		= Default_Handler
#pragma weak TAMPER_IRQHandler		= Default_Handler
#pragma weak RTC_IRQHandler		= Default_Handler
#pragma weak FLASH_IRQHandler		= Default_Handler
#pragma weak RCC_IRQHandler		= Default_Handler
#pragma weak EXTI0_IRQHandler		= Default_Handler
#pragma weak EXTI1_IRQHandler		= Default_Handler
#pragma weak EXTI2_IRQHandler		= Default_Handler
#pragma weak EXTI3_IRQHandler		= Default_Handler
#pragma weak EXTI4_IRQHandler		= Default_Handler
#pragma weak DMA1_Channel1_IRQHandler	= Default_Handler
#pragma weak DMA1_Channel2_IRQHandler	= Default_Handler
#pragma weak DMA1_Channel3_IRQHandler	= Default_Handler
#pragma weak DMA1_Channel4_IRQHandler	= Default_Handler
#pragma weak DMA1_Channel5_IRQHandler	= Default_Handler
#pragma weak DMA1_Channel6_IRQHandler	= Default_Handler
#pragma weak DMA1_Channel7_IRQHandler	= Default_Handler
#pragma weak ADC1_2_IRQHandler		= Default_Handler
#pragma weak USB_HP_CAN1_TX_IRQHandler	= Default_Handler
#pragma weak USB_LP_CAN1_RX0_IRQHandler	= Default_Handler
#pragma weak CAN1_RX1_IRQHandler	= Default_Handler 
#pragma weak CAN1_SCE_IRQHandler	= Default_Handler
#pragma weak EXTI9_5_IRQHandler		= Default_Handler
#pragma weak TIM1_BRK_IRQHandler	= Default_Handler
#pragma weak TIM1_UP_IRQHandler		= Default_Handler
#pragma weak TIM1_TRG_COM_IRQHandler	= Default_Handler
#pragma weak TIM1_CC_IRQHandler		= Default_Handler
#pragma weak TIM2_IRQHandler		= Default_Handler
#pragma weak TIM3_IRQHandler		= Default_Handler
#pragma weak TIM4_IRQHandler		= Default_Handler
#pragma weak I2C1_EV_IRQHandler		= Default_Handler
#pragma weak I2C1_ER_IRQHandler		= Default_Handler
#pragma weak I2C2_EV_IRQHandler		= Default_Handler
#pragma weak I2C2_ER_IRQHandler		= Default_Handler
#pragma weak SPI1_IRQHandler		= Default_Handler
#pragma weak SPI2_IRQHandler		= Default_Handler
#pragma weak USART1_IRQHandler		= Default_Handler
#pragma weak USART2_IRQHandler		= Default_Handler
#pragma weak USART3_IRQHandler		= Default_Handler
#pragma weak EXTI15_10_IRQHandler	= Default_Handler
#pragma weak RTCAlarm_IRQHandler	= Default_Handler
#pragma weak USBWakeUp_IRQHandler	= Default_Handler
#pragma weak TIM8_BRK_IRQHandler	= Default_Handler
#pragma weak TIM8_UP_IRQHandler		= Default_Handler
#pragma weak TIM8_TRG_COM_IRQHandler	= Default_Handler
#pragma weak TIM8_CC_IRQHandler		= Default_Handler
#pragma weak ADC3_IRQHandler		= Default_Handler
#pragma weak FSMC_IRQHandler		= Default_Handler
#pragma weak SDIO_IRQHandler		= Default_Handler
#pragma weak TIM5_IRQHandler		= Default_Handler
#pragma weak SPI3_IRQHandler		= Default_Handler
#pragma weak UART4_IRQHandler		= Default_Handler
#pragma weak UART5_IRQHandler		= Default_Handler
#pragma weak TIM6_IRQHandler		= Default_Handler
#pragma weak TIM7_IRQHandler		= Default_Handler
#pragma weak DMA2_Channel1_IRQHandler	= Default_Handler
#pragma weak DMA2_Channel2_IRQHandler	= Default_Handler
#pragma weak DMA2_Channel3_IRQHandler	= Default_Handler
#pragma weak DMA2_Channel4_5_IRQHandler	= Default_Handler

// TODO: ambigous names!
#pragma weak USB_HP_CAN_TX_IRQHandler   = Dummy_Handler
#pragma weak USB_LP_CAN_RX0_IRQHandler  = Dummy_Handler
#pragma weak CAN_RX1_IRQHandler         = Dummy_Handler
#pragma weak CAN_SCE_IRQHandler         = Dummy_Handler
#pragma weak DebugMonitor               = Dummy_Handler
#pragma weak SVCHandler                 = Dummy_Handler
#pragma weak PendSVC                    = Dummy_Handler

// TODO: ambigous names!
#pragma weak NMIException               = Default_Handler
#pragma weak HardFaultException         = Default_Handler
#pragma weak MemManageException         = Default_Handler
#pragma weak BusFaultException          = Default_Handler
#pragma weak UsageFaultException        = Default_Handler
