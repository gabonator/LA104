#pragma pack(push)
#pragma pack(2)
struct BmpHdr
{
    uint16_t wBfType;
    uint32_t dwBfSize;
    uint16_t wBfReserved1;
    uint16_t wBfReserved2;
    uint32_t dwBfOffset;
    
    uint32_t dwBiSize;
    uint32_t dwBiWidth;
    uint32_t dwBiHeight;
    uint16_t wBiPlanes;
    uint16_t wBiBitDepth;
    uint32_t dwBiCompression;
    uint32_t dwBiSizeImage;
    uint32_t dwBiXPels;
    uint32_t dwBiYPels;
    
    uint32_t dwBiClrUsed;
    uint32_t dwBiClrImportant;
};
#pragma pack(pop)

uint_fast16_t InterpolateColor( uint_fast16_t clrA, uint_fast16_t clrB, uint_fast8_t nLevel )
{
    int br = Get565R(clrB)*nLevel;
    int bg = Get565G(clrB)*nLevel;
    int bb = Get565B(clrB)*nLevel;
    nLevel = 255-nLevel;
    int ar = Get565R(clrA)*nLevel;
    int ag = Get565G(clrA)*nLevel;
    int ab = Get565B(clrA)*nLevel;
    ar = (ar+br) / 256;
    ag = (ag+bg) / 256;
    ab = (ab+bb) / 256;
    return RGB565RGB(ar, ag, ab);
}

uint8_t fileBuffer[400*3];
uint16_t pixelBuffer[400];

static_assert(COUNT(pixelBuffer) >= BIOS::LCD::Width);

bool DrawImage(char* path, int bx, int by)
{
    CBufferedReader reader;
    if (!reader.Open(path))
        return false;
    
    BmpHdr header;
    reader >> CStream(&header, sizeof(header));

    if (header.dwBiWidth > BIOS::LCD::Width || header.dwBiHeight > BIOS::LCD::Height)
      return false;

    if (header.wBiBitDepth != 24 && header.wBiBitDepth != 32)
      return false;

    reader.Seek(header.dwBfOffset);

    CRect line(bx, by + header.dwBiHeight-1, bx + header.dwBiWidth, by + header.dwBiHeight);
    for (int y=0; y<(int)header.dwBiHeight; y++)
    {
      if (header.wBiBitDepth == 24)
      {
        reader >> CStream(fileBuffer, header.dwBiWidth * 3);
        uint8_t* color = fileBuffer;
        for (int x=0; x<(int)header.dwBiWidth; x++, color += 3)
        {
          pixelBuffer[x] = RGB565RGB(color[2], color[1], color[0]); 
        }
      } else if (header.wBiBitDepth == 32) 
      {
        reader >> CStream(fileBuffer, header.dwBiWidth * 4);

        BIOS::LCD::BufferBegin(line);
        BIOS::LCD::BufferRead(pixelBuffer, header.dwBiWidth);
        BIOS::LCD::BufferEnd();

        uint8_t* color = fileBuffer;
        for (int x=0; x<(int)header.dwBiWidth; x++, color += 4)
        {
          if (!color[3])
            continue;

          int c0 = pixelBuffer[x];
          int c1 = RGB565RGB(color[2], color[1], color[0]); 

          pixelBuffer[x] = InterpolateColor(c0, c1, color[3]);
        }
      }

      BIOS::LCD::BufferBegin(line);
      BIOS::LCD::BufferWrite(pixelBuffer, header.dwBiWidth);
      BIOS::LCD::BufferEnd();
      line.top--;
      line.bottom--;
    }

    return true;
}

