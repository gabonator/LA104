#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

export PATH="/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/":"$PATH"
mkdir -p build
cd build

arm-none-eabi-gcc -Wall -Os -fpermissive -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DDS203 -MD -c ../source/BIOS.S
arm-none-eabi-gcc -Wall -Os -fpermissive -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DDS203 -MD -c \
-DSTM32F10X_HD \
../source/Main.c \
../source/Menu.c \
../source/Draw.c \
../source/Calibrat.c \
../source/Process.c \
../source/Files.c \
../source/Function.c \
../source/Interrupt.c \
../source/system.cpp \
-I../../../os_library/include/

arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds \
./Main.o \
./Menu.o \
./Draw.o \
./Calibrat.o \
./Process.o \
./Files.o \
./Function.o \
./Interrupt.o \
./system.o \
-lbios_ds203 -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

nm --print-size --size-sort -gC output.elf | grep " B " > symbols_ram.txt
nm --print-size --size-sort -gC output.elf | grep " T " > symbols_rom.txt
nm --print-size --size-sort -gC output.elf > symbols_all.txt

../../../../tools/elfstrip/elfstrip output.elf 70wildca.elf
#../../../elfdump/elfdump snake.elf
#rm output.elf