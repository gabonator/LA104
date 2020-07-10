@echo off
setlocal ENABLEDELAYEDEXPANSION

set CBASE=C:\Programs\Devel\Gcc\arm-2011.03\bin\

set CROSS=!CBASE!arm-none-eabi

set WIN32_ARM_GCC_AFLAGS=-mcpu=cortex-m3 -mthumb 
set WIN32_ARM_GCC_LDFLAGS=-mcpu=cortex-m3 -mthumb -nostartfiles -mfix-cortex-m3-ldrd -msoft-float -lc -lgcc

set WIN32_ARM_GCC_CFLAGS=-Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -D USE_STDPERIPH_DRIVER -D STM32F10X_HD
set WIN32_ARM_GCC_GPPFLAGS=!WIN32_ARM_GCC_CFLAGS! -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi

set WIN32_ARM_GCC_INCLUDES=-I ../lib/CMSIS/Include -I ../lib/STM32F10x_StdPeriph_Driver/inc  -I ../ -I ../lib/CMSIS/Device/STM32F10x/Include
set A_SRCS=../bios.s
set C_SRCS=../startup.c
set CPP_SRCS=../main.cpp
set OBJS=main.o startup.o
                                               
if not exist build mkdir build
cd build

!CROSS!-gcc !WIN32_ARM_GCC_AFLAGS! -c !A_SRCS!
!CROSS!-gcc !WIN32_ARM_GCC_CFLAGS! !WIN32_ARM_GCC_INCLUDES! -c !C_SRCS!
!CROSS!-g++ !WIN32_ARM_GCC_GPPFLAGS! !WIN32_ARM_GCC_INCLUDES! -c !CPP_SRCS!
!CROSS!-gcc -o output.elf !WIN32_ARM_GCC_LDFLAGS! -T ../app.lds !OBJS!
!CROSS!-objcopy -O ihex output.elf output.hex
!CROSS!-readelf -all output.elf > output.txt
!CROSS!-objdump -d -S output.elf > output.asm
