#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "platform.h"
#include "spi.h"
#include "sd.h"
#include "fat.h"

uint8_t sector[512];
uint8_t sector2[512];

uint32_t SetProgress(uint32_t pos, uint32_t len)
{
  int32_t t0 = BIOS::SYS::GetTick();
  bool first = true;
  CRect rcRect(80, 100, BIOS::LCD::Width-80, 100+20);
  uint16_t* pixels = (uint16_t*)sector;
  CRect rcInner(rcRect);
  rcInner.Deflate(3, 3, 3, 3);
  uint8_t corner[16] = {
     0x00, 0x22, 0x88, 0xff,
     0x22, 0xdd, 0xff, 0xff,
     0x88, 0xff, 0xff, 0xff,
     0xff, 0xff, 0xff, 0xff};

  for (int y=rcRect.top; y<rcRect.bottom; y++)
  {
    CRect rcLine(rcRect.left, y, rcRect.right, y+1);
    BIOS::LCD::BufferBegin(rcLine);
    BIOS::LCD::BufferRead(pixels, rcLine.Width());
    BIOS::LCD::BufferEnd();
    int cornery = 3;
    if (y < rcRect.top + 4)
      cornery = y - rcRect.top;
    if (y >= rcRect.bottom - 4)
      cornery = 3-(y-(rcRect.bottom - 4));

    for (int x=0; x<rcLine.Width(); x++)
    {
      int cornerx = 3;
      if (x < 4)
        cornerx = x;
      if (x >= rcRect.Width() - 4)
        cornerx = 3-(x-(rcRect.Width() - 4));

      int mul = 256-corner[cornery*4+cornerx]/2;
      uint16_t clr = pixels[x];
      int r = Get565R(clr)*mul/256;
      int g = Get565G(clr)*mul/256;
      int b = Get565B(clr)*mul/256;
      clr = RGB565RGB(r, g, b);
      pixels[x] = clr;
    }
    BIOS::LCD::BufferBegin(rcLine);
    BIOS::LCD::BufferWrite(pixels, rcLine.Width());
    BIOS::LCD::BufferEnd();
  }
  while (BIOS::SYS::GetTick()-t0 < 1000)
  {
    BIOS::KEY::EKey key = BIOS::KEY::GetKey();
    if (key == BIOS::KEY::None && !first)
      continue;

    first = false;
    t0 = BIOS::SYS::GetTick();
    if (key == BIOS::KEY::Left)
    {
        pos -= len/128;
        if ((int)pos < 0)
          pos = 0;
    }
    if (key == BIOS::KEY::Right)
    {
        pos += len/128;
        if (pos >= len - 1024*1024)
          pos = len - 1024*1024;
    }

    CRect rcLeft(rcInner.left, rcInner.top, rcInner.left+(uint64_t)rcInner.Width()*(uint64_t)pos/len, rcInner.bottom);
    CRect rcRight(rcLeft.right, rcInner.top, rcInner.right, rcInner.bottom);
    BIOS::LCD::Bar(rcLeft, RGB565(d0d0d0));
    BIOS::LCD::Bar(rcRight, RGB565(404040));
    int percent = (int)((uint64_t)pos*100/len);
    int width = percent < 10 ? 2*4 : 3*4;
    if (percent < 50)
      BIOS::LCD::Printf(rcInner.CenterX()-width, rcInner.top, RGB565(ffffff), RGBTRANS, "%d%%", percent);
    else
      BIOS::LCD::Printf(rcInner.CenterX()-width, rcInner.top, RGB565(000000), RGBTRANS, "%d%%", percent);
  }
  return pos;
}

__attribute__((__section__(".entry")))
int main(void)
{
    CSpi spi;
    CSd sd(spi);
    CFat32 fat(sd, sector, sector2);
    char* path = BIOS::OS::GetArgument();
    if (path)
      path = strstr(path, "SD0:/");
    if (!path || !path[0]) 
      strcpy(path, "SD0:/bigbuc~1.vid");

    CONSOLE::colorBack = RGB565(000000);
    APP::Init("SD Video");
    BIOS::LCD::Print(BIOS::LCD::Width-4-strlen(path)*8, 0, RGB565(b0b0b0), RGBTRANS, path);
    BIOS::LCD::Bar(0, BIOS::LCD::Height-18, BIOS::LCD::Width, BIOS::LCD::Height, RGB565(000000));

    bool initStatus = sd.init();
//    CONSOLE::Print("Init: %d, type: %d, sectors: %d\n",
//        initStatus, sd.mType, sd.mSectors);
    if (!initStatus || sd.mType != 2 || sd.mSectors == 0)
    {
        CONSOLE::Print("Exiting\n");
        BIOS::SYS::DelayMs(5000);
        return 1;
    }

    if (!fat.init())
    {
        CONSOLE::Print("Exiting\n");
        BIOS::SYS::DelayMs(5000);
        return 1;
    }

    if (!path || strstr(path, "SD0:/") == 0 || strstr(path, ".vid") == 0)
    {
       CONSOLE::Print("Wrong argument\n");
       BIOS::SYS::DelayMs(1000);
       return 1;
    }

    path = strstr(path, "SD0:/")+5;
    bool forceSync = false;
    CFat32::direntry_t entry;
    if (fat.FindPath(path, entry))
    {
        BIOS::LCD::BufferBegin(CRect(0, 14, BIOS::LCD::Width, BIOS::LCD::Height-18));
        uint32_t processed = 0;
        entry.readCluster = entry.FstClusLO | (entry.FstClusHI<<16);
        int32_t t0 = BIOS::SYS::GetTick();
        int32_t t00 = t0;
        int lastproc = 0, bufs = 0, frames = 0;
        uint32_t fsec = 0;
        while (processed <= entry.FileSize)
        {
            uint32_t sec = fat.mPartitionBegin + (entry.readCluster - 2)*fat.mBpb.SectorPerCluster + fat.mFirstDataSector;
            for (int i=0; i<fat.mBpb.SectorPerCluster; i++)
            {
              if (fsec != sec)
              {                         
                if (fsec != 0)
                  fat.mSd.readBytes2Stop();
                fsec = sec;
                fat.mSd.readBytes2Start(fsec);
              }
              if (!forceSync)
                fat.mSd.readBytes2(sector, 512);
              else
                fat.mSd.readBytes(nullptr, 512);

              sec++;
              fsec++;
              processed += sizeof(sector);

              if (++bufs>=130*2)
              {
                frames++;
                bufs = 0;
                int32_t t1 = BIOS::SYS::GetTick();
                if (forceSync)
                {
                  t0 = t00 = t1;
                  frames = 0;
                  lastproc = processed;
                }
                if (t1-t0 >= 5000)
                {
                  t0 += 5000;
                  forceSync = true;
                }
                if (forceSync)
                {
                  forceSync = false;

                  BIOS::LCD::BufferEnd();
                  int fps = t1!=t00 ? frames*10000/(t1-t00) : 0;
                  char status[64];
                  sprintf(status, "%d kB/s, %d.%d fps  ", (processed - lastproc)/1024/5, fps/10, fps%10);
                  BIOS::LCD::Print(4, BIOS::LCD::Height-16, RGB565(808080), RGB565(000000), status);
                  BIOS::LCD::BufferBegin(CRect(0, 14, BIOS::LCD::Width, BIOS::LCD::Height-18));
                  lastproc = processed;
                }
              }
            }

            switch (BIOS::KEY::GetKey())
            {
              case BIOS::KEY::None: 
                entry.readCluster = fat.NextCluster2(entry.readCluster, fsec);
              break;
              case BIOS::KEY::Left: 
              case BIOS::KEY::Right: 

                BIOS::LCD::BufferEnd();
                processed = SetProgress(processed, entry.FileSize);
                processed &= ~(512*fat.mBpb.SectorPerCluster-1);
                bufs = (processed / 512) % (130*2);

                entry.readCluster = entry.FstClusLO | (entry.FstClusHI<<16);
                for (uint32_t i=0; i<processed; i+=512*fat.mBpb.SectorPerCluster)
                  entry.readCluster = fat.NextCluster2(entry.readCluster, fsec);

//                BIOS::LCD::BufferBegin(CRect(0, 16, BIOS::LCD::Width, BIOS::LCD::Height-16));
                forceSync = true;
              break;
              default: 
                // exit
                processed = (uint32_t)-1;  
            }
        }
        fat.mSd.readBytes2Stop();
        BIOS::LCD::BufferEnd();
    } else
        CONSOLE::Print("Not found\n");

    spi.end();
    return 0;
}
