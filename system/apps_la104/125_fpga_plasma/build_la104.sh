#!/bin/bash
set -e

mkdir -p build
cd build

SYSROOT=../../..

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float \
    -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -D _ARM \
    -c ../source/main.cpp \
    $SYSROOT/os_host/source/framework/Wnd.cpp \
    $SYSROOT/os_host/source/framework/Serialize.cpp \
    -I$SYSROOT/os_library/include/

arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb \
    -o output.elf -nostartfiles -T ../source/app.lds \
    ./main.o ./Wnd.o ./Serialize.o \
    -lbios_la104 -lm \
    -L$SYSROOT/os_library/build

arm-none-eabi-size output.elf

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 125plasm.elf
rm output.elf

echo ""
echo "Output: build/125plasm.elf"
