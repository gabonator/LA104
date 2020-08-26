#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build

arm-none-eabi-g++  -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -c \
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

arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds \
  ./main.o \
  ./Wnd.o \
  ./Serialize.o \
  ./ir_tx.o \
  ./system_stm32f10x.o ./stm32f10x_gpio.o ./stm32f10x_rcc.o ./stm32f10x_tim.o ./misc.o \
  -lbios_la104 -lm -L../../../os_library/build

#arm-none-eabi-strip --strip-unneeded output.elf
arm-none-eabi-objdump -d -S -C -l output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 41alche.elf
arm-none-eabi-readelf -all output.elf > output.txt

arm-none-eabi-nm --print-size --size-sort -gC output.elf | grep " B " > symbols_ram.txt
arm-none-eabi-nm --print-size --size-sort -gC output.elf | grep " T " > symbols_rom.txt
arm-none-eabi-nm --print-size --size-sort -gC output.elf > symbols_all.txt
