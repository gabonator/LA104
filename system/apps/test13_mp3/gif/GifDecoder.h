#ifndef _GIFDECODER_H_
#define _GIFDECODER_H_

#include <stdint.h>

typedef void (*info_callback)(int16_t width, int16_t height);
typedef void (*pixel_callback)(uint16_t c);

typedef uint8_t (*file_read_callback)(void);
typedef void (*file_read_block_callback)(void * buffer, int numberOfBytes);

// LZW constants
// NOTE: LZW_MAXBITS should be set to 10 or 11 for small displays, 12 for large displays
//   all 32x32-pixel GIFs tested work with 11, most work with 10
//   LZW_MAXBITS = 12 will support all GIFs, but takes 16kB RAM
#define LZW_SIZTABLE  (1 << lzwMaxBits)

template <int lzwMaxBits>
class GifDecoder {
public:
    int startDecoding(void);
    int decodeFrame(void);
    
    void setInfoCallback(info_callback f);
    void setDrawPixelCallback(pixel_callback f);

    void setFileReadCallback(file_read_callback f);
    void setFileReadBlockCallback(file_read_block_callback f);

private:
    bool parseTableBasedImage(void);
    void decompressAndDisplayFrame();
    int parseData(void);
    int parseGIFFileTerminator(void);
    void parseGraphicControlExtension(void);
    void parseGlobalColorTable(void);
    void parseLogicalScreenDescriptor(void);
    bool parseGifHeader(void);
    void copyImageDataRect(uint8_t *dst, uint8_t *src, int x, int y, int width, int height);
    void fillImageData(uint8_t colorIndex);
    void fillImageDataRect(uint8_t colorIndex, int x, int y, int width, int height);
    void readIntoBuffer(void *buffer, int numberOfBytes);
    int readWord(void);
    int readByte(void);

    void lzw_decode_init(int csize);
    void lzw_decode(void (*push)(uint8_t));
    void lzw_setTempBuffer(uint8_t * tempBuffer);
    int lzw_get_code(void);

    // Logical screen descriptor attributes
    int lsdWidth;
    int lsdHeight;
    int lsdPackedField;
    int lsdAspectRatio;
    int lsdBackgroundIndex;

    // Table based image attributes
    int tbiImageX;
    int tbiImageY;
    int tbiWidth;
    int tbiHeight;
    int tbiPackedBits;

    int transparentColorIndex;
    int lzwCodeSize;
    bool keyFrame;
    int rectX;
    int rectY;
    int rectWidth;
    int rectHeight;


    int colorCount;
    uint16_t palette[256];

    char tempBuffer[260];

    info_callback infoCallback;
    pixel_callback drawPixelCallback;
    file_read_callback fileReadCallback;
    file_read_block_callback fileReadBlockCallback;

    // LZW variables
    int bbits;
    int bbuf;
    int cursize;                // The current code size
    int curmask;
    int codesize;
    int clear_code;
    int end_code;
    int newcodes;               // First available code
    int top_slot;               // Highest code for current size
    int extra_slot;
    int slot;                   // Last read code
    int fc, oc;
    int bs;                     // Current buffer size for GIF
    int bcnt;
    uint8_t *sp;
    uint8_t * temp_buffer;

    uint8_t stack  [LZW_SIZTABLE];
    uint8_t suffix [LZW_SIZTABLE];
    uint16_t prefix [LZW_SIZTABLE];

    // Masks for 0 .. 16 bits
    unsigned int mask[17] = {
        0x0000, 0x0001, 0x0003, 0x0007,
        0x000F, 0x001F, 0x003F, 0x007F,
        0x00FF, 0x01FF, 0x03FF, 0x07FF,
        0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF,
        0xFFFF
    };
};

#include "GifDecoder_Impl.h"
#include "LzwDecoder_Impl.h"

#endif
