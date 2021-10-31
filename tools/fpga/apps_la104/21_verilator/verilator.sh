set -e
mkdir -p build
cd build
verilator -cc --exe -Os -x-assign 0 -Wall --assert --coverage --build -j -f ../verilator/input.vc ../source/app.v ../verilator/sim_main.cpp
obj_dir/Vapp