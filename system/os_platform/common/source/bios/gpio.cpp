#include "Bios.h"
#ifdef LA104
namespace BIOS
{
  namespace GPIO
  {
    void DigitalWrite(EPin pin, bool value)
    {
    }

    bool DigitalRead(EPin pin)
    {
        return false;
    }

    void AnalogWrite(EPin pin, int value)
    {
    }

    int AnalogRead(EPin pin)
    {
      return 0;
    }

    void PinMode(EPin pin, EMode mode)
    {
    }

    namespace I2C
    {
      bool BeginTransmission(uint8_t address)
      {
        return true;
      }

      bool RequestFrom(uint8_t address, uint8_t bytes)
      {
        return true;
      }

      bool Write(uint8_t data)
      {
        return true;
      }

      uint8_t Read()
      {
        return 0;
      }

      bool EndTransmission(bool)
      {
        return true;
      }
    }

    namespace UART
    {
        int fd;
      void Setup(int baudrate, EConfig config)
      {
        gHal->UartSetup(baudrate, config);
      }  
        void Close()
        {
            gHal->UartClose();
        }

      bool Available()
      {
          return gHal->UartAvailable();
      }

      uint8_t Read()
      {
          return gHal->UartRead();
      }

      void Write(uint8_t data)
      {
        gHal->UartWrite(data);
      }
    }
  }
}
#endif
