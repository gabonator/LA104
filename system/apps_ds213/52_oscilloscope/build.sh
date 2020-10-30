#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build

arm-none-eabi-g++ -Wall -Os -fpermissive -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DDS213 -MD -c \
-DSTM32F10X_HD \
../app/Main.cpp \
../app/Menu.c \
../app/Draw.c \
../app/Calibrat.c \
../app/Process.c \
../app/Sys.c \
../pub/File.c \
../pub/Func.c \
-I../../../os_library/include/ \
-I../pub/ \
-I../app/

arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds \
./Main.o \
./Menu.o \
./Draw.o \
./Calibrat.o \
./Process.o \
./Sys.o \
./File.o \
./Func.o \
-lbios_ds213 -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

nm --print-size --size-sort -gC output.elf | grep " B " > symbols_ram.txt
nm --print-size --size-sort -gC output.elf | grep " T " > symbols_rom.txt
nm --print-size --size-sort -gC output.elf > symbols_all.txt

../../../../tools/elfstrip/elfstrip output.elf 52osc213.elf
#../../../elfdump/elfdump snake.elf
rm output.elf
