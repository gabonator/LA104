namespace EXPERIMENTS
{
/*
int lastInterrupts = 0;

inline uint32_t  __get_BASEPRI(void)
{
  register uint32_t __regBasePri  __asm("basepri");
  return(__regBasePri);
}
inline void __set_BASEPRI(uint32_t basePri)
{
  register uint32_t __regBasePri  __asm("basepri");
  __regBasePri = (basePri & 0xff);
}

void disableInterrupts(void)
{
  lastInterrupts = __get_BASEPRI();
}

void enableInterrupts(void)
{
  __set_BASEPRI(lastInterrupts);
}
*/
__attribute__( ( always_inline ) ) static inline void enableInterrupts(void)
{
  __asm volatile ("cpsie i");
}


/** \brief  Disable IRQ Interrupts

  This function disables IRQ interrupts by setting the I-bit in the CPSR.
  Can only be executed in Privileged modes.
 */
__attribute__( ( always_inline ) ) static inline void disableInterrupts(void)
{
  __asm volatile ("cpsid i");
}

  void Burst(uint8_t* stream, int len)
  {
    MEMORY::Mask32(0x40010800, 0xf0000, 0x40000);
    MEMORY::Write32(0x40001400, 0x84);
    MEMORY::Write32(0x40007400, 0x0);
    MEMORY::Write32(0x40000800, 0x80);
    MEMORY::Mask32(0x40010C00, 0xf000000, 0x3000000);

  // *0x40010800 #= 0x40000
  // *0x40001400 = 0x84
  // *0x40007400 = 0
  // *0x40000800 = 0x80
  // *0x40010C00 #= 0x3000000

/*
    #define DELAY()  __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); \
       __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); \
       __asm__("nop"); __asm__("nop"); __asm__("nop");
    #define DELAYS() __asm__("nop");
*/

/*
    #define DELAY()  __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); \
       __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); \
       __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop");
    #define DELAYS() __asm__("nop");
*/

/*// max 4
    #define DELAY()  __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); \
       __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); \
       __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop");
    #define DELAYS() __asm__("nop"); __asm__("nop"); __asm__("nop");
*/

    // determined empirically
    #define DELAY()  __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); \
       __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); \
       __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop"); __asm__("nop");
    #define DELAYS() __asm__("nop"); __asm__("nop"); __asm__("nop");

    disableInterrupts();
    uint32_t destptr[2] = {0x40010C14, 0x40010C10};
    while (len--)
    {
      int v = *stream++;
      {
        *((volatile uint32_t*)destptr[v&1]) = 1<<6;
        v>>=1;
        DELAY();
      }
      {
        *((volatile uint32_t*)destptr[v&1]) = 1<<6;
        v>>=1;
        DELAY();
      }
      {
        *((volatile uint32_t*)destptr[v&1]) = 1<<6;
        v>>=1;
        DELAY();
      }
      {
        *((volatile uint32_t*)destptr[v&1]) = 1<<6;
        v>>=1;
        DELAY();
      }
      {
        *((volatile uint32_t*)destptr[v&1]) = 1<<6;
        v>>=1;
        DELAY();
      }
      {
        *((volatile uint32_t*)destptr[v&1]) = 1<<6;
        v>>=1;
        DELAY();
      }
      {
        *((volatile uint32_t*)destptr[v&1]) = 1<<6;
        v>>=1;
        DELAY();
      }
      *((volatile uint32_t*)destptr[v&1]) = 1<<6;
      DELAYS();
    }
    enableInterrupts();
  }

}