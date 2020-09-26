#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build

arm-none-eabi-gcc -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -Wno-psabi -DLA104 -MD \
  -D _ARM -D STM32F10X_HD -D STM32F103xB -DSTM32F1 -c \
  ../source/webusb/webusb.c \
  ../source/webusb/opencm3.c \
  -I../../../os_library/include/ \

arm-none-eabi-g++ -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -D _ARM -D STM32F10X_HD -D STM32F103xB -c \
  ../source/main.cpp \
  ../source/streamer/streamer.cpp \
  -I ../../../os_library/include/ \
  -I ../../../os_host/lib/CMSIS/Device/STM32F10x/Include \
  -I ../../../os_host/lib/STM32F10x_StdPeriph_Driver/inc

arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/app.lds \
  ./main.o \
  ./streamer.o \
  ./webusb.o ./opencm3.o \
  -lbios_la104 -L../../../os_library/build -lm

arm-none-eabi-objdump -d -S output.elf > output.asm
arm-none-eabi-readelf -all output.elf > output.txt

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 79ccosc.elf

nm --print-size --size-sort -gC output.elf | grep " B " > symbols_ram.txt
nm --print-size --size-sort -gC output.elf | grep " T " > symbols_rom.txt
nm --print-size --size-sort -gC output.elf > symbols_all.txt
#objdump -s -j .dynamic output.elf