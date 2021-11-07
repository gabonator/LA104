# bez libm
set -e

mkdir -p build
cd build

arm-none-eabi-g++ -g -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -D LA104 -D _ARM -D STM32F10X_HD -c \
  ../source/main.cpp \
  -I../source/ \
  -I../source/arduino \
  -I../source/terminal-basic \
  -I../../../os_library/include/ 

arm-none-eabi-gcc -g -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/app.lds \
main.o \
  -lbios_la104 -L../../../os_library/build -lm

arm-none-eabi-objdump -d -S output.elf > output.asm

../../../../tools/elfstrip/elfstrip output.elf 119bkpt.elf
