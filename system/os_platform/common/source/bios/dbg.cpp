#include "Bios.h"
#include <stdarg.h>
namespace SPF {
#include <../../../os_host/library/spf/spf.h>
}

extern "C" void js_log(char *);
extern int _DrawChar(int x, int y, unsigned short clrf, unsigned short clrb, char ch);

void BIOS::DBG::Print(const char * format, ...)
{
	static int px = 0;
	static int py = 0;

	char buf[128];
	char* bbuf = buf; 

    va_list args;
        
    va_start( args, format );
    SPF::vsprintf( bbuf, format, args );
    va_end(args);
    
    //fprintf(stdout, buf);
#ifdef EMSCRIPTEN
  js_log(bbuf);
#else
#ifdef __APPLE__
    fprintf(stderr, "%s", buf);
#endif
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
#endif
}

#if defined(DS203) || defined(DS213)
void BIOS::DAC::SetFrequency(int freqHz){}
void BIOS::DAC::SetDuty(int duty){}
int BIOS::DAC::GetFrequency(){return 0;}
int BIOS::DAC::GetDuty(){return 0;}
void BIOS::DAC::SetMode(EMode, uint16_t* buffer, int length) {}
#endif
