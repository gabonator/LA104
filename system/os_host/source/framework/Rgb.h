#define RGB565RGB(r, g, b) (((r)>>3)|(((g)>>2)<<5)|(((b)>>3)<<11))

#define GetHtmlR(rgb) ((rgb) >> 16)
#define GetHtmlG(rgb) (((rgb) >> 8)&0xff)
#define GetHtmlB(rgb) ((rgb)&0xff)

#define Get565R(rgb) (((rgb)&0x1f)<<3)
#define Get565G(rgb) ((((rgb)>>5)&0x3f)<<2)
#define Get565B(rgb) ((((rgb)>>11)&0x1f)<<3)

#define GetColorR(rgb) ((rgb) & 0xff)
#define GetColorG(rgb) (((rgb) >> 8)&0xff)
#define GetColorB(rgb) (((rgb) >> 16)&0xff)

#define RGB565(rgb) RGB565RGB( GetHtmlR(0x##rgb), GetHtmlG(0x##rgb), GetHtmlB(0x##rgb))
#define COUNT(arr) (int)(sizeof(arr)/sizeof(arr[0]))
#define RGBTRANS (RGB565(ff00ff)-1)

#define RGB32(r,g,b) ((r) | ((g)<<8) | ((b)<<16))
