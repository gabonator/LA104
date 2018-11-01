@echo off
setlocal ENABLEDELAYEDEXPANSION

rem DS203 Win32 GCC support by valky.eu
rem USER DEFINED VALUES
rem ===================================================

set CBASE=C:\Programs\Devel\Gcc\arm-2011.03\bin\
rem set CBASE=C:\Programs\Devel\Gcc\yagarto-20121222\bin\
set TARGET=F:\
set TFILE=APP
set APP=2
rem ===================================================

set CROSS=!CBASE!arm-none-eabi-
set CC=!CROSS!gcc
set OBJCOPY=!CROSS!objcopy
set LD=!CROSS!ld
set AS=!CROSS!as
set STRIP=!CROSS!strip
set READELF=!CROSS!readelf

if not exist !CC!.* (
  echo Compiler not found !
  goto :eof
)

rem set ELF_BASE=0x08013000
set CFLAGS=-Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -std=c99
set AFLAGS=-mcpu=cortex-m3 -mthumb
set LDFLAGS=-nostartfiles -mcpu=cortex-m3 -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float 

cd bin

rem Build library
!CC! !CFLAGS! -c -Wall -Werror -fpic ../library.c
!CC! !LDFLAGS! -shared -o libbios.so library.o
rem !readelf! -r libbios.so 
rem !CROSS!nm -g libbios.so

!CC! !CFLAGS! -c ../main.c
rem !CC! -fvisibility=hidden -T ../APP2main.lds !LDFLAGS! -Wl,-dynamic-linker,gloader.1 main.o -o !TFILE!_!APP!.elf -lbios -L.

!CC! -T ../app.lds !LDFLAGS! -Wl,-dynamic-linker,gloader.1 main.o -o main.elf -lbios -L.

!STRIP! -x -s --strip-unneeded --keep-symbol=_estack --remove-section=.ARM.attributes --remove-section=.comment main.elf -o mains.elf


C:\Data\Devel\VC\elftest2\Debug\elftest2.exe mains.elf mainss.elf

!CROSS!readelf -all main.elf > outputss.txt
!CROSS!objdump -d -S main.elf > outputss.asm
