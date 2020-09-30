# run buildall.sh

FROM=release/bin
TO=release/image_la104/

mkdir release
mkdir $TO

cp $FROM/29fileman_la104.elf $TO/shell.elf
cp $FROM/49gpio.elf $TO/gpio.elf
mkdir $TO/midi
cp $FROM/21mplayl.elf $TO/midi/midiplay.elf
cp apps_featured/test21_midiplay/midi/laisla.mid $TO/midi/laisla.mid
cp $FROM/61devin.elf $TO/devinfo.elf
cp $FROM/63ws2812.elf $TO/ws2812.elf
mkdir $TO/i2c
cp $FROM/37icscan.elf $TO/i2c/i2cscan.elf
cp apps_featured/test37_i2cscan/devices.txt $TO/i2c/devices.txt
cp $FROM/85eetest.elf $TO/i2c/eeview.elf
cp $FROM/15charla.elf $TO/charmap.elf
cp $FROM/33temper.elf $TO/dstemper.elf
mkdir $TO/rftool
cp $FROM/80rftool.elf $TO/rftool/rftool.elf
cp $FROM/79ccosc.elf $TO/rftool/wusbosc.elf
cp $FROM/82sanal.elf $TO/rftool/spectrum.elf
cp apps_featured/80_rftool/logs/*.* $TO/rftool/

