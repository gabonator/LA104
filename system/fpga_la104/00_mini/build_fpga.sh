echo \`include \"$1\" > app.v
../common/build_fpga.sh app.v $* ../common/app.asf

mkdir -p build
mv app.fpg build/${1%.*}.fpg
touch -r $1 build/${1%.*}.fpg
mv app_quartus.html build/${1%.*}_quartus.html
rm app.v
exit 0
cp build/${PWD##*/}.fpg ${1%.*}.bin
cp ${1%.*}.bin /Volumes/LA104/bench.fpg
rm -rf build
rm log_quartus.html app.v


