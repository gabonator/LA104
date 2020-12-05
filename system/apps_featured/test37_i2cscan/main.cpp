#include <library.h>
#include "../../os_host/source/framework/BufferedIo.h"

#define BITBANG

using namespace BIOS;

void Draw(int i, int colorBack, int colorText)
{
    int x = i&15;
    int y = i/16;
    CRect rc(x*18+18, 20+16*y+16-4, x*18+16+19, 20+16*y+16+14-4);
    LCD::Bar(rc, colorBack);
    if (i>=4 && i<=7)
      colorText = RGB565(808080);
    LCD::Printf(rc.left+1, rc.top, colorText, colorBack, "%02X", i);
}

void DrawAll()
{
    for (int i=0; i<128; i++)
        Draw(i, RGB565(b0b0b0), RGB565(404040));
}

#ifdef BITBANG
#include "i2c.h"

void InitIo()
{
}

bool Test(int address)
{
    // some devices refuse to communicate when Hs-mode Master was issued on i2c bus (LSM303)
    // probably range 00..07 and 78..7f should not be tested at all, but for debugging 
    // custom implementation of i2c slave devices it could be helpful

    if (address >= 4 && address <= 7)
        return false;

    BIOS::SYS::DelayMs(20);
    return i2c::testPresence(address);
}
#else
void InitIo()
{
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::I2c);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::I2c);
}

bool Test(int address)
{
    if (!BIOS::GPIO::I2C::BeginTransmission(address))
        return false;

    if (!BIOS::GPIO::I2C::EndTransmission())
        return false;

    return true;
}
#endif

void DeinitIo()
{
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Input);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Input);
}

class CTokenizer
{
    char* mStr{nullptr};
    
public:
    CTokenizer(char* str) : mStr(str)
    {
    }

    bool IsEmpty()
    {
        return !mStr || !mStr[0];
    }
    
    char* Get()
    {
        char* delim = strstr(mStr, " ");
        if (delim)
        {
            char* aux = mStr;
            mStr = delim+1;
            *delim = 0;
            return aux;
        }
        char* aux = mStr;
        mStr = nullptr;
        return aux;
    }
};

static int x, y;

void xClear()
{
    x = y = 0;
}

void xPush(char* msg, int c)
{
    LCD::Print(20+x*8, 168+y*14, c, RGBTRANS, msg);
    x += strlen(msg);
}

void xPrint(char* desc, bool highlight = false)
{
    constexpr int countX = 35;
    constexpr int countY = 5;
    constexpr int colorText = RGB565(d0d0d0);
    constexpr int colorHigh = RGB565RGB(39, 101, 217);
    
    CTokenizer tok(desc);
    bool first = true;
    while (!tok.IsEmpty())
    {
        if (first)
            first = false;
        else
            x++;

        char* token = tok.Get();
        int len = strlen(token);
        if (x + len < countX)
        {
            xPush(token, highlight ? colorHigh : colorText);
        } else
        {
            y++;
            if (y >= countY)
                return;
            x = 0;
            xPush(token, highlight ? colorHigh : colorText);
        }
        if (highlight)
            highlight = false;
    }
};

int strncmp( const char * s1, const char * s2, size_t n )
{
    while ( n && *s1 && ( *s1 == *s2 ) )
    {
        ++s1;
        ++s2;
        --n;
    }
    if ( n == 0 )
    {
        return 0;
    }
    else
    {
        return ( *(unsigned char *)s1 - *(unsigned char *)s2 );
    }
}

void GetDescription(int address)
{
    char databasePath[64];
    
    // Get current process path
    strcpy(databasePath, OS::GetArgument());
    char* last = strrchr(databasePath, '/');
    if (last)
        *last = 0;
    else
        strcpy(databasePath, "");

    strcat(databasePath, "/devices.txt");
    
    CBufferedReader f;
    char strLine[128];
    char strCheck[8];
    bool match;

    sprintf(strCheck, "0x%02X", address);
    CRect rcDesc(0, 168, LCD::Width, 168+14*5);

    xClear();
    match = false;

    if (!f.Open((char*)databasePath))
    {
        DBG::Print("Unable to open database file '%s'!\n", databasePath);
        return;
    }

    while (!f.Eof())
    {
        f >> strLine;
        if (match)
        {
            if (strncmp(strLine, "     ", 5) == 0)
            {
                xPush((char*)", ", RGB565(d0d0d0));
                xPrint(strLine+5, true);
            }
            else
                break;
        }
        if (strncmp(strLine, strCheck, 4) == 0)
        {
            GUI::Background(rcDesc, RGB565(404040), RGB565(101010));

            match = true;
            xPush(strCheck, RGB565(ffffff));
            xPush((char*)": ", RGB565(ffffff));
            xPrint(strLine+5, true);
        }
    }
}

uint8_t devices[128];

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    CRect rcClient(0, 0, LCD::Width, LCD::Height);
    GUI::Background(rcClient, RGB565(404040), RGB565(101010));

    CRect rc1(rcClient);
    rc1.bottom = 14;
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
    
    BIOS::LCD::Print(8, 0, RGB565(ffffff), RGBTRANS, "I2C network scanner");
    
    BIOS::KEY::EKey key;
    int nAddress = 0;
    int nLast = -1;
    int lastDesc = -1;
    DrawAll();
    InitIo();
    memset(devices, 0, sizeof(devices));
    
    bool running = true;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
        if (key == KEY::EKey::F1)
            running = !running;
        
        if (Test(nAddress))
        {
            Draw(nAddress, RGB565RGB(39, 101, 217), RGB565(ffffff));
            devices[nAddress] = 1;
            if (nAddress != lastDesc)
            {
                lastDesc = nAddress;
                GetDescription(nAddress);
            }
        } else
        {
            Draw(nAddress, RGB565(404040), RGB565(ffffff));
        }
        
        if (nLast != -1 && nLast != nAddress)
            Draw(nLast, devices[nLast] ? RGB565RGB(39, 101, 217) : RGB565(b0b0b0), RGB565(404040));
        nLast = nAddress;
        
        if (key == KEY::EKey::Left)
        {
            running = false;
            nAddress--;
        }
        
        if (key == KEY::EKey::Right)
        {
            running = false;
            nAddress++;
        }

        if (key == KEY::EKey::Up)
        {
            running = false;
            nAddress -= 16;
        }
        
        if (key == KEY::EKey::Down)
        {
            running = false;
            nAddress += 16;
        }

        if (running)
        {
            nAddress++;
        }

        if (nAddress < 0)
            nAddress += 128;
        if (nAddress >= 128)
            nAddress -= 128;
        devices[nAddress] = 0;
    }
    
    DeinitIo();
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(") [%d]: %s\n", line, cond);
#ifdef __APPLE__
    kill(getpid(), SIGSTOP);
#endif
    while (1);
}

