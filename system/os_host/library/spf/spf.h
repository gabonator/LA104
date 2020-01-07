#include <stdarg.h>

#ifndef WIN32
int sprintf(char *out, const char *format, ...);
int vsprintf(char *out, const char *format, va_list args);
#endif
//int print(char **out, const char *format, va_list args );
//int sfp_print(char **out, const char *format, va_list args );
