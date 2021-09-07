convert co2.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on co2.bmp
convert radioactive.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on radsens.bmp
convert hunter.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on hunter.bmp
convert circuitpython.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on cpython.bmp
convert dht.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on dht.bmp
#convert hammer.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on hammer.bmp

convert folder.png -resize 64x64 -background transparent -gravity Center -extent 64x64 compo1.png
convert hammer.png -resize 32x32 -background transparent -gravity SouthEast -extent 64x64 compo2.png
convert -composite compo1.png compo2.png fdevel.bmp

convert gabuino.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on gabuino.bmp
convert basic.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on basic.bmp
