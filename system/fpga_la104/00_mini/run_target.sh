rm /Volumes/LA104/bench.htm
cp build/${1%.*}.fpg /Volumes/LA104/bench.fpg
sleep 2
# sleep 2 good 1 bad!
diskutil unmount /dev/disk4
sleep 2
#sleep 5
diskutil mount /dev/disk4
cp /Volumes/LA104/bench.htm build/${1%.*}_target.html
touch -r build/${1%.*}.fpg build/${1%.*}_target.html
