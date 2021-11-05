set -e
mkdir -p build
cd build
verilator -cc --exe -Os -x-assign 0 -Wall --assert --coverage --build -j -f ../../common/input.vc ../source/app.v ../../common/sim_main.cpp -CFLAGS "-I$(pwd)/../verify -I$(pwd)/../../common -D_VERILATOR -D_NOSPI"
obj_dir/Vapp