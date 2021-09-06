# bez libm
set -e

mkdir -p build
cd build

#arm-none-eabi-gcc -g -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -Wno-psabi -DLA104 -MD -DARDUINO -c \

#arm-none-eabi-g++ -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -MD -D LA104 -D _ARM -D STM32F10X_HD -c \
arm-none-eabi-g++ -g -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -D LA104 -D _ARM -D STM32F10X_HD -c \
  ../source/main.cpp \
../source/terminal-basic/HALProxyStream.cpp \
../source/terminal-basic/HAL_arduino.cpp \
../source/terminal-basic/HAL_avr8.cpp \
../source/terminal-basic/HAL_esp32.cpp \
../source/terminal-basic/HAL_esp8266.cpp \
../source/terminal-basic/HAL_sam.cpp \
../source/terminal-basic/basic_arduinoio.cpp \
../source/terminal-basic/basic_dataparser.cpp \
../source/terminal-basic/basic_exteeprom.cpp \
../source/terminal-basic/basic_extmemfs.cpp \
../source/terminal-basic/basic_functionblock.cpp \
../source/terminal-basic/basic_gfx.cpp \
../source/terminal-basic/basic_gfx_serial.cpp \
../source/terminal-basic/basic_gfx_tvout.cpp \
../source/terminal-basic/basic_gfx_utft.cpp \
../source/terminal-basic/basic_internalfuncs.cpp \
../source/terminal-basic/basic_interpreter_matrix.cpp \
../source/terminal-basic/basic_interpreterw.cpp \
../source/terminal-basic/basic_lexerw.cpp \
../source/terminal-basic/basic_math.cpp \
../source/terminal-basic/basic_parser_value.cpp \
../source/terminal-basic/basic_parserw.cpp \
../source/terminal-basic/basic_program.cpp \
../source/terminal-basic/bytearray.cpp \
../source/terminal-basic/liquidcrystalprint.cpp \
../source/terminal-basic/seriallight.cpp \
../source/terminal-basic/tvoutprint.cpp \
../source/terminal-basic/vt100.cpp \
../source/terminal-basic/HAL.c \
../source/terminal-basic/basic.c \
../source/terminal-basic/basic_lexer.c \
../source/terminal-basic/basic_lexer_en.c \
../source/terminal-basic/basic_lexer_fr.c \
../source/terminal-basic/basic_lexer_ru.c \
../source/terminal-basic/basic_parser.c \
../source/terminal-basic/basic_value.c \
../source/terminal-basic/tools.c \
  -I../source/ \
  -I../source/arduino \
  -I../source/terminal-basic \
  -I../../../os_library/include/ 

arm-none-eabi-gcc -g -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/app.lds \
main.o \
HAL.o \
HALProxyStream.o \
HAL_arduino.o \
HAL_avr8.o \
HAL_esp32.o \
HAL_esp8266.o \
HAL_sam.o \
basic_parser_value.o \
basic_parserw.o \
basic_program.o \
basic_value.o \
bytearray.o \
liquidcrystalprint.o \
seriallight.o \
tools.o \
tvoutprint.o \
vt100.o \
basic_gfx.o \
basic_gfx_serial.o \
basic_gfx_tvout.o \
basic_gfx_utft.o \
basic_internalfuncs.o \
basic_interpreter_matrix.o \
basic_interpreterw.o \
basic_lexer.o \
basic_lexer_en.o \
basic_lexer_fr.o \
basic_lexer_ru.o \
basic_lexerw.o \
basic_math.o \
basic_parser.o \
basic.o \
basic_arduinoio.o \
basic_dataparser.o \
basic_exteeprom.o \
basic_extmemfs.o \
basic_functionblock.o \
  -lbios_la104 -L../../../os_library/build -lm


arm-none-eabi-objdump -d -S output.elf > output.asm

nm --print-size --size-sort -gC output.elf | grep " B " > symbols_ram.txt
nm --print-size --size-sort -gC output.elf | grep " T " > symbols_rom.txt
nm --print-size --size-sort -gC output.elf > symbols_all.txt

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 118basic.elf
