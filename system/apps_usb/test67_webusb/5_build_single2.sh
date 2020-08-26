#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build


#arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -MD -D _ARM -D STM32F10X_HD -c \
#arm-none-eabi-gcc -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -Wno-psabi -MD -D _ARM -D STM32F10X_HD -c \

 arm-none-eabi-gcc -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -DSTM32F1 -DLA104 -MD -D _ARM -D STM32F10X_HD -c \
  ../source_single2/main.cpp \
  ../source_single2/webusb.c \
  ../source_single2/opencm3.c \
  -I../source/ \
  -I../../../os_library/include/ \
  -I ../lib/libopencm3/include \
  -I ../../../os_host/library/CMSIS/Include \
  -I ../../../os_host/library/STM32_USB-FS-Device_Driver/inc 


#arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds \
arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/app.lds \
  ./main.o \
  ./webusb.o \
  ./opencm3.o \
  -lbios_la104 -lnosys -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 67-6wusb.elf
nm --print-size --size-sort -gC output.elf > symbols_all.txt
cat symbols_all.txt | grep _address

