#include "Bios.h"

namespace BIOS
{
  namespace FAT 
  {
    uint8_t sharedBuffer[BIOS::FAT::SectorSize];
    FILE* f;
      
    PVOID GetSharedBuffer()
    {
      return sharedBuffer;
    }

    EResult Init()
    {
      return EOk;
    }

    EResult Open(const char* strName, ui8 nIoMode)
    {
        if (nIoMode == BIOS::FAT::IoRead)
        {
            f = fopen(strName, "rb");
            return f ? BIOS::FAT::EOk : BIOS::FAT::EIntError;
        }
        return BIOS::FAT::EIntError;
    }

    EResult Read(ui8* pSectorData)
    {
        fread(sharedBuffer, BIOS::FAT::SectorSize, 1, f);
        return BIOS::FAT::EOk;
      //return BIOS::FAT::EIntError;
    }

    EResult Write(ui8* pSectorData)
    {
      return BIOS::FAT::EIntError;
    }

    EResult Close(int nSize /*= -1*/)
    {
      return BIOS::FAT::EIntError;
    }

    EResult Close()
    {
      return BIOS::FAT::EOk;
    }

    EResult OpenDir(char* strPath)
    {
    	return BIOS::FAT::EIntError;
    }

    EResult FindNext(TFindFile* pFile)
    {
    	return BIOS::FAT::EIntError;	
    } 

    ui32 GetFileSize()
    {
    	return 0;
    }

    EResult Seek(ui32 lOffset)
    {
        fseek(f, lOffset, SEEK_SET);
    	return BIOS::FAT::EOk;
    }
  }
}
