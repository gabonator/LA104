#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build
rm output.elf

#cp ../Multibus_Simple.ino code.cpp
cp ../DS18x20_Temperature.ino code.cpp

arm-none-eabi-g++ -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -DARDUINO="100" -DONEWIRE_CRC8_TABLE="0" -I ../arduino/ -I ../ -I ../lib/ -c code.cpp ../arduino/main.cpp ../arduino/Arduino.cpp -I../../library/source/ ../lib/OneWire.cpp ../lib/DallasTemperature.cpp
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../arduino/app.lds ./main.o ./Arduino.o ./OneWire.o ./code.o -lbios_la104 -L../../../os_library/build -lm  
# ./DallasTemperature.o

arm-none-eabi-objdump -d -S output.elf > output.asm

../../../../tools/elfstrip/elfstrip output.elf 18onew.elf
rm output.elf

