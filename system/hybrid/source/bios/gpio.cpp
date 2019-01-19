#include "Bios.h"

#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>

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
        int fd;
      void Setup(int baudrate, EConfig config)
      {
          // Open port
          fd = open("/dev/tty.usbmodemLA104CDC", O_RDWR | O_NOCTTY | O_NDELAY);
          if (fd == -1){
              printf("Device cannot be opened.\n");
              exit(-1);                       // If the device is not open, return -1
          }
          struct termios options;
          
          fcntl(fd, F_SETFL, FNDELAY);                    // Open the device in nonblocking mode
          
          // Set parameters
          tcgetattr(fd, &options);                        // Get the current options of the port
          bzero(&options, sizeof(options));               // Clear all the options
          speed_t         Speed;
          switch (baudrate)                               // Set the speed (baudRate)
          {
              case 110  :     Speed=B110; break;
              case 300  :     Speed=B300; break;
              case 600  :     Speed=B600; break;
              case 1200 :     Speed=B1200; break;
              case 2400 :     Speed=B2400; break;
              case 4800 :     Speed=B4800; break;
              case 9600 :     Speed=B9600; break;
              case 19200 :    Speed=B19200; break;
              case 38400 :    Speed=B38400; break;
              case 57600 :    Speed=B57600; break;
              case 115200 :   Speed=B115200; break;
              default : exit(-4);
          }
          cfsetispeed(&options, Speed);                   // Set the baud rate at 115200 bauds
          cfsetospeed(&options, Speed);
          options.c_cflag |= ( CLOCAL | CREAD |  CS8);    // Configure the device : 8 bits, no parity, no control
          options.c_iflag |= ( IGNPAR | IGNBRK );
          options.c_cc[VTIME]=0;                          // Timer unused
          options.c_cc[VMIN]=0;                           // At least on character before satisfy reading
          tcsetattr(fd, TCSANOW, &options);               // Activate the settings
      }
        
        void Close()
        {
            close(fd);
        }

      bool Available()
      {
          int count;
          ioctl(fd, FIONREAD, &count);
        return count>0;
      }

      uint8_t Read()
      {
          uint8_t data;
          _ASSERT(read(fd, &data, 1) == 1);
          return data;
      }

      void Write(uint8_t data)
      {
          _ASSERT(write(fd, &data, 1) == 1);
      }
    }
  }
}
