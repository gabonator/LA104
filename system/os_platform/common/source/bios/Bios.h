#pragma once
#include <stdint.h>
#include "hal.h"

void Set_Posi(uint16_t x, uint16_t y);
void Set_Pixel(uint16_t Color);
int Get_Pixel();
void Set_Block(int x1, int y1, int x2, int y2);
