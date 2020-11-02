#!/bin/bash
magick=""

magick convert fractal.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on fractal.bmp
magick convert mouse.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on mouse.bmp
magick convert midi.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on midi.bmp
magick convert charmap.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on charmap.bmp
magick convert serial.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on serial.bmp
magick convert imgAnalyser.png -crop 128x128+0+0 -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on official.bmp
magick convert imgSeq.png -crop 128x128+0+0 -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on sequence.bmp
magick convert application.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on app.bmp
magick convert pulseview.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on sump.bmp
magick convert genie.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on genie.bmp
magick convert measure.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on measure.bmp
magick convert magnifier.png -resize 64x64 -background transparent -gravity Center -extent 64x64 -alpha on magnifier.bmp
