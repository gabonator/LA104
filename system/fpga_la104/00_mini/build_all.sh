for f in *.v 
do
  if [[ "$f" -nt build/${f%.*}.fpg ]]; then
    echo Building $f
    ./build_fpga.sh $f
  fi
done