#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build

arm-none-eabi-g++ -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -DARDUINO=1000 -MD -D _ARM -D STM32F10X_HD -c \
  ../source/main.cpp \
  ../../../os_host/source/framework/Serialize.cpp \
  ../../../os_host/source/framework/Wnd.cpp \
  ../source/epd/epd.cpp \
  ../source/epd/epdif.cpp \
  -I../../../os_library/include/ 
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/app.lds \
  ./main.o \
  ./epd.o \
  ./epdif.o \
  ./Serialize.o \
  ./Wnd.o \
  -lbios_la104 -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm
arm-none-eabi-readelf -all output.elf > output.txt

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 90eink.elf
