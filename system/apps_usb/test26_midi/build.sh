#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build


#arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -MD -D _ARM -D STM32F10X_HD -c \
#arm-none-eabi-gcc -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -Wno-psabi -MD -D _ARM -D STM32F10X_HD -c \
 arm-none-eabi-gcc -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -MD -DLA104 -D _ARM -D STM32F10X_HD -c \
  ../main.cpp ../if.c ../usbhelpers.c \
  ../usb/midi_prop.c \
  ../usb/midi_desc.c \
  ../usb/midi_endp.c \
  ../usb/midi_app.c \
  ../usb/midi_pwr.c \
  -I../../../os_library/include/  -I ../../../os_host/library/CMSIS/Device/STM32F10x/Include -I ../../../os_host/library/CMSIS/Include   -I ../../../os_host/library/STM32_USB-FS-Device_Driver/inc 

#arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds \
arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds \
  ./main.o \
  ./if.o \
  ./usbhelpers.o \
  ./midi_prop.o \
  ./midi_app.o \
  ./midi_desc.o \
  ./midi_endp.o \
  ./midi_pwr.o \
  -lbios_la104 -lnosys -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 26midi.elf