#include "Bios.h"

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

      bool EndTransmission()
      {
        return true;
      }
    }

    namespace UART
    {
      void Setup(int baudrate, EConfig config)
      {
      }

      bool Available()
      {
        return false;
      }

      uint8_t Read()
      {
        return 0;
      }

      void Write(uint8_t data)
      {
      }
    }
  }
}
