#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "sd.h"
#include "FFT.h"

int _max(int a, int b)
{
    return a>b?a:b;
}

uint8_t sector[512];
uint8_t sector2[512];

CSpi spi;
CSd sd(spi);
CFat32 fat(sd, sector, sector2);

int16_t waveR[1024];
int16_t waveI[1024];
int16_t input[1024];
CFft<1024> fft;

int8_t blk[1024];
//    uint16_t buf[BIOS::LCD::Height-32];
uint16_t column[BIOS::LCD::Height];
uint16_t blank[BIOS::LCD::Height];

void RenderPcm(CFat32::direntry_t& direntry)
{
    BIOS::LCD::Bar(0, 14, BIOS::LCD::Width, BIOS::LCD::Height-14, RGB565(404040));
    for (int i=0; i<COUNT(blank); i++)
      blank[i] = RGB565(404040);
    int blkindex = 0;
    int filtered[32] = {0};
    int32_t t0 = BIOS::SYS::GetTick();
    uint32_t blocks = 0;
    BIOS::LCD::BufferBegin(CRect(0, 14, BIOS::LCD::Width, BIOS::LCD::Height-14));
    while (fat.ReadFile(direntry))
    {
      blocks++;
      for (int i=0; i<512; i++)
      {
          blk[blkindex++] = sector[i];
          if (blkindex == 1024)
              blkindex = 0;
          
          if ((blkindex & 255) == 0)
          {
              int k = blkindex;
              for (int j=0; j<1024; j++)
              {
                  waveR[j] = (blk[k]*fft.Hann(j)) >> 12;
                  waveI[j] = 0;
                  k = (k+1) &1023;
              }
              fft.Forward(waveR, waveI);
              
              for (int i=0; i<32; i++)
              {
                  int max = 0;
                  for (int j=0; j<8; j++)
                  {
                      int ii = i*8+j;
                      max = _max(max, waveR[ii]*waveR[ii]+waveI[ii]*waveI[ii]);
                  }
                  int y = fft.Sqrt(max);
                  if (y < 0)
                      y = 0;
                  if (y > 150)
                      y = 150;

                  if (y > filtered[i])
                      filtered[i] = y;
                  else
                      filtered[i] = (filtered[i]*3+y)/4;
              }

              for (int x=0; x<32; x++)
              {
                  int yy = filtered[x];

                  for (int y=0; y<BIOS::LCD::Height-14; y++)
                    column[y] = y >= yy ? RGB565(404040) : RGB565(b0b0b0);

                  BIOS::LCD::BufferWrite(column, BIOS::LCD::Height-28);
                  BIOS::LCD::BufferWrite(column, BIOS::LCD::Height-28);
                  BIOS::LCD::BufferWrite(column, BIOS::LCD::Height-28);
                  BIOS::LCD::BufferWrite(column, BIOS::LCD::Height-28);
                  BIOS::LCD::BufferWrite(column, BIOS::LCD::Height-28);
                  BIOS::LCD::BufferWrite(column, BIOS::LCD::Height-28);
                  BIOS::LCD::BufferWrite(column, BIOS::LCD::Height-28);
                  BIOS::LCD::BufferWrite(column, BIOS::LCD::Height-28);
                  BIOS::LCD::BufferWrite(column, BIOS::LCD::Height-28);
                  BIOS::LCD::BufferWrite(blank, BIOS::LCD::Height-28);
              }
              if (BIOS::KEY::GetKey()) 
              {
                BIOS::LCD::BufferEnd();
                return;
              }
          }
      }
      int32_t t1 = BIOS::SYS::GetTick();
      if (t1 - t0 >= 1000)
      {
//          int32_t bps = (uint64_t)blocks*512/(t1-t0);
                BIOS::LCD::BufferEnd();
          BIOS::LCD::Printf(4, BIOS::LCD::Height-14, RGB565(b0b0b0), RGBTRANS, "mp3, 192 kbps  ");
//                            blocks, t1-t0, bps);
//          BIOS::LCD::Printf(4, BIOS::LCD::Height-14, RGB565(b0b0b0), RGB565(000000), "%d, %d, %d kBps  ",
//                            blocks, t1-t0, bps);
          BIOS::LCD::BufferBegin(CRect(0, 14, BIOS::LCD::Width, BIOS::LCD::Height-14));
          blocks = 0;
          t0 += 1000;
      }
    }
   BIOS::LCD::BufferEnd();
};

__attribute__((__section__(".entry")))
int main(void)
{
    CONSOLE::colorBack = RGB565(000000);
    APP::Init("SD Play");
    char* path = BIOS::OS::GetArgument();
    if (path)
      path = strstr(path, "SD0:/");

    if (!path)
      path = (char*)"SD0:/pacman.mp3";

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
        RenderPcm(entry);
    } else
    {
        CONSOLE::Print("Not found\n");
        BIOS::SYS::DelayMs(5000);
    }

    spi.end();
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
  BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
  while(1);
}
