#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build
rm output.elf

cp ../ColorSensor.ino code.cpp
arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -I ../arduino/ -I ../ -c code.cpp ../arduino/main.cpp ../arduino/Arduino.cpp -I../../library/source/ ../SparkFun_APDS9960.cpp
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../arduino/app.lds ./main.o ./Arduino.o ./SparkFun_APDS9960.o ./code.o -lbios_la104 -L../../../os_library/build -lm

arm-none-eabi-objdump -d -S output.elf > output.asm

../../../../tools/elfstrip/elfstrip output.elf 14apds1.elf
rm output.elf

cp ../GestureTest.ino code.cpp
arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -I ../arduino/ -I ../ -c code.cpp ../arduino/main.cpp ../arduino/Arduino.cpp -I../../library/source/ ../SparkFun_APDS9960.cpp
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../arduino/app.lds ./main.o ./Arduino.o ./SparkFun_APDS9960.o ./code.o -lbios_la104 -L../../../os_library/build -lm

arm-none-eabi-objdump -d -S output.elf > output.asm

../../../../tools/elfstrip/elfstrip output.elf 14apds2.elf
rm output.elf

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete
rm code.cpp
