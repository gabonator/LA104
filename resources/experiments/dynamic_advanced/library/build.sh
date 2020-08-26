#!/bin/bash
#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build

echo build library

#arm-none-eabi-gcc -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -std=c99 -c -Wall -Werror -fpic ../library.c
arm-none-eabi-gcc -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -std=c99 -c -Wall -Werror ../library.c
arm-none-eabi-gcc -nostartfiles -mcpu=cortex-m3 -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float -shared -o libbios.so library.o
