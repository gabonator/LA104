#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build

arm-none-eabi-gcc -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -DSTM32F1 -DDS203 -MD -D _ARM -D STM32F10X_HD -c \
  -I../../../os_library/include/ \
  ../source/v3/main.cpp ../source/v3/webusb/webusb.c ../source/v3/webusb/opencm3.c ../source/v3/streamer/streamer.cpp

arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/v3/app.lds \
  ./main.o ./webusb.o ./opencm3.o ./streamer.o -lbios_ds203 -lnosys -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 69webos0.elf
nm --print-size --size-sort -gC output.elf > symbols_all.txt
cat symbols_all.txt | grep _address

#cp 69webosc.elf ../release
#cp 69webosc.elf /Volumes/LA104/69webosc.elf
