#include <library.h>
#include <stddef.h>
#include "mpconfigport.h"
#include "py/mpconfig.h"
            
void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr) {
    BIOS::DBG::Print("Assertion '%s' failed, ");
    BIOS::DBG::Print("at file %s:%d\n", expr, file, line);
    while (1);
}

extern "C" mp_uint_t mp_hal_ticks_ms(void) {
    return BIOS::SYS::GetTick();
}

extern "C" void mp_hal_set_interrupt_char(char c) {
}

extern "C" void mp_hal_stdout_tx_str(const char *str) 
{
    BIOS::DBG::Print(str);
}

extern "C" void mp_hal_stdout_tx_strn(const char *str, size_t len) 
{
  while(len--)
    BIOS::DBG::Print("%c", *str++);
}

extern "C" int mp_hal_stdin_rx_chr(void) 
{ 
  BIOS::DBG::Print("Waiting for char-blocking\n");
  while(1);
  return 0; 
} // blocking

extern "C" void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len) {
    const char *last = str;
    while (len--) {
        if (*str == '\n') {
            if (str > last) {
                mp_hal_stdout_tx_strn(last, str - last);
            }
            mp_hal_stdout_tx_strn("\r\n", 2);
            ++str;
            last = str;
        } else {
            ++str;
        }
    }
    if (str > last) {
        mp_hal_stdout_tx_strn(last, str - last);
    }
}

extern "C" void ddprintf(const char * format, ...)
{
    char buf[128];
    char* bbuf = buf;

    va_list args;
        
    va_start( args, format );
    vsprintf( bbuf, format, args );
    va_end(args);
    BIOS::DBG::Print(buf);
}

#ifdef _APPLE_

#include <stdlib.h>

namespace BIOS {
namespace DBG {

void Print(const char * format, ...)
{
	static int px = 0;
	static int py = 0;

	char buf[128];
	char* bbuf = buf; 

    va_list args;
        
    va_start( args, format );
    vsprintf( bbuf, format, args );
    va_end(args);
    fprintf(stdout, "%s", buf);
}

}
namespace SYS {
uint32_t GetTick()
{
  return 0;
}
}

}

#endif
