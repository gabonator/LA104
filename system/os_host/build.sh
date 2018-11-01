#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

export PATH="/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/":"$PATH"
mkdir -p build
cd build

INCLUDES="-I ../sources -I ../lib/STM32_USB-FS-Device_Driver/inc -I ../lib/MSD -I ../lib/CMSIS/Include -I ../lib/STM32F10x_StdPeriph_Driver/inc  -I .. -I ../source -I ../lib/CMSIS/Device/STM32F10x/Include -I ../lib"

arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -c ../bios.s
arm-none-eabi-gcc -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -D USE_STDPERIPH_DRIVER -D STM32F10X_HD ${INCLUDES} -c ../startup.c ../interrupt.c ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_flash.c ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c ../lib/STM32_USB-FS-Device_Driver/src/usb_init.c ../lib/MSD/USB_scsi.c ../source/usb/Disk.c ../lib/MSD/USB_bot.c ../lib/STM32_USB-FS-Device_Driver/src/usb_mem.c ../lib/STM32_USB-FS-Device_Driver/src/usb_regs.c ../lib/CMSIS/Device/STM32F10x/Source/system_stm32f10x.c ../lib/MSD/USB_prop.c ../lib/STM32_USB-FS-Device_Driver/src/usb_core.c ../lib/STM32_USB-FS-Device_Driver/src/usb_int.c ../lib/MSD/USB_istr.c ../lib/MSD/USB_desc.c ../lib/MSD/USB_pwr.c ../source/bios/fatfs/Ext_Flash.c ../source/bios/imports.c ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_i2c.c ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c ../lib/STM32F10x_StdPeriph_Driver/src/misc.c
arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -c -Wall -Werror -D USE_STDPERIPH_DRIVER -D STM32F10X_HD ${INCLUDES} -c ../main.cpp ../source/Manager.cpp ../source/framework/Wnd.cpp ../source/bios/sys.cpp ../source/bios/lcd.cpp ../source/bios/memory.cpp ../source/bios/fat.cpp ../source/bios/key.cpp ../source/framework/Utils.cpp ../source/bios/dbg.cpp ../source/bios/gpio.cpp ../source/library/spf.c ../source/Execute.cpp ../source/Framework/Serialize.cpp ../source/gui/Gui.cpp
arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.ld ./main.o ./startup.o ./interrupt.o ./sys.o ./Wnd.o ./lcd.o ./memory.o ./fat.o ./key.o ./Utils.o ./dbg.o ./spf.o ./Execute.o ./Serialize.o ./imports.o ./stm32f10x_flash.o ./stm32f10x_spi.o ./stm32f10x_rcc.o ./Manager.o ./usb_init.o ./USB_scsi.o ./Disk.o ./USB_bot.o ./usb_mem.o ./usb_regs.o ./system_stm32f10x.o ./USB_prop.o ./usb_core.o ./usb_int.o ./USB_istr.o ./USB_desc.o ./USB_pwr.o ./Ext_Flash.o ./Gui.o ./stm32f10x_gpio.o ./stm32f10x_tim.o ./gpio.o ./stm32f10x_i2c.o ./stm32f10x_usart.o ./misc.o

arm-none-eabi-objcopy -O binary ./output.elf ./output.bin
arm-none-eabi-objcopy -O ihex ./output.elf ./manager.hex

arm-none-eabi-readelf -all output.elf > output.txt
arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete
