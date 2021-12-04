#include <library.h>

void drawline(int x1, int y1, int x2, int y2, int color);

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
  for (var x=0; x<320; x++)
  {
    var i = ((Math.floor(x/16)+Math.floor(y/16))&1) ? 10 : 20; 
    imgData.data[(y*320+x)*4] = 0; 
    imgData.data[(y*320+x)*4+1] = 0;
    imgData.data[(y*320+x)*4+2] = 0; 
    imgData.data[(y*320+x)*4+3] = i; 
  }

ctx.putImageData(imgData, 0, 0);
mousePressed = false;

$('#my_canvas_id').mousedown(function (e) {
  mousePressed = true;
  Draw(e.pageX - $(this).offset().left, e.pageY - $(this).offset().top, false);
})
.mousemove(function (e) {
  if (mousePressed) {
      Draw(e.pageX - $(this).offset().left, e.pageY - $(this).offset().top, true);
  }
})
.mouseup(function (e) {
  mousePressed = false;
  promise = promise.then( () => BIOS.memWrite(ptrTo, [0xff, 0xff, 0xff, 0xff]) );
})
.mouseleave(function (e) {
  mousePressed = false;
  promise = promise.then( () => BIOS.memWrite(ptrTo, [0xff, 0xff, 0xff, 0xff]) );
});

promise = Promise.resolve();
function Draw(x, y, isDown) 
{
  if (isDown) 
  {
    ctx.beginPath();
    ctx.strokeStyle = "#000000";
    ctx.lineWidth = 2;
    ctx.lineJoin = "round";
    ctx.moveTo(window.lastX, window.lastY);
    ctx.lineTo(x, y);
    ctx.closePath();
    ctx.stroke();
    promise = promise.then( () => BIOS.memWrite(ptrTo, [x&255, x>>8, 0, 0, y&255, y>>8, 0, 0]) );
  }
  lastX = x; 
  lastY = y;
}
</script>
)");

    CPoint from(-1, -1), to(-1, -1);
    BIOS::DBG::Print(R"(<script>ptrTo = 0x%08x;</script>)", &to);

    while (BIOS::KEY::GetKey() == BIOS::KEY::EKey::None)
    {
        if (from != to)
        {
            if (from.x != -1 && to.x != -1)
                drawline(from.x, from.y, to.x, to.y, RGB565(ffffff));

            from = to;
        }
    }
    return 0;
}

void drawline(int x1, int y1, int x2, int y2, int color)
{
    // http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Simplification
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    
    int err = dx - dy;
    int count = 0;
    for(;; count++)
    {
        BIOS::LCD::PutPixel(x1, y1, color);

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

