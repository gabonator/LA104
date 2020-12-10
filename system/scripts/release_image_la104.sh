#!/bin/bash
# prepares filesystem for LA104, all files should be copied into the device root
# contains latest and most useful applications to show capabilities of LA104

# at first run build_full.sh

FROM=../build/apps
TO=../build/image_la104/
ICON=../../tools/shellicons

rm -rf $TO
mkdir $TO

(
  cd $ICON
  ./build.sh
)

if [ $? -eq 1 ]; then
  echo [ERROR] Cannot prepare icons
  exit 1
fi

(
set -e

# shell
#cp $FROM/29fileman_la104.elf $TO/shell.elf
cp $FROM/28shell.elf $TO/shell.elf

cp $FROM/29fileman_la104.elf $TO/fileman.elf
cp $ICON/manager.bmp $TO/fileman.bmp
echo -n "{\"description\":{short:\"File manager\",long:\"File manager\"},\"icon\":\"fileman.bmp\",\"execute\":\"fileman.elf\",order:20}" > $TO/fileman.lnk

cp $FROM/28shell.elf $TO/gui.elf

# root
cp $FROM/17analys.elf $TO/analyser.elf
cp $ICON/official.bmp $TO/analyser.bmp
echo -n "{\"description\":{short:\"Logic analyser\",long:\"Logic analyser\"},\"icon\":\"analyser.bmp\",\"execute\":\"analyser.elf\",order:15}" > $TO/analyser.lnk

cp $FROM/35viewla.elf $TO/viewtxt.elf
cp $ICON/genie.bmp $TO/help.bmp
#cp help.txt $TO/help.txt
echo -n "{\"description\":{short:\"Help\",long:\"Help\"},\"icon\":\"help.bmp\",\"execute\":\"viewtxt.elf help.txt\",order:10}" > $TO/help.lnk

cp $FROM/61devin.elf $TO/devinfo.elf
cp $ICON/info.bmp $TO/devinfo.bmp
echo -n "{\"description\":{\"short\":\"Dev info\",\"long\":\"Device info\"},\"icon\":\"devinfo.bmp\",\"execute\":\"devinfo.elf\",order:-10}" > $TO/devinfo.lnk

# midi
#mkdir $TO/midi
#cp $ICON/fmidi.bmp $TO/midi/midi.bmp
#echo -n "{\"description\":{\"short\":\"MIDI\",\"long\":\"MIDI tools\"},\"icon\":\"midi.bmp\"}" > $TO/midi/index.lnk
#
#cp $FROM/21mplayl.elf $TO/midi/midiplay.elf
#cp ../apps_featured/test21_midiplay/midi/laisla.mid $TO/midi/laisla.mid
#cp $ICON/music.bmp $TO/midi/midiplay.bmp
#echo -n "{\"description\":{\"short\":\"MIDI\",\"long\":\"MIDI player\"},\"icon\":\"midiplay.bmp\",\"execute\":\"midiplay.elf\"}" > $TO/midi/midiplay.lnk

# i2c
mkdir $TO/i2c
cp $ICON/fi2c.bmp $TO/i2c/i2c.bmp
echo -n "{\"description\":{\"short\":\"I2C\",\"long\":\"I2C bus tools\"},\"icon\":\"i2c.bmp\",order:8}" > $TO/i2c/index.lnk

cp $FROM/37icscan.elf $TO/i2c/i2cscan.elf
cp $ICON/magnifier.bmp $TO/i2c/i2cscan.bmp
echo -n "{\"description\":{\"short\":\"I2C scanner\",\"long\":\"I2C bus scanner\"},\"icon\":\"i2cscan.bmp\",execute:\"i2cscan.elf\",order:10}" > $TO/i2c/i2cscan.lnk

cp ../apps_featured/test37_i2cscan/devices.txt $TO/i2c/devices.txt
cp $FROM/85eetest.elf $TO/i2c/eeview.elf
cp $ICON/eeprom.bmp $TO/i2c/eeview.bmp
echo -n "{\"description\":{\"short\":\"EEPROM viewer\",\"long\":\"I2C EEPROM memory viewer\"},\"icon\":\"eeview.bmp\",execute:\"eeview.elf\",order:5}" > $TO/i2c/eeview.lnk


echo -n "{\"description\":{short:\"Help\",long:\"Help\"},\"icon\":\"../help.bmp\",\"execute\":\"../viewtxt.elf help.txt\",order:200}" >> $TO/i2c/help.lnk

cat > $TO/i2c/help.txt <<- EOM
I2C bus tools
-------

  - "I2C bus scanner" - Connect your device to LA104 (SCL to P1, SDA to P2) and run the scanner. It should show you wich addressess are in use. Contains database od 250 i2c devices and automatically shows corresponding device name and description. Most I2C devices can be powered directly by LA104 power pins (GND, 3V)
  - "EEPROM viewer" - Memory viewer and editor compatible with 24LC256 and similar I2C eeprom memories

EOM

# rftools
mkdir $TO/rftools
cp $ICON/fant.bmp $TO/rftools/rftools.bmp
echo -n "{\"description\":{\"short\":\"RF toolkit\",\"long\":\"RF toolkit\"},\"icon\":\"rftools.bmp\",order:6}" > $TO/rftools/index.lnk

cp $FROM/80rftool.elf $TO/rftools/rftool.elf
cp $ICON/antenna.bmp $TO/rftools/rftool.bmp
echo -n "{\"description\":{\"short\":\"RF tool\",\"long\":\"RF analyser synthesizer\"},\"icon\":\"rftool.bmp\",\"execute\":\"rftool.elf\",order:10,youtube:\"Gwyi00NKBNg\"}" > $TO/rftools/rftool.lnk

cp $FROM/79ccosc.elf $TO/rftools/wusbosc.elf
cp $ICON/usbant.bmp $TO/rftools/wusbosc.bmp
echo -n "{\"description\":{\"short\":\"USB analyser\",\"long\":\"WebUSB rf analyser\"},\"icon\":\"wusbosc.bmp\",\"execute\":\"wusbosc.elf\",order:2,youtube:\"Gwyi00NKBNg\"}" > $TO/rftools/wusbosc.lnk

cp $FROM/82sanal.elf $TO/rftools/spectrum.elf
cp $ICON/spectrum.bmp $TO/rftools/spectrum.bmp
echo -n "{\"description\":{\"short\":\"spectrum analyser\",\"long\":\"Spectrum analyser\"},\"icon\":\"spectrum.bmp\",\"execute\":\"spectrum.elf\",order:5,youtube:\"Gwyi00NKBNg\"}" > $TO/rftools/spectrum.lnk
cp ../apps_featured/80_rftool/logs/*.* $TO/rftools/

echo -n "{\"description\":{short:\"Help\",long:\"Help\"},\"icon\":\"../help.bmp\",\"execute\":\"../viewtxt.elf help.txt\",order:200}" >> $TO/rftools/help.lnk

cat > $TO/rftools/help.txt <<- EOM
RF toolkit
-------

  - "RF analyser & synthesizer" - main application which automatically decodes few of most used weather stations and wireless keys
  - "WebUSB rf analyser" - Webusb analyser which uses your computer for signal analysis. Runs in your web browser and integrates RTL433 library for signal decoding
  - "Spectrum analyser" - Simple spectrum analyser which uses RSSI for estimating radiated signal power

All of those applications require CC1101 transceiver attached to LA104. 

CC1101 Wiring:
         VDD   [1] 2    VDD  -> +3V
P1 <-     SI    3  4    SCK  -> P2
CH4 <-    SO    5  6    GDO2
P3 <-    /CS    7  8    GDO0 -> P4
         GND    9 10    GND  -> GND

Wiring using LA104 cable:

| Color  | LA104 | CC1101   |
|--------|-------|----------|
| brown  | CH1   |          |
| red    | CH2   |          |
| orange | CH3   |          |
| yellow | CH4   | 5 - SO   |
| green  | GND   | 10 - GND |
| blue   | P1    | 3 - SI   |
| purple | P2    | 4 - SCK  |
| gray   | P3    | 7 - /CS  |
| white  | P4    | 8 - GDO0 |
| black  | 3V+   | 2 - VDD  |

EOM

# eink
mkdir $TO/display
echo -n "{\"description\":{short:\"Display\",long:\"Display tools\"},\"icon\":\"display.bmp\"}" > $TO/display/index.lnk
cp $ICON/fpicture.bmp $TO/display/display.bmp

cp $FROM/90eink.elf $TO/display/eink.elf
cp $ICON/eink.bmp $TO/display/eink.bmp
cp ../apps_featured/90_epd_image/res/*.bmp $TO/display/
echo -n "{\"description\":{short:\"eInk\",long:\"eInk display image loader\"},\"icon\":\"eink.bmp\",\"execute\":\"eink.elf\"}" > $TO/display/eink.lnk

cp $FROM/81image.elf $TO/display/imgview.elf
cp $ICON/picture.bmp $TO/display/imgview.bmp
cp ../apps/81_image/res/*.bmp $TO/display/
echo -n "{\"description\":{short:\"Image view\",long:\"Image viewer\"},\"icon\":\"imgview.bmp\",\"execute\":\"imgview.elf\"}" > $TO/display/imgview.lnk

cp $FROM/102oled.elf $TO/display/oledssd.elf
cp $ICON/picture.bmp $TO/display/oledssd.bmp
echo -n "{\"description\":{short:\"Oled test\",long:\"SSD1306 oled test\"},\"icon\":\"oledssd.bmp\",\"execute\":\"oledssd.elf\"}" > $TO/display/oledssd.lnk


echo -n "{\"description\":{short:\"Help\",long:\"Help\"},\"icon\":\"../help.bmp\",\"execute\":\"../viewtxt.elf help.txt\",order:200}" >> $TO/display/help.lnk

cat > $TO/display/help.txt <<- EOM
Display tools
-------

  - "Image view" - shows 24 bpp or 32 bpp bitmaps on this screen
  - "eInk display image loader" - compatible with EPD eInk displays with SSD1675 controller. Configured for 2" / 9" display with resolution of 296x128 black/white/red pixels. Wiring:
    - VDD - 3V
    - VSS - GND
    - MOSI (SDA) - P1
    - SCK (SCL) - P2
    - /CS - P3
    - /RST - P4
    - BUSY - CH4
    - D/C - (not used, 3 spi 9 bit mode, BS1=VDD, DC=VSS)
  - "Oled test" - shows test image on SSD1306 128x64 monochromatic oled display through I2C connection. Wiring:
    - SCL - P1
    - SDA - P2

EOM

#tools 
mkdir $TO/tools
cp $ICON/fatom.bmp $TO/tools/tools.bmp
echo -n "{\"description\":\"Tools\",\"icon\":\"tools.bmp\",order:9}" > $TO/tools/index.lnk

cp $FROM/30dcf77.elf $TO/tools/dcf77.elf
cp $FROM/22seqen.elf $TO/tools/sequence.elf
cp $FROM/72uartm.elf $TO/tools/uartmon.elf
cp $FROM/74invt.elf $TO/tools/vfd_invt.elf # rs485
cp $FROM/33temper.elf $TO/tools/dstemper.elf
cp $FROM/15charla.elf $TO/tools/charmap.elf
cp $FROM/63ws_104.elf $TO/tools/ws2812.elf
cp $FROM/49gpio.elf $TO/tools/gpio.elf
cp $FROM/21mplayl.elf $TO/tools/midiplay.elf
cp $FROM/105avrfl.elf $TO/tools/avrflash.elf
cp $FROM/103servo.elf $TO/tools/servo.elf

echo -n "{\"description\":{short:\"Character map\",long:\"Character map\"},\"icon\":\"charmap.bmp\",\"execute\":\"charmap.elf\",order:50}" > $TO/tools/charmap.lnk
echo -n "{\"description\":{short:\"DCF77\",long:\"DCF77 decoder\"},\"icon\":\"dcf77.bmp\",\"execute\":\"dcf77.elf\",order:40}" > $TO/tools/dcf77.lnk
echo -n "{\"description\":{short:\"DS1820\",long:\"DS1820 temperature grapher\"},\"icon\":\"dstemper.bmp\",\"execute\":\"dstemper.elf\",order:70}" > $TO/tools/dstemper.lnk
echo -n "{\"description\":{short:\"GPIO\",long:\"GPIO and PWM controller\"},\"icon\":\"gpio.bmp\",\"execute\":\"gpio.elf\",order:90}" > $TO/tools/gpio.lnk
echo -n "{\"description\":{short:\"Sequencer\",long:\"Sequencer and analyser\"},\"icon\":\"sequence.bmp\",\"execute\":\"sequence.elf\",order:60,youtube:\"D8OnXzE7OwU\"}" > $TO/tools/sequence.lnk
echo -n "{\"description\":{short:\"UART monitor\",long:\"UART monitor\"},\"icon\":\"uartmon.bmp\",\"execute\":\"uartmon.elf\"}" > $TO/tools/uartmon.lnk
echo -n "{\"description\":{short:\"VFD Invt\",long:\"Invt VFD RS485 visualizer\"},\"icon\":\"vfd_invt.bmp\",\"execute\":\"vfd_invt.elf\",youtube:\"0Ns7AHVvmX0\"}" > $TO/tools/vfd_invt.lnk
echo -n "{\"description\":{short:\"WS2812\",long:\"WS2812 addressable led tester\"},\"icon\":\"ws2812.bmp\",\"execute\":\"ws2812.elf\",order:100,youtube:\"4fWqZBbjQPc\"}" > $TO/tools/ws2812.lnk
echo -n "{\"description\":{\"short\":\"MIDI\",\"long\":\"MIDI player\"},\"icon\":\"midiplay.bmp\",\"execute\":\"midiplay.elf\",youtube:\"f3DCqy-3lNo\"}" > $TO/tools/midiplay.lnk
echo -n "{\"description\":{short:\"AvrFlash\",long:\"Atmel firmware flasher\"},\"icon\":\"avrflash.bmp\",\"execute\":\"avrflash.elf\",order:20, youtube:\"yC3a1zbonJ0\"}" > $TO/tools/avrflash.lnk
echo -n "{\"description\":{short:\"Servo\",long:\"Servo controller PCA9685\"},\"icon\":\"servo.bmp\",\"execute\":\"servo.elf\",order:65,youtube:\"heYFrqVF1P4\"}" > $TO/tools/servo.lnk

cp $ICON/charmap.bmp $TO/tools/charmap.bmp
cp $ICON/dcf77.bmp $TO/tools/dcf77.bmp
cp $ICON/temper.bmp $TO/tools/dstemper.bmp
cp $ICON/signal.bmp $TO/tools/gpio.bmp
cp $ICON/sequence.bmp $TO/tools/sequence.bmp
cp $ICON/serial.bmp $TO/tools/uartmon.bmp
cp $ICON/motor.bmp $TO/tools/vfd_invt.bmp
cp $ICON/ws2812.bmp $TO/tools/ws2812.bmp
cp $ICON/music.bmp $TO/tools/midiplay.bmp
cp $ICON/arduino.bmp $TO/tools/avrflash.bmp
cp $ICON/servo.bmp $TO/tools/servo.bmp
cp ../apps_featured/test21_midiplay/midi/laisla.mid $TO/tools/laisla.mid
cp ../apps/105_avrprogrammer/arduino/blink.hex $TO/tools/blink.hex
cp ../apps/105_avrprogrammer/arduino/wheel.hex $TO/tools/wheel.hex
cp ../apps/105_avrprogrammer/arduino/toggle.hex $TO/tools/toggle.hex

echo -n "{\"description\":{short:\"Help\",long:\"Help\"},\"icon\":\"../help.bmp\",\"execute\":\"../viewtxt.elf help.txt\",order:200}" >> $TO/tools/help.lnk

cat > $TO/tools/help.txt <<- EOM
Tools
-------
  - "Character map" - just shows the ascii table and font used by this operating system, 8x14 pixel font is from Genoa EGA BIOS
  - "DCF77" - decodes the DCF77 pulses and shows date and time information. Attach DCF77 receiver to P1 input.
  - "DS1820" - measures temperature from multiple DS1820 thermometer sensors and shows information in form of graph. Measurement is stored in HTML file. Attach DS1820 sensors to the P1 input.
  - "GPIO" - digital pin manipulation application - you can configure P1..P4 pins as inputs or outputs, set their logic level and observe external signals in form of simple graph. This application can be used for generation of 4 different PWM signals with ability to set the modulation frequency. Nice feature for testing RGB LEDs
  - "Sequencer" - simple sequencer with 4 outputs and 8 inputs. You will need two PCF8574 expanders at i2c addresses 0x38 and 0x39. Connect SCL to P1 and SDA to P2. First expander is used for output and second expander for input
  - "UART monitor" - uart monitor with full configuration support - you can set baudrate, parity and data word length. Currently only for 8bit encoding.
  - "VFD Invt" - application for visualizing the performance of Invt motor inverters over RS485 link with MAX485 transceiver. Attach DI=P1 (TXD), DE=P3, RE=P4, RO=P2 (RXD)
  - "WS2812" - applcation for controlling WS2812 addressable leds with set of nice looking animations. Attach your led strip to P1
  - "MIDI player" - plays a midi file on synthesizer. Directly attach P1 and VCC to the Midi Input port using current limiting resistor
  - "Atmel firmware flasher" - AVRISP programmer flashing HEX files (generated by Arduino IDE) over ICSP connector:
    P1   P4  P3
     .   .   .  
     .   .   .
     GND P2  VCC
  - "Servo controller PCA9685" - Use PCA9685 to use up to 16 servo motors

EOM

#devices
mkdir $TO/devices
cp $ICON/fchip.bmp $TO/devices/devices.bmp
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
cp $FROM/92bmp180.elf $TO/devices/bmp180.elf
cp $FROM/95sht20.elf $TO/devices/sht20.elf
cp $FROM/96tm1637.elf $TO/devices/tm1637.elf
cp $FROM/87mlx906.elf $TO/devices/mlx90614.elf
cp $FROM/106mpr12.elf $TO/devices/mpr121.elf

echo -n "{\"description\":{short:\"APDS9960\",long:\"APDS9960 color gesture sensor\"},\"icon\":\"chip.bmp\",\"execute\":\"apds9960.elf\"}" > $TO/devices/apds9960.lnk
echo -n "{\"description\":{short:\"BMP085\",long:\"BMP085 Barometric sensor\"},\"icon\":\"chip.bmp\",\"execute\":\"bmp085.elf\"}" > $TO/devices/bmp085.lnk
echo -n "{\"description\":{short:\"DHT22\",long:\"DHT11/DHT22 temperature humidity\"},\"icon\":\"chip.bmp\",\"execute\":\"dht22.elf\"}" > $TO/devices/dht22.lnk
echo -n "{\"description\":{short:\"DS1307\",long:\"DS1307 real time clock\"},\"icon\":\"chip.bmp\",\"execute\":\"ds1307.elf\"}" > $TO/devices/ds1307.lnk
echo -n "{\"description\":{short:\"DS3231\",long:\"DS3231 real time clock\"},\"icon\":\"chip.bmp\",\"execute\":\"ds3231.elf\"}" > $TO/devices/ds3231.lnk
echo -n "{\"description\":{short:\"ESP8266\",long:\"ESP8266 uart wifi server\"},\"icon\":\"chip.bmp\",\"execute\":\"esp8266.elf\"}" > $TO/devices/esp8266.lnk
echo -n "{\"description\":{short:\"MAX7219\",long:\"MAX7219 display driver\"},\"icon\":\"chip.bmp\",\"execute\":\"max7219.elf\"}" > $TO/devices/max7219.lnk
echo -n "{\"description\":{short:\"PCF8591t\",long:\"PCF8591t I2C ADC\"},\"icon\":\"chip.bmp\",\"execute\":\"pcf8591t.elf\"}" > $TO/devices/pcf8591t.lnk
echo -n "{\"description\":{short:\"SIMCOM\",long:\"SIMCOM gprs modem\"},\"icon\":\"chip.bmp\",\"execute\":\"simcom.elf\"}" > $TO/devices/simcom.lnk
echo -n "{\"description\":{short:\"YX5300\",long:\"YX5300 MP3 player\"},\"icon\":\"chip.bmp\",\"execute\":\"yx5300.elf\"}" > $TO/devices/yx5300.lnk
echo -n "{\"description\":{short:\"BMP180\",long:\"BMP180/BMP085 barometer\"},\"icon\":\"chip.bmp\",\"execute\":\"bmp180.elf\"}" > $TO/devices/bmp180.lnk
echo -n "{\"description\":{short:\"SHT20\",long:\"SHT20 temperature humidity\"},\"icon\":\"chip.bmp\",\"execute\":\"sht20.elf\"}" > $TO/devices/sht20.lnk
echo -n "{\"description\":{short:\"TM1637\",long:\"TM1637 7-segment display\"},\"icon\":\"chip.bmp\",\"execute\":\"tm1637.elf\"}" > $TO/devices/tm1637.lnk
echo -n "{\"description\":{short:\"MLX90614\",long:\"MLX90614 non cotact thermo\"},\"icon\":\"chip.bmp\",\"execute\":\"mlx90614.elf\"}" > $TO/devices/mlx90614.lnk
echo -n "{\"description\":{short:\"MPR121\",long:\"MPR121 12 channel toch sensor\"},\"icon\":\"chip.bmp\",\"execute\":\"mpr121.elf\"}" > $TO/devices/mpr121.lnk

cp $ICON/chip.bmp $TO/devices/chip.bmp

echo -n "{\"description\":{short:\"Help\",long:\"Help\"},\"icon\":\"../help.bmp\",\"execute\":\"../viewtxt.elf help.txt\",order:200}" >> $TO/devices/help.lnk

cat > $TO/devices/help.txt <<- EOM
Device drivers
-------

For I2C devices connect SCL to P1 and SDA to P2. For single wire devices use P1, for uart devices P1 is TX (LA104 transmitting to device) and P2 is RX (LA104 receives on this pin), 
for other devices wiring instructions see the application folder or source code on github:

https://github.com/gabonator/LA104/tree/master/system/apps_experiments

EOM

# usb
mkdir $TO/usb
cp $ICON/fusb.bmp $TO/usb/usb.bmp
echo -n "{\"description\":{\"short\":\"USB Applications\",\"long\":\"USB Applications\"},\"icon\":\"usb.bmp\",order:2}" > $TO/usb/index.lnk

cp $FROM/24ncdc.elf $TO/usb/cdc.elf
cp $FROM/25hid.elf $TO/usb/hid.elf
cp $FROM/26midi.elf $TO/usb/midi.elf
cp $FROM/104avris.elf $TO/usb/avrisp.elf

echo -n "{\"description\":{short:\"Serial link\",long:\"Serial link\"},\"icon\":\"cdc.bmp\",\"execute\":\"cdc.elf\"}" > $TO/usb/cdc.lnk
echo -n "{\"description\":{short:\"HID\",long:\"Human interface device\"},\"icon\":\"hid.bmp\",\"execute\":\"hid.elf\"}" > $TO/usb/hid.lnk
echo -n "{\"description\":{short:\"MIDI\",long:\"Midi to uart\"},\"icon\":\"midi.bmp\",\"execute\":\"midi.elf\"}" > $TO/usb/midi.lnk
echo -n "{\"description\":{short:\"AVRISP\",long:\"Atmel ISP programmer\"},\"icon\":\"avrisp.bmp\",\"execute\":\"avrisp.elf\"}" > $TO/usb/avrisp.lnk

cp $ICON/serial.bmp $TO/usb/cdc.bmp
cp $ICON/mouse.bmp $TO/usb/hid.bmp
cp $ICON/midi.bmp $TO/usb/midi.bmp
cp $ICON/farduino.bmp $TO/usb/avrisp.bmp

echo -n "{\"description\":{short:\"Help\",long:\"Help\"},\"icon\":\"../help.bmp\",\"execute\":\"../viewtxt.elf help.txt\",order:200}" >> $TO/usb/help.lnk

cat > $TO/usb/help.txt <<- EOM
USB Applications
-------

After running any of these applications, disconnect usb cable and reconnect. In case you are using USB-C to USB micro cable, disconnect the cable on both ends, attach on the computer side and then on the LA104 side. The charging led should light up

  - "Serial link" - USB CDC profile
  - "Human interface device" - HID profile, you can move the mouse pointer with encoders
  - "MIDI" - Midi usb bridge which transfers the usb traffic to internal uart (P1: TXD, P2: RXD)
  - "Atmel ISP programmer" - Flash atmel chips from arduino environment using ICSP header:
    P1   P4  P3
     .   .   .  
     .   .   .
     GND P2  VCC

EOM

cat > $TO/help.txt <<- EOM
LA104 
-------
Hello, this is readme file for logic analyser "LA104" produced by Chinese company e-Design. This firmware was programmed by Gabriel Valky with help of github community and it tries to show full potential of this versatile device. Whole project is open sourced here: https://github.com/gabonator/LA104

Use encoders move around icons to choose the application you would like to run. Press first button "SMPL" to open folder or execute program. When you want to exit running application, press second button "MENU".
                                                                               
Application groups
--------------------
  - "File manager" - easy access to the file system, use this shell to run your own programs. To make it default, copy "fileman.elf" to "shell.elf". To revert it to this visual shell rename "gui.elf" to "shell.elf"
  - "Logic analyser" - original logic analyser software provided by manufacturer
  - "Help" - short manual should be present in all application groups for short description of all available programs
  - "Tools" - set of very handy applications for hardware testing and logging (e.g. uart monitor, pwm generator) 
  - "I2C bus tools" - contains I2C bus scanner and eeprom viewer
  - "Device drivers" - various applications for testing of specific components
  - "RF toolkit" - radio frequency analyser and synthesizer with CC1101 transceiver, including spectrum analyser and WebUsb analyser
  - "USB Applications" - mouse simulator, usb to uart converter
  - "Display tools" - Image viewer for LA104 and led/oled/eink displays
  - "Device info" - shows information about this device
  - "Just for fun" - non technical applications

Version information
---------------------
oct/2020 release
EOM

mkdir $TO/fun
cp $ICON/fgames.bmp $TO/fun/fun.bmp
echo -n "{\"description\":{\"short\":\"Fun\",\"long\":\"Just for fun\"},\"icon\":\"fun.bmp\",order:-200}" > $TO/fun/index.lnk

cp $FROM/99cube04.elf $TO/fun/cube.elf
cp $FROM/2import.elf $TO/fun/triangle.elf
cp $FROM/4snake.elf $TO/fun/snake.elf
cp $FROM/39lcd.elf $TO/fun/scroll.elf

echo -n "{\"description\":{short:\"Cube\",long:\"Rotating cube\"},\"icon\":\"app.bmp\",\"execute\":\"cube.elf\"}" > $TO/fun/cube.lnk
echo -n "{\"description\":{short:\"Triangle\",long:\"Sierpinski triangle\"},\"icon\":\"app.bmp\",\"execute\":\"triangle.elf\"}" > $TO/fun/triangle.lnk
echo -n "{\"description\":{short:\"Snake\",long:\"Snake game\"},\"icon\":\"app.bmp\",\"execute\":\"snake.elf\"}" > $TO/fun/snake.lnk
echo -n "{\"description\":{short:\"Scroll\",long:\"Scroll test\"},\"icon\":\"app.bmp\",\"execute\":\"scroll.elf\"}" > $TO/fun/scroll.lnk

cp $ICON/games.bmp $TO/fun/app.bmp
)

if [ $? -eq 1 ]; then
  echo [ERROR] Failed to prepare disk image
  exit 1
fi

cd $TO
rm la104apps.zip > /dev/null 2> /dev/null
zip -r ../la104apps.zip *

if [ $? -eq 1 ]; then
  echo [ERROR] Compression failure
  exit 1
fi

echo [DONE] Packaging finished
