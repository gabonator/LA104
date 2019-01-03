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


void DrawImage(char* path, int bx, int by)
{
    CBufferedReader reader;
    if (!reader.Open(path))
        return;
    
    BmpHdr header;
    reader >> CStream(&header, sizeof(header));
    reader.Seek(header.dwBfOffset);
    
    for (int y=0; y<(int)header.dwBiHeight; y++)
        for (int x=0; x<(int)header.dwBiWidth; x++)
        {
            uint8_t color[4];
            reader >> CStream(color, 4);
            if (!color[3])
                continue;
            int c = RGB565RGB(color[2], color[1], color[0]);
            if (color[3] > 250)
            {
                LCD::PutPixel(bx+x, by+header.dwBiHeight-1-y, c);
                continue;
            }
            int c0 = LCD::GetPixel(bx+x, by+header.dwBiHeight-1-y);
            int c1 = InterpolateColor(c0, c, color[3]);
            LCD::PutPixel(bx+x, by+header.dwBiHeight-1-y, c1);
        }
}

void SaveImage(char* path, CRect rc)
{
#ifdef __APPLE__
    return;
#endif

    CBufferedWriter writer;
    //LCD::BufferBegin(rc); // TODO: not working
    
    writer.Open(path);
#ifdef __APPLE__
    for (int y=rc.bottom-1; y>=rc.top; y--)
        for (int x=rc.left; x<rc.right; x++)
#else
    for (int x=rc.left; x<rc.right; x++)
        for (int y=rc.bottom-1; y>=rc.top; y--)
#endif
            writer << (uint16_t)LCD::GetPixel(x, y);
    
    writer.Close();
}

bool LoadImage(char* path, CRect rc)
{
#ifdef __APPLE__
    return false;
#endif
    CBufferedReader reader;
    if (!reader.Open(path))
        return false;
    
    int offset = 0;
    int fileOffset = 0;
    int pixelCount = BIOS::FAT::SectorSize/2;
    uint16_t* pixelData = (uint16_t*)BIOS::FAT::GetSharedBuffer();

    LCD::BufferBegin(rc);
    for (int y=rc.top; y<rc.bottom; y++)
        for (int x=rc.left; x<rc.right; x++)
        {
            LCD::BufferWrite(pixelData[offset++]);
            if (offset >= pixelCount)
            {
                fileOffset += BIOS::FAT::SectorSize;
                reader.Seek(fileOffset);
                offset = 0;
            }
        }

    return true;
}
