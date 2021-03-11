#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SET_RUN_STATE(state)
#define SET_IDLE_STATE(state)
#define SET_ERROR_STATE(state)

void gdb_ident(char *p, int count);
void platform_buffer_flush(void);
unsigned char gdb_if_getchar_to(int timeout);
unsigned char gdb_if_getchar();
void gdb_if_putchar(unsigned char c, int flush);
void abort();


void* malloc_sector(int p);
void* malloc_breakwatch(int p);
void free_(void*);
void free_sprintf(void*);
void* alloc_target();

int sscanf(const char *str, const char *format, ...);
int vasprintf(char **strp, const char *fmt, va_list ap);
int snprintf(char *str, size_t size, const char *format, ...);

void platform_init();

#ifdef __cplusplus
}


int sprintf(char *str, const char *format, ...);
#endif
