#!/bin/bash
# prepares filesystem for DS203, all files should be copied into the device root
# contains latest and most useful applications to show capabilities of LA104

# at first run build_full.sh
FROM=../build/apps
TO=../build/image_ds203/

rm -rf $TO
mkdir $TO

(
  set -e
  cp $FROM/29fileman_ds203.elf $TO/shell.elf
  cp $FROM/21mplayd.elf $TO/midiplay.elf
  cp ../apps_featured/test21_midiplay/midi/laisla.mid $TO/laisla.mid
  cp $FROM/61devin.elf $TO/devinfo.elf
  cp $FROM/63ws_203.elf $TO/leds.elf
  cp $FROM/69webos0.elf $TO/webosc.elf
  cp $FROM/58gmin03.elf $TO/miniosc.elf
  cp $FROM/60gagen0.elf $TO/minigen.elf
  cp $FROM/99cube03.elf $TO/demo3d.elf
  # official fw not ported yet!        	
)

if [ $? -eq 1 ]; then
  echo [ERROR] File not found
  exit 1
fi

cd $TO
rm ds203apps.zip > /dev/null 2> /dev/null
zip -r ../ds203apps.zip *

if [ $? -eq 1 ]; then
  echo [ERROR] Compression failure
  exit 1
fi

echo [DONE] Packaging finished
