#include "Lcd.h"

namespace BIOS
{
namespace LCD
{

/*static*/ void Line(int x1, int y1, int x2, int y2, unsigned short clr)
{
    // if x1 == x2 or y1 == y2, then it does not matter what we set here
    int16_t delta_x(x2 - x1);
    int8_t ix((delta_x > 0) - (delta_x < 0));
    delta_x = abs(delta_x) << 1;
 
    int16_t delta_y(y2 - y1);
    int8_t iy((delta_y > 0) - (delta_y < 0));
    delta_y = abs(delta_y) << 1;
 
	PutPixel( x1, y1, clr );
 
    if (delta_x >= delta_y)
    {
        // error may go below zero
        int16_t error(delta_y - (delta_x >> 1));
 
        while (x1 != x2)
        {
            if (error >= 0)
            {
                if (error || (ix > 0))
                {
                    y1 += iy;
                    error -= delta_x;
                }
                // else do nothing
            }
            // else do nothing
 
            x1 += ix;
            error += delta_y;
 
			PutPixel( x1, y1, clr );
        }
    }
    else
    {
        // error may go below zero
        int16_t error(delta_x - (delta_y >> 1));
 
        while (y1 != y2)
        {
            if (error >= 0)
            {
                if (error || (iy > 0))
                {
                    x1 += ix;
                    error -= delta_y;
                }
                // else do nothing
            }
            // else do nothing
 
            y1 += iy;
            error += delta_x;
			PutPixel( x1, y1, clr );
        }
    }
}

}
}