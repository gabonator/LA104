#!/bin/bash
mkdir build
cd build 

PATH=$PATH:/Users/gabrielvalky/Documents/git/ext/emsdk/upstream/emscripten
PATH=$PATH:/Users/gabrielvalky/Documents/git/ext/emsdk/upstream/bin

INCLUDES="\
  -I ../../../os_platform/common/include
  -I ../../../os_host/source"

SOURCE="\
  ../../../os_platform/wasm/source/wasmmain.cpp \
  ../source/main.cpp \
  ../source/Manager.cpp \
  ../../../os_platform/common/source/framework/framework.cpp \
  ../../../os_platform/common/source/bios/dbg.cpp \
  ../../../os_platform/common/source/bios/gpio.cpp \
  ../../../os_platform/common/source/bios/key.cpp \
  ../../../os_platform/common/source/bios/lcd.cpp \
  ../../../os_platform/common/source/bios/fat.cpp \
  ../../../os_platform/common/source/bios/memory.cpp \
  ../../../os_platform/common/source/bios/sys.cpp"

EXPORTED="['_appLoop', '_appInit', '_appFinish', '__ZN4BIOS2OS11SetArgumentEPc', '__ZN4BIOS2OS11GetArgumentEv', '__ZN4BIOS2OS11HasArgumentEv']"

emcc ${INCLUDES} ${SOURCE} -g4 -O3 -s TOTAL_STACK=1024 -s TOTAL_MEMORY=65536 -s ENVIRONMENT="web" -s TEXTDECODER=2 -s MALLOC=emmalloc -s WASM=1 -s EXPORTED_FUNCTIONS="${EXPORTED}" -o fileman.js -DEMULATED -DEMSCRIPTEN -DLA104 --std=c++11 -s ERROR_ON_UNDEFINED_SYMBOLS=0 || exit 1
node ../../../os_platform/wasm/htmllite/package.js fileman

#emcc ${INCLUDES} ${SOURCE} -g4 -O3 -s TOTAL_STACK=32768 -s TOTAL_MEMORY=262144 -s MINIMAL_RUNTIME=1 -s WASM=1 -s EXPORTED_FUNCTIONS="${EXPORTED}" -o app.js -DEMSCRIPTEN -DLA104 --std=c++11 -s ERROR_ON_UNDEFINED_SYMBOLS=0
#node ../../../os_platform/wasm/html/package.js

#cp app.wasm /Users/gabrielvalky/Documents/git/LA104/system/release/wasm/apps/fileman.elf.wasm