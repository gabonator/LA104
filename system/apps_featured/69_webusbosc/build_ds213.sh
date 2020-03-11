#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

export PATH="/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/":"$PATH"
mkdir -p build
cd build

arm-none-eabi-gcc -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -DSTM32F1 -DDS213 -MD -D _ARM -D STM32F10X_HD -c \
  -I../../../os_library/include/ \
  ../source/v3_ds213/main.cpp ../source/v3_ds213/ds213/adc.cpp ../source/v3_ds213/ds213/dac.cpp ../source/v3_ds213/webusb/webusb.c ../source/v3_ds213/webusb/opencm3.c ../source/v3_ds213/streamer/streamer.cpp

arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/v3_ds213/app.lds \
  ./main.o ./adc.o ./dac.o ./webusb.o ./opencm3.o ./streamer.o -lbios_ds203 -lnosys -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 69webosc.elf
nm --print-size --size-sort -gC output.elf > symbols_all.txt
cat symbols_all.txt | grep _address

#cp 69webosc.elf ../release
#cp 69webosc.elf /Volumes/LA104/69webosc.elf
