cd ../build/apps
elfstrip=../../../tools/elfstrip/elfstrip

for f in *.elf; do printf "%-13s " "$f"; $elfstrip $f -q; done