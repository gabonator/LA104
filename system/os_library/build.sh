#!/bin/bash
#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

mkdir -p build
cd build

if [ "$1" == "ds203" ]; then
	arm-none-eabi-g++ -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -D DS203 -MD -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -c -Wall -Werror ../source/library.cpp || exit 1
	arm-none-eabi-gcc -fPIC -nostartfiles -mcpu=cortex-m3 -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float -shared -o libbios_ds203.so library.o || exit 1
fi

if [ "$1" == "ds213" ]; then
	arm-none-eabi-g++ -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -D DS213 -MD -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -c -Wall -Werror ../source/library.cpp || exit 1
	arm-none-eabi-gcc -fPIC -nostartfiles -mcpu=cortex-m3 -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float -shared -o libbios_ds213.so library.o || exit 1
fi

if [ "$1" == "la104" ]; then
	arm-none-eabi-g++ -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -D LA104 -MD -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -c -Wall -Werror ../source/library.cpp || exit 1
	arm-none-eabi-gcc -fPIC -nostartfiles -mcpu=cortex-m3 -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float -shared -o libbios_la104.so library.o || exit 1
fi

arm-none-eabi-g++ -Wall -Os -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -D LA104 -D DS213 -MD -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -c -Wall -Werror ../source/library.cpp || exit 1
arm-none-eabi-gcc -fPIC -nostartfiles -mcpu=cortex-m3 -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float -shared -o libbios.so library.o || exit 1

#arm-none-eabi-nm --demangle ./libbios.so
find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

cd ..
if [ "`which node`" == "" ]; then
  if [ "`which apt`" != "" ]; then
    echo "Installing nodejs..."
    sudo apt install -y nodejs
  else
    echo "[ERROR] nodejs not found!"
    exit 1
  fi
fi
node exports.js > ../os_host/source/main/Shared.h
rm -f build/libbios.so
