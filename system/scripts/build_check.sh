#!/bin/bash
echo Checking building environment

if [ "`which arm-none-eabi-gcc`" == "" ]; then
  echo "[ERROR] arm-none-eabi-gcc not found!"
  echo "[ERROR] download here: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads"
  exit 1
fi

if [ "`which node`" == "" ]; then
  echo "[ERROR] node.js not found!"
  exit 1
fi

if [ "`which convert`" == "" ]; then
  echo "[ERROR] Image magick convert not found!"
  exit 1
fi

cd ../../tools/elfstrip/
./build.sh

if [ ! -f elfstrip ]; then
  echo "[ERROR] Failed to build elfstrip utility"
  exit 1
fi
