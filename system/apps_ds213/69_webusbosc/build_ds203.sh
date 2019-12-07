#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

export PATH="/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/":"$PATH"
mkdir -p build
cd build

arm-none-eabi-gcc -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -DSTM32F1 -DDS203 -MD -D _ARM -D STM32F10X_HD -c \
  -I../../../os_library/include/ \
  ../source/main.cpp ../source/webusb.c

arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/app.lds \
  ./main.o ./webusb.o -lbios_ds203 -lnosys -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 68webdrw.elf
nm --print-size --size-sort -gC output.elf > symbols_all.txt
cat symbols_all.txt | grep _address

cp 68webdrw.elf /Volumes/LA104/68webdrw.elf
