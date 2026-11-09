// PC test harness: emulates the LCD window write order of the DS203 BIOS
// (columns left to right, each column bottom to top), plays a scripted key
// sequence and stores screenshots as PPM.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../source/platform.h"

static uint16_t fb[PF::ScreenH][PF::ScreenW];
static int wx, wy, ww, wh, wpos;
static int frame;
static uint32_t now = 1000;
static long writes;

struct TStep { int frames; uint32_t keys; const char* shot; float tx = 0, ty = 0, ta = 0; };
void GameDebugTeleport(float x, float y, float a);
int GameDebugKills(); void GameDebugDump(); int GameDebugHp(); int GameDebugLevel();
const char* GameDebugMap(int level, int y);
static TStep* script;
static int scriptPos, scriptFrame;

static void Save(const char* name)
{
    FILE* f = fopen(name, "wb");
    fprintf(f, "P6\n%d %d\n255\n", PF::ScreenW, PF::ScreenH);
    for (int y = 0; y < PF::ScreenH; y++)
        for (int x = 0; x < PF::ScreenW; x++)
        {
            uint16_t c = fb[y][x];
            uint8_t rgb[3] = {(uint8_t)((c >> 11) << 3), (uint8_t)(((c >> 5) & 63) << 2), (uint8_t)((c & 31) << 3)};
            fwrite(rgb, 1, 3, f);
        }
    fclose(f);
}

namespace PF
{
    void Begin(int x, int y, int w, int h) { wx = x; wy = y; ww = w; wh = h; wpos = 0; }
    void Write(const uint16_t* p, int n)
    {
        while (n--)
        {
            int cx = wx + wpos / wh, cy = wy + wh - 1 - wpos % wh;
            if (cx < wx + ww) fb[cy][cx] = *p;
            p++; wpos++; writes++;
        }
    }
    void End()
    {
        if (wpos != ww * wh) { printf("window not filled %d/%d\n", wpos, ww * wh); exit(1); }
        frame++;
        now += 60;
    }
    void Bar(int x1, int y1, int x2, int y2, uint16_t clr)
    {
        for (int y = y1; y < y2; y++)
            for (int x = x1; x < x2; x++)
                if (x >= 0 && y >= 0 && x < ScreenW && y < ScreenH)
                    fb[y][x] = clr;
    }
    int Print(int x, int y, uint16_t fg, uint16_t bg, const char* text)
    {
        // no font here: draw a block per character
        int n = (int)strlen(text);
        for (int i = 0; i < n; i++)
        {
            Bar(x + i * 8, y, x + i * 8 + 8, y + 14, bg);
            if (text[i] != ' ')
                Bar(x + i * 8 + 1, y + 3, x + i * 8 + 7, y + 11, fg);
        }
        return n * 8;
    }
    uint32_t Keys()
    {
        // advance the script once per frame (Keys is called once per frame)
        TStep& s = script[scriptPos];
        if (s.frames < 0) { static int n; return n++ ? 0 : KeyF2; }
        if (scriptFrame == 0 && s.tx > 0) GameDebugTeleport(s.tx, s.ty, s.ta);
        uint32_t k = s.keys;
        if (++scriptFrame >= s.frames)
        {
            if (s.shot) { Save(s.shot); printf("%s:\n", s.shot); GameDebugDump(); }
            scriptFrame = 0;
            scriptPos++;
        }
        return k;
    }
    uint32_t Tick() { return now; }
    void Beep(int) {}
}

using namespace PF;
static TStep demo[] = {
    {2, 0, "shot0.ppm"},                    // start view
    {14, KeyRight, nullptr},                // turn
    {1, 0, "shot1.ppm"},
    {10, KeyUp, nullptr},
    {1, 0, "shot2.ppm"},
    {12, KeyLeft, nullptr},
    {6, KeyUp, nullptr},
    {1, KeyF3, nullptr},                    // open door
    {15, 0, "shot3.ppm"},
    {20, KeyUp, "shot4.ppm"},
    {3, 0, "shot5.ppm"},
    {30, KeyRight, "shot6.ppm"},
    // look at the robot in the north room and shoot it
    {2, 0, "shot7.ppm", 13.5f, 2.5f, 1.5708f},
    {1, KeyF1, "shot8.ppm"},
    {8, 0, nullptr},
    {1, KeyF1, nullptr},
    {8, 0, nullptr},
    {1, KeyF1, nullptr},
    {12, 0, "shot9.ppm"},
    // exit room of level 1, use the switch
    {2, 0, nullptr, 12.5f, 22.5f, 1.5708f},
    {1, KeyF3, "shot10.ppm"},
    {3, 0, nullptr},
    {1, KeyF1, nullptr},
    {3, 0, "shot11.ppm"},
    {-1, 0, nullptr}
};

// flood fill from the start, doors are passable, exit must be next to a
// reachable cell; also every door needs walls on two opposite sides
static bool CheckLevel(int lv)
{
    auto at = [&](int x, int y) { return GameDebugMap(lv, y)[x]; };
    auto wall = [&](char c) { return strchr("#SWMTGDX", c) != nullptr; };
    static bool seen[24][24];
    memset(seen, 0, sizeof(seen));
    int sx = -1, sy = -1, stack[600][2], sp = 0;
    for (int y = 0; y < 24; y++)
    {
        if (strlen(GameDebugMap(lv, y)) != 24) { printf("level %d row %d bad length\n", lv, y); return false; }
        for (int x = 0; x < 24; x++)
        {
            if (at(x, y) == 'P') { sx = x; sy = y; }
            if (at(x, y) == 'D')
            {
                bool ok = (wall(at(x-1, y)) && wall(at(x+1, y))) || (wall(at(x, y-1)) && wall(at(x, y+1)));
                if (!ok) { printf("level %d door %d,%d without frame\n", lv, x, y); return false; }
            }
        }
    }
    stack[sp][0] = sx; stack[sp++][1] = sy; seen[sy][sx] = true;
    bool exitOk = false;
    int enemies = 0, reach = 0;
    while (sp)
    {
        sp--; int x = stack[sp][0], y = stack[sp][1];
        if (at(x, y) == 'e') reach++;
        static const int d[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        for (auto& dd : d)
        {
            int nx = x + dd[0], ny = y + dd[1];
            char c = at(nx, ny);
            if (c == 'X') exitOk = true;
            if (seen[ny][nx] || (wall(c) && c != 'D')) continue;
            seen[ny][nx] = true;
            stack[sp][0] = nx; stack[sp++][1] = ny;
        }
    }
    for (int y = 0; y < 24; y++) for (int x = 0; x < 24; x++) enemies += at(x, y) == 'e';
    printf("level %d: exit %s, enemies reachable %d/%d\n", lv, exitOk ? "reachable" : "NOT REACHABLE", reach, enemies);
    return exitOk;
}

int main()
{
    bool ok = CheckLevel(0) && CheckLevel(1);
    script = demo;
    GameMain();
    printf("kills %d hp %d level %d\n", GameDebugKills(), GameDebugHp(), GameDebugLevel());
    if (!ok) return 1;
    printf("frames %d, lcd writes per frame %ld\n", frame, writes / (frame ? frame : 1));
    return 0;
}
