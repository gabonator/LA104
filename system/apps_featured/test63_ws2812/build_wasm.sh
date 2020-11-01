#!/bin/bash
mkdir build
cd build 

# git clone https://github.com/emscripten-core/emsdk.git
# ./emsdk install latest
# ./emsdk activate latest
# check "which emcc" and remove references in /usr/local
# call emsdk_env.sh to setup environment, or :
PATH=$PATH:/Users/gabrielvalky/Documents/git/ext/emsdk/upstream/emscripten
PATH=$PATH:/Users/gabrielvalky/Documents/git/ext/emsdk/upstream/bin

INCLUDES="\
  -I ../../../os_platform/common/include
  -I ../../../os_host/source"

SOURCE="\
  ../../../os_platform/wasm/source/wasmmain.cpp \
  ../source/main.cpp \
  ../source/SPI_dummy.cpp \
  ../source/WS2812b.cpp \
  ../../../os_platform/common/source/framework/framework.cpp \
  ../../../os_platform/common/source/bios/dbg.cpp \
  ../../../os_platform/common/source/bios/gpio.cpp \
  ../../../os_platform/common/source/bios/key.cpp \
  ../../../os_platform/common/source/bios/lcd.cpp \
  ../../../os_platform/common/source/bios/memory.cpp \
  ../../../os_platform/common/source/bios/sys.cpp"

#  ../../../os_platform/common/source/bios/fat.cpp \

EXPORTED="['_appLoop', '_appInit', '_appFinish']"

#emcc ${INCLUDES} ${SOURCE} -g4 -O3 -s TOTAL_STACK=1024 -s TOTAL_MEMORY=65536 -s ENVIRONMENT="web" -s TEXTDECODER=2 -s MALLOC=emmalloc  -s WASM=1 -s EXTRA_EXPORTED_RUNTIME_METHODS="['AsciiToString']" -s EXPORTED_FUNCTIONS="${EXPORTED}" -o app.js -DEMULATED -DEMSCRIPTEN -DLA104 --std=c++11 -s ERROR_ON_UNDEFINED_SYMBOLS=0 --js-library ../../../os_platform/wasm/html/library_app.js
#node ../../../os_platform/wasm/html/package.js

emcc ${INCLUDES} ${SOURCE} -g4 -O3 -s TOTAL_STACK=1024 -s TOTAL_MEMORY=65536 -s MINIMAL_RUNTIME=1 -s WASM=1 -s EXPORTED_FUNCTIONS="${EXPORTED}" -o ws2812.js -DEMULATED -DEMSCRIPTEN -DLA104 --std=c++11 -s ERROR_ON_UNDEFINED_SYMBOLS=0 || exit 1
node ../../../os_platform/wasm/htmllite/package.js ws2812


