#include "Bios.h"
#include "imports.h"

#include "fat.h"

static_assert(BIOS_FAT_SectorSize == BIOS::FAT::SectorSize);
static_assert(BIOS_FAT_SectorCount == BIOS::FAT::SectorCount);

void InvalidateFat();

extern "C" 
{
  #include "fatfs/ff.c"

  DSTATUS disk_initialize(BYTE drv)
  {
      // Supports only one drive, no initialization necessary.
      return (drv == 0) ? 0 : STA_NOINIT;
  }

  DSTATUS disk_status (BYTE drv)
  {
      return (drv == 0) ? 0 : STA_NOINIT;
  }

  DRESULT disk_read(BYTE drv, BYTE *buff, DWORD sector, BYTE count)
  {
      if (drv != 0 || count == 0) return RES_PARERR;
      // Operation could be interrupted by USB isr calling the same function
      while (!ExtFlashDataRd(buff, sector * BIOS::FAT::SectorSize, count * BIOS::FAT::SectorSize));

      return RES_OK;
  }

  DRESULT disk_write(BYTE drv, const BYTE *buff, DWORD sector, BYTE count)
  {
      if (drv != 0 || count == 0) return RES_PARERR;

      _ASSERT(count == 1);
      // Operation could be interrupted by USB isr calling the same function
      while (!ExtFlashSecWr((BYTE*)buff, sector * BIOS::FAT::SectorSize));

      return RES_OK;
  }

  DRESULT disk_ioctl(BYTE drv, BYTE ctrl, void *buff)
  {
      if (drv != 0) return RES_PARERR;
      
      if (ctrl == CTRL_SYNC)
      {
          return RES_OK;
      }
      else if (ctrl == GET_SECTOR_COUNT)
      {
          *(DWORD*)buff = BIOS::FAT::SectorCount;
          return RES_OK;
      }
      else if (ctrl == GET_SECTOR_SIZE || ctrl == GET_BLOCK_SIZE)
      {
//          *(DWORD*)buff = 512;
          *(DWORD*)buff = BIOS::FAT::SectorSize;
          return RES_OK;
      }
      else
      {
          return RES_PARERR;
      }
  }

  DWORD get_fattime (void)
  {
          /* Pack date and time into a DWORD variable */
          return    ((DWORD)(2000 - 1980) << 25)
                          | ((DWORD)1 << 21)
                          | ((DWORD)1 << 16)
                          | ((DWORD)13 << 11)
                          | ((DWORD)37 << 5)
                          | ((DWORD)0 >> 1);
  }

  bool gUsbDidWriteToDisk = false;

  bool NeedInvalidateFat()
  {
    bool aux = gUsbDidWriteToDisk;
    gUsbDidWriteToDisk = false;
    return aux;
  }
}

namespace BIOS
{
  namespace FAT 
  {
    void* gSharedBuffer{nullptr};

    FIL g_file;
    DIR g_directory;
    FATFS g_fatfs;

    EResult Result(FRESULT r)
    {
	switch (r)
	{
          case FR_OK:	return EOk;
          case FR_DISK_ERR: return EDiskError;
          case FR_INT_ERR: return EIntError;
          case FR_NO_FILE: return ENoFile;
          case FR_NO_PATH: return ENoPath;
          default:
            return EIntError;
	}
    }

    void SetSharedBuffer(void* sharedBufferPtr)
    {
      gSharedBuffer = sharedBufferPtr;
    }

    void* GetSharedBuffer()
    {
      if (!gSharedBuffer)
        return g_fatfs.win;

      return gSharedBuffer;
    }

    EResult Init()
    {
      FRESULT r = f_mount(0, &g_fatfs);  
#ifdef _VERBOSE
      if (r != 0)
        BIOS::DBG::Print("FAT:INIT RESULT=%d\n", r);
#endif
      return Result(r);
    }

    EResult Open(const char* strName, ui8 nIoMode)
    {
	if (NeedInvalidateFat())
		InvalidateFat();

	if ( nIoMode == BIOS::FAT::IoWrite )
	{
	    FRESULT r = f_open(&g_file, strName, FA_WRITE | FA_CREATE_ALWAYS);
		return Result(r);
	}
	if ( nIoMode == BIOS::FAT::IoRead )
	{
		FRESULT r = f_open(&g_file, strName, FA_READ | FA_OPEN_EXISTING);
		return Result(r);
	}
        return BIOS::FAT::EIntError;
    }

    EResult Read(ui8* pSectorData)
    {
      // TODO: Buffered reader should invalidate its buffer as well?
      if (NeedInvalidateFat())
        InvalidateFat();

      ui32 rcount;
      FRESULT r = f_read(&g_file, pSectorData, BIOS::FAT::SectorSize, &rcount);
#ifdef _VERBOSE
      if (r != 0)
        BIOS::DBG::Print("FAT:READ RESULT=%d\n", r);
#endif
      return Result(r);
    }

    EResult Write(ui8* pSectorData)
    {
      ui32 wcount;
      FRESULT r = f_write(&g_file, pSectorData, BIOS::FAT::SectorSize, &wcount);    
      if (wcount != BIOS::FAT::SectorSize)
        return EDiskFull; // Disk is full
      return Result(r);
    }

    // TODO: rename truncate
    EResult Close(int nSize /*= -1*/)
    {
      FRESULT r;
      if ( nSize != -1 )
      { 
        FRESULT r = f_lseek(&g_file, nSize);
    	if (r != FR_OK)
    	    return Result(r);
        f_truncate(&g_file);
    	if (r != FR_OK)
    	    return Result(r);
       } 
      r = f_close(&g_file);
      return Result(r);
    }

    EResult Close()
    {
      FRESULT r;
      r = f_close(&g_file);
      return Result(r);
    }

    EResult OpenDir(char* strPath)
    {
	if (NeedInvalidateFat())
		InvalidateFat();

    	FRESULT r = f_opendir(&g_directory, (char*)strPath);
    	return Result(r);
    }

    EResult FindNext(TFindFile* pFile)
    {
    	FILINFO* pFileInfo = (FILINFO*)pFile;

    	FRESULT r = f_readdir(&g_directory, pFileInfo);
    	if ( pFileInfo->fname[0] == 0)
    		return BIOS::FAT::ENoFile;

    	return Result(r);	
    } 

    ui32 GetFileSize()
    {
    	return g_file.fsize;
    }

    EResult Seek(ui32 lOffset)
    {
    	FRESULT r = f_lseek(&g_file, lOffset);
    	return Result(r);
    }
  }
}

void InvalidateFat()
{
  f_flush(&BIOS::FAT::g_fatfs);
}
