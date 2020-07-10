#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

export PATH="/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/":"$PATH"
mkdir -p build
cd build

arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -c ../bios.s
arm-none-eabi-gcc -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -I ../ -I ../FWLib/inc -I ../App/inc -c ../main.c ../startup.c
arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds ./main.o ./startup.o

arm-none-eabi-objcopy -O binary ./output.elf ./output.bin
arm-none-eabi-objcopy -O ihex ./output.elf ./output.hex

arm-none-eabi-readelf -all output.elf > output.txt
arm-none-eabi-objdump -d -S output.elf > output.asm
