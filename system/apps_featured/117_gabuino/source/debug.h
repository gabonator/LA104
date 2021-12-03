extern "C" {
#include "lib/stm32f10x.h"
}

#define CoreDebug_DEMCR_MON_EN_Pos         16                                             /*!< CoreDebug DEMCR: MON_EN Position */
#define CoreDebug_DEMCR_MON_EN_Msk         (1UL << CoreDebug_DEMCR_MON_EN_Pos)            /*!< CoreDebug DEMCR: MON_EN Mask */

void _Trap();
uint32_t trappedAddress = 0;

// https://community.st.com/s/question/0D53W00000BKtSj/how-to-ignore-bkpt-instruction-on-cortex-m7
// https://github.com/cheng3100/armdbg.git

typedef struct __attribute__((packed)) ContextStateFrame {
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r12;
  uint32_t lr;
  uint32_t return_address;
  uint32_t xpsr;
} sContextStateFrame;

void Trap();

extern "C" void debug_monitor_handler_c(sContextStateFrame *frame) 
{
  volatile uint32_t *dfsr = (uint32_t *)0xE000ED30;
  const uint32_t dfsr_bkpt_evt_bitmask = (1 << 1);
  *dfsr = dfsr_bkpt_evt_bitmask;

  trappedAddress = frame->return_address;
  Trap();
//  frame->return_address += 2; // jump to next instr?
}

__attribute__((naked))
void DebugMon_Handler(void)
{
//  __asm volatile(
//      "mrs r0, msp\n"
//      "b debug_monitor_handler_c\n");

  __asm volatile(
      "tst lr, #4 \n"
      "ite eq \n"
      "mrseq r0, msp \n"
      "mrsne r0, psp \n"
      "b debug_monitor_handler_c\n");
}

BIOS::OS::TInterruptHandler isrDebugOld;
void BeginDebugSession()
{
    isrDebugOld = BIOS::OS::GetInterruptVector(BIOS::OS::IDebugMonitor);
    
    CoreDebug->DEMCR |= CoreDebug_DEMCR_MON_EN_Msk;
    BIOS::OS::SetInterruptVector(BIOS::OS::IDebugMonitor, DebugMon_Handler);
}

void EndDebugSession()
{
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_MON_EN_Msk;
    BIOS::OS::SetInterruptVector(BIOS::OS::IDebugMonitor, isrDebugOld);
}
