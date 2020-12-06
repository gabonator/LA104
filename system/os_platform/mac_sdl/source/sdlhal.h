#include "../../common/source/bios/Bios.h"

#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

void setPixel(int x, int y, int c);
int getPixel(int x, int y);
bool sdl_running();
int sdl_lastKey();
void sdl_loop();

class CSdlHal : public CHal
{
    int fd;
	FILE* f{nullptr};

public:
	virtual ~CSdlHal()
	{
		if (f)
			fclose(f);
	}

  virtual void SetPixel(int x, int y, uint16_t c) override
  {
    setPixel(x, y, c);
  }
  virtual uint16_t GetPixel(int x, int y) override
  {
    return getPixel(x, y);
  }
  virtual bool IsRunning() override
  {
    sdl_loop();
    return sdl_running();
  }
  virtual char GetKey() override
  {
    switch (sdl_lastKey())
    {
      case SDL_SCANCODE_LEFT: return '-'; break;
      case SDL_SCANCODE_RIGHT: return '+'; break;
      case SDL_SCANCODE_UP: return '<'; break;
      case SDL_SCANCODE_DOWN: return '>'; break;
      case SDL_SCANCODE_RETURN: return '1'; break;
      case SDL_SCANCODE_BACKSPACE: return '2'; break;
      case SDL_SCANCODE_1: return '1'; break;
      case SDL_SCANCODE_2: return '2'; break;
	  case SDL_SCANCODE_3: return '3'; break;
      case SDL_SCANCODE_4: return '4'; break;
    }
    return 0;
  }

#ifdef LA104
    virtual void UartSetup(int baudrate, BIOS::GPIO::UART::EConfig config) override
    {
#ifdef USEUART
        // Open port
        fd = open("/dev/tty.usbmodemLA104CDC", O_RDWR | O_NOCTTY | O_NDELAY);
        if (fd == -1){
            printf("Device cannot be opened.\n");
            exit(-1);                       // If the device is not open, return -1
        }
        struct termios options;
        
        fcntl(fd, F_SETFL, FNDELAY);                    // Open the device in nonblocking mode
        
        // Set parameters
        tcgetattr(fd, &options);                        // Get the current options of the port
        bzero(&options, sizeof(options));               // Clear all the options
        speed_t         Speed;
        switch (baudrate)                               // Set the speed (baudRate)
        {
            case 110  :     Speed=B110; break;
            case 300  :     Speed=B300; break;
            case 600  :     Speed=B600; break;
            case 1200 :     Speed=B1200; break;
            case 2400 :     Speed=B2400; break;
            case 4800 :     Speed=B4800; break;
            case 9600 :     Speed=B9600; break;
            case 19200 :    Speed=B19200; break;
            case 38400 :    Speed=B38400; break;
            case 57600 :    Speed=B57600; break;
            case 115200 :   Speed=B115200; break;
            default : exit(-4);
        }
        cfsetispeed(&options, Speed);                   // Set the baud rate at 115200 bauds
        cfsetospeed(&options, Speed);
        options.c_cflag |= ( CLOCAL | CREAD |  CS8);    // Configure the device : 8 bits, no parity, no control
        options.c_iflag |= ( IGNPAR | IGNBRK );
        options.c_cc[VTIME]=0;                          // Timer unused
        options.c_cc[VMIN]=0;                           // At least on character before satisfy reading
        tcsetattr(fd, TCSANOW, &options);               // Activate the settings
#endif
    }
    
    virtual void UartClose() override
    {
        close(fd);
        fd = 0;
    }
    
    virtual bool UartAvailable() override
    {
#ifdef USEUART
        int count;
        ioctl(fd, FIONREAD, &count);
        return count>0;
#else
        return false;
#endif
    }
    
    virtual uint8_t UartRead() override
    {
#ifdef USEUART
        uint8_t data;
        _ASSERT(read(fd, &data, 1) == 1);
        return data;
#else
        return 0;
#endif
    }
    
    virtual void UartWrite(uint8_t data) override
    {
#ifdef USEUART
        _ASSERT(write(fd, &data, 1) == 1);
#endif
    }
#endif
	
    // SYS
    virtual void Delay(int intervalMs) override
    {
        SDL_Delay(intervalMs);
    }
    
    virtual uint32_t GetTick() override
    {
        return SDL_GetTicks();
    }
    /*
    // FAT
    virtual bool FatInit() override
    {
        return true;
    }
    
    virtual bool FatOpen(const char* strName, ui8 nIoMode) override
    {
        if (nIoMode == BIOS::FAT::IoRead)
        {
            f = fopen(strName, "rb");
            return !!f;
        }
        if (nIoMode == BIOS::FAT::IoWrite)
        {
            f = fopen(strName, "wb");
            return !!f;
        }
        return false;
    }
    
    virtual bool FatRead(uint8_t* pSectorData, int length) override
    {
        fread(pSectorData, length, 1, f);
        return true;
    }
    
    virtual bool FatWrite(uint8_t* pSectorData, int length) override
    {
        fwrite(pSectorData, length, 1, f);
        return true;
    }
    
    virtual bool FatClose() override
    {
        fclose(f);
        return true;
    }
    
    virtual bool FatOpenDir(char* strPath) override
    {
        char fullPath[512];
#ifdef __APPLE__
        static char* rootPath = (char*)"/Users/gabrielvalky/Documents/git/LA104/system/release/";
        if (strPath[0] == '/')
            strcpy(fullPath, strPath);
        else
        {
            strcpy(fullPath, rootPath);
            for (int i=0; i<strlen(strPath); i++)
                strPath[i] = tolower(strPath[i]);
            strcat(fullPath, strPath);
        }
#else
        strcpy(fullPath, strPath);
#endif
        
        dirp = opendir(fullPath);
        return !!dirp;
    }
    
    virtual bool FatFindNext(BIOS::FAT::TFindFile* pFile) override
    {
        struct dirent * dp;
        dp = readdir(dirp);
        
        if (!dp)
        {
            closedir(dirp);
            return false;
        }
        
        pFile->nAtrib = dp->d_type & DT_DIR ? BIOS::FAT::EAttribute::EDirectory : BIOS::FAT::EAttribute::EArchive;
        pFile->nFileLength = 0;
        strcpy(pFile->strName, dp->d_name);
        return true;

    }
    
    virtual uint32_t FatGetFileSize() override
    {
        int prev = ftell(f);
        fseek(f, 0, SEEK_END);
        int len = ftell(f);
        fseek(f, prev, SEEK_SET);
        return len;
    }
    
    virtual bool FatSeek(uint32_t offset) override
    {
        fseek(f, offset, SEEK_SET);
        return true;
    }
	*/
	
	virtual void FlashRead(uint8_t* buff, int offset, int length) override
	{
		if (!f)
		{
            char buf[1024];
            getcwd(buf, 1024);
            // /Users/gabrielvalky/Documents/git/LA104/system/apps_featured/80_rftool/build/Debug
            char *psystem = strstr(buf, "system");
            if (!psystem)
            {
                _ASSERT(0);
                return;
            }
            strcpy(psystem, "/system/os_platform/mac_sdl/data/la104.fat");
            
			f = fopen(buf, "rb+");
			_ASSERT(f);
		}
		fseek(f, offset, SEEK_SET);
		fread(buff, length, 1, f);
		int err = ferror(f);
		_ASSERT(err==0);
	}
	
	virtual void FlashWrite(const uint8_t* buff, int offset, int length) override
	{
		_ASSERT(f);
		fseek(f, offset, SEEK_SET);
		fwrite(buff, length, 1, f);
		int err = ferror(f);
		_ASSERT(err==0);
	}

};
