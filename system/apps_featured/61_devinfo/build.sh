#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build

arm-none-eabi-g++ -g3 -ggdb -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -MD -D DS213 -D _ARM -D STM32F10X_HD -c ../main.cpp ../../../os_host/source/framework/Serialize.cpp -I../../../os_library/include/
arm-none-eabi-gcc -g3 -ggdb -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds ./main.o ./Serialize.o -lbios_ds213 -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm
arm-none-eabi-readelf -all output.elf > output.txt

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 61devin.elf

nm --print-size --size-sort -gC output.elf | grep " B " > symbols_ram.txt
nm --print-size --size-sort -gC output.elf | grep " T " > symbols_rom.txt
nm --print-size --size-sort -gC output.elf > symbols_all.txt
#objdump -s -j .dynamic output.elf



#arm-none-eabi-g++ -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -MD -D LA104 -D _ARM -D STM32F10X_HD -c ../main.cpp ../ds203.cpp ../ds213.cpp ../la104.cpp -I../../../os_library/include/
#arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds ./main.o ./ds203.o ./ds213.o ./la104.o -lbios_la104 -L../../../os_library/build
#../../../../tools/elfstrip/elfstrip output.elf 61devin_la104.elf
#arm-none-eabi-g++ -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -MD -D DS203 -D _ARM -D STM32F10X_HD -c ../main.cpp ../ds203.cpp ../ds213.cpp ../la104.cpp -I../../../os_library/include/
#arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds ./main.o ./ds203.o ./ds213.o ./la104.o -lbios_ds203 -L../../../os_library/build
#../../../../tools/elfstrip/elfstrip output.elf 61devin_ds203.elf
#arm-none-eabi-g++ -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -MD -D DS213 -D _ARM -D STM32F10X_HD -c ../main.cpp ../ds203.cpp ../ds213.cpp ../la104.cpp -I../../../os_library/include/
#arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds ./main.o ./ds203.o ./ds213.o ./la104.o -lbios_ds213 -L../../../os_library/build
#../../../../tools/elfstrip/elfstrip output.elf 61devin_ds213.elf
