#!/bin/bash
echo Checking building environment

if [ "`which arm-none-eabi-gcc`" == "" ]; then
  echo "[ERROR] arm-none-eabi-gcc not found!"
  exit 1
fi

if [ "`which node`" == "" ]; then
  echo "[ERROR] node.js not found!"
  exit 1
fi

cd ../../tools/elfstrip/
./build.sh

if [ ! -f elfstrip ]; then
  echo "[ERROR] Failed to build elfstrip utility"
  exit 1
fi
