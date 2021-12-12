#include <dirent.h>
#include "../../system/os_host/source/bios/Bios.h"
//#include "../../system/os_host/source/framework/Classes.h"

uint16_t display[BIOS::LCD::Width*BIOS::LCD::Height];
CRect drawRect;
CPoint drawPoint;
DIR* dirp;



namespace BIOS
{
  namespace FAT
  {
    void* GetSharedBuffer()
    {
      static uint8_t buffer[4096];
      return buffer;
    }

    EResult OpenDir(char* strPath)
    {
        char fullPath[512];
        strcpy(fullPath, strPath);
        
        dirp = opendir(fullPath);
        return dirp ? EResult::EOk : EResult::EDiskError;
    }
    
    EResult FindNext(BIOS::FAT::TFindFile* pFile)
    {
        struct dirent * dp;
        dp = readdir(dirp);
        
        if (!dp)
        {
            closedir(dirp);
            return EResult::EDiskError;
        }
        
        pFile->nAtrib = dp->d_type & DT_DIR ? BIOS::FAT::EAttribute::EDirectory : BIOS::FAT::EAttribute::EArchive;
        pFile->nFileLength = 0;
        strcpy(pFile->strName, dp->d_name);
        return EResult::EOk;
    }

  }

  namespace LCD
  {
    void PutPixel(int x, int y, unsigned short clr) 
    {
      display[y*Width+x] = clr;
    }

    uint16_t GetPixel(int x, int y)
    {
      return display[y*Width+x];
    }

    void RoundRect(int x1, int y1, int x2, int y2, unsigned short clr)
    {
    }
    void RoundRect(const CRect& rc, unsigned short clr)
    {
    }
    void BufferBegin(const CRect& rc)
    {
      drawRect = rc;
//      drawPoint = rc.TopLeft();
      drawPoint.x = rc.left;
      drawPoint.y = rc.bottom-1;
//      printf("draw(%d, %d, %d, %d)\n", rc.left, rc.top, rc.right, rc.bottom);
    }
    void BufferWrite(uint16_t buffer)
    {
      PutPixel(drawPoint.x, drawPoint.y, buffer);
/*
      if (++drawPoint.x >= drawRect.right)
      {
        drawPoint.x = drawRect.left;
        drawPoint.y--;
      }
*/
      if (--drawPoint.y < drawRect.top)
      {
        drawPoint.y = drawRect.bottom-1;
        drawPoint.x++;
      }
    }
    void BufferWrite(uint16_t* buffer, int len)
    {
    }
//    void BufferRead(uint16_t* buffer, int len)
//    {
//    }
    uint16_t BufferRead()
    {
      uint16_t p = GetPixel(drawPoint.x, drawPoint.y);
      if (--drawPoint.y < drawRect.top)
      {
        drawPoint.y = drawRect.bottom-1;
        drawPoint.x++;
      }
      return p;
    }
    void BufferEnd()
    {
    }

  }
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    printf("Assertion failed in %s [%d]: %s\n", file, line, cond);
    exit(1);
}
