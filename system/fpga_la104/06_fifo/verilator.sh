set -e
mkdir -p build
cd build
cp ../source/identify.txt .
verilator --compiler gcc -cc --exe -Os -x-assign 0 -Wall --assert --coverage --build -j -f ../../common/input.vc ../source/app.v ../source/spi.v ../source/fifo.v ../../common/sim_main.cpp -CFLAGS "-I$(pwd)/../source -I$(pwd)/../../common -D_VERILATOR"
#clear
obj_dir/Vapp