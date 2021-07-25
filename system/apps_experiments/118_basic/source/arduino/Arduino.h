#pragma once
#define PGM_P const char*
#define PROGMEM 

#define VARSIZE IDSIZE
namespace BASIC {
enum{ SINGLE_PROGSIZE =1024};
}
#define PROGSTRINGSIZE 256

#define pgm_read_ptr(x) *x
	
//void* pgm_read_ptr(void* p);	

//uintptr_t pgm_read_ptr(const void* p);// { return *((uint32_t*)p); }
enum {HEX};

//enum ASCII{ SPACE=0x32};

namespace BASIC {
//template <typename T> T min(T a, T b) { return a < b ? a : b; }
}
#define strcpy_P strcpy
#define CLI_PROMPT_NEWLINE 1
#define USEMATH 1
#define M_TRIGONOMETRIC         1
