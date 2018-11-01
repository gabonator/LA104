#!/bin/bash
#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

export PATH="/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/":"$PATH"
mkdir -p build
cd build

echo build library

#arm-none-eabi-gcc -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -std=c99 -c -Wall -Werror -fpic ../library.c
arm-none-eabi-g++ -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -c -Wall -Werror ../source/library.cpp
arm-none-eabi-gcc -fPIC -nostartfiles -mcpu=cortex-m3 -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float -shared -o libbios.so library.o

#arm-none-eabi-nm --demangle ./libbios.so
find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete
