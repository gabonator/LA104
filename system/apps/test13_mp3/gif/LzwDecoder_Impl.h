
#define LZWDEBUG 0

#include "GifDecoder.h"

template <int lzwMaxBits>
void GifDecoder<lzwMaxBits>::lzw_setTempBuffer(uint8_t * tempBuffer) {
    temp_buffer = tempBuffer;
}

// Initialize LZW decoder
//   csize initial code size in bits
//   buf input data
template <int lzwMaxBits>
void GifDecoder<lzwMaxBits>::lzw_decode_init (int csize) {

    // Initialize read buffer variables
    bbuf = 0;
    bbits = 0;
    bs = 0;
    bcnt = 0;

    // Initialize decoder variables
    codesize = csize;
    cursize = codesize + 1;
    curmask = mask[cursize];
    top_slot = 1 << cursize;
    clear_code = 1 << codesize;
    end_code = clear_code + 1;
    slot = newcodes = clear_code + 2;
    oc = fc = -1;
    sp = stack;
}

//  Get one code of given number of bits from stream
template <int lzwMaxBits>
int GifDecoder<lzwMaxBits>::lzw_get_code() {

    while (bbits < cursize) {
        if (bcnt == bs) {
            // get number of bytes in next block
            readIntoBuffer(temp_buffer, 1);
            bs = temp_buffer[0];
            readIntoBuffer(temp_buffer, bs);
            bcnt = 0;
        }
        bbuf |= temp_buffer[bcnt] << bbits;
        bbits += 8;
        bcnt++;
    }
    int c = bbuf;
    bbuf >>= cursize;
    bbits -= cursize;
    return c & curmask;
}

// Decode given number of bytes
//   buf 8 bit output buffer
//   len number of pixels to decode
//   returns the number of bytes decoded
template <int lzwMaxBits>
void GifDecoder<lzwMaxBits>::lzw_decode(void (*push)(uint8_t))
{
    int c, code;

    if (end_code < 0)
        return;

    for (;;) {
        while (sp > stack)
                push(*(--sp));

        c = lzw_get_code();
        if (c == end_code) {
            break;

        }
        else if (c == clear_code) {
            cursize = codesize + 1;
            curmask = mask[cursize];
            slot = newcodes;
            top_slot = 1 << cursize;
            fc= oc= -1;

        }
        else    {

            code = c;
            if ((code == slot) && (fc >= 0)) {
                *sp++ = fc;
                code = oc;
            }
            else if (code >= slot) {
                break;
            }
            while (code >= newcodes) {
                *sp++ = suffix[code];
                code = prefix[code];
            }
            *sp++ = code;
            if ((slot < top_slot) && (oc >= 0)) {
                suffix[slot] = code;
                prefix[slot++] = oc;
            }
            fc = code;
            oc = c;
            if (slot >= top_slot) {
                if (cursize < lzwMaxBits) {
                    top_slot <<= 1;
                    curmask = mask[++cursize];
                }
            }
        }
    }
    end_code = -1;
}
