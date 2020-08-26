#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build
rm output.elf

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -D ENTRY -I ../ -c ../main.cpp ../MD_YX5300.cpp ../Arduino.cpp ../../../os_host/source/framework/Wnd.cpp -I../../../os_library/include/ 
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds ./main.o ./Arduino.o ./Wnd.o ./MD_YX5300.o -lbios_la104 -L../../../os_library/build
arm-none-eabi-objdump -d -S output.elf > output.asm

../../../../tools/elfstrip/elfstrip output.elf 13mp3.elf
arm-none-eabi-readelf -all output.elf > output.txt


find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete
#rm output.elf