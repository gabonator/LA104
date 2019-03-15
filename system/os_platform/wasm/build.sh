#!/bin/bash
INCLUDES="\
  -I ../../os_platform/common/include"

SOURCE="\
  source/wasmmain.cpp \
  ../../apps/test41_alchemy/source/main.cpp \
  ../common/source/framework/framework.cpp \
  ../common/source/bios/dbg.cpp \
  ../common/source/bios/fat.cpp \
  ../common/source/bios/gpio.cpp \
  ../common/source/bios/key.cpp \
  ../common/source/bios/lcd.cpp \
  ../common/source/bios/memory.cpp \
  ../common/source/bios/sys.cpp"

emcc ${INCLUDES} ${SOURCE} -g4 -s WASM=1 -s EXTRA_EXPORTED_RUNTIME_METHODS="['AsciiToString']" -s EXPORTED_FUNCTIONS="['AsciiToString', '_appLoop', '_appInit', '_appFinish', '_appAnalyse', '_appAnalyseResultPtr', '_appSetDataCount', '_appGetDataPtr']" -o app.js -DEMULATED -DEMSCRIPTEN --std=c++11 -s ERROR_ON_UNDEFINED_SYMBOLS=0
