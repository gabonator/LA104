#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -c ../main.cpp ../../../os_host/source/framework/Wnd.cpp ../CharMap.cpp -I../../../os_library/include/
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds ./main.o ./Wnd.o ./CharMap.o -lbios_la104 -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 15charla.elf

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DDS203 -MD -c ../main.cpp ../../../os_host/source/framework/Wnd.cpp ../CharMap.cpp -I../../../os_library/include/
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds ./main.o ./Wnd.o ./CharMap.o -lbios_ds203 -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 15chards.elf

rm output.elf