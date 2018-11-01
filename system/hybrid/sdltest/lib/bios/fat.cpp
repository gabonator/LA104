#include "Bios.h"

namespace BIOS
{
  namespace FAT 
  {
    PVOID GetSharedBuffer()
    {
      return nullptr;
    }

    EResult Init()
    {
      return EIntError;
    }

    EResult Open(const char* strName, ui8 nIoMode)
    {
        return BIOS::FAT::EIntError;
    }

    EResult Read(ui8* pSectorData)
    {
      return BIOS::FAT::EIntError;
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
      return BIOS::FAT::EIntError;
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
    	return BIOS::FAT::EIntError;
    }
  }
}