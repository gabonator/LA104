// based on https://svn.kapsi.fi/jpa/dsoquad/pawn/DS203/
extern unsigned long _etext;
extern unsigned long _sidata;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;
extern unsigned long _estack;
extern uint32_t _stext, _etext, _estack;

#define _fprintf(dev, ...) BIOS::DBG::Print(__VA_ARGS__)
#define __irq__ __attribute__((interrupt("IRQ"), optimize("O2")))

void print_callers(uint32_t *sp);
void __irq__ print_hardfault();

// These are separate from the memory dump stuff because they are collected
// at a different time, i.e. they are not necessarily coherent with the dump.
static uint32_t *SP;
static uint32_t R4, R5, R6, R7, R8, R9, R10, R11;

void __attribute__((naked)) HardFaultException()
{
    // Rescue stack pointer and register values
    asm("mrs %0, msp" : "=r"(SP) : :);
    asm("str r4, %0" : "=m"(R4) : :);
    asm("str r5, %0" : "=m"(R5) : :);
    asm("str r6, %0" : "=m"(R6) : :);
    asm("str r7, %0" : "=m"(R7) : :);
    asm("str r8, %0" : "=m"(R8) : :);
    asm("str r9, %0" : "=m"(R9) : :);
    asm("str r10, %0" : "=m"(R10) : :);
    asm("str r11, %0" : "=m"(R11) : :);
    
    // Print message to screen and halt.
    print_hardfault();
}

void __irq__ print_hardfault()
{    
//    BIOS::LCD::Clear(0);
    _fprintf(stderr, "\n\n   HARDFAULT   \n");
    _fprintf(stderr, "SP:%08x PC:%08x LR:%08x\n",
            (uint32_t)SP, *(SP + 6), *(SP + 5));
    _fprintf(stderr, "SCB HFSR:%08x CFSR:%08x\nBFAR:%08x\n",
            SCB->HFSR, SCB->CFSR, SCB->BFAR);
    
    _fprintf(stderr, "R0:%08x R1:%08x R2:%08x\nR3:%08x ",
            SP[0], SP[1], SP[2], SP[3]);
    _fprintf(stderr, "R4:%08x R5:%08x\nR6:%08x R7:%08x ",
            R4, R5, R6, R7);
    _fprintf(stderr, "R8:%08x\nR9:%08x ",
            R8, R9);
    _fprintf(stderr, "R10:%08x R11:%08x\nR12:%08x\n", R10, R11, *(SP + 4));
    
    
    print_callers(SP + 7);
    while(1);
}

void print_callers(uint32_t *sp)
{
    uint32_t callers[5] = {0};
    int i = 0;
    uint32_t *p = sp;
    while (i < 5 && p < &_estack)
    {
        if (*p >= (uint32_t)&_stext && *p <= (uint32_t)&_etext)
            callers[i++] = *p;
        
        p++;
    }
    _fprintf(stderr, "Callers: %08x %08x %08x\n%08x %08x\n",
            callers[0], callers[1], callers[2], callers[3], callers[4]);
}

