# prepares filesystem for LA104, all files should be copied into the device root
# contains latest and most useful applications to show capabilities of LA104

# at first run buildall.sh
FROM=release/bin
TO=release/image_ds213/
mkdir release
mkdir $TO

# shell
cp $FROM/29fileman_ds213.elf $TO/shell.elf
cp $FROM/21mplay2.elf $TO/midiplay.elf
cp $FROM/61devin.elf $TO/devinfo.elf
cp $FROM/63ws_213.elf $TO/leds.elf
cp $FROM/69webos1.elf $TO/webosc.elf
cp $FROM/58gmin13.elf $TO/miniosc.elf
cp $FROM/60gagen1.elf $TO/minigen.elf
cp $FROM/52osc213.elf $TO/oscillo.elf

cd $TO
rm ds213apps.zip
zip -r ../ds213apps.zip *
