#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build
rm output.elf

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -I ../ -c ../main.cpp -I../../../os_library/include/ 
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds ./main.o -lbios_la104 -L../../../os_library/build -lm

arm-none-eabi-objdump -d -S output.elf > output.asm

../../../../tools/elfstrip/elfstrip output.elf 11i2c.elf
# ../../../elfdump/elfdump vimeas1.elf

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete
