set -e
for f in *.v 
do
  if [[ "$f" -nt build/${f%.*}_verilated.html ]]; then
    echo Verilating $f
    ./build_verilator.sh $f
  fi

  if [[ "$f" -nt build/${f%.*}.fpg ]]; then
    echo Building $f
    ./build_fpga.sh $f
  fi

  if [[ "$f" -nt build/${f%.*}_target.html ]]; then
    echo Running $f
    ./run_target.sh $f
  fi
done

exit 0
for f in *.v 
do
  if [[ "$f" -nt build/${f%.*}_verilated.html ]]; then
    echo Verilating $f
    ./build_verilator.sh $f
  fi
done

for f in *.v 
do
  if [[ "$f" -nt build/${f%.*}.fpg ]]; then
    echo Building $f
    ./build_fpga.sh $f
  fi
done