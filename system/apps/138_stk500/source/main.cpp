#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include "../../../os_host/source/framework/BufferedIo.h"
#include "ihex.h"
#include "gpio.h"

#include "file/shapes.h"
#include "file/layout.h"
#include "file/file.h"

using namespace BIOS;
uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

static int bufferBase = -1;
static uint8_t buffer[128];
//static int bufferLen = 0;

bool HexProcess(uint32_t address, uint8_t* data, int length, bool(*commitHandler)(uint32_t address, uint8_t* data, int length))
{
/*
    int base = address & ~127;
    if (bufferBase != base)
    {
        // commit
        if (bufferLen > 0)
        {
            if (!commitHandler(bufferBase, buffer, bufferLen))
                return false;
            bufferLen = 0;
        }
        bufferBase = base;
    }
    
    for (int i=0; i<length; i++)
    {
        buffer[bufferLen++] = data[i];
        if (bufferLen == 128)
        {
            if (!commitHandler(bufferBase, buffer, bufferLen))
                return false;
            bufferLen = 0;
            bufferBase += 128;
        }
    }
    
    return true;
*/

    int base = address & ~127;
    if (bufferBase != -1 && bufferBase != base)
    {
      // commit
      if (!commitHandler(bufferBase, buffer, 128))
        return false;
      for (int i=0; i<COUNT(buffer); i++)
        buffer[i] = 0xff;
    }
    bufferBase = base;

    // fill
    for (int i=0; i<length; i++)
    {
        buffer[address++ & 127] = data[i];
        if ((address & 127) == 0)
        {
            if (!commitHandler(bufferBase, buffer, 128))
                return false;
            bufferBase += 128;
            for (int i=0; i<COUNT(buffer); i++)
              buffer[i] = 0xff;
        }
    }
    
    return true;

//  CONSOLE::Print("(%04x:%x)", address, length);
/*
  if (length == 0)
    return true;
  return commitHandler(address, data, length);
*/
}

bool HexLoad(char* strFile, bool(*commitHandler)(uint32_t offset, uint8_t* data, int len) )
{
//    bufferLen = 0;
    bufferBase = -1;
    for (int i=0; i<COUNT(buffer); i++)
      buffer[i] = 0xff;

    CBufferedReader fw;
    if ( !fw.Open( strFile ) )
        return false;

    IHexRecord irec;
    uint16_t addressOffset = 0x00;
    uint32_t address = 0x0;
    int ihexError;

    while ((ihexError = Read_IHexRecord(&irec, fw)) == IHEX_OK)
    {
        switch(irec.type)
        {
            case IHEX_TYPE_00:    /**< Data Record */
                address = (((uint32_t) addressOffset) << 16)+ irec.address;
                if (!HexProcess(address, irec.data, irec.dataLen, commitHandler))
                {
                    fw.Close();
                    return false;
                }
            break;

            case IHEX_TYPE_04:    /**< Extended Linear Address Record */
                addressOffset = (((uint16_t) irec.data[0]) << 8 ) + irec.data[1];
                //CONSOLE::Print("adr %08x\n", addressOffset);
            break;

            case IHEX_TYPE_01:    /**< End of File Record */
            case IHEX_TYPE_05:    /**< Start Linear Address Record */
            break;

            case IHEX_TYPE_02:    /**< Extended Segment Address Record */
            case IHEX_TYPE_03:    /**< Start Segment Address Record */
                fw.Close();
            return false;
        }

        if ( irec.type == IHEX_TYPE_01 )
            break;
    }
    
    HexProcess((((uint32_t) -1) << 16), 0, 0, commitHandler);

    fw.Close();
    return true;
}

bool DoFlashing(char* name, uint8_t* signature)
{
    constexpr uint8_t signature84[] =   {0x1e, 0x93, 0x0c};
    constexpr uint8_t signature45[] =   {0x1e, 0x92, 0x06};
    constexpr uint8_t signature328p[] = {0x1e, 0x95, 0x0f};

    CONSOLE::Print("Chip signature: %02x%02x%02x: ", signature[0], signature[1], signature[2]);
    
    if (signature[0] == signature84[0] && signature[1] == signature84[1] && signature[2] == signature84[2])
    {
        CONSOLE::Print("ATTiny84\n"); 
    } else
    if (signature[0] == signature45[0] && signature[1] == signature45[1] && signature[2] == signature45[2])
    {    
        CONSOLE::Print("ATTiny45\n"); 
    } else
    if (signature[0] == signature328p[0] && signature[1] == signature328p[1] && signature[2] == signature328p[2])
    {    
        CONSOLE::Print("ATMega328p\n"); 
    } else
    {
        CONSOLE::Color(RGB565(ff0000));
        CONSOLE::Print("Unknown\n");
        CONSOLE::Color(RGB565(b0b0b0));
    }
    
    CONSOLE::Print("Write:  ");
    bool ok = HexLoad(name, [](uint32_t address, uint8_t* data, int len) -> bool
    {
        address /= 2;
        CONSOLE::Print("W");

        // STK_LOAD_ADDRESS
        GPIO::UART::Write('U');
        GPIO::UART::Write(address & 255); // address in words
        GPIO::UART::Write(address >> 8);
        GPIO::UART::Write(' ');

        // STK_INSYNC
        while (!GPIO::UART::Available());
        if (GPIO::UART::Read() != 0x14)
          return false;
        // STK_OK
        while (!GPIO::UART::Available());
        if (GPIO::UART::Read() != 0x10)
          return false;

        // STK_PROG_PAGE
        GPIO::UART::Write('d');
        GPIO::UART::Write(0);
        GPIO::UART::Write(len); // buffer length in bytes, 128
        GPIO::UART::Write(0x46);
        for (int i=0; i<len; i++)
          GPIO::UART::Write(data[i]);
        GPIO::UART::Write(' ');

        // STK_INSYNC
        while (!GPIO::UART::Available());
        if (GPIO::UART::Read() != 0x14)
          return false;
        // STK_OK
        while (!GPIO::UART::Available());
        if (GPIO::UART::Read() != 0x10)
          return false;
        
        return true;
    });
    
    CONSOLE::Print("\n");

    if (!ok)
      return false;
                   
    CONSOLE::Print("Verify: ");
    ok = HexLoad(name, [](uint32_t address, uint8_t* data, int len) -> bool
    {
        address /= 2;
        CONSOLE::Print("R");

        // STK_LOAD_ADDRESS
        GPIO::UART::Write('U');
        GPIO::UART::Write(address & 255);
        GPIO::UART::Write(address >> 8);
        GPIO::UART::Write(' ');

        // STK_INSYNC
        while (!GPIO::UART::Available());
        if (GPIO::UART::Read() != 0x14)
          return false;
        // STK_OK
        while (!GPIO::UART::Available());
        if (GPIO::UART::Read() != 0x10)
          return false;

        // STK_READ_PAGE
        GPIO::UART::Write('t');
        GPIO::UART::Write(0);
        GPIO::UART::Write(len);
        GPIO::UART::Write(0x46);
        GPIO::UART::Write(' ');

        // STK_INSYNC
        while (!GPIO::UART::Available());
        if (GPIO::UART::Read() != 0x14)
          return false;

        for (int i=0; i<len; i++)
        {
          while (!GPIO::UART::Available());
          if (GPIO::UART::Read() != data[i])
            return false;
        }

        // STK_OK
        while (!GPIO::UART::Available());
        if (GPIO::UART::Read() != 0x10)
          return false;

        return true;
    });
    CONSOLE::Print("\n");

    if (ok)
    {
      // STK_LEAVE_PROGMODE
      GPIO::UART::Write('Q');
      GPIO::UART::Write(' ');
      CONSOLE::Print("Reset to app\n");
    }
    return ok;
}

CFileDialog mFile;
CFileFilterSuffix filter(".HEX");

#ifndef __APPLE__
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
#ifdef __APPLE__
    BIOS::OS::SetArgument((char*)"avr/avrflash.elf");
#endif
    
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);

    APP::Init("STK500");
    APP::Status("Scanning...");
    CONSOLE::Print("Press reset on arduino\nboard to enter bootloader.\n\n\n\n");

    GPIO::PinMode(GPIO::P1, GPIO::Uart);
    GPIO::PinMode(GPIO::P2, GPIO::Uart);

    Gpio::SetState(Gpio::BASEB, Gpio::P3, Gpio::StateInputPull);
    Gpio::SetLevel(Gpio::BASEB, Gpio::P3, 1);

    BIOS::KEY::EKey key;
    int baudrates[] = {57600}; //9600, 19200, 38400, 57600, 115200};
    uint8_t signature[3];
    bool found = false;
//    bool finished = false;
    int32_t inhibit = 0;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
      if (inhibit && BIOS::SYS::GetTick() - inhibit < 5000)
        continue;
/*
      if (finished)
      {
        if (key != BIOS::KEY::None)
        {
          finished = false;
          found = false;
          APP::Status("Scanning...");
        }
        continue;
      }
*/
      if (found)
      {
        if (key == BIOS::KEY::F1)
        {
          if (DoFlashing((char*)"bl100ms.hex", signature))
          {
              CONSOLE::Color(RGB565(ffffff));
              CONSOLE::Print("Flashed successfully!\n");
              //finished = true;
              inhibit = BIOS::SYS::GetTick();
          } else
          {
              CONSOLE::Color(RGB565(ffffff));
              CONSOLE::Print("Flashing failed!\n");
          }
          APP::Status("Done.");
          continue;
        }
        if (key == BIOS::KEY::F3)
        {
          if (DoFlashing((char*)"bl1s.hex", signature))
          {
              CONSOLE::Color(RGB565(ffffff));
              CONSOLE::Print("Flashed successfully!\n");
              //finished = true;
              inhibit = BIOS::SYS::GetTick();
          } else
          {
              CONSOLE::Color(RGB565(ffffff));
              CONSOLE::Print("Flashing failed!\n");
          }
          APP::Status("Done.");
          continue;
        }

        GPIO::UART::Write('u');
        GPIO::UART::Write(' ');
        BIOS::SYS::DelayMs(200);
        if (GPIO::UART::Available()!=5)
          found = false;
        else if (BIOS::GPIO::UART::Read() != 0x14)
          found = false;
        else if (BIOS::GPIO::UART::Read() != signature[0])
          found = false;
        else if (BIOS::GPIO::UART::Read() != signature[1])
          found = false;
        else if (BIOS::GPIO::UART::Read() != signature[2])
          found = false;
        else if (BIOS::GPIO::UART::Read() != 0x10)
          found = false;

        if (!found)
          APP::Status("Connection lost, scanning...");
        while (GPIO::UART::Available())
          GPIO::UART::Read();
      } else {
        for (int i=0; i<COUNT(baudrates); i++)
        {
          // discard input
          while (GPIO::UART::Available())
            GPIO::UART::Read();

          // send STK_READ_SIGN
          GPIO::UART::Setup(baudrates[i], (BIOS::GPIO::UART::EConfig)0);

          GPIO::UART::Write('u');
          GPIO::UART::Write(' ');
          BIOS::SYS::DelayMs(10);
          int avail = GPIO::UART::Available();
/*          if (avail != 0) {
           CONSOLE::Print("%d [", avail);
           while (GPIO::UART::Available())
             CONSOLE::Print("%02x ", BIOS::GPIO::UART::Read());
           CONSOLE::Print("]\n");
           avail = 0;
          }*/
          if (avail==5)
          {
            // 14 [1e 95 0f] 10
            if (BIOS::GPIO::UART::Read() == 0x14)
            {
              for (int j=0; j<3; j++)
                signature[j] = BIOS::GPIO::UART::Read();
              if (BIOS::GPIO::UART::Read() == 0x10)
              {
                char temp[64];
                sprintf(temp, "Found at baudrate %d (%02x %02x %02x)", 
                  baudrates[i], signature[0], signature[1], signature[2]);
                APP::Status(temp);
                found = true;
                break;
              }
            }
          }
        }
      }
    }
/*
    {
        if (mFile.ModalShow(nullptr, "Load firmware", &filter))
        {
            APP::Init("AVRISP");
            APP::Status("ATTiny84, 1MHz internal oscillator");
            
            CONSOLE::Print("Flashing '%s'...\n", mFile.GetFilename());

            CONSOLE::Color(RGB565(b0b0b0));
            if (DoFlashing(mFile.GetFilename()))
            {
                CONSOLE::Color(RGB565(ffffff));
                CONSOLE::Print("Flashed successfully!\n");
            } else
            {
                CONSOLE::Color(RGB565(ffffff));
                CONSOLE::Print("Flashing failed!\n");
            }

            // Wait for key press
            while ((key = BIOS::KEY::GetKey()) == BIOS::KEY::None);
        } else
        {
            break;
        }

    }
*/
    BIOS::FAT::SetSharedBuffer(nullptr);
    return 0;
}


void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

