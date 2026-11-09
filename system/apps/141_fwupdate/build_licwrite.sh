#!/bin/bash
set -e

mkdir -p build
cd build

arm-none-eabi-g++ -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float \
  -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi \
  -D DS203 -D STM32F10X_HD \
  -c ../source/licwrite.cpp -o licwrite.o

arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -o licwrite.elf -nostartfiles \
  -T ../app.lds ./licwrite.o

arm-none-eabi-objdump -d -S licwrite.elf > licwrite.asm

find . -type f -name '*.o' -delete

nm --print-size --size-sort -gC licwrite.elf > licwrite_symbols.txt

../../../../tools/elfstrip/elfstrip licwrite.elf licwrite.elf
echo "Output: build/licwrite.elf"
