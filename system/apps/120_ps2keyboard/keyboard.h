#include "scancode.h"
#include "periph.h"

class CKeyboard
{
  BIOS::OS::TInterruptHandler mOldHandler{nullptr};
  volatile uint32_t mReady{0};
  volatile uint32_t mLocal{0};
  bool mShift{false};
  bool mRelease{false};
  volatile bool mError{false};

public:
  void Attach()
  {
    static CKeyboard* self;
    self = this;
    mOldHandler = BIOS::OS::GetInterruptVector(BIOS::OS::IEXTI15_10_IRQ);
    BIOS::OS::SetInterruptVector(BIOS::OS::IEXTI15_10_IRQ, [](){ 
      self->IsrHandler();
    });

    InitInterrupt();
  }

  void Detach()
  {
    StopInterrupt();
    BIOS::OS::SetInterruptVector(BIOS::OS::IEXTI15_10_IRQ, mOldHandler);
  }

  char Get()
  {
      if (mError)
      {
        CONSOLE::Color(RGB565(ff0000));
        CONSOLE::Print("ERR ");
        CONSOLE::Color(RGB565(ffffff));
        mError = false;
      }

      int code = GetScancode();

      if (!code)
        return 0;

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
              return mShift ? scan_codes[i+2] : scan_codes[i+1];
            }

          CONSOLE::Color(RGB565(ff0000));
          CONSOLE::Print("%02x ", code);
          CONSOLE::Color(RGB565(ffffff));
        }
        mRelease = false;
      }
      return 0;
  }

private:
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
  //  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
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
        bitn++;
      }
    }
    else if (bitn >= 1 && bitn < 9)
    {
      buf >>= 1;
      if (bit)
        buf |= 128;
      bitn++;
    } else
    if (bitn == 9)
    {
      // skip parity
      bitn++;
    }
    else if (bitn == 10)
    {
      // stop bit
      if (!bit)
        mError = true;
      if (mReady & 0xff000000)
        mError = true;
      mReady <<= 8;
      mReady |= buf;
      bitn = 0;
    }
    EXTI_ClearITPendingBit(EXTI_Line10);
  }
};
