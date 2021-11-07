#include <library.h>

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

/*
 801eda0:	4b03      	ldr	r3, [pc, #12]	; (801edb0 <_Z16DebugMon_Handlerv+0x10>)
 801eda2:	4a04      	ldr	r2, [pc, #16]	; (801edb4 <_Z16DebugMon_Handlerv+0x14>)

 801eda4:	9806      	ldr	r0, [sp, #24]
 801eda6:	6018      	str	r0, [r3, #0]
 801eda8:	6810      	ldr	r0, [r2, #0]   <<< we do not want dereference
		4610      	mov	r0, r2         <<< we want value
 801edac:	9006      	str	r0, [sp, #24]
 801edae:	4770      	bx	lr
*/

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

void Trap()
{
    // shift trappedAddress by 2 (length of BKPT) otherwise it would jump to the same 
    // BKPT instruction
    trappedAddress += 2;
    trappedAddress |= 1;

    BIOS::DBG::Print("TRAP{");
    for (int i=0; i<10; i++)
    {
      BIOS::SYS::DelayMs(100);
      BIOS::DBG::Print("%d", i);
    }
    BIOS::DBG::Print("}");
}

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

static uint32_t *SP;

uint32_t GetStackPointer()
{
  asm("mrs %0, msp" : "=r"(SP) : :);
  return (uint32_t)SP;
}

__attribute__((__section__(".entry")))
int main(void)
{
    BIOS::DBG::Print("SP=0x%08x\n", GetStackPointer());

    BeginDebugSession();
    BIOS::DBG::Print("Before breakpoint\n");
    __asm("bkpt");
    BIOS::DBG::Print("After first breakpoint\n");       
    __asm("bkpt");
    BIOS::DBG::Print("After second breakpoint\n");       
    EndDebugSession();

    BIOS::DBG::Print("Finished!\n"); 
    BIOS::DBG::Print("SP=0x%08x\n", GetStackPointer());

    BIOS::SYS::DelayMs(5000);
    return 0;
}
