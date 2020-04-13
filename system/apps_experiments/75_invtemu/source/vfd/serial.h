class CVfdComm
{
  enum EPins
  {
    pinDI = GPIO::EPin::P1, // TXD
    pinDE = GPIO::EPin::P3,
    pinRE = GPIO::EPin::P4,
    pinRO = GPIO::EPin::P2, // RXD

    UartRX = pinRO,
    UartTX = pinDI,

    ControlDe = pinDE,
    ControlRe = pinRE,
  };
    
public:
  void setup()
  {    
    GPIO::PinMode((GPIO::EPin)ControlDe, GPIO::EMode::Output);
    GPIO::PinMode((GPIO::EPin)ControlRe, GPIO::EMode::Output);
    GPIO::DigitalWrite((GPIO::EPin)ControlDe, false);
    GPIO::DigitalWrite((GPIO::EPin)ControlRe, false);
    GPIO::PinMode((GPIO::EPin)UartRX, GPIO::Uart);
    GPIO::PinMode((GPIO::EPin)UartTX, GPIO::Uart);
    GPIO::UART::Setup(19200, GPIO::UART::EConfig(GPIO::UART::length8 | GPIO::UART::parityEven | GPIO::UART::stopBits1));
  }
  
  void write(const uint8_t* buf, int length)
  {
    GPIO::DigitalWrite((GPIO::EPin)ControlDe, true);
    GPIO::DigitalWrite((GPIO::EPin)ControlRe, true);
    
//    SYS::DelayMs(2); // 3.5bytes   0.57ms per byte    
    while (length--)
      GPIO::UART::Write(*buf++);
    
//    delay(1); // wait for transmission of last byte - UART::Write waits for finish
//    SYS::DelayMs(2); 
    
    GPIO::DigitalWrite((GPIO::EPin)ControlDe, false);
    GPIO::DigitalWrite((GPIO::EPin)ControlRe, false);        
  }

  bool available()
  {
    return GPIO::UART::Available();
  }

  uint8_t read()
  {
    uint8_t v = GPIO::UART::Read();

//    CONSOLE::Color(RGB565(00ffff));
//    CONSOLE::Print("%02x ", v);
//    CONSOLE::Color(RGB565(ffffff));
    return v;
  }
};
