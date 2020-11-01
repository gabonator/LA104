#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build

arm-none-eabi-g++ -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-use-cxa-atexit -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -D _ARM -D STM32F10X_HD -c ../source/main.cpp ../source/WS2812B.cpp ../source/SPI_la104.cpp ../source/SPI_ds203.cpp ../source/SPI_dummy.cpp ../../../os_host/source/framework/Wnd.cpp -I../../../os_library/include/ -I ../../../os_host/lib/CMSIS/Device/STM32F10x/Include -I ../../../os_host/lib/STM32F10x_StdPeriph_Driver/inc -I ../../../os_host/lib/CMSIS/Include
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -fno-use-cxa-atexit -nostartfiles -T ../source/app.lds ./main.o ./WS2812B.o ./SPI_ds203.o ./SPI_la104.o ./SPI_dummy.o ./Wnd.o -lbios_la104 -lm -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm
arm-none-eabi-readelf -all output.elf > output.txt

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 63ws_104.elf

nm --print-size --size-sort -gC output.elf | grep " B " > symbols_ram.txt
nm --print-size --size-sort -gC output.elf | grep " T " > symbols_rom.txt
nm --print-size --size-sort -gC output.elf > symbols_all.txt
#objdump -s -j .dynamic output.elf