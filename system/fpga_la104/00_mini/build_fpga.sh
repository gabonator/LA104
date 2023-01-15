echo \`include \"$1\" > app.v
../common/build_fpga.sh app.v $* ../common/app.asf
cp build/${PWD##*/}.fpg ${1%.*}.bin
cp ${1%.*}.bin /Volumes/LA104/bench.fpg
rm -rf build
rm log_quartus.html app.v


