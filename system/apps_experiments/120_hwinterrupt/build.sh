#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
set -e                                                      

mkdir -p build
cd build
#  ../../../os_host/library/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c 

#arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -D _ARM -D STM32F10X_HD -c 
arm-none-eabi-g++ -g -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -D LA104 -D _ARM -D STM32F10X_HD -c \
  ../main.cpp \
  ../isr.c \
  -I../../../os_library/include/ -I ../../../os_host/library/CMSIS/Device/STM32F10x/Include -I ../../../os_host/library/STM32F10x_StdPeriph_Driver/inc -I ../../../os_host/library/CMSIS/Include -I ../../../os_host/library/
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds \
  ./main.o \
  ./isr.o \
  -lbios_la104 -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm
arm-none-eabi-readelf -all output.elf > output.txt

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 115keyb.elf