#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads


mkdir -p build
cd build

arm-none-eabi-g++ -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -MD -D DS213 -D _ARM -D STM32F10X_HD -c \
  ../source/main.cpp \
  ../source/MainWnd.cpp \
  ../source/ToolBar.cpp \
  ../source/Shapes.cpp \
  ../source/Settings.cpp \
  ../source/Oscilloscope/Controls/GraphOsc.cpp \
  ../source/Oscilloscope/Core/CoreOscilloscope.cpp \
  ../source/Oscilloscope/Disp/MenuDisp.cpp \
  ../source/Oscilloscope/Input/MenuInput.cpp \
  ../source/Oscilloscope/Marker/MenuMarker.cpp \
  ../source/Oscilloscope/Mask/MenuMask.cpp \
  ../source/Oscilloscope/Math/ChannelMath.cpp \
  ../source/Oscilloscope/Math/FirFilter.cpp \
  ../source/Oscilloscope/Math/MenuMath.cpp \
  ../source/Oscilloscope/Meas/MenuMeas.cpp \
  ../source/Oscilloscope/Meas/Statistics.cpp \
  ../../../os_host/source/framework/Wnd.cpp \
  ../../../os_host/source/framework/Utils.cpp \
  ../../../os_host/source/framework/Serialize.cpp \
  ../../../os_host/source/gui/Controls.cpp \
  ../source/Framework/Sampler.cpp \
  -I../../../os_library/include/ \
  -I../source/

arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/app.lds \
  ./main.o \
  ./MainWnd.o \
  ./ToolBar.o \
  ./Wnd.o \
  ./Utils.o \
  ./Shapes.o \
  ./GraphOsc.o ./CoreOscilloscope.o ./MenuDisp.o ./MenuInput.o ./MenuMarker.o ./MenuMask.o \
  ./ChannelMath.o ./FirFilter.o ./MenuMath.o ./MenuMeas.o ./Statistics.o \
  ./Sampler.o ./Settings.o ./Controls.o ./Serialize.o \
  -lbios_ds213 -L../../../os_library/build -lm

arm-none-eabi-objdump -d -S output.elf > output.asm
arm-none-eabi-readelf -all output.elf > output.txt

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 55gosc.elf

nm --print-size --size-sort -gC output.elf | grep " B " > symbols_ram.txt
nm --print-size --size-sort -gC output.elf | grep " T " > symbols_rom.txt
nm --print-size --size-sort -gC output.elf > symbols_all.txt

#objdump -s -j .dynamic output.elf
