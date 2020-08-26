#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build
rm output.elf

arm-none-eabi-gcc -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -DLA104 -MD -D USE_STDPERIPH_DRIVER -D STM32F10X_HD -I ../ -I ../lib/STM32F10x_StdPeriph_Driver/inc -I ../lib/CMSIS/Include -I ../lib/CMSIS/Device/STM32F10x/Include -c ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_i2c.c
arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -DLA104 -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -D USE_STDPERIPH_DRIVER -D STM32F10X_HD -MD -I ../ -I ../lib/STM32F10x_StdPeriph_Driver/inc -I ../lib/CMSIS/Include -I ../lib/CMSIS/Device/STM32F10x/Include -c ../main.cpp -I../../../os_library/include/ 
#arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -Wl,--unresolved-symbols=ignore-all -T ../app.lds ./main.o 
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds ./main.o ./stm32f10x_i2c.o ./stm32f10x_gpio.o ./stm32f10x_rcc.o -lbios_la104 -L../../../os_library/build -lm #-lc -lm

arm-none-eabi-objdump -d -S output.elf > output.asm

../../../../tools/elfstrip/elfstrip output.elf 10i2c.elf
# ../../../elfdump/elfdump vimeas1.elf

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete
