#magick convert folder.png -resize 64x64 -alpha off -background "#ff00ff" -flatten -colors 256 -compress none folder_min.bmp
magick convert folder.png -resize 64x64 -background transparent -gravity Center -extent 64x64 compo1.png
magick convert games.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
magick convert -composite compo1.png compo2.png fgames.bmp

magick convert chip.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
magick convert -composite compo1.png compo2.png fchip.bmp

magick convert settings.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
magick convert -composite compo1.png compo2.png fsett.bmp

magick convert fractal.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
magick convert -composite compo1.png compo2.png ffractal.bmp

magick convert oscillo.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
magick convert -composite compo1.png compo2.png foscillo.bmp

magick convert atom.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
magick convert -composite compo1.png compo2.png fatom.bmp

magick convert usb.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
magick convert -composite compo1.png compo2.png fusb.bmp

magick convert binary.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
magick convert -composite compo1.png compo2.png fbinary.bmp

magick convert measure.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
magick convert -composite compo1.png compo2.png fmeasure.bmp

magick convert manager.png -resize 60x60 -background transparent -gravity Center -extent 64x64 manager.bmp

magick convert chip.png -resize 60x60 -background transparent -gravity Center -extent 64x64 chip.bmp

#https://www.clipartmax.com/download/m2i8H7K9N4i8N4m2_free-vector-or-logic-functions-digital-electronics-digital-electronics/

rm compo1.png
rm compo2.png