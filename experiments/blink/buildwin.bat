@echo off
setlocal ENABLEDELAYEDEXPANSION

set CBASE=C:\Programs\Devel\Gcc\arm-2011.03\bin\

set CROSS=!CBASE!arm-none-eabi

set WIN32_ARM_GCC_AFLAGS=-mcpu=cortex-m3 -mthumb
set WIN32_ARM_GCC_LDFLAGS=-mcpu=cortex-m3 -mthumb -nostartfiles
set WIN32_ARM_GCC_CFLAGS=-Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD

set WIN32_ARM_GCC_INCLUDES=-I ../FWLib/inc -I ../
set C_SRCS=../main.c ../startup.c
set OBJS=main.o startup.o
                                               
if not exist build mkdir build
cd build
!CROSS!-gcc !WIN32_ARM_GCC_CFLAGS! !WIN32_ARM_GCC_INCLUDES! -c !C_SRCS!
!CROSS!-gcc -o output.elf !WIN32_ARM_GCC_LDFLAGS! -T ../app.lds !OBJS!
!CROSS!-objcopy -O ihex output.elf output.hex
!CROSS!-readelf -all output.elf > output.txt
!CROSS!-objdump -d -S output.elf > output.asm
