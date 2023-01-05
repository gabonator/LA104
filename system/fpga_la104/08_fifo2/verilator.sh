set -e
mkdir -p build
cd build
verilator -cc --exe -x-assign 0 --assert --coverage --build -j -D_VERILATOR -f ../../common/input.vc \
  ../source/app.v ../../common/sim_main.cpp \
  -CFLAGS '-I../../source -I../../../common -D_VERILATOR -DVL_PRINTF=my_printf -DVL_VERILATED_INCLUDE=\"../../../common/override.h\"'

obj_dir/Vapp