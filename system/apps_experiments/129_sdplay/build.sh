#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
set -e

mkdir -p build
cd build

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -c \
  ../source/main.cpp ../source/FFT.cpp \
  -I../../../os_library/include/

arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/app.lds ./main.o ./FFT.o -lbios_la104 -lm -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 129sdpcm.elf
