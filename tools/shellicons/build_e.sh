convert fileelf.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on fileelf.bmp
convert i2c.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on i2c.bmp
convert eeprom.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on eeprom.bmp
convert antenna.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on antenna.bmp
convert eink.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on eink.bmp
convert spectrum.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on spectrum.bmp
convert ws2812.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on ws2812.bmp
convert music.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on music.bmp
convert picture.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on picture.bmp
convert temper.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on temper.bmp
convert motor.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on motor.bmp
convert dcf77.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on dcf77.bmp
convert signal.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on signal.bmp
convert info.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on info.bmp

convert antenna.png -resize 64x64 -background transparent -gravity Center -extent 64x64 compo1.png
convert usb2.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
convert -composite compo1.png compo2.png usbant.bmp


convert folder.png -resize 64x64 -background transparent -gravity Center -extent 64x64 compo1.png
convert i2c.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
convert -composite compo1.png compo2.png fi2c.bmp

convert folder.png -resize 64x64 -background transparent -gravity Center -extent 64x64 compo1.png
convert antenna.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
convert -composite compo1.png compo2.png fant.bmp

convert folder.png -resize 64x64 -background transparent -gravity Center -extent 64x64 compo1.png
convert picture.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
convert -composite compo1.png compo2.png fpicture.bmp

convert folder.png -resize 64x64 -background transparent -gravity Center -extent 64x64 compo1.png
convert midi.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
convert -composite compo1.png compo2.png fmidi.bmp

convert games.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on games.bmp

convert arduino.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on arduino.bmp

convert arduino.png -resize 64x64 -background transparent -gravity Center -extent 64x64 compo1.png
convert usb2.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
convert -composite compo1.png compo2.png farduino.bmp

convert servo.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on servo.bmp

rm compo1.png compo2.png
