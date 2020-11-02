#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

mkdir -p build
cd build

arm-none-eabi-g++ -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -MD -D DS213 -D _ARM -D STM32F10X_HD -c \
  ../main.cpp \
  ../MainMenu.cpp \
  ../MainWnd.cpp \
  ../ToolBar.cpp \
  ../Shapes.cpp \
  ../../../os_host/source/framework/Wnd.cpp \
  ../../../os_host/source/framework/Utils.cpp \
  -I../../../os_library/include/ 

arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds \
  ./main.o \
  ./MainMenu.o \
  ./MainWnd.o \
  ./ToolBar.o \
  ./Wnd.o \
  ./Utils.o \
  ./Shapes.o \
  -lbios_ds213 -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm
arm-none-eabi-readelf -all output.elf > output.txt

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 54shell.elf

nm --print-size --size-sort -gC output.elf | grep " B " > symbols_ram.txt
nm --print-size --size-sort -gC output.elf | grep " T " > symbols_rom.txt
nm --print-size --size-sort -gC output.elf > symbols_all.txt
#cat symbols_all.txt | grep _address
