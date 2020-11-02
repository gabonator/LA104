#!/bin/bash

(
  echo Building library...
  cd ../os_library
  rm -rf build
  ./build.sh
  if [ ! -f build/libbios_la104.so ]; then
    echo Unable to build shared library prototype for LA104
    return 1
  fi
  if [ ! -f build/libbios_ds203.so ]; then
    echo Unable to build shared library prototype for DS203
    return 1
  fi
  if [ ! -f build/libbios_ds213.so ]; then
    echo Unable to build shared library prototype for DS213
    return 1
  fi
)

if [ $? -eq 1 ]; then
    exit 1
fi

(
  echo Building host...
  cd ../os_host
  rm -rf build
  #TODO: docker - cant see any output from this script??
  ./build.sh
  if [ ! -f build/system_la104.hex ]; then
    echo Unable to build operating system firmware for LA104
    return 1
  fi
  if [ ! -f build/system_ds203.hex ]; then
    echo Unable to build operating system firmware for DS203
    return 1
  fi
  if [ ! -f build/system_ds213.hex ]; then
    echo Unable to build operating system firmware for DS213
    return 1
  fi
)

if [ $? -eq 1 ]; then
    exit 1
fi
