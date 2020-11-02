#convert folder.png -resize 64x64 -alpha off -background "#ff00ff" -flatten -colors 256 -compress none folder_min.bmp
convert folder.png -resize 64x64 -background transparent -gravity Center -extent 64x64 compo1.png
convert games.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
convert -composite compo1.png compo2.png fgames.bmp

convert chip.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
convert -composite compo1.png compo2.png fchip.bmp

convert settings.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
convert -composite compo1.png compo2.png fsett.bmp

convert fractal.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
convert -composite compo1.png compo2.png ffractal.bmp

convert oscillo.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
convert -composite compo1.png compo2.png foscillo.bmp

convert atom.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
convert -composite compo1.png compo2.png fatom.bmp

convert usb.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
convert -composite compo1.png compo2.png fusb.bmp

convert binary.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
convert -composite compo1.png compo2.png fbinary.bmp

convert measure.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
convert -composite compo1.png compo2.png fmeasure.bmp

convert manager.png -resize 60x60 -background transparent -gravity Center -extent 64x64 manager.bmp

convert chip.png -resize 60x60 -background transparent -gravity Center -extent 64x64 chip.bmp

#https://www.clipartmax.com/download/m2i8H7K9N4i8N4m2_free-vector-or-logic-functions-digital-electronics-digital-electronics/

rm compo1.png
rm compo2.png