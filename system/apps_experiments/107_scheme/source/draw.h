int blend(int fg, int bg, int alpha)
{
    int fg_per_2 = (fg & 0xF7DE) >> 1;
    int fg_per_4 = (fg & 0xE79C) >> 2;
    int fg_per_8 = (fg & 0xC718) >> 3;
    
    int bg_per_2 = (bg & 0xF7DE) >> 1;
    int bg_per_4 = (bg & 0xE79C) >> 2;
    int bg_per_8 = (bg & 0xC718) >> 3;
    
    if (alpha > 224)
        return fg; // 100% blend
    else if (alpha > 192)
        return (fg - fg_per_8 + bg_per_8); // 88% blend
    else if (alpha > 128)
        return (fg - fg_per_4 + bg_per_4); // 75% blend
    else if (alpha > 64)
        return (fg_per_2 + bg_per_2); // 50% blend
    else if (alpha > 32)
        return (fg_per_4 + bg - bg_per_4); // 25% blend
    else
        return bg; // 0% blend
}


typedef int fix16_t;

// Draws antialiased lines
// Xiaolin Wu's algorithm, using x/256 fixed point values
void drawline_aa(fix16_t x1, fix16_t y1, fix16_t x2, fix16_t y2, int color)
{
    bool reverse_xy = false;
    
    auto swap = [](int *x, int *y) {
        int temp = *x;
        *x = *y;
        *y = temp;
    };
    
    // plot the pixel at (x, y) with brightness c
    auto plot = [&](int x, int y, int c) {
        if (reverse_xy)
            swap(&x, &y);
        
        u16 oldcolor = BIOS::LCD::GetPixel(x >> 8, y >> 8);
        BIOS::LCD::PutPixel(x >> 8, y >> 8, blend(color, oldcolor, c));
    };
    
    // Integer part of x
    auto ipart = [](int x) -> int {
        return x & (~0xFF);
    };
    
    auto round = [&](int x) -> int {
        return ipart(x + 128);
    };
    
    // Fractional part of x
    auto fpart = [](int x) -> int {
        return x & 0xFF;
    };
    
    // Remaining fractional part of x
    auto rfpart = [&](int x) -> int {
        return 256 - fpart(x);
    };

    int dx = x2 - x1;
    int dy = y2 - y1;
    if (abs(dx) < abs(dy))
    {
        swap(&x1, &y1);
        swap(&x2, &y2);
        swap(&dx, &dy);
        reverse_xy = true;
    }
    
    if (x2 < x1)
    {
        swap(&x1, &x2);
        swap(&y1, &y2);
    }
    
    int gradient = dy * 256 / dx;
    
    // handle first endpoint
    int xend = round(x1);
    int yend = y1 + gradient * (xend - x1) / 256;
    int xgap = rfpart(x1 + 128);
    int xpxl1 = xend;  // this will be used in the main loop
    int ypxl1 = ipart(yend);
    plot(xpxl1, ypxl1, rfpart(yend) * xgap / 256);
    plot(xpxl1, ypxl1 + 256, fpart(yend) * xgap / 256);
    int intery = yend + gradient; // first y-intersection for the main loop
    
    // handle second endpoint
    xend = round(x2);
    yend = y2 + gradient * (xend - x2) / 256;
    xgap = fpart(x2 + 128);
    int xpxl2 = xend;  // this will be used in the main loop
    int ypxl2 = ipart(yend);
    plot(xpxl2, ypxl2, rfpart(yend) * xgap / 256);
    plot(xpxl2, ypxl2 + 256, fpart(yend) * xgap / 256);
    
    // main loop
    for (int x = xpxl1 + 1; x <= xpxl2 - 1; x += 256)
    {
        plot(x, ipart(intery), rfpart(intery));
        plot(x, ipart(intery) + 256, fpart(intery));
        intery = intery + gradient;
    }
}

