#pragma once

// Map legend
//  walls: # brick, S stone, W wood, M metal, T tech, G moss
//         D door (needs walls on two opposite sides), X exit switch
//  P player start (facing east), e enemy, h medkit, a ammo, t treasure
//  b barrel, p pillar (both block), l lamp
enum { MapW = 24, MapH = 24, NumLevels = 2 };

struct TLevel
{
    const char* name;
    uint32_t ceilRgb;
    uint32_t floorRgb;
    const char* rows[MapH];
};

static const TLevel g_levels[NumLevels] = {
{
    "E1: Old Bunker", 0x383848, 0x4a4238,
    {
    "SSSSSSSSSSSSSSSSSSSSSSSS",
    "S......S.....l.....S...S",
    "S.P....D...........D.e.S",
    "S......S..b.....b..S...S",
    "SSSSDSSSSSS..e..SSSSS.aS",
    "S....S...S.......S...S.S",
    "S.h..S.t.S..p.p..S.e.S.S",
    "S....S...SS.....SS...S.S",
    "S.e..SSDSSSSSDSSSSSDSS.S",
    "S....S.................S",
    "SSDSSS..e...........t..S",
    "#......#####D#####.....S",
    "#..a...#.........#..e..S",
    "#......#..l...l..#.....S",
    "#..e...D....e....D.....S",
    "#......#..p...p..#..h..S",
    "#..t...#.........#.....S",
    "######D###########SSDSSS",
    "#....................e.S",
    "#.b.....MMMMDMMMM......S",
    "#.......M...e...M...a..S",
    "#..e..h.M.t...t.M......S",
    "#.......M.......M......S",
    "SSSSSSSSMMMMXMMMMSSSSSSS",
    }
},
{
    "E2: Machine Halls", 0x30343c, 0x3c3c40,
    {
    "MMMMMMMMMMMMMMMMMMMMMMMM",
    "M.P..M.....e....M......M",
    "M....D..........D..e...M",
    "M..a.M..T....T..M...h..M",
    "MMDMMM..........MMMMDMMM",
    "W....M....e.....M......W",
    "W.e..MMMMMDMMMMMM..t...W",
    "W....W.........W.......W",
    "W.t..W..b...b..W...e...W",
    "W....D....e....D.......W",
    "W..h.W.........W..a....W",
    "WWDWWW..b...b..WWWWDWWWW",
    "T......WWWWWWWWW.......T",
    "T..e...W..l.l..W..e....T",
    "T......D...t...D.......T",
    "T.a....W..e....W...p...T",
    "T......WWWWWWWWW.......T",
    "TTTDTTTTTTTTTTTTTTTTDTTT",
    "G......e.....G.........G",
    "G..p.....p...D....e....G",
    "G............G.....h...G",
    "G..t..e....a.G..e......G",
    "G............G.........G",
    "GGGGGGGGGGGGGGGGGGGGXGGG",
    }
},
};
