magick convert fileelf.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on fileelf.bmp
magick convert i2c.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on i2c.bmp
magick convert eeprom.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on eeprom.bmp
magick convert antenna.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on antenna.bmp
magick convert eink.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on eink.bmp
magick convert spectrum.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on spectrum.bmp
magick convert ws2812.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on ws2812.bmp
magick convert music.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on music.bmp
magick convert picture.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on picture.bmp
magick convert temper.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on temper.bmp
magick convert motor.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on motor.bmp
magick convert dcf77.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on dcf77.bmp
magick convert signal.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on signal.bmp
magick convert info.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on info.bmp

magick convert antenna.png -resize 64x64 -background transparent -gravity Center -extent 64x64 compo1.png
magick convert usb2.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
magick convert -composite compo1.png compo2.png usbant.bmp


magick convert folder.png -resize 64x64 -background transparent -gravity Center -extent 64x64 compo1.png
magick convert i2c.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
magick convert -composite compo1.png compo2.png fi2c.bmp

magick convert folder.png -resize 64x64 -background transparent -gravity Center -extent 64x64 compo1.png
magick convert antenna.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
magick convert -composite compo1.png compo2.png fant.bmp

rm compo1.png compo2.png
