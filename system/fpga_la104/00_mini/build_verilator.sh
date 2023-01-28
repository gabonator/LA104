set -e
mkdir -p build
cd build

fname=$(basename $1)
fbname=${fname%.*}

cp ../${1} app.v
verilator -cc --exe -x-assign 0 --assert --coverage --build -j -D_VERILATOR -f ../../common/input.vc \
  app.v ../test.cpp \
  -CFLAGS '-DNAME_VERILOG=\"${1}\" -DNAME_TEST=\"test.cpp\" -I../../source -I../../../common -D_VERILATOR -DVL_PRINTF=my_printf -DVL_VERILATED_INCLUDE=\"../../../common/override.h\"' > /dev/null

rm app.v
obj_dir/Vapp > ${fbname}_verilated.html 
touch -r ../${fname} ${fbname}_verilated.html

#> {$1}_verilated.html
#cat log.html