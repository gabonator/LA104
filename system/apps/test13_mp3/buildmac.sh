#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

export PATH="/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/":"$PATH"
mkdir -p build
cd build
rm output.elf

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -MD -D ENTRY -I ../ -c ../main.cpp ../MD_YX5300.cpp ../Arduino.cpp ../../host/source/framework/Wnd.cpp -I../../library/source/ 
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.ld ./main.o ./Arduino.o ./Wnd.o ./MD_YX5300.o -lbios -L../../library/build
arm-none-eabi-objdump -d -S output.elf > output.asm

../../../tools/elfstrip/elfstrip output.elf 13mp3.elf

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete
