#!/bin/bash

echo Building script for apple M1 computers. Toolchain is currently available only as x86 build, and needs to be executed through rosetta.
echo Use: "softwareupdate --install-rosetta" to install rosetta.
echo Download and install the toolchain package \(gcc-arm-none-eabi-xxxx.pkg\) from here "https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads"

ERRORS=0
VERBOSE=0
if [ "$1" == "verbose" ]; then
  set -x
  VERBOSE=1
fi

OUTPUT=../build
rm -rf $OUTPUT
mkdir $OUTPUT
mkdir $OUTPUT/firmware
mkdir $OUTPUT/apps

PATH=/Applications/ARM/bin/:$PATH
#./build_check.sh
#if [ $? -eq 1 ]; then
#  echo [ERROR] Building environment not configured properly
#  exit 1
#fi

/usr/bin/arch -x86_64 ./build_os.sh
if [ $? -eq 1 ]; then
  echo [ERROR] Building OS failed!
  exit 1
fi

/usr/bin/arch -x86_64 ./build_apps.sh
if [ $? -eq 1 ]; then
  echo [ERROR] Building Apps failed!
  exit 1
fi

./generate_memlist.sh

./release_image_ds203.sh
./release_image_ds213.sh
./release_image_la104.sh
