namespace MEMORY
{
  uint8_t buffer[2048];

  uint32_t GetBufferSize()
  {
    return sizeof(buffer);
  }

  uint32_t GetBufferPtr()
  {
    return (uint32_t)buffer;
  }

  int hex(char ch)
  {
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    _ASSERT(0);
    return 0;
  }

  void MemoryWrite(uint32_t address, char* buf)
  {
    _ASSERT(buf);
    uint8_t* ptr = (uint8_t*)address;

    while (buf[0] && buf[1])
    {
      *ptr++ = (hex(buf[0]) << 4) | hex(buf[1]);
      buf+=2;
    }
  }

  void MemoryRead(uint32_t ptr, int n)
  {
    TERMINAL::Print("{raw:%d}", n);
    uint8_t* p = (uint8_t*)ptr;
    while (n--)
    {
      TERMINAL::Print("%02x", *p++);
//      TERMINAL::Write((uint8_t*)ptr, n);
    }
  }

  uint32_t Read32(uint32_t ptr)
  {
    uint32_t* p = (uint32_t*)ptr;
    return *p;
  }

  uint32_t Write32(uint32_t ptr, uint32_t value)
  {
    uint32_t* p = (uint32_t*)ptr;
    *p = value;
    return *p;
  }

  uint32_t Mask32(uint32_t ptr, uint32_t mask, uint32_t value)
  {
    uint32_t* p = (uint32_t*)ptr;
    *p &= ~mask;
    *p |= value;
    return *p;
  }
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
    Mask32(0x40010800, 0xf0000, 0x40000);
    Write32(0x40001400, 0x84);
    Write32(0x40007400, 0x0);
    Write32(0x40000800, 0x80);
    Mask32(0x40010C00, 0xf000000, 0x3000000);

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
