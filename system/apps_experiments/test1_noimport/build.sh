#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build

arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -c ../bios.s
arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -c ../main.cpp
arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds ./main.o

arm-none-eabi-objcopy -O binary ./output.elf ./output.bin
arm-none-eabi-objcopy -O ihex ./output.elf ./output.hex

arm-none-eabi-readelf -all output.elf > output.txt
arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 1noimp.elf