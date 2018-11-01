/*
 * Animated GIFs Display Code for SmartMatrix and 32x32 RGB LED Panels
 *
 * This file contains code to parse animated GIF files
 *
 * Written by: Craig A. Lindley
 *
 * Copyright (c) 2014 Craig A. Lindley
 * Minor modifications by Louis Beaudoin (pixelmatix)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "../Arduino.h"

// This file contains C code, and ESP32 Arduino has changed to use the C++ template version of min()/max() which we can't use with C, so we can't depend on a #define min() from Arduino anymore

#include "GifDecoder.h"


// Error codes
#define ERROR_NONE                 0
#define ERROR_DONE_PARSING         1
#define ERROR_WAITING              2
#define ERROR_FILEOPEN             -1
#define ERROR_FILENOTGIF           -2
#define ERROR_BADGIFFORMAT         -3
#define ERROR_UNKNOWNCONTROLEXT    -4

#define GIFHDRTAGNORM   "GIF87a"  // tag in valid GIF file
#define GIFHDRTAGNORM1  "GIF89a"  // tag in valid GIF file
#define GIFHDRSIZE 6

// Global GIF specific definitions
#define COLORTBLFLAG    0x80
#define INTERLACEFLAG   0x40
#define TRANSPARENTFLAG 0x01

#define NO_TRANSPARENT_INDEX -1

// Disposal methods
#define DISPOSAL_NONE       0
#define DISPOSAL_LEAVE      1
#define DISPOSAL_BACKGROUND 2
#define DISPOSAL_RESTORE    3

template <int lzwMaxBits>
void GifDecoder<lzwMaxBits>::setInfoCallback(info_callback f) {
    infoCallback = f;
}

template <int lzwMaxBits>
void GifDecoder<lzwMaxBits>::setDrawPixelCallback(pixel_callback f) {
    drawPixelCallback = f;
}

template <int lzwMaxBits>
void GifDecoder<lzwMaxBits>::setFileReadCallback(file_read_callback f) {
    fileReadCallback = f;
}

template <int lzwMaxBits>
void GifDecoder<lzwMaxBits>::setFileReadBlockCallback(file_read_block_callback f) {
    fileReadBlockCallback = f;
}

// Read a file byte
template <int lzwMaxBits>
int GifDecoder<lzwMaxBits>::readByte() {
    return fileReadCallback();
}

// Read a file word
template <int lzwMaxBits>
int GifDecoder<lzwMaxBits>::readWord() {

    int b0 = readByte();
    int b1 = readByte();
    return (b1 << 8) | b0;
}

// Read the specified number of bytes into the specified buffer
template <int lzwMaxBits>
void GifDecoder<lzwMaxBits>::readIntoBuffer(void *buffer, int numberOfBytes) {
    fileReadBlockCallback(buffer, numberOfBytes);
}

// Make sure the file is a Gif file
template <int lzwMaxBits>
bool GifDecoder<lzwMaxBits>::parseGifHeader() {

    char buffer[10];

    readIntoBuffer(buffer, GIFHDRSIZE);
    if ((strncmp(buffer, GIFHDRTAGNORM,  GIFHDRSIZE) != 0) &&
        (strncmp(buffer, GIFHDRTAGNORM1, GIFHDRSIZE) != 0))  {
        return false;
    }
    else    {
        return true;
    }
}

// Parse the logical screen descriptor
template <int lzwMaxBits>
void GifDecoder<lzwMaxBits>::parseLogicalScreenDescriptor() {

    lsdWidth = readWord();
    lsdHeight = readWord();
    lsdPackedField = readByte();
    lsdBackgroundIndex = readByte();
    lsdAspectRatio = readByte();
}

// Parse the global color table
template <int lzwMaxBits>
void GifDecoder<lzwMaxBits>::parseGlobalColorTable() {

    // Does a global color table exist?
    if (lsdPackedField & COLORTBLFLAG) {

        // A GCT was present determine how many colors it contains
        colorCount = 1 << ((lsdPackedField & 7) + 1);

        // Read color values into the palette array
        for (int i=0; i<colorCount; i++)
        {
            uint8_t rgb[3];
            readIntoBuffer(rgb, 3);
            palette[i] = RGB565RGB(rgb[0], rgb[1], rgb[2]);
        }
    }
}

// Parse a graphic control extension
template <int lzwMaxBits>
void GifDecoder<lzwMaxBits>::parseGraphicControlExtension() {

    int len = readByte();   // Check length
    if (len != 4) {
        Serial.println("Bad graphic control extension");
    }

    int packedBits = readByte();
    readWord();
    transparentColorIndex = readByte();

    if ((packedBits & TRANSPARENTFLAG) == 0) {
        // Indicate no transparent index
        transparentColorIndex = NO_TRANSPARENT_INDEX;
    }
    int disposalMethod = (packedBits >> 2) & 7;
    if (disposalMethod > 3) {
        disposalMethod = 0;
        Serial.println("Invalid disposal value");
    }

    readByte(); // Toss block end
}

// Parse file terminator
template <int lzwMaxBits>
int GifDecoder<lzwMaxBits>::parseGIFFileTerminator() {
    uint8_t b = readByte();
    if (b != 0x3B) {
        Serial.println("Bad GIF file format - Bad terminator");
        return ERROR_BADGIFFORMAT;
    }
    else    {
        return ERROR_NONE;
    }
}

// Parse table based image data
template <int lzwMaxBits>
bool GifDecoder<lzwMaxBits>::parseTableBasedImage() {
    // Parse image descriptor
    tbiImageX = readWord();
    tbiImageY = readWord();
    tbiWidth = readWord();
    tbiHeight = readWord();
    tbiPackedBits = readByte();

    infoCallback(tbiWidth, tbiHeight);
    
    // Is this image interlaced ?
    bool tbiInterlaced = ((tbiPackedBits & INTERLACEFLAG) != 0);

    // Does this image have a local color table ?
    bool localColorTable =  ((tbiPackedBits & COLORTBLFLAG) != 0);
    if (tbiInterlaced || localColorTable)
        return false;
    
    // Read the min LZW code size
    lzwCodeSize = readByte();

    // Initialize the LZW decoder for this frame
    lzw_decode_init(lzwCodeSize);
    lzw_setTempBuffer((uint8_t*)tempBuffer);

    // Decompress LZW data and display the frame
    decompressAndDisplayFrame();

    // Graphic control extension is for a single frame
    transparentColorIndex = NO_TRANSPARENT_INDEX;
    return true;
}

// Parse gif data
template <int lzwMaxBits>
int GifDecoder<lzwMaxBits>::decodeFrame()
{
    bool parsedFrame = false;
    while (!parsedFrame) {

        // Determine what kind of data to process
        uint8_t b = readByte();

        if (b == 0x2c) {
            // Parse table based image
            if (!parseTableBasedImage())
                return ERROR_BADGIFFORMAT;
            parsedFrame = true;

        }
        else if (b == 0x21) {
            // Parse extension
            b = readByte();
            // Determine which kind of extension to parse
            switch (b) {
            case 0xf9:
                // Graphic control extension
                parseGraphicControlExtension();
                break;
            default:
                return ERROR_UNKNOWNCONTROLEXT;
            }
        }
        else    {

            // Push unprocessed byte back into the stream for later processing
            _ASSERT(0);

            return ERROR_DONE_PARSING;
        }
    }
    return ERROR_NONE;
}

template <int lzwMaxBits>
int GifDecoder<lzwMaxBits>::startDecoding(void) {
    keyFrame = true;
    transparentColorIndex = NO_TRANSPARENT_INDEX;
    if (! parseGifHeader()) {
        Serial.println("Not a GIF file");
        return ERROR_FILENOTGIF;
    }
    parseLogicalScreenDescriptor();
    parseGlobalColorTable();
    return ERROR_NONE;
}

template <int lzwMaxBits>
void GifDecoder<lzwMaxBits>::decompressAndDisplayFrame() {
    static typeof(this) _this;
    _this = this;
    
    lzw_decode([](uint8_t pixel){
        _this->drawPixelCallback(_this->palette[pixel]);
    });
}
