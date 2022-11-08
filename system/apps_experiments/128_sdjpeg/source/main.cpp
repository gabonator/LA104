#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "sd.h"
#include "tjpgd.h"

uint8_t sector[512];
uint8_t sector2[512];

JDEC decoder;
uint8_t tjpg_work[TJPGD_WORKSPACE_SIZE];
CFat32::direntry_t tjpg_direntry;
int tjpg_offset = 0;
int tjpg_x = 0;
int tjpg_y = 14;
int tjpg_scale = 0;
bool tjpg_exit = false;

CSpi spi;
CSd sd(spi);
CFat32 fat(sd, sector, sector2);

int _min(int a, int b)
{
    return a<b?a:b;
}

size_t tjpgd_data_reader(JDEC *decoder, uint8_t *buffer, size_t size)
{
    if (tjpg_direntry.readCluster == (uint32_t)-1)
        fat.ReadFile(tjpg_direntry);

    int rsize = size;
    int baseCurrent = (tjpg_direntry.readSectors-1) * fat.mBpb.BytesPerSector;
    int baseRequest = tjpg_offset & ~511;
    _ASSERT(baseCurrent == baseRequest);
    while (size > 0)
    {
        int ofsRequest = tjpg_offset & 511;
        int toRead = _min(512-ofsRequest, size);
        if (buffer)
            memcpy(buffer, sector+ofsRequest, toRead);
        
        size -= toRead;
        buffer += toRead;
        tjpg_offset += toRead;
        if ((tjpg_offset & 511) == 0)
        {
            fat.ReadFile(tjpg_direntry);
            baseCurrent += 512;
        }
    }
    return rsize;
}

int tjpgd_data_writer(JDEC* decoder, void* bitmap, JRECT* rectangle)
{
    switch (BIOS::KEY::GetKey())
    {
        case BIOS::KEY::Escape: tjpg_exit = true; return 0;
        case BIOS::KEY::Up: tjpg_y -= 16; break;
        case BIOS::KEY::Down: tjpg_y += 16; break;
        case BIOS::KEY::Left: tjpg_x -= 16; break;
        case BIOS::KEY::Right: tjpg_x += 16; break;
        default: break;
    }

    CRect rc(rectangle->left, rectangle->top, rectangle->right+1, rectangle->bottom+1);
    rc.Offset(tjpg_x, tjpg_y);
    CRect rcScreen(0, 0, BIOS::LCD::Width, BIOS::LCD::Height);
    if (rc.top >= rcScreen.bottom-14)
        return 0;
    if (rc.right > rcScreen.right || rc.bottom > rcScreen.bottom)
        return 1;
    if (rc.left < 0 || rc.top < 0)
        return 1;

    BIOS::LCD::BufferBegin(rc);
    BIOS::LCD::BufferWrite((uint16_t*)bitmap, rc.Width()*rc.Height());
    BIOS::LCD::BufferEnd();
    return 1;
}

void RenderJpeg(const CFat32::direntry_t& direntry)
{
    decoder.swap = true;

    BIOS::KEY::EKey key;
    bool first = true;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape && !tjpg_exit)
    {
        switch (key)
        {
            case BIOS::KEY::Up: tjpg_y -= 16; break;
            case BIOS::KEY::Down: tjpg_y += 16; break;
            case BIOS::KEY::Left: tjpg_x -= 16; break;
            case BIOS::KEY::Right: tjpg_x += 16; break;
            case BIOS::KEY::Enter: tjpg_scale = (tjpg_scale+1)%3; break;
            case BIOS::KEY::None:
                if (first)
                    first = false;
                else
                    continue;
                break;
            default:
                return;
        }

        tjpg_direntry.FstClusHI = direntry.FstClusHI;
        tjpg_direntry.FstClusLO = direntry.FstClusLO;
        tjpg_direntry.FileSize = direntry.FileSize;
        tjpg_direntry.readSectors = 0;
        tjpg_direntry.readCluster = -1;
        tjpg_offset = 0;
     

        JRESULT result;
        result = jd_prepare(&decoder, tjpgd_data_reader, tjpg_work, sizeof(tjpg_work), 0);

        if (tjpg_x==0 && decoder.width < 320)
          tjpg_x = (320-decoder.width)/2;

        if (tjpg_x + (decoder.width<<tjpg_scale) < BIOS::LCD::Width)
            BIOS::LCD::Bar(tjpg_x + decoder.width, 14, BIOS::LCD::Width, BIOS::LCD::Height, 0);
        if (tjpg_y + (decoder.height<<tjpg_scale) < BIOS::LCD::Height)
            BIOS::LCD::Bar(0, tjpg_y + decoder.height, BIOS::LCD::Width, BIOS::LCD::Height-14, 0);
        if (tjpg_x > 0)
            BIOS::LCD::Bar(0, 14, tjpg_x, BIOS::LCD::Height-14, 0);
        if (tjpg_y > 14)
            BIOS::LCD::Bar(0, 14, BIOS::LCD::Width, tjpg_y, 0);

        char status[64];
        sprintf(status, "jpeg %dx%d %d%%   ", decoder.width, decoder.height, 100>>tjpg_scale);
        APP::Status(status);

        _ASSERT (result == JDR_OK);
        result = jd_decomp(&decoder, tjpgd_data_writer, tjpg_scale);
        APP::Status(status);
//        _ASSERT (result == JDR_OK);
    }
};

__attribute__((__section__(".entry")))
int main(void)
{
    CONSOLE::colorBack = RGB565(000000);
    APP::Init("SD View");
    char* path = BIOS::OS::GetArgument();
    if (path)
      path = strstr(path, "SD0:/");

    char shortPath[40];
    if (strlen(path) <= 30)
      strcpy(shortPath, path);
    else
    {
      strcpy(shortPath, "...");
      strcat(shortPath, path+strlen(path)-(30-3));
    }

    BIOS::LCD::Print(BIOS::LCD::Width-4-strlen(shortPath)*8, 0, RGB565(b0b0b0), RGBTRANS, shortPath);

    bool initStatus = sd.init();
    if (!initStatus || sd.mType != 2 || sd.mSectors == 0)
    {
        CONSOLE::Print("SD init failed\n");
        BIOS::SYS::DelayMs(5000);
        return 1;
    }

    if (!fat.init())
    {
        CONSOLE::Print("No fat\n");
        BIOS::SYS::DelayMs(5000);
        return 1;
    }

    if (!path || strstr(path, "SD0:/") == 0)
    {
       CONSOLE::Print("Wrong argument\n");
       BIOS::SYS::DelayMs(1000);
       return 1;
    }
    path = strstr(path, "SD0:/")+5;

    CFat32::direntry_t entry;
    if (fat.FindPath(path, entry))
    {
        RenderJpeg(entry);
    } else
        CONSOLE::Print("Not found\n");

    spi.end();
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
  BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
  while(1);
}
