#include <library.h>

using namespace BIOS;

typedef int fix16_t;
void drawline_aa(fix16_t fx1, fix16_t fy1, fix16_t fx2, fix16_t fy2, int color);


int main(void)
{
    BIOS::DBG::Print(R"(
    <canvas id="my_canvas_id" width=320 height=240></script>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
    <script>
var can = document.getElementById('my_canvas_id');
var ctx = can.getContext('2d');
imgData = ctx.getImageData(0, 0, can.width, can.height);

for (var y=0; y<240; y++)
{
  for (var x=0; x<320; x++)
  {
    var i = ((Math.floor(x/16)+Math.floor(y/16))&1) ? 255 : 240; 
    imgData.data[(y*320+x)*4] = ((Math.floor(x/32)+Math.floor(y/32))&1) ? 255 : 32; 
    imgData.data[(y*320+x)*4+1] = 32;
    imgData.data[(y*320+x)*4+2] = 32; 
    imgData.data[(y*320+x)*4+3] = 255; 
  }
}

ctx.putImageData(imgData, 0, 0);
mousePressed = false;
var can = $('#my_canvas_id');

can.mousedown(function (e) {
  mousePressed = true;
  Draw(e.pageX - $(this).offset().left, e.pageY - $(this).offset().top, false);
});

$('#my_canvas_id').mousemove(function (e) {
  if (mousePressed) {
      Draw(e.pageX - $(this).offset().left, e.pageY - $(this).offset().top, true);
  }
});

$('#my_canvas_id').mouseup(function (e) {
  mousePressed = false;
});

$('#my_canvas_id').mouseleave(function (e) {
  mousePressed = false;
});

promise = Promise.resolve();
function Draw(x, y, isDown) {
  if (isDown) {
    ctx.beginPath();
    ctx.strokeStyle = "#ffffff";
    ctx.lineWidth = 4;
    ctx.lineJoin = "round";
    ctx.moveTo(window.lastX, window.lastY);
    ctx.lineTo(x, y);
    ctx.closePath();
    ctx.stroke();
    rawTo = [x&255, x>>8, 0, 0, y&255, y>>8, 0, 0];
    promise = promise.then( () => BIOS.memWrite(window.ptrTo, rawTo) );
  }
  window.lastX = x; window.lastY = y;
}
  window.lastX = 160; window.lastY = 120;
</script>
)");

    CPoint from(160, 120), to(160, 120); // TODO: global init

    BIOS::DBG::Print(R"(<script>window.ptrFrom=0x%08x; window.ptrTo=0x%08x;</script>)", &from, &to);
    while (BIOS::KEY::GetKey() == BIOS::KEY::EKey::None)
    {
        if (from != to)
        {
            BIOS::DBG::Print("%d, %d - %d, %d\n", from.x, from.y, to.x, to.y);
            drawline_aa(from.x*256, from.y*256, to.x*256, to.y*256,
                RGB565(ffffff));
            from = to;
        }
    }
    return 0;
}


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
        
        uint16_t oldcolor = BIOS::LCD::GetPixel(x >> 8, y >> 8);
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

/* Non-antialiased line drawing */

void drawline(int x1, int y1, int x2, int y2, int color, int dots)
{
    // Algorithm from here: http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Simplification
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    
    int err = dx - dy;
    int count = 0;
    for(;; count++)
    {
        if (!dots || (count >> (dots - 1)) & 1)
        {
            //__Point_SCR(x1, y1);
            //__LCD_SetPixl(color);
        }
        
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        else if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}
