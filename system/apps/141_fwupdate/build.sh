#!/bin/bash
#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

set -e

mkdir -p build
cd build

arm-none-eabi-g++ -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float \
  -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi \
  -D DS203 -D STM32F10X_HD \
  -c ../source/main.cpp -o main.o

arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles \
  -T ../app.lds ./main.o

arm-none-eabi-objdump -d -S output.elf > output.asm
arm-none-eabi-readelf -all output.elf > output.txt

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

nm --print-size --size-sort -gC output.elf > symbols_all.txt
cat symbols_all.txt | grep _address 2>/dev/null || true

../../../../tools/elfstrip/elfstrip output.elf fwupd203.elf
echo "Output: build/fwupd203.elf"
