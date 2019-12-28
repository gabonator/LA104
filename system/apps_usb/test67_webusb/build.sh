cd joiner
./join_opencm3.sh
./join_webusb_minimal.sh
cp *.c ../source_single2
cp *.h ../source_single2
cd ..
./5_build_single2.sh
