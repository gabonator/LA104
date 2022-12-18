#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include "../../../os_host/source/framework/BufferedIo.h"
#ifndef __APPLE__
#include "../../../os_host/source/framework/Serialize.cpp"
#endif
#include "gpio.h"
#include "tjpgd.h"

CBufferedWriter mWriter;
uint32_t* mGpioStatus = nullptr;

// https://github.com/sperner/SerialCameras/blob/master/mca25.pl
// http://web.archive.org/web/20130410163851/http://avr.auctionant.de/avr-ip-webcam/mca-25-protokoll.html
// http://web.archive.org/web/20100414054050/http://avr.auctionant.de/avr-ip-webcam/mca-25-protokoll-livepreview.txt
// https://www.programmersought.com/article/13038985632/
// https://github.com/ya-jeks/gsmmux/blob/master/buffer.c
// https://www.mikrocontroller.net/topic/29509?page=single
// http://ww2.cs.lamar.edu/doc/bluetooth/image-pro.pdf

const unsigned char r_crctable[256] = { //reversed, 8-bit, poly=0x07
    0x00, 0x91, 0xE3, 0x72, 0x07, 0x96, 0xE4, 0x75,
    0x0E, 0x9F, 0xED, 0x7C, 0x09, 0x98, 0xEA, 0x7B,
    0x1C, 0x8D, 0xFF, 0x6E, 0x1B, 0x8A, 0xF8, 0x69,
    0x12, 0x83, 0xF1, 0x60, 0x15, 0x84, 0xF6, 0x67,
    0x38, 0xA9, 0xDB, 0x4A, 0x3F, 0xAE, 0xDC, 0x4D,
    0x36, 0xA7, 0xD5, 0x44, 0x31, 0xA0, 0xD2, 0x43,
    0x24, 0xB5, 0xC7, 0x56, 0x23, 0xB2, 0xC0, 0x51,
    0x2A, 0xBB, 0xC9, 0x58, 0x2D, 0xBC, 0xCE, 0x5F,
    0x70, 0xE1, 0x93, 0x02, 0x77, 0xE6, 0x94, 0x05,
    0x7E, 0xEF, 0x9D, 0x0C, 0x79, 0xE8, 0x9A, 0x0B,
    0x6C, 0xFD, 0x8F, 0x1E, 0x6B, 0xFA, 0x88, 0x19,
    0x62, 0xF3, 0x81, 0x10, 0x65, 0xF4, 0x86, 0x17,
    0x48, 0xD9, 0xAB, 0x3A, 0x4F, 0xDE, 0xAC, 0x3D,
    0x46, 0xD7, 0xA5, 0x34, 0x41, 0xD0, 0xA2, 0x33,
    0x54, 0xC5, 0xB7, 0x26, 0x53, 0xC2, 0xB0, 0x21,
    0x5A, 0xCB, 0xB9, 0x28, 0x5D, 0xCC, 0xBE, 0x2F,
    0xE0, 0x71, 0x03, 0x92, 0xE7, 0x76, 0x04, 0x95,
    0xEE, 0x7F, 0x0D, 0x9C, 0xE9, 0x78, 0x0A, 0x9B,
    0xFC, 0x6D, 0x1F, 0x8E, 0xFB, 0x6A, 0x18, 0x89,
    0xF2, 0x63, 0x11, 0x80, 0xF5, 0x64, 0x16, 0x87,
    0xD8, 0x49, 0x3B, 0xAA, 0xDF, 0x4E, 0x3C, 0xAD,
    0xD6, 0x47, 0x35, 0xA4, 0xD1, 0x40, 0x32, 0xA3,
    0xC4, 0x55, 0x27, 0xB6, 0xC3, 0x52, 0x20, 0xB1,
    0xCA, 0x5B, 0x29, 0xB8, 0xCD, 0x5C, 0x2E, 0xBF,
    0x90, 0x01, 0x73, 0xE2, 0x97, 0x06, 0x74, 0xE5,
    0x9E, 0x0F, 0x7D, 0xEC, 0x99, 0x08, 0x7A, 0xEB,
    0x8C, 0x1D, 0x6F, 0xFE, 0x8B, 0x1A, 0x68, 0xF9,
    0x82, 0x13, 0x61, 0xF0, 0x85, 0x14, 0x66, 0xF7,
    0xA8, 0x39, 0x4B, 0xDA, 0xAF, 0x3E, 0x4C, 0xDD,
    0xA6, 0x37, 0x45, 0xD4, 0xA1, 0x30, 0x42, 0xD3,
    0xB4, 0x25, 0x57, 0xC6, 0xB3, 0x22, 0x50, 0xC1,
    0xBA, 0x2B, 0x59, 0xC8, 0xBD, 0x2C, 0x5E, 0xCF };

unsigned char make_fcs(const unsigned char *input, int count)
{
    unsigned char fcs = 0xFF;
    int i;
    for (i = 0; i < count; i++)
        fcs = r_crctable[fcs^input[i]];
    return 0xFF-fcs;
}

class CJpeg
{
  static CJpeg* self;
  JDEC decoder;
  void (*requestFrame)(uint8_t**, int*);
  uint8_t* frameBuffer{nullptr};
    int frameBytes{0};
public:
  uint8_t tjpg_work[TJPGD_WORKSPACE_SIZE];
private:
  
  static size_t tjpgd_data_reader(JDEC *decoder, uint8_t *buffer, size_t size)
  {
      uint8_t* rbuffer = buffer;

      size_t rsize = size;
      if (self->frameBytes > 0 && self->frameBytes < size)
      {
          _ASSERT(self->frameBuffer);
          if (buffer)
              memcpy(buffer, self->frameBuffer, self->frameBytes);
         size -= self->frameBytes;
         buffer += self->frameBytes;
         self->frameBytes = 0;
      }
      if (self->frameBytes == 0)
      {
          self->requestFrame(&self->frameBuffer, &self->frameBytes);
      }
      if (self->frameBytes >= size)
      {
         if (buffer)
             memcpy(buffer, self->frameBuffer, size);
         self->frameBuffer += size;
         self->frameBytes -= size;
      }
      return rsize;
  }

  static int tjpgd_data_writer(JDEC* decoder, void* bitmap, JRECT* rectangle)
  {
      CRect rc(rectangle->left, rectangle->top, rectangle->right+1, rectangle->bottom+1);
      rc.Offset(0, 0);

      CRect rcScreen(0, 0, BIOS::LCD::Width, BIOS::LCD::Height);
      if (rc.right > rcScreen.right || rc.bottom > rcScreen.bottom)
          return 1;
      if (rc.left < 0 || rc.top < 14)
          return 1;

      BIOS::LCD::BufferBegin(rc);
      BIOS::LCD::BufferWrite((uint16_t*)bitmap, rc.Width()*rc.Height());
      BIOS::LCD::BufferEnd();
      return 1;
  }
  public:
  void Render(void (*req)(uint8_t**, int*))
  {
      frameBuffer = nullptr;
      frameBytes = 0;

    self = this;
    requestFrame = req;
    decoder.swap = true;
    JRESULT result;
    result = jd_prepare(&decoder, tjpgd_data_reader, tjpg_work, sizeof(tjpg_work), 0);
      if (result != JDR_OK)
      {
          CONSOLE::Print("jd_prepare error %d\n", result);
          return;
      }
    _ASSERT (result == JDR_OK);
    result = jd_decomp(&decoder, tjpgd_data_writer, 0);
    _ASSERT (result == JDR_OK);
    requestFrame = nullptr;
  }
};

CJpeg jpeg;
CJpeg* CJpeg::self{nullptr};

class CMca25
{
    char buffer[256];
    char buffermux[64];
    int readMuxBytes{0};
    int bufferi{0};
    uint8_t extrabuf[512];

public:
    char* Process()
    {
        if (!BIOS::GPIO::UART::Available())
            return nullptr;
        char c = BIOS::GPIO::UART::Read();

        if (bufferi < 30)
        {
            if (c == 0x0d)
            {
                buffer[bufferi] = 0;
                bufferi = 0;
                return buffer;
            } else {
                buffer[bufferi++] = c;
            }
        } else {
            if (c == 0x0d)
                bufferi = 0;
        }
        return nullptr;
    }

    bool Init()
    {
        mGpioStatus = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioStatus);

        BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
        BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);
        BIOS::GPIO::UART::Setup(9600, (BIOS::GPIO::UART::EConfig)0);

        APP::Status("AT mode @ 9600 bps");

        Gpio::SetState(Gpio::BASEB, Gpio::P3, Gpio::StateOutput10Mhz | Gpio::StateOutputPushPull);
        Gpio::SetLevel(Gpio::BASEB, Gpio::P3, 0);
        BIOS::SYS::DelayMs(5);

        while (BIOS::GPIO::UART::Available())
            BIOS::GPIO::UART::Read();

        BIOS::SYS::DelayMs(5);
        Gpio::SetLevel(Gpio::BASEB, Gpio::P3, 1);
        CONSOLE::Print("Camera init...\n");
        if (!EnterCmux())
        {
            CONSOLE::Print("Failed! Check your wiring:\n");
            CONSOLE::Print("  Camera        LA104\n");           
            CONSOLE::Print("  1             \n");  
            CONSOLE::Print("  2             \n");  
            CONSOLE::Print("  3             \n");  
            CONSOLE::Print("  4 tx          P2 - rx\n");  
            CONSOLE::Print("  5 rx          P1 - tx\n");  
            CONSOLE::Print("  6             \n");  
            CONSOLE::Print("  7 acc. ctl    P3\n");
            CONSOLE::Print("  8             \n");  
            CONSOLE::Print("  (9)           \n");  
            CONSOLE::Print("  10 gnd        GND\n");  
            CONSOLE::Print("  11 vcc        3.3V\n");  

            Gpio::SetLevel(Gpio::BASEB, Gpio::P3, 0);
            return false;
        }
        CONSOLE::Print("Ok!\n");
        CONSOLE::Print("Init cmux... ");
        if (!InitCmux())
        {
            CONSOLE::Print("Failed!\n");
            Gpio::SetLevel(Gpio::BASEB, Gpio::P3, 0);
            return false;
        }
        CONSOLE::Print("Ok!\n");
        CONSOLE::Print("Configure camera... ");
        if (!Configure())
        {
            CONSOLE::Print("Failed!\n");
            Gpio::SetLevel(Gpio::BASEB, Gpio::P3, 0);
            return false;
        }
        CONSOLE::Print("Ok!\n");
        CRect preview(BIOS::LCD::Width-60, BIOS::LCD::Height-80, BIOS::LCD::Width, BIOS::LCD::Height);
        BIOS::KEY::EKey key;
        bool pause = false;
        bool save = false;
        int index = GetFreeIndex();
        while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
        {
            if (key == BIOS::KEY::F4)
              save = !save;
            if (key == BIOS::KEY::F3)
              pause = !pause;
            if (pause)
            {
              Flush();
              continue;
            }

            APP::Status("Preview 80x60px 8bpp");
            for (int i=0; i<10; i++)
                if (!GrabPreview())
                {
                    CONSOLE::Print("#");
                    Flush();
                    BIOS::LCD::BufferBegin(preview);
                }
            APP::Status("Transfer 320x240 jpeg");
            DisplayJpeg();
            if (save)
            {
              char name[16];
              char temp[32];
              sprintf(name, "img%03d.jpg", index++);
              sprintf(temp, "Saving %s...", name);
              APP::Status(temp);
              if (!GrabJpeg(name))
              {
                APP::Status("Saving Failed!");
                BIOS::SYS::DelayMs(200);
              }
            }
        }
        Gpio::SetLevel(Gpio::BASEB, Gpio::P3, 0);
        return true;
    }

    int GetFreeIndex()
    {
        for (int i=0; i<99; i++)
        {
            char test[16];
            sprintf(test, "img%03d.jpg", i);
            if (!Exists(test))
                return i;
        }
        _ASSERT(!"Too many files");
        return 100;
    }

    bool Exists(char* fileName)
    {
        if (BIOS::FAT::Open(fileName, BIOS::FAT::EIoMode::IoRead) != BIOS::FAT::EResult::EOk)
            return false;
        
        BIOS::FAT::Close();
        return true;
    }

    void Flush()
    {
        int32_t t0 = BIOS::SYS::GetTick();
        while (BIOS::SYS::GetTick() - t0 < 100)
        {
            if (BIOS::GPIO::UART::Available())
                t0 = BIOS::SYS::GetTick();
            while(BIOS::GPIO::UART::Available())
                BIOS::GPIO::UART::Read();
        }
    }
    
    void _Send(const char* buf, size_t len)
    {
        while (len--)
            BIOS::GPIO::UART::Write(*buf++);
    }

    void Check(const char* msg)
    {
        return;
        if (*mGpioStatus != 0)
        {
            CONSOLE::Print("%s gpio state: %x\n", msg, *mGpioStatus);
            *mGpioStatus = 0;
        }
    }
    const char* ReadMux(bool check = true, bool quiet = false)
    {
        bool marked = false;
        Check("Pre read");
        readMuxBytes = 0;
        int32_t t0 = BIOS::SYS::GetTick();
        memset(buffer, 0, sizeof(buffer));
        for(int cnt=0; cnt < sizeof(buffer); cnt++)
        {
            if (cnt == sizeof(buffer)-1)
            {
                CONSOLE::Print("Small buffer!");
                return "";
            }
            while (!BIOS::GPIO::UART::Available())
            {
                if (BIOS::SYS::GetTick()-t0 > 400)
                {
                    if (!quiet)
                    {
                        CONSOLE::Print("Timeout!");
                        Check("Tmo");
                    }
                    return "";
                }
            }
            uint8_t rd = BIOS::GPIO::UART::Read();
            buffer[cnt] = rd;

            if (cnt > 3 && cnt >= 5 + (buffer[3] >> 1))
            {
                if (rd != 0xf9)
                {
                    //CONSOLE::Print("*"); // TODO: fixme!
                    while (!BIOS::GPIO::UART::Available());
                    rd = BIOS::GPIO::UART::Read();
                    _ASSERT(rd == 0xf9);
                    buffer[++cnt] = rd;
                }
                readMuxBytes = cnt;
                break;
            }

            if (cnt==0 && buffer[cnt] != '\xF9')
            {
                CONSOLE::Print("Wrong start! (%02x)", rd);
                for (int i=0; i<8; i++)
                {
                    while (!BIOS::GPIO::UART::Available());
                    CONSOLE::Print("%02x ", BIOS::GPIO::UART::Read());
                }
                return "";
            }
        }
        if (BIOS::GPIO::UART::Available() && check)
        {
            CONSOLE::Print("Not drained!");
            Check("Drn");
            return "";
        }
        Check("Post read");
        return buffer;
    }

    #define Send(str) _Send(str, sizeof(str)-1)
    #define CheckMux(str) (memcmp(ReadMux(), str, sizeof(str)-1) == 0)
    #define Send2(addr, control, str) _Send(BuildMux(addr, control, str, sizeof(str)-1), sizeof(str)-1+6)
    #define CheckMux2(addr, control, str) (memcmp(ReadMux(), BuildMux(addr, control, str, sizeof(str)-1), sizeof(str)-1+6) == 0)
    #define CheckMux2P(addr, control, str) (memcmp(ReadMux(), BuildMux(addr, control, str, sizeof(str)-1, 1), sizeof(str)-1+6) == 0)

    char* BuildMux(int addr, int control, const char* payload, int len, int ext = 0)
    {
        buffermux[0] = 0xf9;
        buffermux[1] = addr;
        buffermux[2] = control;
        buffermux[3] = ((len-ext)<<1) | 1;
        memcpy(buffermux+4, payload, len);
        buffermux[4+len] = make_fcs((uint8_t*)buffermux+1, 3);
        buffermux[5+len] = 0xf9;
        return buffermux;
    }

    bool EnterCmux()
    {
        int32_t t0 = BIOS::SYS::GetTick();
        while (BIOS::SYS::GetTick() - t0 < 5000)
        {
            char* msg = Process();
            if (!msg)
                continue;

            if (strcmp(msg, "AT&F") == 0)
                Send("OK\r\n");
            if (strcmp(msg, "AT+IPR=?") == 0)
                Send("+IPR: (),(1200,2400,4800,9600,19200,38400,57600,115200,460800)\r\nOK\r\n");
            if (strcmp(msg, "AT+IPR=460800") == 0)
            {
                Send("\r\nOK\r\n");
                CONSOLE::Print("Switching to 460.8 kbps\n");
                BIOS::GPIO::UART::Setup(460800, (BIOS::GPIO::UART::EConfig)0);
                APP::Status("AT mode @ 460800 bps");
            }
            if (strcmp(msg, "AT+CMUX=?") == 0)
                Send("\r\n+CMUX: (0),(0),(1-7),(31),(10),(3),(30),(10),(1-7)\r\nOK\r\n");
            if (strcmp(msg, "AT+CMUX=0,0,7,31") == 0)
            {
                Send("OK\r\n");
                return true;
            }
        }
        CONSOLE::Print("Timeout!\n");
        return false;
    }

    bool InitCmux()
    {
        APP::Status("MUX mode @ 460800 bps");
        if (!CheckMux2(0x03, 0x3F, ""))
            return false;
        Send2(0x03, 0x73, "");
        if (!CheckMux2(0x23, 0x3F, ""))
            return false;
        Send2(0x23, 0x73, "");
        if (!CheckMux2(0x03, 0xEF, "\xE3\x05\x23\x8D"))
            return false;
        Send2(0x01, 0xEF, "\xE3\x07\x23\x0C\x01");
        if (!CheckMux2P(0x03, 0xEF, "\xE1\x07\x23\x0C\x01"))
            return false;
        Send2(0x01, 0xEF, "\xE1\x05\x23\x8D");
        if (!CheckMux2(0x23, 0xEF, "AT*EACS=17,1\r"))
            return false;
        Send2(0x21, 0xEF, "\r\nOK\r\n");
        if (!CheckMux2(0x23, 0xEF, "AT+CSCC=1,199\r"))
            return false;
        Send2(0x21, 0xEF, "\r\n+CSCC: E3\r\n");
        Send2(0x21, 0xEF, "\r\nOK\r\n");
        if (!CheckMux2(0x23, 0xEF, "AT+CSCC=2,199,B9\r"))
            return false;
        Send2(0x21, 0xEF, "\r\nOK\r\n");
        Send2(0x81, 0x3F, "");
        if (!CheckMux2(0x81, 0x73, ""))
            return false;
        if (!CheckMux2(0x03, 0xEF, "\xE3\x05\x83\x8D"))
            return false;
        Send2(0x01, 0xEF, "\xE1\x05\x83\x8D");
        Send2(0x81, 0xEF, "\x80\x00\x1A\x10\x00\x02\x00\x46\x00\x13\xE3\x3D\x95\x45\x83\x74\x4A\xD7\x9E\xC5\xC1\x6B\xE3\x1E\xDE\x8E\x61");
        Send2(0x21, 0xEF, "\r\nOK\r\n");
        if (!CheckMux2(0x83, 0xEF, "\xA0\x00\x1F\x10\x00\x20\x00\xCB\x00"
                       "\x00\x00\x01\x4A\x00\x13\xE3\x3D\x95\x45\x83\x74\x4A"
                       "\xD7\x9E\xC5\xC1\x6B\xE3\x1E\xDE\x8E"))
            return false;
        return true;
    }

    bool Configure()
    {
        // power consumption request
        if (!CheckMux2(0x23, 0xEF, "AT*ECUR=41\r"))
            return false;
        Send2(0x21, 0xEF, "\r\nOK\r\n");
        Send2(0x81, 0xef, "\x82\x01\x3b\x01\x00\x03\x49\x01\x35" "<camera-settings versi");
        Send2(0x81, 0xef, "on=\"1.0\" white-balance=\"OFF\" co");
        Send2(0x81, 0xef, "lor-compensation=\"13\" fun-layer");
        Send2(0x81, 0xef, "=\"0\"><monitoring-format encodin");
        Send2(0x81, 0xef, "g=\"EBMP\" pixel-size=\"80*60\" col");
        Send2(0x81, 0xef, "or-depth=\"8\"/>\r\n<thumbnail-form");
        Send2(0x81, 0xef, "at encoding=\"EBMP\" pixel-size=\"");
        Send2(0x81, 0xef, "101*80\" color-depth=\"8\"/>\r\n<nat");
        Send2(0x81, 0xef, "ive-format encoding=\"\" pixel-si");
        Send2(0x81, 0xef, "ze=\"160*120\"/>\r\n</camera-settin");
        Send2(0x81, 0xef, "gs>\r\n");
        if (!CheckMux2(0x83, 0xEF, "\xA0\x00\x03"))
            return false;
        Send2(0x81, 0xEF, "\x83\x00\x17\x42\x00\x14" "x-bt/camera-info" "\x00");
        // Wait for the first packet
        ReadMux(false);
        BIOS::SYS::DelayMs(10);

        while (BIOS::GPIO::UART::Available())
        {
            ReadMux(false);
            BIOS::SYS::DelayMs(10);
        }
        // returns camera info XML terminated with "</camera-info>"
        return true;
    }
    
    void SendAck()
    {
        Send2(0x81, 0xEF, "\x83\x00\x03");
    }

    template <typename T> bool Transfer(int length, T handler)
    {
        // TODO: join!
        if (length == 506)
        {
            int extrabufi = 0;
            while (extrabufi < 512)
            {
                ReadMux(false, true);
                if (readMuxBytes == 0)
                {
                    break;
                }
                if (buffer[1] != 0x83 || buffer[2] != 0xef)
                {
                    CONSOLE::Print("e2!");
                    return false;
                }
                int packetLen = buffer[3] >> 1;
                for (int i=0; i<packetLen; i++)
                    extrabuf[extrabufi++] = (uint8_t)buffer[4+i];
            }
            handler(extrabuf+6, extrabufi-6, 506);
            return true;
        }
        int extrabufi = 0;
        int readBytes = 0;
        int realBytes = 0;
        int sentBytes = 0;

        while (length <= 0 || realBytes < length)
        {
            ReadMux(false, true);
            if (readMuxBytes == 0)
            {
                //CONSOLE::Print("tmo");
                // timeout
                if (extrabufi != 0)
                    handler(extrabuf+6, extrabufi-6, sentBytes);
                return true;
            }
            if (buffer[1] != 0x83 || buffer[2] != 0xef)
            {
                return false;
            }

            int packetLen = buffer[3] >> 1;
            for (int i=0; i<packetLen; i++)
                extrabuf[extrabufi++] = (uint8_t)buffer[4+i];

            readBytes += packetLen;
            if ((readBytes & 511) == 0)
            {
                handler(extrabuf+6, extrabufi-6, sentBytes);
                sentBytes += extrabufi-6;
                extrabufi = 0;
                realBytes -= 6;
                _ASSERT(!BIOS::GPIO::UART::Available());
                SendAck(); //F9 81 EF 07 83 00 03 A6 F9
            } else
            if (length > 0 && realBytes + packetLen >= length)
            {
                // TODO: -6 ???
                _ASSERT(realBytes + packetLen-6 == length);
                _ASSERT(extrabufi > 6);
                // last packet, finished
                handler(extrabuf+6, extrabufi-6, sentBytes);
            }
            realBytes += packetLen;
        }
        return true;
    }
    
    bool GrabPreview()
    {
        auto PushPixel = [](uint8_t d) {
            int r = ((d & 0b11100000) >> 5) << 5;
            int g = ((d & 0b00011100) >> 2) << 5;
            int b = ((d & 0b00000011) >> 0) << 6;
            uint16_t c = RGB565RGB(r, g, b);
            BIOS::LCD::BufferWrite(c);
        };

        // Everything should be drained
        _ASSERT(!BIOS::GPIO::UART::Available());
        Send2(0x81, 0xef, "\x83\x00iq\x00?<monitoring-command versi");
        Send2(0x81, 0xef, "on=\"1.0\" take-pic=\"YES\" zoom=\"1");
        Send2(0x81, 0xef, "0\"/>B\x00!x-bt/imaging-monitoring-");
        Send2(0x81, 0xef, "image\x00\x4c\x00\x06\x06\x01\x80");

        while (1)
        {
            if (Transfer(80*60+20, [&](uint8_t* buf, int len, int ofs)
            {
              for (int i=0; i<len; i++)
              {
                if (ofs+i-20 < 0)
                  continue;
                if ((ofs+i-20)%80 == 0)
                {
                  int row = (ofs+i-20)/80;
                  BIOS::LCD::BufferBegin(CRect(BIOS::LCD::Width-80, BIOS::LCD::Height-60-14+row,
                    BIOS::LCD::Width, BIOS::LCD::Height-60-14+row+1));
                }
                PushPixel(buf[i]);
                if ((ofs+i-20)%80 == 79)
                  BIOS::LCD::BufferEnd();
              }
            }))
            {
                break;
            }
            if (memcmp(buffer, BuildMux(0x23, 0xef, "AT*ECUR=1000\r", sizeof("AT*ECUR=1000\r")-1), sizeof("AT*ECUR=1000\r")-1+6) == 0)
                continue;
            else
                break;
        }
        _ASSERT(!BIOS::GPIO::UART::Available())
        return true;
    }
    
    bool GrabJpeg(const char* name)
    {
        _ASSERT(sizeof(jpeg.tjpg_work) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
        BIOS::FAT::SetSharedBuffer(jpeg.tjpg_work);
        mWriter.Open((char*)name);

        Send2(0x81, 0xef, "\x83\x00\x82q\x00X<monitoring-command versi");
        Send2(0x81, 0xef, "on=\"1.0\" take-pic=\"NO\" send-pix");
        Send2(0x81, 0xef, "el-size=\"640*480\" zoom=\"10\"/>B\x00");
        Send2(0x81, 0xef, "!x-bt/imaging-monitoring-image\x00");
        Send2(0x81, 0xef, "L\x00\x06\x06\x01\x80");
        bool ok = Transfer(0, [](uint8_t* buf, int len, int ofs)
        {
          mWriter << CStream(buf, len);
        });
        mWriter.Close();
        BIOS::FAT::SetSharedBuffer(nullptr);
        return ok;
    }

    void DisplayJpeg()
    {
        Send2(0x81, 0xef, "\x83\x00\x82q\x00X<monitoring-command versi");
        Send2(0x81, 0xef, "on=\"1.0\" take-pic=\"NO\" send-pix");
        Send2(0x81, 0xef, "el-size=\"320*240\" zoom=\"10\"/>B\x00");
        Send2(0x81, 0xef, "!x-bt/imaging-monitoring-image\x00");
        Send2(0x81, 0xef, "L\x00\x06\x06\x01\x80");

        static uint8_t* tempBuf;
        static int tempLen;
        static CMca25* self;
        bool ok = Transfer(506, [](uint8_t* buf, int len, int ofs) {
          tempBuf = buf;
          tempLen = len;
        });
        _ASSERT(ok);
        static bool first;
        self = this;
        first = true;
        jpeg.Render([](uint8_t** buf, int* len)
        {
          if (!first)
          {
            // transfer single 512 chunk, should end with ACK, we have
            // plenty time to decode jpeg stream
            self->SendAck();
            bool ok = self->Transfer(506, [](uint8_t* buf, int len, int ofs) {
              tempBuf = buf;
              tempLen = len;
            });
            _ASSERT(ok);
          } else {
            first = false;
          }
          *buf = tempBuf;
          *len = tempLen;
        });
        Send2(0x01, 0xEF, "\xE3\x07\x23\x0C\x01");
        if (!CheckMux2P(0x03, 0xEF, "\xe1\x07\x23\x0c\x01"))
        {
            _ASSERT(0);
            return;
        }
        
        _ASSERT (!BIOS::GPIO::UART::Available());
    }
};

CMca25 camera;
#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    APP::Init("MCA25 Camera test");
    APP::Status("");
#ifdef __APPLE__
    jpeg.Render([](uint8_t** buf, int* len)
    {
        static FILE* f = nullptr;
        static uint8_t buffer[512];
        if (!f)
            f = fopen("/Users/gabrielvalky/Documents/git/LA104/system/apps_experiments/131_mca25/_temp/4/MCA25A.JPG", "rb");
        fread(buffer, 506, 1, f);
        *buf = buffer;
        *len = 506;
    });
#else

    camera.Init();
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Input);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Input);
#endif
    CONSOLE::Print("Press any key to exit...\n");
    while (!BIOS::KEY::GetKey());
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
    while (1);
}
