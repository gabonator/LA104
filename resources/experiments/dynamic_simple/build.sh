#!/bin/bash
#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

export PATH="/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/":"$PATH"
mkdir -p build
cd build

echo build library

#arm-none-eabi-gcc -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -std=c99 -c -Wall -Werror -fpic ../library.c
arm-none-eabi-gcc -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -std=c99 -c -Wall -Werror ../library.c
arm-none-eabi-gcc -nostartfiles -mcpu=cortex-m3 -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float -shared -o libbios.so library.o

echo build app

arm-none-eabi-gcc -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -std=c99 -c ../main.c
arm-none-eabi-gcc -fvisibility=hidden -T ../app.lds -nostartfiles -mcpu=cortex-m3 -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float  -Wl,-dynamic-linker,gloader.1 main.o -o main.elf -lbios -L.
arm-none-eabi-strip -x -s --strip-unneeded --keep-symbol=_estack --remove-section=.ARM.attributes --remove-section=.comment main.elf -o mains.elf


arm-none-eabi-readelf -all main.elf > outputss.txt
arm-none-eabi-objdump -d -S main.elf > outputss.asm
