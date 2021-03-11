# bez libm
set -e

mkdir -p build
cd build

arm-none-eabi-gcc -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -Wno-psabi -DLA104 -MD -fno-builtin-sscanf -fno-builtin-snprintf -fno-builtin-vasprintf -DPC_HOSTED=1 -D STM32F10X_HD -DSTM32F1 -c \
../source/blackmagic/gdb_main.c \
../source/blackmagic/gdb_packet.c \
../source/blackmagic/gdb_hostio.c \
../source/blackmagic/hex_utils.c \
../source/blackmagic/command.c \
../source/blackmagic/crc32.c \
../source/blackmagic/morse.c \
../source/target/target.c \
../source/blackmagic/exception.c \
../source/webusb/opencm3.c \
../source/webusb/webusb.c \
  -I../source/blackmagic \
  -I../source/ \
  -I../source/target

arm-none-eabi-g++ -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-builtin-sscanf -fno-builtin-snprintf -fno-builtin-vasprintf  -Wno-psabi -MD -D DS213 -D _ARM -D STM32F10X_HD -c \
  ../source/platform.cpp \
  ../source/main.cpp \
  -I../source/ \
  -I../source/target \
  -I../../../os_library/include/

#arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/app.lds \
# -specs=nosys.specs
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -fno-builtin-sscanf -fno-builtin-snprintf -fno-builtin-vasprintf -nostartfiles \
  -T ../source/app.lds \
  main.o \
  gdb_main.o \
  gdb_packet.o \
  gdb_hostio.o \
  hex_utils.o \
  command.o \
  exception.o \
  target.o \
  crc32.o \
  morse.o \
  opencm3.o \
  webusb.o \
  platform.o \
  -lbios_la104 -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm

nm --print-size --size-sort -gC output.elf | grep " B " > symbols_ram.txt
nm --print-size --size-sort -gC output.elf | grep " T " > symbols_rom.txt
nm --print-size --size-sort -gC output.elf > symbols_all.txt

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 112stlin.elf
