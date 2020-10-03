class CPinIoBase
{
public:
    enum EPin {SCK, RESET, MOSI, CS, BUSY};
    enum EDir {Input, Output};
    
    virtual void Init() = 0;
    virtual void PinMode(EPin p, EDir d) = 0;
    virtual void Set(EPin p, bool b) = 0;
    virtual bool Get(EPin p) = 0;
};

class CPinIoGpio : public CPinIoBase
{
public:
    virtual void Init() override
    {
    }

    BIOS::GPIO::EPin Pin(EPin p)
    {
        switch (p)
        {
            case CPinIoBase::MOSI: return BIOS::GPIO::EPin::P1;
            case CPinIoBase::SCK: return BIOS::GPIO::EPin::P2;
            case CPinIoBase::CS: return BIOS::GPIO::EPin::P3;
            case CPinIoBase::RESET: return BIOS::GPIO::EPin::P4;
            case CPinIoBase::BUSY: return BIOS::GPIO::EPin::CH4;
            default:
                _ASSERT(0);
                return BIOS::GPIO::EPin::P1;
        }
    }
    
    virtual void PinMode(EPin p, EDir d) override
    {
        if (d == EDir::Input)
            BIOS::GPIO::PinMode(Pin(p), BIOS::GPIO::EMode::Input);
        else
            BIOS::GPIO::PinMode(Pin(p), BIOS::GPIO::EMode::Output);
    }
    
    virtual void Set(EPin p, bool b) override
    {
        BIOS::GPIO::DigitalWrite(Pin(p), b);
    }
    
    virtual bool Get(EPin p) override
    {
        return BIOS::GPIO::DigitalRead(Pin(p));
    }
};

class CSpi : public CPinIoGpio
{
  // https://github.com/Flickerstrip/SoftwareSPI/blob/master/SoftwareSPI.cpp
public:

  void begin() 
  {
      PinMode(CPinIoBase::CS, CPinIoBase::Output);
      PinMode(CPinIoBase::SCK, CPinIoBase::Output);
      PinMode(CPinIoBase::MOSI, CPinIoBase::Output);
      PinMode(CPinIoBase::BUSY, CPinIoBase::Input); 
      PinMode(CPinIoBase::RESET, CPinIoBase::Output);

      Set(CPinIoBase::CS, true);
      Set(CPinIoBase::SCK, true);
      Set(CPinIoBase::MOSI, false);
      Set(CPinIoBase::RESET, true);
      deselect();
  }
    
    void end()
    {
        PinMode(CPinIoBase::CS, CPinIoBase::Input);
        PinMode(CPinIoBase::SCK, CPinIoBase::Input);
        PinMode(CPinIoBase::MOSI, CPinIoBase::Input);
        PinMode(CPinIoBase::BUSY, CPinIoBase::Input);
        PinMode(CPinIoBase::RESET, CPinIoBase::Input);
    }

  uint8_t transfer9(uint16_t b) 
  {
    uint8_t rec = 0;

    for (int i = 0; i < 9; i++) 
    {
        Set(CPinIoBase::MOSI, b & (1<<(8-i))); //MSB first
_Delay();
        Set(CPinIoBase::SCK, true);
_Delay();
        Set(CPinIoBase::SCK, false);
_Delay();
    }
      
    return rec;
  }

  void select() 
  {
      Set(CPinIoBase::CS, false);
  }

  void deselect() 
  {
      Set(CPinIoBase::CS, true);
  }

  void reset() 
  {
      Set(CPinIoBase::RESET, false);
      BIOS::SYS::DelayMs(200);
      Set(CPinIoBase::RESET, true);
      BIOS::SYS::DelayMs(200);
  }

  void _Delay()
  {
    for (volatile int i=0; i<10; i++);
  }

  void wait()
  {
    enum { HIGH = 1, LOW = 0 };
    int busy_value = HIGH;

    while(1){
EVERY(500) {
  BIOS::DBG::Print("#");
}
      busy_value = Get(CPinIoBase::BUSY);
      if(busy_value == LOW){
        break;
      }
    }
  }
};

