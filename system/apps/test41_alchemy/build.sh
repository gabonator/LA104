#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

export PATH="/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/":"$PATH"
mkdir -p build
cd build

arm-none-eabi-g++ -g -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -MD -c \
  -D USE_STDPERIPH_DRIVER -D STM32F10X_HD \
  ../source/main.cpp \
  ../source/devices/infra/ir_tx.c \
  ../../../os_host/source/framework/Wnd.cpp \
  ../../../os_host/source/framework/Serialize.cpp \
  ../source/devices/infra/lib/CMSIS/Device/STM32F10x/Source/system_stm32f10x.c \
  ../source/devices/infra/lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c \
  ../source/devices/infra/lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c \
  ../source/devices/infra/lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c \
  ../source/devices/infra/lib/STM32F10x_StdPeriph_Driver/src/misc.c \
  -I../../../os_library/include/ \
  -I ../source/devices/infra/lib/STM32F10x_StdPeriph_Driver/inc -I ../source/devices/infra/lib/CMSIS/Include -I ../source/devices/infra/lib/CMSIS/Device/STM32F10x/Include -I ../source/devices/infra/lib

#arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -Wl,--unresolved-symbols=ignore-all -T ../app.lds ./main.o 
arm-none-eabi-gcc -g -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds \
  ./main.o \
  ./Wnd.o \
  ./Serialize.o \
  ./ir_tx.o \
  ./system_stm32f10x.o ./stm32f10x_gpio.o ./stm32f10x_rcc.o ./stm32f10x_tim.o ./misc.o \
  -lbios -lm -L../../../os_library/build

arm-none-eabi-objdump -d -S -C -l output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 41alche.elf
arm-none-eabi-readelf -all output.elf > output.txt
