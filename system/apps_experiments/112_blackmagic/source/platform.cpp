#include <library.h>
//#include <stddef.h>

//extern "C" void gdb_ident(char *p, int count);


#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

extern "C" {

// String formatting
char temp[128];

int sscanf(const char *str, const char *format, ...)
{
  BIOS::DBG::Print("scan with format '%s' not supported\n", str);
  abort();
  return 0;
}

int sprintf(char **strp, const char *format, ...)
{
 return 0;
}
int vasprintf(char **strp, const char *fmt, va_list ap)
{
  *strp = temp;
  vsprintf(temp, fmt, ap);
  _ASSERT(strlen(temp) < 64);
  return strlen(temp);
}

int snprintf(char *str, size_t size, const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vsprintf(str, format, args);
  _ASSERT(strlen(str) < size);
  return strlen(str);
}

void* _sbrk() { return 0; }
// Core
void platform_init()
{
//	cl_init(&cl_opts, argc, argv);
//	info.bmp_type = BMP_TYPE_STLINKV2;
//	if (stlink_init( &info))
//	  exit(-1);
//	int ret = -1;
//	if (cl_opts.opt_mode != BMP_MODE_DEBUG) {
//		ret = cl_execute(&cl_opts);
//	} else {
//		gdb_if_init();
//		return;
//	}
//	exit(ret);

}

void platform_buffer_flush(void) {}
void abort() { while (1); }

void gdb_ident(char *p, int count)
{
const char* manufacturer = "man";
const char* product = "prod";
const char* version = "ver";

//	sprintf(p, "%s (%s), %s", manufacturer, product,
//			 version);
}

// Communication
unsigned char gdb_if_getchar_to(int timeout)
{ return 0; }
unsigned char gdb_if_getchar()
{ return 0; }
void gdb_if_putchar(unsigned char c, int flush)
{}

// Wire?
void platform_srst_set_val(bool assert) {}
void platform_max_frequency_set(uint32_t frequency) {}
uint32_t platform_max_frequency_get(void) { return 0; }
const char *platform_target_voltage(void) { return "0.00V"; }
int platform_adiv5_swdp_scan(uint32_t targetid) { return 0; }
int platform_jtag_scan(const uint8_t *lrlens) { return 0; }

// Allocations
void* malloc_sector(int p) { return 0; }
void* malloc_breakwatch(int p) { return 0; }
void free_(void* p) {}
void* alloc_target() { return 0; } 
void free_sprintf(void*) { }

// Logging
void DEBUG_WARN(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vsprintf(temp, format, args);
  BIOS::DBG::Print("WARN: %s\n", temp);
}

void DEBUG_INFO(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vsprintf(temp, format, args);
  BIOS::DBG::Print("INFO: %s\n", temp);
}
void DEBUG_GDB(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vsprintf(temp, format, args);
  BIOS::DBG::Print("GDB: %s\n", temp);
}
void DEBUG_GDB_WIRE(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vsprintf(temp, format, args);
  BIOS::DBG::Print("GDB_WIRE: %s\n", temp);
}
void DEBUG_TARGET(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vsprintf(temp, format, args);
  BIOS::DBG::Print("TARGET: %s\n", temp);
}
void DEBUG_PROBE(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vsprintf(temp, format, args);
  BIOS::DBG::Print("PROBE: %s\n", temp);
}
void DEBUG_WIRE(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vsprintf(temp, format, args);
  BIOS::DBG::Print("WIRE: %s\n", temp);
}
}
