#include "Bios.h"
#include "../library/spf.h"
#include <stdarg.h>

int px = 0;
int py = 0;

extern int _DrawChar(int x, int y, unsigned short clrf, unsigned short clrb, char ch);

extern "C" {
void dbgPrint(const char* msg, ...)
{
  char buf[64];
  char* bbuf = buf; 

  va_list args;

  va_start( args, msg );
  vsprintf( bbuf, msg, args );

  _ASSERT(strlen(buf) < sizeof(buf)-8);
  BIOS::DBG::Print(buf);
}

//TODO: remove
char dbgPushBuf[64];
void dbgPushPrint(const char* msg, ...)
{
  char buf[64];
  char* bbuf = buf; 

  va_list args;

  va_start( args, msg );
  vsprintf( bbuf, msg, args );
  _ASSERT(strlen(buf) < sizeof(buf)-8);

  if (strlen(buf) + strlen(dbgPushBuf) + 2 > sizeof(dbgPushBuf))
  {
    BIOS::DBG::Print(dbgPushBuf);
    strcpy(dbgPushBuf, "");
    _ASSERT(0);
  }
  strcat(dbgPushBuf, buf);
}

void dbgPopPrint()
{
  if (strlen(dbgPushBuf) > 0)
  {
    BIOS::DBG::Print(dbgPushBuf);
    strcpy(dbgPushBuf, "");
  }
}
}

void BIOS::DBG::Print(const char * format, ...)
{
  if (format[0] == '$')
  {
    px = 0;
    py = 0;
    format++;
  }


	char buf[128];
	char* bbuf = buf; 

        va_list args;
        
        va_start( args, format );
        vsprintf( bbuf, format, args );
	for ( bbuf = buf; *bbuf; bbuf++ )
	{
		if ( *bbuf == '\n' || px > BIOS::LCD::Width-8 )
		{
			px = 0;
			py+=14;
			if (py > BIOS::LCD::Height-16)
				py = 0;
			continue;
		}
		px += _DrawChar(px, py, RGB565(ffffff), RGB565(0000B0), *bbuf);
	}
}

