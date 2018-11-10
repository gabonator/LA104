#include <library.h>
#include "../../os_host/source/framework/BufferedIo.h"
#include "shapes.h"

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

uint_fast8_t hex(uint_fast8_t c)
{
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if (c >= 'A' && c <= 'F')
    return c - 'a' + 10;  

  BIOS::DBG::Print("Invalid hex character: '%c'\n", c);
  _ASSERT(0);
  return 0;
}

int _atoi(char* a)
{
  int aux = 0;
  while (*a >= '0' && *a <= '9')
  {
    aux *= 10;
    aux += *a++ - '0';
  }
  return aux;
}

//__attribute__((__section__(".entry"))) int main(void);

class CMidi
{
    CBufferedReader2 mReader;
    int mBytes;
    long mCurrentTime;
    long mCurrentTick;
    long mBaseTime;
    int mDivision;
    int mTickDurationUs{5000};
    int mBpm{120};
    
private:
    static uint16_t fixEndian(uint16_t v)
    {
        return (v << 8) | (v >> 8);
    }
    
    static uint32_t fixEndian(uint32_t v)
    {
        return (v<<24) | ((v<<8) & 0x00ff0000) | ((v>>8) & 0x0000ff00) | (v>>24);
    }

    int getDeltaTime()
    {
        int time = 0;
        int quantity;
        while ((quantity = get()) & 0x80)
        {
            time |= quantity & 0x7f;
            time <<= 7;
        }
        time |= quantity;
        return time;
    }
    
    int get()
    {
        _ASSERT(mBytes > 0);
        mBytes--;
        
        uint8_t value;
        mReader >> value;
        return value;
    }
    
    void send(int v)
    {
        BIOS::GPIO::UART::Write(v);
    }

    struct MThd
    {
        char chunk[4];
        uint32_t length;
        uint16_t format;
        uint16_t tracks;
        uint16_t division;
        
        bool isValid()
        {
            return memcmp(chunk, "MThd", 4) == 0 && length == 6 && (format == 1 || format == 0);
        }
        void fixEndian()
        {
            length = CMidi::fixEndian(length);
            format = CMidi::fixEndian(format);
            tracks = CMidi::fixEndian(tracks);
            division = CMidi::fixEndian(division);
        }
    } __attribute__((packed));
    
    struct MTrk
    {
        char chunk[4];
        uint32_t length;
        void fixEndian()
        {
            length = CMidi::fixEndian(length);
        }
        bool isValid()
        {
            return memcmp(chunk, "MTrk", 4) == 0 && length < 100000;
        }
    } __attribute__((packed));

    void drawInit()
    {
        BIOS::LCD::Clear(RGB565(404040));
        for(int x=0; x<16; x++)
        {
            for (int y=36; y<=72; y++)
                drawKey(x, y, RGB565(808080));
            
            char strChannel[4];
            sprintf(strChannel, "%d", x+1);
            if (strlen(strChannel) == 1)
                BIOS::LCD::Print(30 + x*18-2+4, 184, RGB565(ffffff), RGBTRANS, strChannel);
            else
                BIOS::LCD::Print(30 + x*18-2, 184, RGB565(ffffff), RGBTRANS, strChannel);
        }

        const uint8_t keyMap[13] = ".#.#].#.#.#]";
        
        for (int k=36; k<=72; k++)
        {
            int x = 14;
            int y = 180 - (k-36)*4 - 1;
            CRect rc(x, y, x + 15, y+4);
            //BIOS::LCD::Bar(rc, RGB565(ffffff));

            int ktype = keyMap[k%12];
            if (ktype == '.')
            {
                CRect rc1(x, y, x + 15, y+4);
                BIOS::LCD::Bar(rc1, RGB565(ffffff));
            }
            else if (ktype == ']')
            {
                CRect rc1(x, y+1, x + 15, y+4);
                BIOS::LCD::Bar(rc1, RGB565(ffffff));
                CRect rc2(x, y, x + 15, y+1);
                BIOS::LCD::Bar(rc2, RGB565(b0b0b0));
            }
            else
            {
                CRect rc1(x, y, x + 6, y+4);
                BIOS::LCD::Bar(rc1, RGB565(ffffff));
                CRect rc3(x, y+2, x + 6, y+3);
                BIOS::LCD::Bar(rc3, RGB565(b0b0b0));
                CRect rc2(x+6, y, x + 15, y+4);
                BIOS::LCD::Bar(rc2, RGB565(000000));
            }
        }
    }
    
    void drawKey(int channel, int key, int color)
    {
        if (key >= 48-12 && key <= 72) // C2..C5
        {
            int x = 30 + channel*18;
            int y = 180 - (key-36)*4;
            CRect rc(x, y, x + 15, y+2);
            BIOS::LCD::Bar(rc, color);
        }
    }
    
    void pressKey(int channel, int key)
    {
        drawKey(channel, key, RGB565(ffff00));
    }
    
    void releaseKey(int channel, int key)
    {
        drawKey(channel, key, RGB565(8080b0));
    }
    
    void prepareChannel(int channel)
    {
        for (int y=36; y<=72; y++)
            drawKey(channel, y, RGB565(8080b0));
    }

public:
    bool Open(char* file)
    {
        if (!mReader.Open(file))
            return false;
        
        MThd head;
        mReader >> CStream(&head, sizeof(head));
        
        head.fixEndian();
        if (!head.isValid())
        {
            BIOS::DBG::Print("CMidi: Invalid MThd\n");
            return false;
        }
        
        mDivision = head.division;
        
        MTrk track;
        mReader >> CStream(&track, sizeof(track));
        track.fixEndian();
        if (!track.isValid())
        {
            BIOS::DBG::Print("CMidi: Invalid MTrk\n");
            return false;
        }
        
        mBytes = track.length;
        mBaseTime = BIOS::SYS::GetTick();
        mCurrentTick = 0;
        
        drawInit();
        return true;
    }
    
    
    bool Do()
    {
        if (mBytes <= 0)
            return false;
        
        Wait();
        while ((long)BIOS::SYS::GetTick() - mCurrentTime < 0)
#ifdef _ARM
            ;
#else
            BIOS::KEY::GetKey();
#endif
        Event();
        return true;
    }
    
    void Wait()
    {
        int wait = getDeltaTime();
        mCurrentTick += wait;
        mCurrentTime = mBaseTime + (mCurrentTick * mTickDurationUs)/1000;
    }
    
    void Event()
    {
        int event = get();
        switch (event)
        {
            case 0xff: // meta
            {
                // tempo & time signature
                int meta = get();
                int len = get();
                if (meta == 0x51) // tempo
                {
                    _ASSERT(len == 3);
                    int tempo = get();
                    tempo <<= 8;
                    tempo |= get();
                    tempo <<= 8;
                    tempo |= get();
                    
                    mBpm = 60000000UL / tempo;
                    mTickDurationUs = tempo / mDivision;
                } else
                {
                    for (int i=0; i<len; i++)
                        get();
                }
                break;
            }
            case 0x80 ... 0x8f: // release
            case 0x90 ... 0x9f: // press
            case 0xa0 ... 0xaf: // aftertouch
            {
                int key = get();
                int velocity = get();
                
                send(event);
                send(key);
                send(velocity);
                
                if ((event & 0xf0) == 0x80 || velocity == 0x00)
                    releaseKey(event & 0x0f, key);
                else if ((event & 0xf0) == 0x90)
                    pressKey(event & 0x0f, key);
                break;
            }
            case 0xb0 ... 0xbf: // controller
            {
                send(event);
                send(get()); // cc
                send(get()); // value
                break;
            }
            case 0xc0 ... 0xcf: // patch change
            {
                send(event);
                send(get()); // instrument number
                prepareChannel(event & 0x0f);
                break;
            }
            case 0xe0 ... 0xef: // pitchbend
            {
                send(event);
                send(get()); // lsb
                send(get()); // msb
                break;
            }
            default:
                _ASSERT(0);
        }
    }
    
    void Panic()
    {
        send(0xb0);
        send(0x7b);
    }
};

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{ 
    BIOS::LCD::Clear(RGB565(101010));
    BIOS::DBG::Print("Built " __DATE__ " " __TIME__ "\n");

    char* fileName = BIOS::OS::GetArgument();
    if (strlen(fileName) == 0)
    {
        BIOS::DBG::Print("No file was selected, press F4 to select file\n");
        BIOS::SYS::DelayMs(1500);
        return 1;
    }

    CMidi midi;
    if (!midi.Open(fileName))
    {
        BIOS::DBG::Print("Unable to open source file, exiting\n");
        BIOS::SYS::DelayMs(1500);
        return 1;
    }

    
    CRect rc1(0, 0, 320, 14);
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
    
    char strDisplayName[20];
    if (strlen(fileName)>15)
    {
        strcpy(strDisplayName, "...");
        strcat(strDisplayName, fileName + strlen(fileName)-15);
    } else
    {
        strcpy(strDisplayName, fileName);
    }
    
    int x = 0;
    x += BIOS::LCD::Print(x, 0, RGB565(000000), RGB565(b0b0b0), " MidiPlayer ");
    x += BIOS::LCD::Draw( x, 0, RGB565(b0b0b0), RGBTRANS, CShapes_sel_right);
    x += 8;
    x += BIOS::LCD::Draw( x, 0, RGB565(ffffff), RGBTRANS, CShapes_sel_left);
    x += BIOS::LCD::Print(x, 0, RGB565(000000), RGB565(ffffff), strDisplayName);
    x += BIOS::LCD::Draw( x, 0, RGB565(ffffff), RGBTRANS, CShapes_sel_right);

    
    
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);
    BIOS::GPIO::UART::Setup(31250, BIOS::GPIO::UART::length8);

  BIOS::KEY::EKey key;

  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape) 
  {
      if (!midi.Do())
          break;
  }
    
  midi.Panic();

  BIOS::DBG::Print("Finished playing\n"); 
  BIOS::SYS::DelayMs(1500);
  return 0;
}
