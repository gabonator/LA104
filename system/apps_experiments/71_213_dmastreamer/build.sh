#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build

arm-none-eabi-gcc -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -Wno-psabi -MD -D STM32F10X_HD -D DS213 -D _ARM -D STM32F1 -D STM32F10X_HD -c ../source/main.cpp ../source/streamer/streamer.cpp ../source/webusb/opencm3.c ../source/webusb/webusb.c -I../../../os_library/include/
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/app.lds main.o streamer.o opencm3.o webusb.o -lbios_ds213 -L../../../os_library/build
../../../../tools/elfstrip/elfstrip output.elf 71strm.elf
