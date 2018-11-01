#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

export PATH="/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/":"$PATH"
mkdir -p build
cd build

arm-none-eabi-gcc -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -I ../ -c ../main.c -D USE_STDPERIPH_DRIVER -D STM32F10X_HD -I ../ -I ../lib/STM32F10x_StdPeriph_Driver/inc -I ../lib/CMSIS/Include -I ../lib/CMSIS/Device/STM32F10x/Include ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c ../lib/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c
arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds ./main.o ./stm32f10x_rcc.o ./stm32f10x_tim.o ./stm32f10x_gpio.o

arm-none-eabi-objcopy -O binary ./output.elf ./output.bin
arm-none-eabi-objcopy -O ihex ./output.elf ./output.hex
arm-none-eabi-readelf -all output.elf > output.txt
arm-none-eabi-objdump -d -S output.elf > output.asm

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

cp output.hex 6pwm.hex