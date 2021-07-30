#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

mkdir -p build
cd build

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -D DS213 -D _ARM -D STM32F10X_HD -MD -c \
	../main.cpp \
	../../../os_host/source/framework/Wnd.cpp \
  ../../../os_host/source/framework/Utils.cpp \
  ../../../os_host/source/framework/Serialize.cpp \
  ../../../os_host/source/gui/Controls.cpp \
  -I../../../os_library/include/ 
#arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -Wl,--unresolved-symbols=ignore-all -T ../app.lds ./main.o 
arm-none-eabi-gcc -Os -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds \
 ./main.o \
 ./Serialize.o \
 ./Utils.o -lbios_ds213 -L../../../os_library/build -lm

arm-none-eabi-objdump -d -S output.elf > output.asm

../../../../tools/elfstrip/elfstrip output.elf 9dht.elf
# ../../../elfdump/elfdump vimeas1.elf

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete