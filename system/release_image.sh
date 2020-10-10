# prepares filesystem for LA104, all files be copied into the device root
# contains latest and most useful applications to show capabilities of LA104

# at first run buildall.sh
FROM=release/bin
TO=release/image_la104/
ICON=../tools/shellicons
mkdir release
mkdir $TO


# shell
#cp $FROM/29fileman_la104.elf $TO/shell.elf
cp $FROM/28shell.elf $TO/shell.elf

cp $FROM/29fileman_la104.elf $TO/fileman.elf
cp $ICON/manager.bmp $TO/fileman.bmp
echo -n "{\"description\":{short:\"File manager\",long:\"File manager\"},\"icon\":\"fileman.bmp\",\"execute\":\"fileman.elf\",order:20}" > $TO/fileman.lnk

cp $FROM/28shell.elf $TO/gui.elf

# root
cp $FROM/35viewla.elf $TO/viewtxt.elf
cp $ICON/genie.bmp $TO/help.bmp
cp help.txt $TO/help.txt
echo -n "{\"description\":{short:\"Help\",long:\"Help\"},\"icon\":\"help.bmp\",\"execute\":\"viewtxt.elf help.txt\",order:10}" > $TO/help.lnk

cp $FROM/61devin.elf $TO/devinfo.elf
cp $ICON/info.bmp $TO/devinfo.bmp
echo -n "{\"description\":{\"short\":\"Dev info\",\"long\":\"Device info\"},\"icon\":\"devinfo.bmp\",\"execute\":\"devinfo.elf\",order:-10}" > $TO/devinfo.lnk

# midi
mkdir $TO/midi
cp $ICON/midi.bmp $TO/midi/midi.bmp
echo -n "{\"description\":{\"short\":\"MIDI\",\"long\":\"MIDI tools\"},\"icon\":\"midi.bmp\"}" > $TO/midi/index.lnk

cp $FROM/21mplayl.elf $TO/midi/midiplay.elf
cp apps_featured/test21_midiplay/midi/laisla.mid $TO/midi/laisla.mid
cp $ICON/music.bmp $TO/midi/midiplay.bmp
echo -n "{\"description\":{\"short\":\"MIDI\",\"long\":\"MIDI player\"},\"icon\":\"midiplay.bmp\",\"execute\":\"midiplay.elf\"}" > $TO/midi/midiplay.lnk

# i2c
mkdir $TO/i2c
cp $ICON/fi2c.bmp $TO/i2c/i2c.bmp
echo -n "{\"description\":{\"short\":\"I2C\",\"long\":\"I2C bus tools\"},\"icon\":\"i2c.bmp\",order:8}" > $TO/i2c/index.lnk

cp $FROM/37icscan.elf $TO/i2c/i2cscan.elf
cp $ICON/magnifier.bmp $TO/i2c/i2cscan.bmp
echo -n "{\"description\":{\"short\":\"I2C scanner\",\"long\":\"I2C bus scanner\"},\"icon\":\"i2cscan.bmp\",execute:\"i2cscan.elf\",order:10}" > $TO/i2c/i2cscan.lnk

cp apps_featured/test37_i2cscan/devices.txt $TO/i2c/devices.txt
cp $FROM/85eetest.elf $TO/i2c/eeview.elf
cp $ICON/eeprom.bmp $TO/i2c/eeview.bmp
echo -n "{\"description\":{\"short\":\"EEPROM viewer\",\"long\":\"I2C EEPROM memory viewer\"},\"icon\":\"eeview.bmp\",execute:\"eeview.elf\",order:5}" > $TO/i2c/eeview.lnk

# rftools
mkdir $TO/rftools
cp $ICON/fant.bmp $TO/rftools/rftools.bmp
echo -n "{\"description\":{\"short\":\"RF toolkit\",\"long\":\"RF toolkit\"},\"icon\":\"rftools.bmp\",order:6}" > $TO/rftools/index.lnk

cp $FROM/80rftool.elf $TO/rftools/rftool.elf
cp $ICON/antenna.bmp $TO/rftools/rftool.bmp
echo -n "{\"description\":{\"short\":\"RF tool\",\"long\":\"RF analyser synthesizer\"},\"icon\":\"rftool.bmp\",\"execute\":\"rftool.elf\",order:10}" > $TO/rftools/rftool.lnk

cp $FROM/79ccosc.elf $TO/rftools/wusbosc.elf
cp $ICON/usbant.bmp $TO/rftools/wusbosc.bmp
echo -n "{\"description\":{\"short\":\"USB analyser\",\"long\":\"WebUSB rf analyser\"},\"icon\":\"wusbosc.bmp\",\"execute\":\"wusbosc.elf\",order:2}" > $TO/rftools/wusbosc.lnk

cp $FROM/82sanal.elf $TO/rftools/spectrum.elf
cp $ICON/spectrum.bmp $TO/rftools/spectrum.bmp
echo -n "{\"description\":{\"short\":\"spectrum analyser\",\"long\":\"spectrum analyser\"},\"icon\":\"spectrum.bmp\",\"execute\":\"spectrum.elf\",order:5}" > $TO/rftools/spectrum.lnk
cp apps_featured/80_rftool/logs/*.* $TO/rftools/

# eink
mkdir $TO/eink
echo -n "{\"description\":{short:\"eInk\",long:\"eInk display tools\"},\"icon\":\"eink.bmp\"}" > $TO/eink/index.lnk
cp $ICON/eink.bmp $TO/eink/eink.bmp

cp $FROM/90eink.elf $TO/eink/displimg.elf
cp $ICON/picture.bmp $TO/eink/displimg.bmp
echo -n "{\"description\":{short:\"eInk\",long:\"eInk display image loader\"},\"icon\":\"displimg.bmp\",\"execute\":\"displimg.elf\"}" > $TO/eink/displimg.lnk

cp apps_featured/90_epd_image/res/*.bmp $TO/eink/

#tools 
mkdir $TO/tools
cp $ICON/fsett.bmp $TO/tools/tools.bmp
echo -n "{\"description\":\"Tools\",\"icon\":\"tools.bmp\",order:9}" > $TO/tools/index.lnk

cp $FROM/30dcf77.elf $TO/tools/dcf77.elf
cp $FROM/22seqen.elf $TO/tools/sequence.elf
cp $FROM/72uartm.elf $TO/tools/uartmon.elf
cp $FROM/74invt.elf $TO/tools/vfd_invt.elf # rs485
cp $FROM/33temper.elf $TO/tools/dstemper.elf
cp $FROM/15charla.elf $TO/tools/charmap.elf
cp $FROM/63ws_la.elf $TO/tools/ws2812.elf
cp $FROM/49gpio.elf $TO/tools/gpio.elf

#charmap.bmp {"description":{short:"Character map",long:"Character map"},"icon":"charmap.bmp","execute":"charmap.elf",order:50}
#dcf77.bmp {"description":{short:"DCF77",long:"DCF77 decoder"},"icon":"dcf77.bmp","execute":"dcf77.elf",order:40}
#temper.bmp {"description":{short:"DS1820",long:"DS1820 temperature grapher"},"icon":"dstemper.bmp","execute":"dstemper.elf",order:70}
#sigmal.bmp {"description":{short:"GPIO",long:"GPIO and PWM controller"},"icon":"gpio.bmp","execute":"gpio.elf",order:90}
#fatom.bmp {"description":"Tools","icon":"folder.bmp",order:9}
#sequence.bmp {"description":{short:"Sequencer",long:"Sequencer and analyser"},"icon":"sequence.bmp","execute":"sequence.elf",order:60}
#serial.bmp {"description":{short:"UART monitor",long:"UART monitor"},"icon":"uartmon.bmp","execute":"uartmon.elf"}
#motor.bmp {"description":{short:"VFD Invt",long:"Invt VFD RS485 visualizer"},"icon":"vfd_invt.bmp","execute":"vfd_invt.elf"}
#ws2812.bmp {"description":{short:"WS2812",long:"WS2812 addressable led tester"},"icon":"ws2812.bmp","execute":"ws2812.elf",order:100}

#devices
mkdir $TO/devices
cp $ICON/fchip.bmp $TO/tools/devices.bmp
echo -n "{\"description\":{\"short\":\"Devices\",\"long\":\"Device drivers\"},\"icon\":\"devices.bmp\",order:7}" > $TO/devices/index.lnk

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

#chip.bmp {"description":{short:"APDS9960",long:"APDS9960 color gesture sensor"},"icon":"chip.bmp","execute":"apds9960.elf"}
#{"description":{short:"BMP085",long:"BMP085 Barometric sensor"},"icon":"chip.bmp","execute":"bmp085.elf"}
#{"description":{short:"DHT22",long:"DHT11/DHT22 temperature humidity"},"icon":"chip.bmp","execute":"dht22.elf"}
#{"description":{short:"DS1307",long:"DS1307 real time clock"},"icon":"chip.bmp","execute":"ds1307.elf"}
#{"description":{short:"DS3231",long:"DS3231 real time clock"},"icon":"chip.bmp","execute":"ds3231.elf"}
#{"description":{short:"ESP8266",long:"ESP8266 uart wifi server"},"icon":"chip.bmp","execute":"esp8266.elf"}
#{"description":{short:"MAX7219",long:"MAX7219 display driver"},"icon":"chip.bmp","execute":"max7219.elf"}
#{"description":{short:"PCF8591t",long:"PCF8591t I2C ADC"},"icon":"chip.bmp","execute":"pcf8591t.elf"}
#{"description":{short:"SIMCOM",long:"SIMCOM gprs modem"},"icon":"chip.bmp","execute":"simcom.elf"}
#{"description":{short:"YX5300",long:"YX5300 MP3 player"},"icon":"chip.bmp","execute":"yx5300.elf"}


cd release/image_la104
rm la104apps.zip
zip -r ../la104apps.zip *
