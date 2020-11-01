namespace SPF {
#include "../../../os_host/library/spf/spf.h"
#include "../../../os_host/library/spf/spf.c"
}

#include "../../../../os_host/source/framework/Wnd.cpp"
#include "../../../os_host/source/framework/BufferedIo.h"
#include "../../../os_host/source/framework/Serialize.cpp"
#include "../../../os_host/source/gui/Gui.cpp"
#include "../../../os_host/source/gui/Controls.h"
#include "../../../os_host/source/gui/Controls.cpp"
#include "../../../os_host/source/framework/Utils.cpp"

#ifdef EMSCRIPTEN
int sprintf(char *out, const char *format, ...)
{
        va_list args;
        
        va_start( args, format );
        return SPF::vsprintf( out, format, args );
}

int vsprintf(char *out_, const char *format, va_list args)
{
	return SPF::vsprintf(out_, format, args);
}

#endif
