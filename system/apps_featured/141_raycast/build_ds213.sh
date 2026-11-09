#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
set -e

mkdir -p build
cd build

python3 ../tools/gen_assets.py ../source/assets.h

arm-none-eabi-g++ -Wall -O2 -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DDS213 -MD -D _ARM -c ../source/main.cpp ../source/game.cpp -I../../../os_library/include/
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/app_ds213.lds ./main.o ./game.o -lbios_ds213 -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm
arm-none-eabi-size output.elf

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf raycst13.elf
