#!/bin/bash

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

./build_check.sh
if [ $? -eq 1 ]; then
  echo [ERROR] Building environment not configured properly
  exit 1
fi

./build_os.sh
if [ $? -eq 1 ]; then
  echo [ERROR] Building OS failed!
  exit 1
fi

./build_apps.sh
if [ $? -eq 1 ]; then
  echo [ERROR] Building Apps failed!
  exit 1
fi
