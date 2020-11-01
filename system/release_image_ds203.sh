# prepares filesystem for LA104, all files should be copied into the device root
# contains latest and most useful applications to show capabilities of LA104

# at first run buildall.sh
FROM=release/bin
TO=release/image_ds203/
mkdir release
mkdir $TO

# shell
cp $FROM/29fileman_ds203.elf $TO/shell.elf
cp $FROM/21mplayd.elf $TO/midiplay.elf
cp $FROM/61devin.elf $TO/devinfo.elf
cp $FROM/63ws_203.elf $TO/leds.elf
cp $FROM/69webos0.elf $TO/webosc.elf
cp $FROM/58gmin03.elf $TO/miniosc.elf
cp $FROM/60gagen0.elf $TO/minigen.elf
# official fw not ported yet!        	

cd $TO
rm ds203apps.zip
zip -r ../ds203apps.zip *
