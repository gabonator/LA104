#include "Bios.h"
#include <sys/types.h>
#include <dirent.h>

DIR* dirp;

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
        if (nIoMode == BIOS::FAT::IoWrite)
        {
            f = fopen(strName, "wb");
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
        fwrite(sharedBuffer, BIOS::FAT::SectorSize, 1, f);
        return BIOS::FAT::EOk;
    }

    EResult Close(int nSize /*= -1*/)
    {
//        fclose(f);
      return BIOS::FAT::EIntError;
    }

    EResult Close()
    {
        fclose(f);
      return BIOS::FAT::EOk;
    }

    EResult OpenDir(char* strPath)
    {
        char fullPath[512];
#ifdef __APPLE__
        static char* rootPath = (char*)"/Users/gabrielvalky/Documents/git/LA104/system/release/";
        strcpy(fullPath, rootPath);
        for (int i=0; i<strlen(strPath); i++)
            strPath[i] = tolower(strPath[i]);
        strcat(fullPath, strPath);
#else
        strcpy(fullPath, strPath);
#endif
        
        dirp = opendir(fullPath);
        return dirp ? BIOS::FAT::EOk : BIOS::FAT::EIntError;
    }

    EResult FindNext(TFindFile* pFile)
    {
        struct dirent * dp;
        dp = readdir(dirp);
        
        if (!dp)
        {
            closedir(dirp);
            return BIOS::FAT::ENoFile;
        }
        
        pFile->nAtrib = dp->d_type & DT_DIR ? FAT::EAttribute::EDirectory : FAT::EAttribute::EArchive;
        pFile->nFileLength = 0;
        strcpy(pFile->strName, dp->d_name);
        return BIOS::FAT::EOk;
    }

    ui32 GetFileSize()
    {
        int prev = ftell(f);
        fseek(f, 0, SEEK_END);
        int len = ftell(f);
        fseek(f, prev, SEEK_SET);
    	return len;
    }

    EResult Seek(ui32 lOffset)
    {
        fseek(f, lOffset, SEEK_SET);
    	return BIOS::FAT::EOk;
    }
  }
}
