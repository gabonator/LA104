#include "scancode.h"
#include "gpio.h"
#include "periph.h"

class CKeyboard
{
public:
  enum 
  {
    None = 0,
    ParityBit = 1,
    StopBit = 2,
    BufferFull = 3
  };

private:
  BIOS::OS::TInterruptHandler mOldHandler{nullptr};
  volatile uint32_t mReady{0};
  volatile uint32_t mLocal{0};
  bool mShift{false};
  bool mRelease{false};
  volatile uint8_t mError{None};

public:
  void Attach()
  {
#ifndef __APPLE__
    static CKeyboard* self;
    self = this;
    mOldHandler = BIOS::OS::GetInterruptVector(BIOS::OS::IEXTI15_10_IRQ);
    BIOS::OS::SetInterruptVector(BIOS::OS::IEXTI15_10_IRQ, [](){ 
      self->IsrHandler();
    });

    InitInterrupt();
#endif
  }

  void Detach()
  {
#ifndef __APPLE__
    StopInterrupt();
    BIOS::OS::SetInterruptVector(BIOS::OS::IEXTI15_10_IRQ, mOldHandler);
#endif
  }
  void DelayUs(int us)
  {
      us = us*12;
      while (us--)
      {
          __asm__("");
      }
  }

  bool Write(int d)
  {
    Detach();
    #define CLK Gpio::BASEB, Gpio::P1
    #define DATA Gpio::BASEB, Gpio::P2
    #define ReadClk() (GPIOB->IDR & GPIO_Pin_10)
    #define ReadData() (GPIOB->IDR & GPIO_Pin_11)
    //#define ReadClk() Gpio::GetLevel(CLK)

    Gpio::SetState(CLK, Gpio::StateOutput10Mhz | Gpio::StateOutputPushPull);
    Gpio::SetLevel(CLK, 0);
    DelayUs(10);
    Gpio::SetState(DATA, Gpio::StateOutput10Mhz | Gpio::StateOutputPushPull);
    Gpio::SetLevel(DATA, 0);
    DelayUs(10);
    Gpio::SetState(CLK, Gpio::StateInputPull);
    Gpio::SetLevel(CLK, 1);
    DelayUs(5);
    while (ReadClk());

    int code = d, parity = 1;
    for (int i=0; i<8; i++)
    {
      Gpio::SetLevel(DATA, code & 1);
      parity ^= code;
      code >>= 1;
      while (!ReadClk());
      while (ReadClk());
    }

    // parity
    Gpio::SetLevel(DATA, parity & 1); // fill to odd number of ones
    while (!ReadClk());
    while (ReadClk());

    // stop bit
    Gpio::SetLevel(DATA, 1);
    while (!ReadClk());

    Gpio::SetState(DATA, Gpio::StateInputPull);
    Gpio::SetLevel(DATA, 1);

    while (ReadClk());
    int ack = ReadData();

    Attach();
    return ack == 0;
  }

  bool GetEvent(char& ch, bool& press)
  {
    int code = GetScancode();

    if (!code)
      return false;

    if (code == 0x12 || code == 0x59)
    {
      mShift = !mRelease;
      mRelease = true; // dont show code
    }

    if (code == 0xf0) // release
    {
      mRelease = true;
    } else
    {
      if (!mRelease)
      {
        for (int i=0; i<COUNT(scan_codes); i+=3)
          if (scan_codes[i] == code)
          {
            //mRelease = true; // TODO!
            ch = mShift ? scan_codes[i+2] : scan_codes[i+1];
            press = true;
            return true;
          }

        CONSOLE::Color(RGB565(ff0000));
        CONSOLE::Print("%02x ", code);
        CONSOLE::Color(RGB565(ffffff));
      } else {
        mRelease = false;
        for (int i=0; i<COUNT(scan_codes); i+=3)
          if (scan_codes[i] == code)
          {
            //mRelease = true; // TODO!
            ch = mShift ? scan_codes[i+2] : scan_codes[i+1];
            press = false;
            return true;
          }
      }
    }
    return false;
  }

  char Get()
  {
    if (mError)
    {
      CONSOLE::Color(RGB565(ff0000));
      CONSOLE::Print("ERR%d ", mError);
      CONSOLE::Color(RGB565(ffffff));
      mError = None;
    }
    char c;
    bool press;
    if (GetEvent(c, press))
    {
      if (press)
        return c;
    }
    return 0;
  }

public:
  uint8_t GetScancode()
  {
    if (mLocal == 0)
    {
      // lock
      mLocal = mReady;
      mReady = 0;
      // unlock
    }
    uint32_t front = mLocal >> 24;
    mLocal <<= 8;
    return front;
  }

  uint8_t WaitScancode(int timeoutMs)
  {
    long timeout = BIOS::SYS::GetTick() + timeoutMs;
    uint8_t aux = 0;

    while (BIOS::SYS::GetTick() - timeout > 0 && aux == 0)
      aux = GetScancode();

    return aux;
  }

  bool WaitAck()
  {
    uint8_t ack = WaitScancode(100);
    if (ack != 0xfa)
    {
      CONSOLE::Color(RGB565(ff0000));
      CONSOLE::Print("ACK=%02x ", ack);
      CONSOLE::Color(RGB565(ffffff));
    }
    return ack == 0xfa;
  }

private:
  void InitInterrupt()
  {
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10); // 10;

    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Clear SC EXTI Line Pending Bit */
    EXTI_ClearITPendingBit(EXTI_Line10);

    /* Configure the NVIC Preemption Priority Bits */
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    /* Clear the SC_EXTI IRQ Pendinpbg Bit */
    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  }

  void StopInterrupt()
  {
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
  }

  void IsrHandler(void)
  {   
    static uint32_t buf = 0;
    static int bitn = 0;
    static int parity = 1;

    if (!EXTI_GetITStatus(EXTI_Line10))
      return;

    //Handle the interrupt
    bool bit = GPIOB->IDR & GPIO_Pin_11;
    if (bitn == 0)
    {
      if (bit == 0)
      {
        // start bit
        buf = 0;
        parity = 1;
        bitn++;
      }
    }
    else if (bitn >= 1 && bitn < 9)
    {
      buf >>= 1;
      if (bit)
      {
        buf |= 128;
        parity ^= 1;
      }
      bitn++;
    } else
    if (bitn == 9)
    {
      // skip parity
      if (bit != parity)
        mError = ParityBit;
      bitn++;
    }
    else if (bitn == 10)
    {
      // stop bit
      if (!bit)
        mError = StopBit;
      if (mReady & 0xff000000)
        mError = BufferFull;
      mReady <<= 8;
      mReady |= buf;
      bitn = 0;
    }
    EXTI_ClearITPendingBit(EXTI_Line10);
  }
};
