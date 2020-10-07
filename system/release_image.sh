# prepares filesystem for LA104, all files be copied into the device root
# contains latest and most useful applications to show capabilities of LA104

# at first run buildall.sh

FROM=release/bin
TO=release/image_la104/

mkdir release
mkdir $TO

cp $FROM/29fileman_la104.elf $TO/shell.elf
cp $FROM/29fileman_la104.elf $TO/fileman.elf
mkdir $TO/midi
cp $FROM/21mplayl.elf $TO/midi/midiplay.elf
cp apps_featured/test21_midiplay/midi/laisla.mid $TO/midi/laisla.mid
cp $FROM/61devin.elf $TO/devinfo.elf

mkdir $TO/i2c
cp $FROM/37icscan.elf $TO/i2c/i2cscan.elf
cp apps_featured/test37_i2cscan/devices.txt $TO/i2c/devices.txt
cp $FROM/85eetest.elf $TO/i2c/eeview.elf

mkdir $TO/rftool
cp $FROM/80rftool.elf $TO/rftool/rftool.elf
cp $FROM/79ccosc.elf $TO/rftool/wusbosc.elf
cp $FROM/82sanal.elf $TO/rftool/spectrum.elf
cp apps_featured/80_rftool/logs/*.* $TO/rftool/
mkdir $TO/eink
cp $FROM/90eink.elf $TO/eink/displimg.elf
cp apps_featured/90_epd_image/res/*.bmp $TO/eink/

cp $FROM/35viewla.elf $TO/viewtxt.elf

mkdir $TO/tools
cp $FROM/30dcf77.elf $TO/tools/dcf77.elf
cp $FROM/22seqen.elf $TO/tools/sequen.elf
cp $FROM/72uartm.elf $TO/tools/uartmon.elf
cp $FROM/73invt.elf $TO/tools/vfd_invt.elf # rs485
cp $FROM/33temper.elf $TO/tools/dstemper.elf
cp $FROM/15charla.elf $TO/tools/charmap.elf
cp $FROM/63ws_la.elf $TO/tools/ws2812.elf
cp $FROM/49gpio.elf $TO/tools/gpio.elf

mkdir $TO/devices
cp $FROM/45simcom.elf $TO/devices/simcom.elf
cp $FROM/14apds1.elf $TO/devices/apds9960.elf
cp $FROM/9dht.elf $TO/devices/dht22.elf
cp $FROM/57ds3231.elf $TO/devices/ds3231.elf
cp $FROM/34scope.elf $TO/devices/pcf8591t.elf
cp $FROM/11i2c.elf $TO/devices/bmp085.elf
cp $FROM/56ds1307.elf $TO/devices/ds1307.elf
cp $FROM/36espwww.elf $TO/devices/esp8266.elf
cp $FROM/84maxdis.elf $TO/devices/max7219.elf
cp $FROM/13mp3.elf $TO/devices/yx5300.elf
