set -e
mkdir -p build
cd build
verilator --compiler gcc -cc --exe -Os -x-assign 0 -Wall --assert --coverage --build -j -f ../../common/input.vc ../source/app.v ../../common/sim_main.cpp -CFLAGS "-I$(pwd)/../source -I$(pwd)/../../common -D_VERILATOR"
obj_dir/Vapp