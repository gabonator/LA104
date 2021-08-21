typedef struct
{
  uint32_t DHCSR;                   /*!< Offset: 0x000 (R/W)  Debug Halting Control and Status Register    */
  uint32_t DCRSR;                   /*!< Offset: 0x004 ( /W)  Debug Core Register Selector Register        */
  uint32_t DCRDR;                   /*!< Offset: 0x008 (R/W)  Debug Core Register Data Register            */
  uint32_t DEMCR;                   /*!< Offset: 0x00C (R/W)  Debug Exception and Monitor Control Register */
} CoreDebug_Type;

#define CoreDebug_DEMCR_MON_EN_Pos         16                                             /*!< CoreDebug DEMCR: MON_EN Position */
#define CoreDebug_DEMCR_MON_EN_Msk         (1UL << CoreDebug_DEMCR_MON_EN_Pos)            /*!< CoreDebug DEMCR: MON_EN Mask */
#define CoreDebug_BASE      (0xE000EDF0UL)
#define CoreDebug           ((CoreDebug_Type *)     CoreDebug_BASE)

void _Trap();
uint32_t trappedAddress = 0;

__attribute__((naked))
void DebugMon_Handler(void) 
{
  asm volatile(
    "ldr r0, [sp, #24]\n"  // PC is at offset 24 on the exception stack frame
    "str r0, %0\n"         // store to trappedAddress var
    "ldr r0, %1\n"         // override the return address to point to _Trap
    "mov r0, r2\n"         // GCC loads *r2 instead of r2 where is temporarily stored _Trap address
    "str r0, [sp, #24]\n"  // jump to _Trap
    "bx lr\n"
  : "=m"(trappedAddress) : "m"(_Trap));
}

void Trap();

__attribute__((naked))
void _Trap()
{
    Trap();
    asm volatile(       // jump back to trappedAddress
	"ldr r0, %0\n"
        "bx r0"
    : : "m"(trappedAddress));
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
