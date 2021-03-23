QENUM(MP_QSTR_mini)
QENUM(MP_QSTR_lcd)
QENUM(MP_QSTR_width)
QENUM(MP_QSTR_height)
QENUM(MP_QSTR_line)
QENUM(MP_QSTR_bar)
QENUM(MP_QSTR_color)
QENUM(MP_QSTR_back)
QENUM(MP_QSTR_delay)
//QENUM(MP_QSTR_get)

QDEF(MP_QSTR_mini, (const byte*)"\xe6\x04" "mini")
QDEF(MP_QSTR_lcd, (const byte*)"\xae\x03" "lcd")
QDEF(MP_QSTR_width, (const byte*)"\x23\x05" "width")
QDEF(MP_QSTR_height, (const byte*)"\xfa\x06" "height")
QDEF(MP_QSTR_line, (const byte*)"\xcb\x04" "line")
QDEF(MP_QSTR_bar, (const byte*)"\xf4\x03" "bar")
QDEF(MP_QSTR_color, (const byte*)"\xd8\x05" "color")
QDEF(MP_QSTR_back, (const byte*)"\xee\x04" "back")
QDEF(MP_QSTR_delay, (const byte*)"\x50\x05" "delay")
//QDEF(MP_QSTR_key, (const byte*)"\x32\x03" "key")


/*
https://www.onlinegdb.com/online_c_compiler

#include <stdio.h>
#include <stdint.h>

typedef int mp_uint_t;
typedef uint8_t byte;
#define Q_HASH_MASK 0xff

mp_uint_t qstr_compute_hash(const byte *data, size_t len) {
    // djb2 algorithm; see http://www.cse.yorku.ca/~oz/hash.html
    mp_uint_t hash = 5381;
    for (const byte *top = data + len; data < top; data++) {
        hash = ((hash << 5) + hash) ^ (*data); // hash * 33 ^ data
    }
    hash &= Q_HASH_MASK;
    // Make sure that valid hash is never zero, zero means "hash not computed"
    if (hash == 0) {
        hash++;
    }
    return hash;
}

int main()
{
    const char* ident = "gc";
    printf("%02x%02x", qstr_compute_hash(ident, strlen(ident)), strlen(ident));

    return 0;
}

*/
