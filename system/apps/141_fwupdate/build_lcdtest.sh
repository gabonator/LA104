#!/bin/bash
#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

set -e

mkdir -p build
cd build

arm-none-eabi-g++ -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float \
  -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi \
  -D DS203 -D STM32F10X_HD \
  -c ../source/lcdtest.cpp -o lcdtest.o

arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -o lcdtest.elf -nostartfiles \
  -T ../app.lds ./lcdtest.o

arm-none-eabi-objdump -d -S lcdtest.elf > lcdtest.asm
arm-none-eabi-readelf -all lcdtest.elf > lcdtest.txt

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

nm --print-size --size-sort -gC lcdtest.elf > lcdtest_symbols.txt
cat lcdtest_symbols.txt | grep _address 2>/dev/null || true

../../../../tools/elfstrip/elfstrip lcdtest.elf lcdtest.elf
echo "Output: build/lcdtest.elf"
