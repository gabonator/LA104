#export PATH="/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/":"$PATH"
#https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

mkdir _buildarm
cd _buildarm
cmake .. -DARM=1 -DCMAKE_INSTALL_PREFIX=../_install -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE:PATH="..\toolchain.cmake" -DCMAKE_TOOLCHAIN_PATH="/Users/gabrielvalky/Downloads/gcc-arm-none-eabi-7-2018-q2-update/bin/"
cmake --build .