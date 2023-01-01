set -e
mkdir -p build
cd build
verilator -cc --exe -Os -x-assign 0 -Wall --assert --coverage --build -j -f ../../common/input.vc ../source/app.v ../source/spi_slave.v ../../common/sim_main.cpp \
  -CFLAGS '-I../../source -I../../../common -D_VERILATOR -DVL_PRINTF=my_printf -DVL_VERILATED_INCLUDE=\"../../../common/override.h\"'

#verilator -cc --exe -Os -x-assign 0 -Wall --assert --coverage --build -j -f ../../common/input.vc ../source/app.v ../source/spi_slave.v ../../common/sim_main.cpp \
#  -CFLAGS '-I/Users/gabrielvalky/Documents/git/LA104/system/fpga_la104/98_spi2/source -I/Users/gabrielvalky/Documents/git/LA104/system/fpga_la104/98_spi2//../common -D_VERILATOR -DVL_PRINTF=my_printf -DVL_VERILATED_INCLUDE=\"../../../common/override.h\"'

#verilator -cc --exe -Os -x-assign 0 -Wall --assert --coverage --build -j -f ../../common/input.vc ../source/app.v ../source/spi_slave.v ../../common/sim_main.cpp -CFLAGS "-I$(pwd)/../source -I$(pwd)/../../common -D_VERILATOR" -FI hovno.h
obj_dir/Vapp