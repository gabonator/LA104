#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build


#arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -MD -D _ARM -D STM32F10X_HD -c \
#arm-none-eabi-gcc -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -Wno-psabi -MD -D _ARM -D STM32F10X_HD -c \

 arm-none-eabi-gcc -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -DSTM32F1 -DLA104 -MD -D _ARM -D STM32F10X_HD -c \
  ../source_full_clean/main.cpp \
  ../source_full_clean/usb_conf.c \
  ../source_full_clean/webusb.c \
  ../source_full_clean/cdc.c \
  ../source_full_clean/usb21_standard.c \
  ../source_full_clean/winusb.c \
  ../lib/libopencm3/lib/usb/usb.c \
  ../lib/libopencm3/lib/usb/usb_standard.c \
  ../lib/libopencm3/lib/usb/usb_control.c \
  ../lib/libopencm3/lib/stm32/common/st_usbfs_core.c \
  ../lib/libopencm3/lib/stm32/common/rcc_common_all.c \
  ../lib/libopencm3/lib/stm32/desig.c \
  ../lib/libopencm3/lib/stm32/st_usbfs_v1.c \
  ../lib/libopencm3/lib/cm3/assert.c \
  -I../source/ \
  -I../../../os_library/include/ \
  -I ../lib/libopencm3/include \
  -I ../../../os_host/library/CMSIS/Include \
  -I ../../../os_host/library/STM32_USB-FS-Device_Driver/inc 


#arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds \
arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/app.lds \
  ./main.o \
  ./usb_conf.o \
  ./webusb.o \
  ./cdc.o \
  ./usb.o \
  ./usb_standard.o \
  ./rcc_common_all.o \
  ./usb_control.o \
  ./st_usbfs_v1.o \
  ./st_usbfs_core.o \
  ./assert.o \
  ./desig.o \
  ./usb21_standard.o \
  ./winusb.o \
  -lbios_la104 -lnosys -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 67webusb.elf
nm --print-size --size-sort -gC output.elf > symbols_all.txt
cat symbols_all.txt | grep _address

cp 67webusb.elf /Volumes/LA104/66webusb.elf
