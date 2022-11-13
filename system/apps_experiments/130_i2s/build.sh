#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
set -e

mkdir -p build
cd build

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -c \
  -I ../../../os_host/library/CMSIS/Include/ \
  -I ../../../os_host/library/STM32F10x_StdPeriph_Driver/inc/ \
  -I ../../../os_host/library/CMSIS/Device/STM32F10x/Include/ \
  -I ../../../os_host/source/ \
  -D USE_STDPERIPH_DRIVER \
  -D STM32F10X_HD \
  ../source/main.cpp \
  ../../../os_host/library/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c \
  ../../../os_host/library/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c \
  -I../../../os_library/include/

arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/app.lds \
  ./main.o ./stm32f10x_spi.o ./stm32f10x_rcc.o -lbios_la104 -lm -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 130mic.elf
