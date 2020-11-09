#if defined(__APPLE__) || defined(WIN32)
class CMidiDevice
{
public:
    void Begin()
    {
    }
    void End()
    {
    }
    void Send(int v)
    {
    }
};

#else

#ifdef LA104
class CMidiDevice
{
public:
    void Begin()
    {
      BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
      BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);
      BIOS::GPIO::UART::Setup(31250, BIOS::GPIO::UART::length8);
    }
    void End()
    {
      BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Input);
      BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Input);
    }
    void Send(int v)
    {
        BIOS::GPIO::UART::Write(v);
    }
};
#endif

#if defined(DS203) || defined(DS203HD) || defined(DS213)
class CMidiDevice
{
    enum {
      LogicZero = (int)BIOS::DAC::EMode::LogicHigh,
      LogicOne = (int)BIOS::DAC::EMode::LogicLow
    };

public:
    void Begin()
    {
        BIOS::DAC::SetMode((BIOS::DAC::EMode)LogicOne, nullptr, 0);
    }

    void End()
    {
    }

    void Send(int v)
    {
        SendBit(0); // start
        for (int i=0; i<8; i++)
        {
          SendBit(v & 1); 
          v >>= 1;
        }
        SendBit(1); // stop
    }

    void SendBit(int b)
    {
        BIOS::DAC::SetMode(b ? (BIOS::DAC::EMode)LogicOne : (BIOS::DAC::EMode)LogicZero, nullptr, 0);
        // 31250 baud -> 32 us per bit
        DelayUs(32-7);
    }

    void DelayUs(int us)
    {
      us = us*12;
      while (us--)
      {
        __asm__("");
      }
    }
};
#endif

#endif
