#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
set -e
APPNAME=${PWD##*/}

mkdir -p build
cd build

arm-none-eabi-g++ -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -DLA104 -MD -D _ARM -c ../verify/verify.cpp -I../../../../../system/os_library/include/
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../../common/verify.lds ./verify.o -lbios_la104 -lm -L../../../../../system/os_library/build

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../../tools/elfstrip/elfstrip output.elf $APPNAME.vlf