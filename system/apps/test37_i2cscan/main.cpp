#include <library.h>
#include "../../os_host/source/framework/BufferedIo.h"
using namespace BIOS;

void Draw(int i, int color)
{
    int x = i&15;
    int y = i/16;
    CRect rc(x*18+18, 20+16*y+16, x*18+16+19, 20+16*y+16+14);
    LCD::Bar(rc, color);
    LCD::Printf(rc.left, rc.top, RGB565(404040), color, "%02X", i);
}

void DrawAll()
{
    for (int i=0; i<128; i++)
        Draw(i, RGB565(b0b0b0));
}

void InitIo()
{
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::I2c);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::I2c);
}

bool Test(int address)
{
#ifdef __APPLE__
    SYS::DelayMs(30);
    if (address == 0x1d)
        return true;
    return false;
#endif

      bool ok = BIOS::GPIO::I2C::BeginTransmission(address);
      ok &= BIOS::GPIO::I2C::EndTransmission();

      return ok;
}

CBufferedReader f;
char strLine[128];
char strCheck[8];

static int x, y;
bool match;
void xPrint(char* desc)
{
    if (x + strlen(desc) > 40)
    {
        x = 0;
        y ++;
    }
    if (y < 5)
    {
        LCD::Print(20+x*8, 168+y*14, RGB565(d0d0d0), RGBTRANS, desc);
        x += strlen(desc);
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

    sprintf(strCheck, "0x%02X", address);
    CRect rcDesc(20, 168, LCD::Width-20, 168+14*5);

    x = y = 0;
    match = false;
    /*
    static int lines;
    static int x, y;
    lines = 0;
    x = 0;
    y = 0;
    bool match = false;
    

    auto xPrint = [](char* desc)
    {
        if (x + strlen(desc) > 40)
        {
            x = 0;
            y ++;
        }
        if (y < 5)
        {
            LCD::Print(20+x*8, 168+y*14, RGB565(d0d0d0), RGBTRANS, desc);
            x += strlen(desc);
        }
    };
     CBufferedReader f;
    */

#ifdef __APPLE__
    if (!f.Open((char*)"/Users/gabrielvalky/Documents/git/LA104/system/apps/test37_i2cscan/devices.txt"))
        return;
#else
    if (!f.Open((char*)"devices.txt"))
        return;
#endif
    while (!f.Eof())
    {
        f >> strLine;
        if (match)
        {
            if (strncmp(strLine, "     ", 5) == 0)
                xPrint(strLine+5);
            else
                break;
        }
        if (strncmp(strLine, strCheck, 4) == 0)
        {
            GUI::Background(rcDesc, RGB565(404040), RGB565(101010));

            match = true;
            xPrint(strCheck);
            xPrint((char*)": ");
            xPrint(strLine+5);
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
    DrawAll();
    InitIo();
    memset(devices, 0, sizeof(devices));
    
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
        if (Test(nAddress))
        {
            Draw(nAddress, RGB565(00ff00));
            devices[nAddress] = 1;
            GetDescription(nAddress);
        } else
        {
            Draw(nAddress, RGB565(00b000));
        }
        if (nLast != -1)
            Draw(nLast, devices[nLast] ? RGB565(00b0b0) : RGB565(b0b0b0));
        nLast = nAddress;
        
        nAddress++;
        if (nAddress >= 128)
            nAddress = 0;
        devices[nAddress] = 0;
    }
    
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

