#!/bin/bash

set -x
mkdir output

cd ./system/os_library
./build.sh
cd ../os_host
./build.sh
cp ./build/system_la104.hex ../../output/
cd ../../tools/elfstrip
./build.sh
cd ../../system/apps_shell/test29_fileman 
./build.sh
cp ./build/29fileman_la104.elf ../../../output/shell.elf

