#pragma once
#include <assert.h>
#include <algorithm>
#include <string.h>
#include <stdint.h>
#include "../library/spf.h"
#include "Rgb.h"

//#define _ASSERT assert
#define _ASSERT(e) {if(!(e)) {while (1);}}
#define ToWord(a, b) (ui16)(((a)<<8)|(b))
#define ToDword(a, b, c, d) (ui32)((ToWord(d, c)<<16)|ToWord(b,a))
//#define NULL nullptr
#define min(a,b) std::min(a,b)
#define max(a,b) std::max(a,b)
#define COUNT(arr) (int)(sizeof(arr)/sizeof(arr[0]))
#define NATIVEENUM uint8_t

//typedef const unsigned short uc16;
typedef unsigned char ui8;
//typedef unsigned char ui8;
typedef unsigned short u16;
typedef signed short si16;
typedef signed char si8;
typedef unsigned short ui16;
typedef unsigned long ui32;
typedef signed long si32;
typedef signed short si16;
typedef void* PVOID;
typedef bool BOOL;
typedef float FLOAT;
typedef int INT;
typedef const char * PCSTR;
typedef char * PSTR;
typedef char CHAR;
typedef ui32 UINT;
