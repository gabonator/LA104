#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
set -e

mkdir -p build
cd build

arm-none-eabi-gcc -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -Wno-psabi -DDS213 -MD \
  -g -D _ARM -D STM32F10X_HD -D STM32F103xB -DSTM32F1 -c \
  ../source/webusb/webusb.c \
  ../source/webusb/opencm3.c \
  -I../../../os_library/include/ \

arm-none-eabi-g++ -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DDS213 -MD -D _ARM -D STM32F10X_HD -D STM32F103xB -c \
  -g ../source/main.cpp \
  -I ../../../os_library/include/ \
  -I ../../../os_host/lib/CMSIS/Device/STM32F10x/Include \
  -I ../../../os_host/lib/STM32F10x_StdPeriph_Driver/inc

arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/app.lds \
  -g ./main.o \
  ./webusb.o ./opencm3.o \
  -lbios_la104 -L../../../os_library/build -lm

arm-none-eabi-objdump -d -S output.elf > output.asm
arm-none-eabi-readelf -all output.elf > output.txt

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 117rt213.elf

nm --print-size --size-sort -gC output.elf | grep " B " > symbols_ram.txt
nm --print-size --size-sort -gC output.elf | grep " T " > symbols_rom.txt
nm --print-size --size-sort -gC output.elf > symbols_all.txt

# calculate crc from .text section raw data
arm-none-eabi-objcopy -O binary --only-section=.text output.elf output.text
CRC=`crc32 output.text`
node ../service/nmparse.js symbols_all.txt ds213_gabuino > ../symbols/ds213_gabuino_${CRC}.js
