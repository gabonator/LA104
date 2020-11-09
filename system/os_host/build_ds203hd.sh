#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

set -e

GITREVISION=`git log --pretty=format:'%h' -n 1`
TARGET=DS203HD


mkdir -p build
cd build

INCLUDES="\
  -I ../sources \
  -I ../library/STM32_USB-FS-Device_Driver/inc \
  -I ../library/MSD \
  -I ../library/CDC \
  -I ../library/CMSIS/Include \
  -I ../library/STM32F10x_StdPeriph_Driver/inc \
  -I .. \
  -I ../source \
  -I ../library/CMSIS/Device/STM32F10x/Include \
  -I ../library"

arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -c ../source/bios/ds203/BIOS.s -o BIOS.o

arm-none-eabi-gcc -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD \
  -D USE_STDPERIPH_DRIVER \
  -D STM32F10X_HD \
  -D $TARGET \
  ${INCLUDES} -c \
  ../source/startup.c \
  ../source/interrupt.c \
  ../library/CMSIS/Device/STM32F10x/Source/system_stm32f10x.c \
  ../source/bios/imports.c \
  ../library/STM32F10x_StdPeriph_Driver/src/misc.c

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -c -Wall -Werror \
  -D __USER__=\"$USER\" \
  -D __OSTYPE__=\"$OSTYPE\" \
  -D __GITREVISION__=\"$GITREVISION\" \
  -D DISABLE_USB \
  -D DISABLE_GPIO \
  -D USE_STDPERIPH_DRIVER \
  -D STM32F10X_HD \
  -D $TARGET \
  ${INCLUDES} -c \
  ../source/main.cpp \
  ../source/bios/sys.cpp \
  ../source/bios/lcd.cpp \
  ../source/bios/memory.cpp \
  ../source/bios/fat.cpp \
  ../source/bios/key.cpp \
  ../source/bios/os.cpp \
  ../source/bios/dbg.cpp \
  ../source/bios/adc.cpp \
  ../source/bios/dac.cpp \
  ../source/bios/private.cpp \
  ../library/spf/spf.c \
  ../source/main/Execute.cpp \
  ../source/gui/Gui.cpp \
  ../source/framework/Serialize.cpp \

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
  ./spf.o \
  ./Execute.o \
  ./imports.o \
  ./system_stm32f10x.o \
  ./misc.o \
  ./Gui.o \
  ./Serialize.o \
  ./adc.o \
  ./dac.o \
  ./private.o \

arm-none-eabi-objcopy -O binary ./output.elf ./output.bin
arm-none-eabi-objcopy -O ihex ./output.elf ./system_ds203hd.hex

arm-none-eabi-readelf -all output.elf > output.txt
arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

nm --print-size --size-sort -gC output.elf | grep " B " > symbols_ram.txt
nm --print-size --size-sort -gC output.elf | grep " T " > symbols_rom.txt
nm --print-size --size-sort -gC output.elf > symbols_all.txt
nm output.elf > symbols_all2.txt
cat symbols_all.txt | grep _address
