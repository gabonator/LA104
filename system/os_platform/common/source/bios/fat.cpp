#include "Bios.h"

extern "C"
{
#include <../../../os_host/library/fatfs/ff.h>
#include <../../../os_host/library/fatfs/ff.c>
#include <../../../os_host/library/fatfs/diskio.h>
	
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
		gHal->FlashRead(buff, sector * BIOS::FAT::SectorSize, count * BIOS::FAT::SectorSize);
		
		return RES_OK;
	}
	
	DRESULT disk_write(BYTE drv, const BYTE *buff, DWORD sector, BYTE count)
	{
		if (drv != 0 || count == 0) return RES_PARERR;
		
		_ASSERT(count == 1);
		// Operation could be interrupted by USB isr calling the same function
		gHal->FlashWrite(buff, sector * BIOS::FAT::SectorSize, 1 * BIOS::FAT::SectorSize);
		
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
#ifdef DS203
			*(DWORD*)buff = 4096;  // TODO: move to bios
#endif
#ifdef DS213
			*(DWORD*)buff = 2047;  // TODO: move to bios
#endif
#ifdef LA104
			*(DWORD*)buff = 2048;
#endif
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
	
	void InvalidateFat()
	{
		f_flush(0);
	}
	
	
	bool NeedInvalidateFat()
	{
		return false;
	}
}

namespace BIOS
{
	namespace FAT
	{
		uint8_t* gSharedBuffer{nullptr};
		
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
			gSharedBuffer = (uint8_t*)sharedBufferPtr;
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
			if (r != 0)
				BIOS::DBG::Print("FAT:INIT RESULT=%d\n", r);
			return Result(r);
		}
		
		EResult Open(const char* strName, ui8 nIoMode)
		{
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
			
			UINT rcount;
			FRESULT r = f_read(&g_file, pSectorData, BIOS::FAT::SectorSize, &rcount);
			if (r != 0)
				BIOS::DBG::Print("FAT:READ RESULT=%d\n", r);
			
			return Result(r);
		}
		
		EResult Write(ui8* pSectorData)
		{
			UINT wcount;
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

#if 0
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

#endif
