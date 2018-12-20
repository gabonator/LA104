#ifndef __MYDEFINE_H
#define __MYDEFINE_H

#include <library.h>
__attribute__((__section__(".entry"))) int main(void);

#include <string.h>
#include <stdint.h>

typedef uint16_t u16;
typedef uint8_t u8;
typedef int8_t s8;
typedef const uint8_t uc8;
typedef int16_t s16;
typedef const uint16_t uc16;
typedef const uint32_t uc32;
typedef uint32_t u32;
typedef int32_t s32;
typedef volatile uint32_t vu32;
typedef uint64_t u64;

#include "Analyze.h"
#include "AppBios.h"
#include "Ctrl.h"
#include "Files.h"
#include "Func.h"
#include "GUI.h"
#include "Menu.h"

void ExitApplication();

#endif
