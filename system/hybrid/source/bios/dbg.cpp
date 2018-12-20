#include "Bios.h"
#include <stdarg.h>

extern int _DrawChar(int x, int y, unsigned short clrf, unsigned short clrb, char ch);

void BIOS::DBG::Print(const char * format, ...)
{
	static int px = 0;
	static int py = 0;

	char buf[128];
	char* bbuf = buf; 

    va_list args;
        
    va_start( args, format );
    sfp_print( bbuf, format, args );
    va_end(args);
    
	for ( bbuf = buf; *bbuf; bbuf++ )
	{
		if ( *bbuf == '\n' || px >= BIOS::LCD::Width-4 )
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

