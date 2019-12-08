#include "Bios.h"
#include "library/spf/spf.h"
#include <stdarg.h>

int px = 0;
int py = 0;

extern int _DrawChar(int x, int y, unsigned short clrf, unsigned short clrb, char ch);

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

