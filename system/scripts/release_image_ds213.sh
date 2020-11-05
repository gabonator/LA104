#!/bin/bash
# prepares filesystem for DS213, all files should be copied into the device root
# contains latest and most useful applications to show capabilities of LA104

# at first run build_full.sh
FROM=../build/apps
TO=../build/image_ds213/

rm -rf $TO
mkdir $TO

(
  set -e
  cp $FROM/29fileman_ds213.elf $TO/shell.elf
  cp $FROM/21mplay2.elf $TO/midiplay.elf
  cp ../apps_featured/test21_midiplay/midi/laisla.mid $TO/laisla.mid
  cp $FROM/61devin.elf $TO/devinfo.elf
  cp $FROM/63ws_213.elf $TO/leds.elf
  cp $FROM/69webos1.elf $TO/webosc.elf
  cp $FROM/58gmin13.elf $TO/miniosc.elf
  cp $FROM/60gagen1.elf $TO/minigen.elf
  cp $FROM/52osc213.elf $TO/oscillo.elf
  cp $FROM/99cube13.elf $TO/demo3d.elf
)

if [ $? -eq 1 ]; then
  echo [ERROR] File not found
  exit 1
fi

cd $TO
rm ds213apps.zip > /dev/null 2> /dev/null
zip -r ../ds213apps.zip *

if [ $? -eq 1 ]; then
  echo [ERROR] Compression failure
  exit 1
fi

echo [DONE] Packaging finished
