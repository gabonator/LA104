#pragma once
#include <stdint.h>

// Thin layer between the game and the LA104 BIOS, so the game can be run and
// tested on a PC as well.
namespace PF
{
    enum {
        KeyF1 = 1<<0, KeyF2 = 1<<1, KeyF3 = 1<<2, KeyF4 = 1<<3,
        KeyUp = 1<<8, KeyDown = 1<<9, KeyRight = 1<<10, KeyLeft = 1<<11
    };
    enum { ScreenW = 400, ScreenH = 240 };

    // Opens a drawing window. Pixels are then written column by column
    // (left to right), every column from the bottom row to the top row
    void Begin(int x, int y, int w, int h);
    void Write(const uint16_t* p, int n);
    void End();
    void Bar(int x1, int y1, int x2, int y2, uint16_t clr);
    int Print(int x, int y, uint16_t fg, uint16_t bg, const char* text);
    uint32_t Keys();
    uint32_t Tick();
    void Beep(int ms);
}

int GameMain();
