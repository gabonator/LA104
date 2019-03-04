#include "Bios.h"
#include <sys/types.h>
#include <dirent.h>

namespace BIOS
{
  namespace FAT 
  {
    uint8_t sharedBuffer[BIOS::FAT::SectorSize];
      
    PVOID GetSharedBuffer()
    {
      return sharedBuffer;
    }

    EResult Init()
    {
      return gHal->FatInit() ? EOk : EIntError;
    }

    EResult Open(const char* strName, ui8 nIoMode)
    {
      return gHal->FatOpen(strName, nIoMode) ? BIOS::FAT::EOk : BIOS::FAT::EIntError;
    }

    EResult Read(ui8* pSectorData)
    {
      return gHal->FatRead(pSectorData,BIOS::FAT::SectorSize) ? BIOS::FAT::EOk : BIOS::FAT::EIntError;
    }

    EResult Write(ui8* pSectorData)
    {
      return gHal->FatRead(pSectorData,BIOS::FAT::SectorSize) ? BIOS::FAT::EOk : BIOS::FAT::EIntError;
    }

    EResult Close(int nSize /*= -1*/)
    {
      return BIOS::FAT::EIntError;
    }

    EResult Close()
    {
      return gHal->FatClose() ? BIOS::FAT::EOk : BIOS::FAT::EIntError;
    }

    EResult OpenDir(char* strPath)
    {
      return gHal->FatOpenDir(strPath) ? BIOS::FAT::EOk : BIOS::FAT::EIntError;
    }

    EResult FindNext(TFindFile* pFile)
    {
      return gHal->FatFindNext(pFile) ? BIOS::FAT::EOk : BIOS::FAT::ENoFile;
    }

    ui32 GetFileSize()
    {
        return gHal->FatGetFileSize();
    }

    EResult Seek(ui32 lOffset)
    {
        return gHal->FatSeek(lOffset) ? BIOS::FAT::EOk : BIOS::FAT::EIntError;
    }
  }
}
