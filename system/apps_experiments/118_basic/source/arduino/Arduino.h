#pragma once

namespace BASIC {
enum{ SINGLE_PROGSIZE =1024};
}


#define PGM_P const char*
#define PROGMEM
#define pgm_read_byte(a) (*((const uint8_t*)a))
#define pgm_read_word(a) (*a)
#define pgm_read_ptr(a) (*a)
#define strcpy_P(a,b) strcpy(a,b)
#define strcmp_P(a,b) strcmp(a,b)
#define memcpy_P(a,b,c) memcpy(a,b,c)

//#define __BEGIN_DECLS
//#define BOOLEAN bool
#define PROGSTRINGSIZE 80
#define VARSIZE 5

//#define pgm_read_ptr(x) *x
	
//void* pgm_read_ptr(void* p);	

//uintptr_t pgm_read_ptr(const void* p);// { return *((uint32_t*)p); }
enum {HEX};

//enum ASCII{ SPACE=0x32};

namespace BASIC {
//template <typename T> T min(T a, T b) { return a < b ? a : b; }
}
#define CLI_PROMPT_NEWLINE 1
//#define USEMATH 1
#define M_TRIGONOMETRIC         1
