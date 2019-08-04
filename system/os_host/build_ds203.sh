#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

GITREVISION=`git log --pretty=format:'%h' -n 1`

export PATH="/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/":"$PATH"
mkdir -p build
cd build

INCLUDES="\
  -I ../sources \
  -I ../lib/STM32_USB-FS-Device_Driver/inc \
  -I ../lib/MSD \
  -I ../lib/CDC \
  -I ../lib/CMSIS/Include \
  -I ../lib/STM32F10x_StdPeriph_Driver/inc \
  -I .. \
  -I ../source \
  -I ../lib/CMSIS/Device/STM32F10x/Include \
  -I ../lib"

arm-none-eabi-gcc -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD \
  -D USE_STDPERIPH_DRIVER \
  -D STM32F10X_HD \
  -D DS203 \
  ${INCLUDES} -c \
  ../startup.c \
  ../interrupt.c \
  ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_flash.c \
  ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c \
  ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c \
  ../lib/STM32_USB-FS-Device_Driver/src/usb_init.c \
  ../lib/STM32_USB-FS-Device_Driver/src/usb_mem.c \
  ../lib/STM32_USB-FS-Device_Driver/src/usb_core.c \
  ../lib/STM32_USB-FS-Device_Driver/src/usb_int.c \
  ../lib/STM32_USB-FS-Device_Driver/src/usb_regs.c \
  ../lib/STM32_USB-FS-Device_Driver/src/usb_sil.c \
  ../lib/CMSIS/Device/STM32F10x/Source/system_stm32f10x.c \
  ../source/bios/imports.c \
  ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c \
  ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c \
  ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_i2c.c \
  ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c \
  ../lib/STM32F10x_StdPeriph_Driver/src/misc.c \
  ../lib/CDC/cdcusb_endp.c \
  ../lib/CDC/cdcusb_desc.c \
  ../lib/CDC/cdcusb_prop.c \
  ../lib/CDC/cdcusb_app.c \
  ../source/usb/Disk.c \
  ../lib/MSD/msdusb_scsi.c \
  ../lib/MSD/msdusb_bot.c \
  ../lib/MSD/msdusb_desc.c \
  ../lib/MSD/msdusb_prop.c \
  ../lib/MSD/msdusb_endp.c \
  ../lib/COMMON/commonusb_pwr.c \
  ../lib/COMMON/commonusb_istr.c \
  ../lib/COMMON/commonusb_app.c \


#  ../lib/MSD/msdusb_istr.c \
#  ../lib/MSD/msdusb_pwr.c \

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -c -Wall -Werror \
  -D __USER__=\"$USER\" \
  -D __OSTYPE__=\"$OSTYPE\" \
  -D __GITREVISION__=\"$GITREVISION\" \
  -D USE_STDPERIPH_DRIVER \
  -D STM32F10X_HD \
  -D DS203 \
  ${INCLUDES} -c \
  ../main.cpp \
  ../source/bios/sys.cpp \
  ../source/bios/lcd.cpp \
  ../source/bios/memory.cpp \
  ../source/bios/fat.cpp \
  ../source/bios/key.cpp \
  ../source/bios/os.cpp \
  ../source/bios/usb.cpp \
  ../source/bios/dbg.cpp \
  ../source/bios/gpio.cpp \
  ../source/library/spf.c \
  ../source/Execute.cpp \
  ../source/gui/Gui.cpp \
  ../source/Framework/Serialize.cpp \

#  ../source/framework/Wnd.cpp \
#  ../source/framework/Utils.cpp \
#  ../source/Manager.cpp \

arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app_ds203.ld \
  ./main.o \
  ./startup.o \
  ./interrupt.o \
  ./sys.o \
  ./lcd.o \
  ./memory.o \
  ./fat.o \
  ./key.o \
  ./dbg.o \
  ./os.o \
  ./usb.o \
  ./spf.o \
  ./Execute.o \
  ./imports.o \
  ./stm32f10x_flash.o \
  ./stm32f10x_spi.o \
  ./stm32f10x_rcc.o \
  ./usb_init.o \
  ./usb_mem.o \
  ./usb_regs.o \
  ./usb_core.o \
  ./usb_int.o \
  ./usb_sil.o \
  ./system_stm32f10x.o \
  ./stm32f10x_gpio.o \
  ./stm32f10x_tim.o \
  ./gpio.o \
  ./stm32f10x_i2c.o \
  ./stm32f10x_usart.o \
  ./misc.o \
  ./cdcusb_prop.o \
  ./cdcusb_desc.o \
  ./cdcusb_endp.o \
  ./cdcusb_app.o \
  ./Disk.o \
  ./msdusb_scsi.o \
  ./msdusb_bot.o \
  ./msdusb_prop.o \
  ./msdusb_desc.o \
  ./msdusb_endp.o \
  ./commonusb_pwr.o \
  ./commonusb_istr.o \
  ./commonusb_app.o \
  ./Gui.o \
  ./Serialize.o \

#  ./Wnd.o \
#  ./Utils.o \
#  ./Manager.o \
#  ./msdusb_istr.o \
#  ./msdusb_pwr.o \


arm-none-eabi-objcopy -O binary ./output.elf ./output.bin
arm-none-eabi-objcopy -O ihex ./output.elf ./system.hex

arm-none-eabi-readelf -all output.elf > output.txt
arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

nm --print-size --size-sort -gC output.elf | grep " B " > symbols_ram.txt
nm --print-size --size-sort -gC output.elf | grep " T " > symbols_rom.txt
nm --print-size --size-sort -gC output.elf > symbols_all.txt
nm output.elf > symbols_all2.txt
