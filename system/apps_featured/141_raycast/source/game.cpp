// Wolfenstein style raycaster for LA104 / DS203
//
// The 3D view is rendered one column at a time straight into the LCD window,
// no frame buffer is needed (the whole application uses only a few kB of RAM).
// Rays are cast for every second screen column, walls are textured with
// 64x64 palette images and fogged by distance with precomputed shade tables.
#include <stdint.h>
#include <string.h>
#include "platform.h"
#include "assets.h"
#include "levels.h"

namespace {

// own math: libm's wrappers pull errno and with it the whole newlib stdio,
// which needs syscalls the LA104 apps do not have
const float Pi = 3.14159265f;

float Sin(float x)
{
    while (x > Pi) x -= 2 * Pi;
    while (x < -Pi) x += 2 * Pi;
    if (x > Pi / 2) x = Pi - x;
    else if (x < -Pi / 2) x = -Pi - x;
    float x2 = x * x;
    return x * (1.0f - x2 / 6.0f * (1.0f - x2 / 20.0f * (1.0f - x2 / 42.0f)));
}

float Cos(float x)
{
    return Sin(x + Pi / 2);
}

float Sqrt(float x)
{
    if (x <= 0)
        return 0;
    union { float f; uint32_t i; } u = {x};
    u.i = 0x1fbd1df5 + (u.i >> 1);
    float r = u.f;
    r = 0.5f * (r + x / r);
    r = 0.5f * (r + x / r);
    return r;
}

float Fabs(float x)
{
    return x < 0 ? -x : x;
}

// ------------------------------------------------------------ layout
enum {
    ViewX = 0, ViewY = 20, ViewW = 320, ViewH = 200,
    ColW = 2,                         // screen pixels per ray
    Rays = ViewW / ColW,
    PanelX = 320,
    MapPx = 3,
    MiniX = PanelX + 4, MiniY = 146,
    GunW = 48, GunScale = 2
};
const float Plane = 0.66f;
// projection constant: wall of height 1 at distance 1 is K pixels tall
const int K = (int)(ViewW / 2 / Plane);

inline uint16_t Rgb(uint32_t c)
{
    return ((c >> 19) & 0x1f) << 11 | ((c >> 10) & 0x3f) << 5 | ((c >> 3) & 0x1f);
}
const uint16_t ClrPanel = 0x2104;   // dark grey
const uint16_t ClrText = 0xffff;
const uint16_t ClrDim = 0x8410;

// ------------------------------------------------------------ world
enum { CellEmpty = 0, CellDoor = 0x40, CellExit = 0x41 };

uint8_t g_map[MapH][MapW];   // 0 empty, 1..n wall texture + 1, CellDoor, CellExit

struct TDoor
{
    uint8_t x, y, axis;      // axis 0: door plane x = const, 1: y = const
    uint8_t state;           // 0 closed, 1 opening, 2 open, 3 closing
    int32_t open;            // 0..65536
    int32_t timer;           // ms
};
enum { MaxDoors = 24 };
TDoor g_doors[MaxDoors];
int g_nDoors;

enum EObj { ObjEnemy, ObjMedkit, ObjAmmo, ObjTreasure, ObjBarrel, ObjPillar, ObjLamp };
enum EEnemyState { EnIdle, EnChase, EnShoot, EnPain, EnDying, EnDead };

struct TObj
{
    float x, y;
    uint8_t type;
    uint8_t state;
    uint8_t active;
    int8_t hp;
    int16_t timer;           // ms in current state
    int16_t cooldown;        // ms till next shot
    uint8_t anim;
};
enum { MaxObjs = 64 };
TObj g_objs[MaxObjs];
int g_nObjs;

struct TPlayer
{
    float x, y, a;
    int hp, ammo, score;
    int kills, killsTotal;
    int treasure, treasureTotal;
    int fireTimer;           // ms of muzzle flash / cooldown
    int hurtTimer;
    float bob;
} P;

int g_level;
uint16_t g_bg[ViewH];         // ceiling / floor gradient, already fogged
uint32_t g_rand = 12345;

int Rand(int n)
{
    g_rand = g_rand * 1103515245u + 12345u;
    return (int)((g_rand >> 16) % (uint32_t)n);
}

// ------------------------------------------------------------ text helpers
char* Itoa(int v, char* p)
{
    char tmp[12];
    int n = 0;
    bool neg = v < 0;
    if (neg) v = -v;
    do { tmp[n++] = '0' + v % 10; v /= 10; } while (v);
    if (neg) *p++ = '-';
    while (n) *p++ = tmp[--n];
    *p = 0;
    return p;
}

void PrintPadded(int x, int y, uint16_t clr, const char* label, int value, int width)
{
    char buf[24];
    char* p = buf;
    while (*label) *p++ = *label++;
    char num[12];
    Itoa(value, num);
    int len = (int)strlen(num);
    int used = (int)(p - buf);
    while (used + len < width) { *p++ = ' '; used++; }
    strcpy(p, num);
    PF::Print(x, y, clr, ClrPanel, buf);
}

const char* g_msg = nullptr;
int g_msgTimer = 0;
bool g_msgDirty = true;

void Message(const char* msg, int ms = 2000)
{
    g_msg = msg;
    g_msgTimer = ms;
    g_msgDirty = true;
}

// ------------------------------------------------------------ level
bool IsWallChar(char c)
{
    return c == '#' || c == 'S' || c == 'W' || c == 'M' || c == 'T' || c == 'G' || c == 'D' || c == 'X';
}

uint8_t WallFor(char c)
{
    switch (c)
    {
        case '#': return Wall_brick + 1;
        case 'S': return Wall_stone + 1;
        case 'W': return Wall_wood + 1;
        case 'M': return Wall_metal + 1;
        case 'T': return Wall_tech + 1;
        case 'G': return Wall_moss + 1;
        case 'D': return CellDoor;
        case 'X': return CellExit;
    }
    return CellEmpty;
}

void AddObj(EObj type, int x, int y)
{
    if (g_nObjs >= MaxObjs)
        return;
    TObj& o = g_objs[g_nObjs++];
    memset(&o, 0, sizeof(o));
    o.x = x + 0.5f;
    o.y = y + 0.5f;
    o.type = type;
    o.active = 1;
    o.state = EnIdle;
    o.hp = 3;
    o.cooldown = 500 + Rand(1000);
}

void BuildBackground()
{
    const TLevel& lv = g_levels[g_level];
    for (int y = 0; y < ViewH; y++)
    {
        bool ceil = y < ViewH / 2;
        int d = ceil ? (ViewH / 2 - y) : (y - ViewH / 2 + 1);
        // distance of the floor point seen in this row, in 1/256 cells
        int dist = (K * 128) / d;
        int shade = dist * 3 / 512;
        if (shade > 15) shade = 15;
        float f = 1.0f - shade / 17.0f;
        f = f * f;
        uint32_t c = ceil ? lv.ceilRgb : lv.floorRgb;
        int r = (int)(((c >> 16) & 0xff) * f), g = (int)(((c >> 8) & 0xff) * f), b = (int)((c & 0xff) * f);
        g_bg[ViewH - 1 - y] = Rgb((r << 16) | (g << 8) | b);   // stored bottom-up
    }
}

void LoadLevel(int level, bool keepStats)
{
    g_level = level;
    g_nDoors = 0;
    g_nObjs = 0;
    const TLevel& lv = g_levels[level];
    if (!keepStats)
    {
        P.hp = 100;
        P.ammo = 16;
        P.score = 0;
    }
    P.kills = P.killsTotal = P.treasure = P.treasureTotal = 0;
    P.a = 0;
    P.fireTimer = P.hurtTimer = 0;
    for (int y = 0; y < MapH; y++)
        for (int x = 0; x < MapW; x++)
        {
            char c = lv.rows[y][x];
            g_map[y][x] = WallFor(c);
            switch (c)
            {
                case 'P': P.x = x + 0.5f; P.y = y + 0.5f; break;
                case 'e': AddObj(ObjEnemy, x, y); P.killsTotal++; break;
                case 'h': AddObj(ObjMedkit, x, y); break;
                case 'a': AddObj(ObjAmmo, x, y); break;
                case 't': AddObj(ObjTreasure, x, y); P.treasureTotal++; break;
                case 'b': AddObj(ObjBarrel, x, y); break;
                case 'p': AddObj(ObjPillar, x, y); break;
                case 'l': AddObj(ObjLamp, x, y); break;
            }
            if (c == 'D' && g_nDoors < MaxDoors)
            {
                TDoor& d = g_doors[g_nDoors++];
                memset(&d, 0, sizeof(d));
                d.x = x;
                d.y = y;
                // walls left and right -> corridor goes along y -> plane y = const
                bool wallsX = x > 0 && x < MapW - 1 && IsWallChar(lv.rows[y][x - 1]) && IsWallChar(lv.rows[y][x + 1]);
                d.axis = wallsX ? 1 : 0;
            }
        }
    BuildBackground();
}

TDoor* FindDoor(int x, int y)
{
    for (int i = 0; i < g_nDoors; i++)
        if (g_doors[i].x == x && g_doors[i].y == y)
            return &g_doors[i];
    return nullptr;
}

bool Blocked(int x, int y)
{
    if (x < 0 || y < 0 || x >= MapW || y >= MapH)
        return true;
    uint8_t c = g_map[y][x];
    if (c == CellEmpty)
        return false;
    if (c == CellDoor)
    {
        TDoor* d = FindDoor(x, y);
        return !d || d->open < 60000;
    }
    return true;
}

bool SolidObjAt(float x, float y, const TObj* self)
{
    for (int i = 0; i < g_nObjs; i++)
    {
        const TObj& o = g_objs[i];
        if (&o == self || !o.active)
            continue;
        bool solid = o.type == ObjBarrel || o.type == ObjPillar || (o.type == ObjEnemy && o.state < EnDying);
        if (!solid)
            continue;
        float dx = o.x - x, dy = o.y - y;
        if (dx * dx + dy * dy < 0.5f * 0.5f)
            return true;
    }
    return false;
}

bool CanStand(float x, float y, float r, const TObj* self)
{
    if (Blocked((int)(x - r), (int)(y - r)) || Blocked((int)(x + r), (int)(y - r)) ||
        Blocked((int)(x - r), (int)(y + r)) || Blocked((int)(x + r), (int)(y + r)))
        return false;
    return !SolidObjAt(x, y, self);
}

// ------------------------------------------------------------ ray casting
struct THit
{
    int32_t dist;     // perpendicular distance, 16.16
    uint8_t tex;
    uint8_t u;        // texture column
    uint8_t side;
};

inline int32_t MulQ16(int32_t a, int32_t b)
{
    return (int32_t)(((int64_t)a * b) >> 16);
}

// px, py: position 16.16, rdx, rdy: ray direction 16.16
bool CastRay(int32_t px, int32_t py, int32_t rdx, int32_t rdy, THit& h)
{
    const int32_t MaxDelta = 1 << 24;
    int mapX = px >> 16, mapY = py >> 16;
    // |1 / ray direction|, 16.16; rays almost parallel to an axis get a huge
    // but finite step
    uint32_t ax = rdx < 0 ? -rdx : rdx, ay = rdy < 0 ? -rdy : rdy;
    int32_t ddx = ax <= 256 ? MaxDelta : (int32_t)(0xffffffffu / ax);
    int32_t ddy = ay <= 256 ? MaxDelta : (int32_t)(0xffffffffu / ay);
    int stepX, stepY;
    int32_t sdx, sdy;
    if (rdx < 0) { stepX = -1; sdx = MulQ16(px & 0xffff, ddx); }
    else { stepX = 1; sdx = MulQ16(0x10000 - (px & 0xffff), ddx); }
    if (rdy < 0) { stepY = -1; sdy = MulQ16(py & 0xffff, ddy); }
    else { stepY = 1; sdy = MulQ16(0x10000 - (py & 0xffff), ddy); }

    for (int i = 0; i < 64; i++)
    {
        int side;
        if (sdx < sdy) { sdx += ddx; mapX += stepX; side = 0; }
        else { sdy += ddy; mapY += stepY; side = 1; }
        if (mapX < 0 || mapY < 0 || mapX >= MapW || mapY >= MapH)
            return false;
        uint8_t c = g_map[mapY][mapX];
        if (c == CellEmpty)
            continue;
        if (c == CellDoor)
        {
            TDoor* d = FindDoor(mapX, mapY);
            if (!d || d->axis != side)
                continue;
            int32_t dist = side == 0 ? sdx - ddx / 2 : sdy - ddy / 2;
            if (dist >= (side == 0 ? sdy : sdx))
                continue;   // ray leaves the cell before reaching the door
            int32_t w = side == 0 ? py + MulQ16(dist, rdy) : px + MulQ16(dist, rdx);
            int32_t frac = w & 0xffff;
            if (frac < d->open)
                continue;   // through the open part
            h.dist = dist;
            h.tex = Wall_door;
            h.u = (uint8_t)((frac - d->open) >> 10);
            h.side = side;
            return true;
        }
        int32_t dist = side == 0 ? sdx - ddx : sdy - ddy;
        int32_t w = side == 0 ? py + MulQ16(dist, rdy) : px + MulQ16(dist, rdx);
        int u = (w & 0xffff) >> 10;
        if (side == 0 && rdx > 0) u = 63 - u;
        if (side == 1 && rdy < 0) u = 63 - u;
        h.dist = dist;
        h.u = (uint8_t)u;
        h.side = side;
        if (c == CellExit)
            h.tex = Wall_exit;
        else
        {
            h.tex = c - 1;
            int prevX = side == 0 ? mapX - stepX : mapX;
            int prevY = side == 1 ? mapY - stepY : mapY;
            if (g_map[prevY][prevX] == CellDoor)
                h.tex = Wall_frame;
        }
        return true;
    }
    return false;
}

bool LineOfSight(float x0, float y0, float x1, float y1)
{
    float dx = x1 - x0, dy = y1 - y0;
    float len = Sqrt(dx * dx + dy * dy);
    int steps = (int)(len * 4) + 1;
    for (int i = 1; i < steps; i++)
    {
        float t = (float)i / steps;
        if (Blocked((int)(x0 + dx * t), (int)(y0 + dy * t)))
            return false;
    }
    return true;
}

// ------------------------------------------------------------ rendering
struct TVis
{
    int32_t depth;     // 16.16
    int16_t c0, c1;    // ray columns covered
    int16_t cStart;    // ray column of the sprite's left edge (may be < 0)
    int16_t size;      // height in screen pixels
    int32_t uStep;     // texture columns per ray column, 16.16
    uint8_t sprite;
    uint8_t shade;
};
enum { MaxVis = 32 };
TVis g_vis[MaxVis];
int g_nVis;

int SpriteFor(const TObj& o)
{
    switch (o.type)
    {
        case ObjEnemy:
            switch (o.state)
            {
                case EnShoot: return Spr_en_shoot;
                case EnPain: return Spr_en_pain;
                case EnDying: return Spr_en_die;
                case EnDead: return Spr_en_dead;
                case EnChase: return (o.anim & 1) ? Spr_en_walk2 : Spr_en_walk1;
                default: return Spr_en_walk1;
            }
        case ObjMedkit: return Spr_medkit;
        case ObjAmmo: return Spr_ammo;
        case ObjTreasure: return Spr_treasure;
        case ObjBarrel: return Spr_barrel;
        case ObjPillar: return Spr_pillar;
        default: return Spr_lamp;
    }
}

inline int ShadeFor(int32_t dist)
{
    int s = (dist * 3) >> 17;    // 1.5 levels per cell
    return s > NumShades - 1 ? NumShades - 1 : s;
}

void PrepareSprites(float dirX, float dirY, float planeX, float planeY)
{
    g_nVis = 0;
    float invDet = 1.0f / (planeX * dirY - dirX * planeY);
    for (int i = 0; i < g_nObjs; i++)
    {
        const TObj& o = g_objs[i];
        if (!o.active)
            continue;
        float sx = o.x - P.x, sy = o.y - P.y;
        float tx = invDet * (dirY * sx - dirX * sy);
        float ty = invDet * (-planeY * sx + planeX * sy);
        if (ty < 0.2f)
            continue;
        int size = (int)(K / ty);
        if (size > 4 * ViewH) size = 4 * ViewH;
        // tx / ty is in camera plane units (-1..1 across the view)
        int centre = (int)(Rays / 2 * (1.0f + tx / ty));
        int width = size / ColW;
        if (width < 1) width = 1;
        int cStart = centre - width / 2;
        int c0 = cStart < 0 ? 0 : cStart;
        int c1 = cStart + width > Rays ? Rays : cStart + width;
        if (c0 >= c1)
            continue;
        if (g_nVis >= MaxVis)
            break;
        TVis& v = g_vis[g_nVis++];
        v.depth = (int32_t)(ty * 65536.0f);
        v.c0 = c0;
        v.c1 = c1;
        v.cStart = cStart;
        v.size = size;
        v.uStep = (TexSize << 16) / width;
        v.sprite = SpriteFor(o);
        v.shade = o.type == ObjLamp ? 0 : ShadeFor(v.depth);
    }
    // far to near
    for (int i = 1; i < g_nVis; i++)
        for (int j = i; j > 0 && g_vis[j - 1].depth < g_vis[j].depth; j--)
        {
            TVis t = g_vis[j]; g_vis[j] = g_vis[j - 1]; g_vis[j - 1] = t;
        }
}

uint16_t g_col[ViewH];       // one column, bottom to top
int32_t g_zCentre;           // wall distance in the screen centre

void RenderView()
{
    float dirX = Cos(P.a), dirY = Sin(P.a);
    float planeX = -dirY * Plane, planeY = dirX * Plane;
    PrepareSprites(dirX, dirY, planeX, planeY);

    int32_t px = (int32_t)(P.x * 65536.0f), py = (int32_t)(P.y * 65536.0f);
    int32_t dx = (int32_t)(dirX * 65536.0f), dy = (int32_t)(dirY * 65536.0f);
    int32_t plx = (int32_t)(planeX * 65536.0f), ply = (int32_t)(planeY * 65536.0f);

    // weapon placement
    const uint8_t* gun = P.fireTimer > 220 ? g_img_gun_fire : g_img_gun;
    const uint8_t (*gunSpan)[2] = P.fireTimer > 220 ? g_img_gun_fire_span : g_img_gun_span;
    int gunC0 = Rays / 2 - GunW / 2;
    int bob = (int)(Fabs(Sin(P.bob)) * 8);
    int gunTop = ViewH - GunW * GunScale + bob;    // screen row of gun texture row 0
    bool hurt = P.hurtTimer > 0;
    uint16_t hurtClr = 0xf800;

    PF::Begin(ViewX, ViewY, ViewW, ViewH);
    for (int c = 0; c < Rays; c++)
    {
        // camera x from -1 to 1, 16.16
        int32_t camX = ((2 * c + 1) << 16) / Rays - 0x10000;
        int32_t rdx = dx + MulQ16(plx, camX);
        int32_t rdy = dy + MulQ16(ply, camX);
        THit h;
        uint16_t* col = g_col;
        memcpy(col, g_bg, sizeof(g_bg));
        int32_t z = 0x7fffffff;
        if (CastRay(px, py, rdx, rdy, h))
        {
            z = h.dist;
            int32_t dist = h.dist < 0x400 ? 0x400 : h.dist;
            int lineH = (int)(((int64_t)K << 16) / dist);
            int top = ViewH / 2 - lineH / 2;        // screen row, from top
            int y0 = top < 0 ? 0 : top;
            int y1 = top + lineH > ViewH ? ViewH : top + lineH;
            int32_t step = (TexSize << 16) / (lineH ? lineH : 1);
            int32_t tp = (y0 - top) * step;
            int shade = ShadeFor(dist) + h.side;
            if (shade > NumShades - 1) shade = NumShades - 1;
            const uint16_t* pal = g_shade[shade];
            const uint8_t* tex = g_walls[h.tex] + h.u * TexSize;
            uint16_t* out = col + (ViewH - 1 - y0);
            for (int y = y0; y < y1; y++)
            {
                *out-- = pal[tex[(tp >> 16) & 63]];
                tp += step;
            }
        }
        if (c == Rays / 2)
            g_zCentre = z;

        // sprites, far to near
        for (int i = 0; i < g_nVis; i++)
        {
            const TVis& v = g_vis[i];
            if (c < v.c0 || c >= v.c1 || v.depth >= z)
                continue;
            int u = ((c - v.cStart) * v.uStep) >> 16;
            if (u > 63) u = 63;
            const uint8_t* span = g_spriteSpans[v.sprite][u];
            if (span[0] >= span[1])
                continue;
            const uint8_t* tex = g_sprites[v.sprite] + u * TexSize;
            const uint16_t* pal = g_shade[v.shade];
            int top = ViewH / 2 - v.size / 2;
            int y0 = top + span[0] * v.size / TexSize;
            int y1 = top + span[1] * v.size / TexSize;
            if (y0 < 0) y0 = 0;
            if (y1 > ViewH) y1 = ViewH;
            int32_t step = (TexSize << 16) / v.size;
            int32_t tp = (y0 - top) * step;
            uint16_t* out = col + (ViewH - 1 - y0);
            for (int y = y0; y < y1; y++, out--)
            {
                uint8_t ix = tex[(tp >> 16) & 63];
                tp += step;
                if (ix)
                    *out = pal[ix];
            }
        }

        // weapon
        int gu = c - gunC0;
        if (gu >= 0 && gu < GunW && gunSpan[gu][0] < gunSpan[gu][1])
        {
            const uint8_t* tex = gun + gu * GunW;
            int y0 = gunTop + gunSpan[gu][0] * GunScale;
            int y1 = gunTop + gunSpan[gu][1] * GunScale;
            if (y1 > ViewH) y1 = ViewH;
            for (int y = y0; y < y1; y++)
            {
                uint8_t ix = tex[(y - gunTop) / GunScale];
                if (ix)
                    col[ViewH - 1 - y] = g_shade[0][ix];
            }
        }

        if (hurt)
        {
            for (int y = 0; y < 3; y++)
                col[y] = col[ViewH - 1 - y] = hurtClr;
            if (c < 2 || c >= Rays - 2)
                for (int y = 0; y < ViewH; y++)
                    col[y] = hurtClr;
        }
        // crosshair
        if (c == Rays / 2)
        {
            col[ViewH / 2 - 3] = col[ViewH / 2 + 2] = 0xffff;
            col[ViewH / 2 - 4] = col[ViewH / 2 + 3] = 0xffff;
        }
        for (int i = 0; i < ColW; i++)
            PF::Write(col, ViewH);
    }
    PF::End();
}

// ------------------------------------------------------------ HUD
struct THudCache { int hp, ammo, score, level, kills, treasure, fps; } g_hud;
bool g_showMap = true;
int g_mapTimer;

void DrawFrame()
{
    PF::Bar(0, 0, PF::ScreenW, ViewY, ClrPanel);
    PF::Bar(0, ViewY + ViewH, PF::ScreenW, PF::ScreenH, ClrPanel);
    PF::Bar(PanelX, ViewY, PF::ScreenW, ViewY + ViewH, ClrPanel);
    PF::Print(4, ViewY + ViewH + 3, ClrDim, ClrPanel, "F1 fire F3 use F4 map/strafe F2 quit");
    memset(&g_hud, 0xff, sizeof(g_hud));
    g_msgDirty = true;
    g_mapTimer = 0;
}

void DrawMiniMap()
{
    int x0 = MiniX, y0 = MiniY;
    if (!g_showMap)
    {
        PF::Bar(x0, y0, x0 + MapW * MapPx, y0 + MapH * MapPx, ClrPanel);
        return;
    }
    for (int y = 0; y < MapH; y++)
    {
        // run length of equal cells, fewer LCD calls
        int x = 0;
        while (x < MapW)
        {
            uint8_t c = g_map[y][x];
            uint16_t clr = c == CellEmpty ? 0x0000 : c == CellDoor ? 0xfd20 : c == CellExit ? 0x07e0 : 0x7bef;
            int x1 = x + 1;
            while (x1 < MapW && g_map[y][x1] == c) x1++;
            PF::Bar(x0 + x * MapPx, y0 + y * MapPx, x0 + x1 * MapPx, y0 + (y + 1) * MapPx, clr);
            x = x1;
        }
    }
    for (int i = 0; i < g_nObjs; i++)
    {
        const TObj& o = g_objs[i];
        if (!o.active || o.type != ObjEnemy || o.state >= EnDying)
            continue;
        int ex = x0 + (int)(o.x * MapPx), ey = y0 + (int)(o.y * MapPx);
        PF::Bar(ex - 1, ey - 1, ex + 1, ey + 1, 0xf800);
    }
    int ppx = x0 + (int)(P.x * MapPx), ppy = y0 + (int)(P.y * MapPx);
    PF::Bar(ppx - 1, ppy - 1, ppx + 2, ppy + 2, 0xffe0);
    int hx = ppx + (int)(Cos(P.a) * 5), hy = ppy + (int)(Sin(P.a) * 5);
    PF::Bar(hx, hy, hx + 1, hy + 1, 0xffff);
}

void DrawHud(int fps, int dt)
{
    int x = PanelX + 4;
    if (g_hud.hp != P.hp)
    {
        g_hud.hp = P.hp;
        PrintPadded(x, ViewY + 4, P.hp > 30 ? ClrText : 0xf800, "HP   ", P.hp, 9);
    }
    if (g_hud.ammo != P.ammo)
    {
        g_hud.ammo = P.ammo;
        PrintPadded(x, ViewY + 20, P.ammo ? ClrText : 0xf800, "AMMO ", P.ammo, 9);
    }
    if (g_hud.score != P.score)
    {
        g_hud.score = P.score;
        PrintPadded(x, ViewY + 36, 0xffe0, "PTS ", P.score, 9);
    }
    if (g_hud.kills != P.kills)
    {
        g_hud.kills = P.kills;
        char buf[16], *p = buf;
        p = Itoa(P.kills, p); *p++ = '/'; p = Itoa(P.killsTotal, p); *p++ = ' '; *p++ = ' '; *p = 0;
        PF::Print(x, ViewY + 60, ClrDim, ClrPanel, "KILLS");
        PF::Print(x, ViewY + 74, ClrText, ClrPanel, buf);
    }
    if (g_hud.treasure != P.treasure)
    {
        g_hud.treasure = P.treasure;
        char buf[16], *p = buf;
        p = Itoa(P.treasure, p); *p++ = '/'; p = Itoa(P.treasureTotal, p); *p++ = ' '; *p++ = ' '; *p = 0;
        PF::Print(x, ViewY + 92, ClrDim, ClrPanel, "GOLD");
        PF::Print(x, ViewY + 106, ClrText, ClrPanel, buf);
    }
    if (g_hud.fps != fps)
    {
        g_hud.fps = fps;
        PrintPadded(x, ViewY + ViewH + 3, ClrDim, "FPS ", fps, 8);
    }
    if (g_hud.level != g_level)
    {
        g_hud.level = g_level;
        PF::Bar(PanelX, MiniY - 1, PF::ScreenW, ViewY + ViewH, ClrPanel);
        g_mapTimer = 0;
    }
    if (g_msgDirty)
    {
        g_msgDirty = false;
        PF::Bar(0, 0, PF::ScreenW, ViewY, ClrPanel);
        if (g_msg)
            PF::Print(4, 3, 0xffe0, ClrPanel, g_msg);
        else
            PF::Print(4, 3, ClrDim, ClrPanel, g_levels[g_level].name);
    }
    g_mapTimer -= dt;
    if (g_mapTimer <= 0)
    {
        g_mapTimer = 400;
        DrawMiniMap();
    }
}

// ------------------------------------------------------------ game logic
void UpdateDoors(int dt)
{
    for (int i = 0; i < g_nDoors; i++)
    {
        TDoor& d = g_doors[i];
        const int speed = 65536 * dt / 600;     // 0.6 s to open
        switch (d.state)
        {
            case 1:
                d.open += speed;
                if (d.open >= 65536) { d.open = 65536; d.state = 2; d.timer = 4000; }
                break;
            case 2:
                d.timer -= dt;
                if (d.timer <= 0)
                {
                    // do not close on somebody
                    bool busy = (int)P.x == d.x && (int)P.y == d.y;
                    for (int j = 0; j < g_nObjs && !busy; j++)
                        busy = g_objs[j].active && g_objs[j].type == ObjEnemy &&
                            (int)g_objs[j].x == d.x && (int)g_objs[j].y == d.y;
                    if (busy)
                        d.timer = 500;
                    else
                        d.state = 3;
                }
                break;
            case 3:
                d.open -= speed;
                if (d.open <= 0) { d.open = 0; d.state = 0; }
                break;
        }
    }
}

void OpenDoor(TDoor* d)
{
    if (d && (d->state == 0 || d->state == 3))
        d->state = 1;
}

void Hurt(int dmg)
{
    P.hp -= dmg;
    if (P.hp < 0) P.hp = 0;
    P.hurtTimer = 180;
    PF::Beep(30);
}

void UpdateEnemy(TObj& o, int dt)
{
    o.timer += dt;
    if (o.cooldown > 0)
        o.cooldown -= dt;
    float dx = P.x - o.x, dy = P.y - o.y;
    float dist = Sqrt(dx * dx + dy * dy);
    switch (o.state)
    {
        case EnIdle:
            if (o.timer > 300)
            {
                o.timer = 0;
                if (dist < 10 && LineOfSight(o.x, o.y, P.x, P.y))
                    o.state = EnChase;
            }
            break;
        case EnChase:
        {
            if ((o.timer / 250) & 1) o.anim = 1; else o.anim = 0;
            if (o.timer > 400)
                o.timer = 0;
            if (dist > 1.2f)
            {
                float v = 1.4f * dt / 1000.0f / dist;
                float nx = o.x + dx * v, ny = o.y + dy * v;
                if (CanStand(nx, ny, 0.25f, &o)) { o.x = nx; o.y = ny; }
                else if (CanStand(nx, o.y, 0.25f, &o)) o.x = nx;
                else if (CanStand(o.x, ny, 0.25f, &o)) o.y = ny;
                // open doors in the way
                int cx = (int)(o.x + dx / dist * 0.6f), cy = (int)(o.y + dy / dist * 0.6f);
                if (cx >= 0 && cy >= 0 && cx < MapW && cy < MapH && g_map[cy][cx] == CellDoor)
                    OpenDoor(FindDoor(cx, cy));
            }
            if (o.cooldown <= 0 && dist < 12)
            {
                if (LineOfSight(o.x, o.y, P.x, P.y))
                {
                    o.state = EnShoot;
                    o.timer = 0;
                }
                o.cooldown = 900 + Rand(1200);
            }
            break;
        }
        case EnShoot:
            if (o.timer > 350)
            {
                // chance to hit drops with distance
                if (Rand(100) < 90 - (int)(dist * 7))
                    Hurt(4 + Rand(8));
                o.state = EnChase;
                o.timer = 0;
            }
            break;
        case EnPain:
            if (o.timer > 250) { o.state = EnChase; o.timer = 0; }
            break;
        case EnDying:
            if (o.timer > 400) { o.state = EnDead; o.timer = 0; }
            break;
    }
}

void Fire()
{
    if (P.fireTimer > 0)
        return;
    if (P.ammo <= 0)
    {
        Message("Out of ammo!", 1000);
        P.fireTimer = 300;
        return;
    }
    P.ammo--;
    P.fireTimer = 350;
    PF::Beep(15);
    // nearest enemy in the crosshair, in front of the wall
    float dirX = Cos(P.a), dirY = Sin(P.a);
    TObj* best = nullptr;
    float bestDepth = g_zCentre / 65536.0f;
    for (int i = 0; i < g_nObjs; i++)
    {
        TObj& o = g_objs[i];
        if (!o.active || o.type != ObjEnemy || o.state >= EnDying)
            continue;
        float sx = o.x - P.x, sy = o.y - P.y;
        float depth = sx * dirX + sy * dirY;
        float side = -sx * dirY + sy * dirX;
        if (depth < 0.2f || depth >= bestDepth)
            continue;
        if (Fabs(side) > 0.28f)
            continue;
        best = &o;
        bestDepth = depth;
    }
    // enemies hear the shot
    for (int i = 0; i < g_nObjs; i++)
    {
        TObj& o = g_objs[i];
        if (o.active && o.type == ObjEnemy && o.state == EnIdle)
        {
            float dx = o.x - P.x, dy = o.y - P.y;
            if (dx * dx + dy * dy < 8 * 8)
                o.state = EnChase;
        }
    }
    if (!best)
        return;
    int dmg = bestDepth < 2.0f ? 3 : (bestDepth < 5.0f ? 2 : 1);
    if (Rand(100) < 15) dmg++;
    best->hp -= dmg;
    best->timer = 0;
    if (best->hp <= 0)
    {
        best->state = EnDying;
        P.kills++;
        P.score += 100;
    }
    else
        best->state = EnPain;
}

// first non empty cell in front of the player, within reach
bool CellInFront(int& cx, int& cy)
{
    for (float r = 0.5f; r <= 1.3f; r += 0.2f)
    {
        cx = (int)(P.x + Cos(P.a) * r);
        cy = (int)(P.y + Sin(P.a) * r);
        if (cx < 0 || cy < 0 || cx >= MapW || cy >= MapH)
            return false;
        if (g_map[cy][cx] != CellEmpty)
            return true;
    }
    return false;
}

void Use()
{
    int cx, cy;
    if (!CellInFront(cx, cy))
        return;
    if (g_map[cy][cx] == CellDoor)
    {
        TDoor* d = FindDoor(cx, cy);
        if (d && d->state == 2)
            d->timer = 0;   // close
        else
            OpenDoor(d);
    }
}

bool FacingExit()
{
    int cx, cy;
    return CellInFront(cx, cy) && g_map[cy][cx] == CellExit;
}

void PickItems()
{
    for (int i = 0; i < g_nObjs; i++)
    {
        TObj& o = g_objs[i];
        if (!o.active)
            continue;
        float dx = o.x - P.x, dy = o.y - P.y;
        if (dx * dx + dy * dy > 0.45f * 0.45f)
            continue;
        switch (o.type)
        {
            case ObjMedkit:
                if (P.hp >= 100) break;
                P.hp = P.hp + 25 > 100 ? 100 : P.hp + 25;
                o.active = 0;
                Message("Medkit +25");
                PF::Beep(20);
                break;
            case ObjAmmo:
                P.ammo += 8;
                o.active = 0;
                Message("Ammo +8");
                PF::Beep(20);
                break;
            case ObjTreasure:
                P.score += 500;
                P.treasure++;
                o.active = 0;
                Message("Treasure +500");
                PF::Beep(20);
                break;
            case ObjEnemy:
                // a dead robot sometimes leaves ammo
                break;
        }
    }
}

void MovePlayer(float fwd, float strafe, float turn, int dt)
{
    P.a += turn * dt / 1000.0f;
    if (P.a > 6.2831853f) P.a -= 6.2831853f;
    if (P.a < 0) P.a += 6.2831853f;
    float dirX = Cos(P.a), dirY = Sin(P.a);
    float mx = (dirX * fwd - dirY * strafe) * dt / 1000.0f;
    float my = (dirY * fwd + dirX * strafe) * dt / 1000.0f;
    if (mx == 0 && my == 0)
        return;
    const float r = 0.22f;
    if (CanStand(P.x + mx, P.y, r, nullptr)) P.x += mx;
    if (CanStand(P.x, P.y + my, r, nullptr)) P.y += my;
    P.bob += (Fabs(fwd) + Fabs(strafe)) * dt / 300.0f;
}

void WaitRelease()
{
    while (PF::Keys())
        ;
}

enum EGameState { GsPlay, GsDead, GsLevelDone, GsWon };

} // namespace

#ifdef HOST
#include <stdio.h>
// test hooks for the PC harness
void GameDebugTeleport(float x, float y, float a) { P.x = x; P.y = y; P.a = a; }
int GameDebugKills() { return P.kills; }
void GameDebugDump()
{
    for (int i = 0; i < g_nObjs; i++)
        if (g_objs[i].type == ObjEnemy)
            printf("enemy %d at %.2f %.2f state %d hp %d\n", i, g_objs[i].x, g_objs[i].y, g_objs[i].state, g_objs[i].hp);
    printf("player %.2f %.2f a %.2f zc %.2f\n", P.x, P.y, P.a, g_zCentre / 65536.0f);
}
int GameDebugHp() { return P.hp; }
int GameDebugLevel() { return g_level; }
const char* GameDebugMap(int level, int y) { return g_levels[level].rows[y]; }
#endif

int GameMain()
{
    g_rand ^= PF::Tick();
    LoadLevel(0, false);
    DrawFrame();
    Message("Find the exit switch. F3 opens doors.", 4000);
    WaitRelease();

    EGameState gs = GsPlay;
    uint32_t last = PF::Tick();
    uint32_t fpsTime = last;
    int frames = 0, fps = 0;
    uint32_t prevKeys = 0;
    bool f4Used = false;

    for (;;)
    {
        uint32_t now = PF::Tick();
        int dt = (int)(now - last);
        if (dt > 100) dt = 100;
        last = now;

        uint32_t keys = PF::Keys();
        uint32_t pressed = keys & ~prevKeys;
        uint32_t released = prevKeys & ~keys;
        prevKeys = keys;

        if (pressed & PF::KeyF2)
        {
            WaitRelease();
            return 0;
        }

        if (gs == GsPlay)
        {
            float fwd = 0, strafe = 0, turn = 0;
            bool strafeMode = keys & PF::KeyF4;
            if (keys & PF::KeyUp) fwd += 2.8f;
            if (keys & PF::KeyDown) fwd -= 2.0f;
            if (keys & PF::KeyLeft) { if (strafeMode) strafe -= 2.2f; else turn -= 2.4f; }
            if (keys & PF::KeyRight) { if (strafeMode) strafe += 2.2f; else turn += 2.4f; }
            if (strafeMode && (keys & (PF::KeyLeft | PF::KeyRight)))
                f4Used = true;
            if (pressed & PF::KeyF4)
                f4Used = false;
            if ((released & PF::KeyF4) && !f4Used)
            {
                g_showMap = !g_showMap;
                g_mapTimer = 0;
            }
            MovePlayer(fwd, strafe, turn, dt);
            if (keys & PF::KeyF1)
                Fire();
            if (pressed & PF::KeyF3)
            {
                if (FacingExit())
                {
                    gs = g_level + 1 < NumLevels ? GsLevelDone : GsWon;
                    P.score += P.hp * 5;
                    Message(gs == GsWon ? "YOU WIN!  F1 to play again" : "Level complete!  F1 to continue", 0);
                    PF::Beep(200);
                }
                else
                    Use();
            }
            PickItems();
            for (int i = 0; i < g_nObjs; i++)
                if (g_objs[i].active && g_objs[i].type == ObjEnemy)
                    UpdateEnemy(g_objs[i], dt);
            if (P.hp <= 0)
            {
                gs = GsDead;
                Message("You are dead.  F1 to restart the level", 0);
            }
        }
        else if (pressed & PF::KeyF1)
        {
            if (gs == GsLevelDone)
                LoadLevel(g_level + 1, true);
            else if (gs == GsWon)
                LoadLevel(0, false);
            else
            {
                int score = P.score;
                LoadLevel(g_level, false);
                P.score = score / 2;
            }
            gs = GsPlay;
            g_msg = nullptr;
            DrawFrame();
            prevKeys = keys;
        }

        UpdateDoors(dt);
        if (P.fireTimer > 0) P.fireTimer -= dt;
        if (P.hurtTimer > 0) P.hurtTimer -= dt;
        if (g_msg && g_msgTimer > 0)
        {
            g_msgTimer -= dt;
            if (g_msgTimer <= 0) { g_msg = nullptr; g_msgDirty = true; }
        }

        RenderView();
        DrawHud(fps, dt);

        frames++;
        if (now - fpsTime >= 1000)
        {
            fps = frames;
            frames = 0;
            fpsTime = now;
        }
    }
}
