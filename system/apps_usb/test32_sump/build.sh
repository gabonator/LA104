#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build


#arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -MD -D _ARM -D STM32F10X_HD -c \
#arm-none-eabi-gcc -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -Wno-psabi -MD -D _ARM -D STM32F10X_HD -c \
 arm-none-eabi-gcc -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -DLA104 -MD -D _ARM -D STM32F10X_HD -c \
  ../main.cpp ../if.c ../usbhelpers.c \
  ../sump/sump.c \
  ../sump/gpio.c \
  ../usb/sump_prop.c \
  ../usb/sump_app.c \
  ../usb/sump_desc.c \
  ../usb/sump_endp.c \
  ../usb/sump_pwr.c \
  -I../../../os_library/include/  -I ../../../os_host/library/CMSIS/Device/STM32F10x/Include -I ../../../os_host/library/CMSIS/Include   -I ../../../os_host/library/STM32_USB-FS-Device_Driver/inc 


#arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds \
arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds \
  ./main.o \
  ./if.o \
  ./usbhelpers.o \
  ./sump.o \
  ./gpio.o \
  ./sump_prop.o \
  ./sump_app.o \
  ./sump_desc.o \
  ./sump_endp.o \
  ./sump_pwr.o \
  -lbios_la104 -lnosys -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 32sump.elf