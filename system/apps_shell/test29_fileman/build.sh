#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build
rm output.elf

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -I ../ -I ../../../os_host/source/ -c ../source/main.cpp ../source/Manager.cpp ../../../os_host/source/framework/Wnd.cpp -I../../../os_library/include/ 
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds ./main.o ./Manager.o ./Wnd.o -lbios_la104 -L../../../os_library/build
arm-none-eabi-objdump -d -S output.elf > output.asm
../../../../tools/elfstrip/elfstrip output.elf 29fileman_la104.elf

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DDS213 -MD -I ../ -I ../../../os_host/source/ -c ../source/main.cpp ../source/Manager.cpp ../../../os_host/source/framework/Wnd.cpp -I../../../os_library/include/ 
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app.lds ./main.o ./Manager.o ./Wnd.o -lbios_ds213 -L../../../os_library/build
arm-none-eabi-objdump -d -S output.elf > output.asm
../../../../tools/elfstrip/elfstrip output.elf 29fileman_ds213.elf

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DDS203 -MD -I ../ -I ../../../os_host/source/ -c ../source/main.cpp ../source/Manager.cpp ../../../os_host/source/framework/Wnd.cpp -I../../../os_library/include/ 
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app_ds203.lds ./main.o ./Manager.o ./Wnd.o -lbios_ds203 -L../../../os_library/build
arm-none-eabi-objdump -d -S output.elf > output.asm
../../../../tools/elfstrip/elfstrip output.elf 29fileman_ds203.elf

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DDS203HD -MD -I ../ -I ../../../os_host/source/ -c ../source/main.cpp ../source/Manager.cpp ../../../os_host/source/framework/Wnd.cpp -I../../../os_library/include/ 
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../app_ds203.lds ./main.o ./Manager.o ./Wnd.o -lbios_ds203 -L../../../os_library/build
arm-none-eabi-objdump -d -S output.elf > output.asm
../../../../tools/elfstrip/elfstrip output.elf 29fileman_ds203hd.elf

echo DS203 memory placement:
nm --print-size --size-sort -gC output.elf > symbols_all.txt
cat symbols_all.txt | grep _address

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete
