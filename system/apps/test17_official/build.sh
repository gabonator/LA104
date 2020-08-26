#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -c ../source/Main.c  ../source/Menu.c ../source/GUI.c ../source/Func.c ../source/Ctrl.c ../source/Analyze.c ../source/Interrupt.c ../source/AppBios.c -I../../../os_library/include/

#exit 1
#arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -Wl,--unresolved-symbols=ignore-all -T ../app.lds ./main.o 
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds ./Main.o ./Menu.o ./GUI.o ./Func.o ./Ctrl.o ./Analyze.o ./Interrupt.o ./AppBios.o -lbios_la104 -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 17analys.elf
#../../../elfdump/elfdump snake.elf

nm --print-size --size-sort -gC output.elf | grep " B " > symbols_ram.txt
nm --print-size --size-sort -gC output.elf | grep " T " > symbols_rom.txt
nm --print-size --size-sort -gC output.elf > symbols_all.txt

rm output.elf