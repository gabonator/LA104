set -e
mkdir -p build
cd build
cp ../cnt4bitonsck_msb3.v app.v
verilator -cc --exe -x-assign 0 --assert --coverage --build -j -D_VERILATOR -f ../../common/input.vc \
  app.v ../test.cpp \
  -CFLAGS '-I../../source -I../../../common -D_VERILATOR -DVL_PRINTF=my_printf -DVL_VERILATED_INCLUDE=\"../../../common/override.h\"'
cd ..
build/obj_dir/Vapp > log.js
cat log.js