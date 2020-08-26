#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build

arm-none-eabi-g++ -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -D _ARM -D STM32F10X_HD -D STM32F103xB -c \
  ../source/main.cpp \
  ../source/streamer/streamer.cpp \
  -I ../../../os_library/include/ \
  -I ../../../os_host/lib/CMSIS/Device/STM32F10x/Include \
  -I ../../../os_host/lib/STM32F10x_StdPeriph_Driver/inc \
  -I ../source/streamer/Drivers/CMSIS/Include \
  -I ../source/streamer/Drivers/CMSIS/Device/ST/STM32F1xx/Include \
  -I ../source/streamer/Drivers/CMSIS/Device/ST/STM32F1xx/Include \
  -I ../source/streamer/Drivers/STM32F1xx_HAL_Driver/Inc \
  -I ../source/streamer/Drivers

#  ../source/streamer/dma2.cpp \
#  ../source/streamer/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
#  ../source/streamer/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
#  ../source/streamer/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
#  ../source/streamer/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
#  ../source/streamer/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
#  ../source/streamer/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
#  ../source/streamer/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \

#  -I ../../../os_host/lib/CMSIS/Include 

arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/app.lds \
  ./main.o \
  ./streamer.o \
  -lbios_la104 -L../../../os_library/build -lm

#  ./stm32f1xx_hal.o \
#  ./stm32f1xx_hal_cortex.o \
#  ./stm32f1xx_hal_tim.o \
#  ./stm32f1xx_hal_tim_ex.o \
#  ./stm32f1xx_hal_dma.o \
#  ./stm32f1xx_hal_gpio.o \
#  ./stm32f1xx_hal_rcc.o \
#

arm-none-eabi-objdump -d -S output.elf > output.asm
arm-none-eabi-readelf -all output.elf > output.txt

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 76sender.elf

nm --print-size --size-sort -gC output.elf | grep " B " > symbols_ram.txt
nm --print-size --size-sort -gC output.elf | grep " T " > symbols_rom.txt
nm --print-size --size-sort -gC output.elf > symbols_all.txt
#objdump -s -j .dynamic output.elf