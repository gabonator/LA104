#!/bin/bash
#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

export PATH="/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/":"$PATH"
mkdir -p build
cd build

SOURCES="../startup.c ../source/Main.c ../source/Menu.c ../source/Interrupt.c ../source/GUI.c ../source/AppBios.c ../source/GUI.c ../source/Ctrl.c ../source/Analyze.c ../source/Func.c ../source/Files.c ../source/Disk.c ../source/FAT12.c ../source/Ext_Flash.c ../lib/spf.c ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_flash.c ../lib/STM32F10x_StdPeriph_Driver/src/misc.c ../source/STM32F103_System.c ../lib/STM32_USB-FS-Device_Driver/src/usb_init.c ../lib/MSD/USB_bot.c ../lib/MSD/USB_scsi.c ../lib/STM32_USB-FS-Device_Driver/src/usb_regs.c ../lib/STM32_USB-FS-Device_Driver/src/usb_mem.c ../lib/MSD/USB_pwr.c ../lib/MSD/USB_prop.c ../lib/MSD/USB_desc.c ../lib/STM32_USB-FS-Device_Driver/src/usb_core.c ../lib/STM32_USB-FS-Device_Driver/src/usb_int.c ../lib/MSD/USB_istr.c"
OBJECTS="bios.o startup.o Main.o Menu.o Interrupt.o GUI.o AppBios.o Ctrl.o Analyze.o Func.o Files.o Disk.o FAT12.o spf.o Ext_Flash.o stm32f10x_spi.o stm32f10x_rcc.o stm32f10x_flash.o misc.o STM32F103_System.o usb_init.o USB_bot.o USB_scsi.o usb_regs.o usb_mem.o USB_pwr.o USB_prop.o USB_desc.o usb_core.o usb_int.o USB_istr.o"
INCLUDES="-I ../sources -I ../lib/STM32_USB-FS-Device_Driver/inc -I ../lib/MSD -I ../lib/CMSIS/Include -I ../lib/STM32F10x_StdPeriph_Driver/inc  -I .. -I ../source -I ../lib/CMSIS/Device/STM32F10x/Include"
DEFINES="-D USE_STDPERIPH_DRIVER -D STM32F10X_HD"

arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -c ../bios.s -o bios.o
arm-none-eabi-gcc  -Os -Werror -fno-common -mcpu=cortex-m3 -msoft-float -mthumb -MD ${DEFINES} ${INCLUDES} -c ${SOURCES}
arm-none-eabi-gcc  -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.ld ${OBJECTS} -lnosys -specs=nano.specs -specs=nosys.specs

arm-none-eabi-objcopy -O binary ./output.elf ./output.bin
arm-none-eabi-objcopy -O ihex ./output.elf ./output.hex

arm-none-eabi-readelf -all output.elf > output.txt
arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete
rm output.bin

