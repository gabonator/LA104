examples = 
{
  'aalines.cpp': '#include <library.h>\n' +
    '\n' +
    'using namespace BIOS;\n' +
    '\n' +
    'typedef int fix16_t;\n' +
    'void drawline_aa(fix16_t fx1, fix16_t fy1, fix16_t fx2, fix16_t fy2, int color);\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    int x = 0, y = 0;\n' +
    '    BIOS::KEY::EKey key;\n' +
    '    while ((key = KEY::GetKey()) != BIOS::KEY::EKey::Escape)\n' +
    '    {\n' +
    '        int x1 = rand() % BIOS::LCD::Width;\n' +
    '        int y1 = rand() % BIOS::LCD::Height;\n' +
    '        int c = rand() & 0xffff;\n' +
    '        drawline_aa(x*256, y*256, x1*256, y1*256, c);\n' +
    '        x = x1;\n' +
    '        y = y1;\n' +
    '    }\n' +
    '    \n' +
    '    return 0;\n' +
    '}\n' +
    '\n' +
    '\n' +
    '// https://github.com/PetteriAimonen/QuadPawn/blob/master/Runtime/drawing.c\n' +
    '\n' +
    '/* Antialiased line drawing */\n' +
    '\n' +
    '// Alpha-blend two colors together. Alpha is 0 to 255.\n' +
    '// The ratios have been biased a bit to make the result look\n' +
    '// better on a cheap TFT.\n' +
    'int blend(int fg, int bg, int alpha)\n' +
    '{\n' +
    '    int fg_per_2 = (fg & 0xF7DE) >> 1;\n' +
    '    int fg_per_4 = (fg & 0xE79C) >> 2;\n' +
    '    int fg_per_8 = (fg & 0xC718) >> 3;\n' +
    '    \n' +
    '    int bg_per_2 = (bg & 0xF7DE) >> 1;\n' +
    '    int bg_per_4 = (bg & 0xE79C) >> 2;\n' +
    '    int bg_per_8 = (bg & 0xC718) >> 3;\n' +
    '    \n' +
    '    if (alpha > 224)\n' +
    '        return fg; // 100% blend\n' +
    '    else if (alpha > 192)\n' +
    '        return (fg - fg_per_8 + bg_per_8); // 88% blend\n' +
    '    else if (alpha > 128)\n' +
    '        return (fg - fg_per_4 + bg_per_4); // 75% blend\n' +
    '    else if (alpha > 64)\n' +
    '        return (fg_per_2 + bg_per_2); // 50% blend\n' +
    '    else if (alpha > 32)\n' +
    '        return (fg_per_4 + bg - bg_per_4); // 25% blend\n' +
    '    else\n' +
    '        return bg; // 0% blend\n' +
    '}\n' +
    '\n' +
    '\n' +
    '// Draws antialiased lines\n' +
    "// Xiaolin Wu's algorithm, using x/256 fixed point values\n" +
    'void drawline_aa(fix16_t x1, fix16_t y1, fix16_t x2, fix16_t y2, int color)\n' +
    '{\n' +
    '    bool reverse_xy = false;\n' +
    '    \n' +
    '    auto swap = [](int *x, int *y) {\n' +
    '        int temp = *x;\n' +
    '        *x = *y;\n' +
    '        *y = temp;\n' +
    '    };\n' +
    '    \n' +
    '    // plot the pixel at (x, y) with brightness c\n' +
    '    auto plot = [&](int x, int y, int c) {\n' +
    '        if (reverse_xy)\n' +
    '            swap(&x, &y);\n' +
    '        \n' +
    '        uint16_t oldcolor = BIOS::LCD::GetPixel(x >> 8, y >> 8);\n' +
    '        BIOS::LCD::PutPixel(x >> 8, y >> 8, blend(color, oldcolor, c));\n' +
    '    };\n' +
    '    \n' +
    '    // Integer part of x\n' +
    '    auto ipart = [](int x) -> int {\n' +
    '        return x & (~0xFF);\n' +
    '    };\n' +
    '    \n' +
    '    auto round = [&](int x) -> int {\n' +
    '        return ipart(x + 128);\n' +
    '    };\n' +
    '    \n' +
    '    // Fractional part of x\n' +
    '    auto fpart = [](int x) -> int {\n' +
    '        return x & 0xFF;\n' +
    '    };\n' +
    '    \n' +
    '    // Remaining fractional part of x\n' +
    '    auto rfpart = [&](int x) -> int {\n' +
    '        return 256 - fpart(x);\n' +
    '    };\n' +
    '\n' +
    '    int dx = x2 - x1;\n' +
    '    int dy = y2 - y1;\n' +
    '    if (abs(dx) < abs(dy))\n' +
    '    {\n' +
    '        swap(&x1, &y1);\n' +
    '        swap(&x2, &y2);\n' +
    '        swap(&dx, &dy);\n' +
    '        reverse_xy = true;\n' +
    '    }\n' +
    '    \n' +
    '    if (x2 < x1)\n' +
    '    {\n' +
    '        swap(&x1, &x2);\n' +
    '        swap(&y1, &y2);\n' +
    '    }\n' +
    '    \n' +
    '    int gradient = dy * 256 / dx;\n' +
    '    \n' +
    '    // handle first endpoint\n' +
    '    int xend = round(x1);\n' +
    '    int yend = y1 + gradient * (xend - x1) / 256;\n' +
    '    int xgap = rfpart(x1 + 128);\n' +
    '    int xpxl1 = xend;  // this will be used in the main loop\n' +
    '    int ypxl1 = ipart(yend);\n' +
    '    plot(xpxl1, ypxl1, rfpart(yend) * xgap / 256);\n' +
    '    plot(xpxl1, ypxl1 + 256, fpart(yend) * xgap / 256);\n' +
    '    int intery = yend + gradient; // first y-intersection for the main loop\n' +
    '    \n' +
    '    // handle second endpoint\n' +
    '    xend = round(x2);\n' +
    '    yend = y2 + gradient * (xend - x2) / 256;\n' +
    '    xgap = fpart(x2 + 128);\n' +
    '    int xpxl2 = xend;  // this will be used in the main loop\n' +
    '    int ypxl2 = ipart(yend);\n' +
    '    plot(xpxl2, ypxl2, rfpart(yend) * xgap / 256);\n' +
    '    plot(xpxl2, ypxl2 + 256, fpart(yend) * xgap / 256);\n' +
    '    \n' +
    '    // main loop\n' +
    '    for (int x = xpxl1 + 1; x <= xpxl2 - 1; x += 256)\n' +
    '    {\n' +
    '        plot(x, ipart(intery), rfpart(intery));\n' +
    '        plot(x, ipart(intery) + 256, fpart(intery));\n' +
    '        intery = intery + gradient;\n' +
    '    }\n' +
    '}\n' +
    '\n' +
    '/* Non-antialiased line drawing */\n' +
    '\n' +
    'void drawline(int x1, int y1, int x2, int y2, int color, int dots)\n' +
    '{\n' +
    '    // Algorithm from here: http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Simplification\n' +
    '    int dx = abs(x2 - x1);\n' +
    '    int dy = abs(y2 - y1);\n' +
    '    \n' +
    '    int sx = (x1 < x2) ? 1 : -1;\n' +
    '    int sy = (y1 < y2) ? 1 : -1;\n' +
    '    \n' +
    '    int err = dx - dy;\n' +
    '    int count = 0;\n' +
    '    for(;; count++)\n' +
    '    {\n' +
    '        if (!dots || (count >> (dots - 1)) & 1)\n' +
    '        {\n' +
    '            //__Point_SCR(x1, y1);\n' +
    '            //__LCD_SetPixl(color);\n' +
    '        }\n' +
    '        \n' +
    '        if (x1 == x2 && y1 == y2) break;\n' +
    '        \n' +
    '        int e2 = 2 * err;\n' +
    '        if (e2 > -dy)\n' +
    '        {\n' +
    '            err -= dy;\n' +
    '            x1 += sx;\n' +
    '        }\n' +
    '        else if (e2 < dx)\n' +
    '        {\n' +
    '            err += dx;\n' +
    '            y1 += sy;\n' +
    '        }\n' +
    '    }\n' +
    '}\n' +
    '\n' +
    '\n',
  'bars.cpp': '#include <library.h>\n' +
    '\n' +
    'using namespace BIOS;\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::KEY::EKey key;\n' +
    '    while ((key = KEY::GetKey()) != BIOS::KEY::EKey::Escape)\n' +
    '    {\n' +
    '        int x1 = rand() % BIOS::LCD::Width;\n' +
    '        int y1 = rand() % BIOS::LCD::Height;\n' +
    '        int x2 = rand() % BIOS::LCD::Width;\n' +
    '        int y2 = rand() % BIOS::LCD::Height;\n' +
    '        int c = rand() & 0xffff;\n' +
    '        if (x2 > x1 && y2 > y1)\n' +
    '            BIOS::LCD::Bar(x1, y1, x2, y2, c);\n' +
    '    }\n' +
    '    \n' +
    '    return 0;\n' +
    '}\n',
  'chart.cpp': '// Not working\n' +
    '#include <library.h>\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::DBG::Print(R"(\n' +
    '        <script src="https://code.highcharts.com/highcharts.js"></script>\n' +
    '        <div id="container" style="width:300px; background:#808080;"></div>)");\n' +
    '\n' +
    '    BIOS::DBG::Print(R"(<script>\n' +
    "        Highcharts.chart('container', {\n" +
    '            chart: {\n' +
    '                type: "spline",\n' +
    '                events: {\n' +
    '                    load: function () {\n' +
    '                        addPoint = y => this.series[0].addPoint([(new Date()).getTime(), y]);\n' +
    '                    }\n' +
    '                }\n' +
    '            },\n' +
    '            time: {useUTC: false},\n' +
    "            title: {text: 'Live random data'},\n" +
    "            xAxis: {type: 'datetime', tickPixelInterval: 150},\n" +
    "            yAxis: {title: {text: 'Value'}},\n" +
    '            series: [{\n' +
    "                name: 'Random data',\n" +
    '                data: []\n' +
    '            }]\n' +
    '        });\n' +
    '    </script>)");\n' +
    '    \n' +
    '\n' +
    '    while (BIOS::KEY::GetKey() == BIOS::KEY::EKey::None)\n' +
    '    {\n' +
    '        BIOS::SYS::DelayMs(1000);\n' +
    '        //BIOS::DBG::Print(R"(<script>addPoint(Math.random())</script>)");\n' +
    '    }\n' +
    '    return 0;\n' +
    '}\n',
  'colorpicker.cpp': '#include <library.h>\n' +
    '\n' +
    'uint32_t color = RGB32(255, 0, 0);\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::DBG::Print(R"-(<input type="color" onChange="updateColor(this.value)" value="#ff0000">)-");\n' +
    '\n' +
    '    BIOS::DBG::Print(R"(<script>\n' +
    '      function updateColor(html)\n' +
    '      {\n' +
    '        var color = parseInt("0x"+html.substr(1));\n' +
    '        BIOS.memWrite(colorPtr, [color >> 16, color >> 8, color]);\n' +
    '      }\n' +
    '    </script>)");\n' +
    '    BIOS::DBG::Print(R"(<script>colorPtr = 0x%08x;</script>)", &color);\n' +
    '\n' +
    '    while (BIOS::KEY::GetKey() == BIOS::KEY::EKey::None)\n' +
    '    {\n' +
    '        BIOS::LCD::Bar(40, 40, BIOS::LCD::Width-40, BIOS::LCD::Height-40, \n' +
    '            RGB32TO565(color));\n' +
    '        BIOS::SYS::DelayMs(20);\n' +
    '    }\n' +
    '    return 0;\n' +
    '}\n',
  'counter.cpp': '#include <library.h>\n' +
    '\n' +
    'using namespace BIOS;\n' +
    '\n' +
    'int counter = 0;\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::KEY::EKey key;\n' +
    '    while ((key = KEY::GetKey()) != BIOS::KEY::EKey::Escape)\n' +
    '    {\n' +
    '        BIOS::DBG::Print("Test %d, ", counter++);\n' +
    '        BIOS::SYS::DelayMs(200);\n' +
    '    }\n' +
    '    \n' +
    '    return 0;\n' +
    '}',
  'demo.cpp': '#include <library.h>\n' +
    '\n' +
    'using namespace BIOS;\n' +
    '\n' +
    'typedef int fix16_t;\n' +
    'void drawline_aa(fix16_t fx1, fix16_t fy1, fix16_t fx2, fix16_t fy2, int color);\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    int x = 100, y = 100, c = 0;\n' +
    '    KEY::EKey key;\n' +
    '    while ((key = KEY::GetKey()) != KEY::EKey::Escape)\n' +
    '    {\n' +
    '        int x1 = rand() % LCD::Width;\n' +
    '        int y1 = 16+(rand() % (LCD::Height-32));\n' +
    '        int c = rand() & 0xffff;\n' +
    '        drawline_aa(x*256, y*256, x1*256, y1*256, c);\n' +
    '        x = x1;\n' +
    '        y = y1;\n' +
    '\n' +
    '        if ((c++ % 1000) == 0)\n' +
    '            DBG::Print("<b>%d</b> lines, ", c);\n' +
    '    }\n' +
    '    \n' +
    '    return 0;\n' +
    '}\n' +
    '\n' +
    '\n' +
    '\n' +
    '\n' +
    '// https://github.com/PetteriAimonen/QuadPawn/blob/master/Runtime/drawing.c\n' +
    '\n' +
    '/* Antialiased line drawing */\n' +
    '\n' +
    '// Alpha-blend two colors together. Alpha is 0 to 255.\n' +
    '// The ratios have been biased a bit to make the result look\n' +
    '// better on a cheap TFT.\n' +
    'int blend(int fg, int bg, int alpha)\n' +
    '{\n' +
    '    int fg_per_2 = (fg & 0xF7DE) >> 1;\n' +
    '    int fg_per_4 = (fg & 0xE79C) >> 2;\n' +
    '    int fg_per_8 = (fg & 0xC718) >> 3;\n' +
    '    \n' +
    '    int bg_per_2 = (bg & 0xF7DE) >> 1;\n' +
    '    int bg_per_4 = (bg & 0xE79C) >> 2;\n' +
    '    int bg_per_8 = (bg & 0xC718) >> 3;\n' +
    '    \n' +
    '    if (alpha > 224)\n' +
    '        return fg; // 100% blend\n' +
    '    else if (alpha > 192)\n' +
    '        return (fg - fg_per_8 + bg_per_8); // 88% blend\n' +
    '    else if (alpha > 128)\n' +
    '        return (fg - fg_per_4 + bg_per_4); // 75% blend\n' +
    '    else if (alpha > 64)\n' +
    '        return (fg_per_2 + bg_per_2); // 50% blend\n' +
    '    else if (alpha > 32)\n' +
    '        return (fg_per_4 + bg - bg_per_4); // 25% blend\n' +
    '    else\n' +
    '        return bg; // 0% blend\n' +
    '}\n' +
    '\n' +
    '\n' +
    '// Draws antialiased lines\n' +
    "// Xiaolin Wu's algorithm, using x/256 fixed point values\n" +
    'void drawline_aa(fix16_t x1, fix16_t y1, fix16_t x2, fix16_t y2, int color)\n' +
    '{\n' +
    '    bool reverse_xy = false;\n' +
    '    \n' +
    '    auto swap = [](int *x, int *y) {\n' +
    '        int temp = *x;\n' +
    '        *x = *y;\n' +
    '        *y = temp;\n' +
    '    };\n' +
    '    \n' +
    '    // plot the pixel at (x, y) with brightness c\n' +
    '    auto plot = [&](int x, int y, int c) {\n' +
    '        if (reverse_xy)\n' +
    '            swap(&x, &y);\n' +
    '        \n' +
    '        uint16_t oldcolor = BIOS::LCD::GetPixel(x >> 8, y >> 8);\n' +
    '        BIOS::LCD::PutPixel(x >> 8, y >> 8, blend(color, oldcolor, c));\n' +
    '    };\n' +
    '    \n' +
    '    // Integer part of x\n' +
    '    auto ipart = [](int x) -> int {\n' +
    '        return x & (~0xFF);\n' +
    '    };\n' +
    '    \n' +
    '    auto round = [&](int x) -> int {\n' +
    '        return ipart(x + 128);\n' +
    '    };\n' +
    '    \n' +
    '    // Fractional part of x\n' +
    '    auto fpart = [](int x) -> int {\n' +
    '        return x & 0xFF;\n' +
    '    };\n' +
    '    \n' +
    '    // Remaining fractional part of x\n' +
    '    auto rfpart = [&](int x) -> int {\n' +
    '        return 256 - fpart(x);\n' +
    '    };\n' +
    '\n' +
    '    int dx = x2 - x1;\n' +
    '    int dy = y2 - y1;\n' +
    '    if (abs(dx) < abs(dy))\n' +
    '    {\n' +
    '        swap(&x1, &y1);\n' +
    '        swap(&x2, &y2);\n' +
    '        swap(&dx, &dy);\n' +
    '        reverse_xy = true;\n' +
    '    }\n' +
    '    \n' +
    '    if (x2 < x1)\n' +
    '    {\n' +
    '        swap(&x1, &x2);\n' +
    '        swap(&y1, &y2);\n' +
    '    }\n' +
    '    \n' +
    '    int gradient = dy * 256 / dx;\n' +
    '    \n' +
    '    // handle first endpoint\n' +
    '    int xend = round(x1);\n' +
    '    int yend = y1 + gradient * (xend - x1) / 256;\n' +
    '    int xgap = rfpart(x1 + 128);\n' +
    '    int xpxl1 = xend;  // this will be used in the main loop\n' +
    '    int ypxl1 = ipart(yend);\n' +
    '    plot(xpxl1, ypxl1, rfpart(yend) * xgap / 256);\n' +
    '    plot(xpxl1, ypxl1 + 256, fpart(yend) * xgap / 256);\n' +
    '    int intery = yend + gradient; // first y-intersection for the main loop\n' +
    '    \n' +
    '    // handle second endpoint\n' +
    '    xend = round(x2);\n' +
    '    yend = y2 + gradient * (xend - x2) / 256;\n' +
    '    xgap = fpart(x2 + 128);\n' +
    '    int xpxl2 = xend;  // this will be used in the main loop\n' +
    '    int ypxl2 = ipart(yend);\n' +
    '    plot(xpxl2, ypxl2, rfpart(yend) * xgap / 256);\n' +
    '    plot(xpxl2, ypxl2 + 256, fpart(yend) * xgap / 256);\n' +
    '    \n' +
    '    // main loop\n' +
    '    for (int x = xpxl1 + 1; x <= xpxl2 - 1; x += 256)\n' +
    '    {\n' +
    '        plot(x, ipart(intery), rfpart(intery));\n' +
    '        plot(x, ipart(intery) + 256, fpart(intery));\n' +
    '        intery = intery + gradient;\n' +
    '    }\n' +
    '}\n' +
    '\n' +
    '/* Non-antialiased line drawing */\n' +
    '\n' +
    'void drawline(int x1, int y1, int x2, int y2, int color, int dots)\n' +
    '{\n' +
    '    // Algorithm from here: http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Simplification\n' +
    '    int dx = abs(x2 - x1);\n' +
    '    int dy = abs(y2 - y1);\n' +
    '    \n' +
    '    int sx = (x1 < x2) ? 1 : -1;\n' +
    '    int sy = (y1 < y2) ? 1 : -1;\n' +
    '    \n' +
    '    int err = dx - dy;\n' +
    '    int count = 0;\n' +
    '    for(;; count++)\n' +
    '    {\n' +
    '        if (!dots || (count >> (dots - 1)) & 1)\n' +
    '        {\n' +
    '            //__Point_SCR(x1, y1);\n' +
    '            //__LCD_SetPixl(color);\n' +
    '        }\n' +
    '        \n' +
    '        if (x1 == x2 && y1 == y2) break;\n' +
    '        \n' +
    '        int e2 = 2 * err;\n' +
    '        if (e2 > -dy)\n' +
    '        {\n' +
    '            err -= dy;\n' +
    '            x1 += sx;\n' +
    '        }\n' +
    '        else if (e2 < dx)\n' +
    '        {\n' +
    '            err += dx;\n' +
    '            y1 += sy;\n' +
    '        }\n' +
    '    }\n' +
    '}\n' +
    '\n' +
    '\n',
  'flappybird.cpp': '// Play flappy bird in browser, press F1 on LA104 to jump\n' +
    '// Based on Tiny flappy bird by Daniel Bark:\n' +
    '//   https://github.com/danba340/tiny-flappy-bird\n' +
    '\n' +
    '#include <library.h>\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::DBG::Print(R"(<canvas id="c" width="400" height="400" style="background:gray"></canvas>)");\n' +
    '\n' +
    '    BIOS::DBG::Print(R"(<script>\n' +
    '    context = document.getElementById("c").getContext("2d");\n' +
    '    bird = new Image();\n' +
    '    bird.src = "https://raw.githubusercontent.com/danba340/tiny-flappy-bird/master/bird.png";\n' +
    '    birdX = birdDY = score = bestScore = 0;\n' +
    '    interval = birdSize = pipeWidth = topPipeBottomY = 24;\n' +
    '    birdY = pipeGap = 200;\n' +
    '    canvasSize = pipeX = 400;\n' +
    '    c.onclick = () => (birdDY = 9);\n' +
    '    setInterval(() => {\n' +
    '      context.fillStyle = "skyblue";\n' +
    '      context.fillRect(0,0,canvasSize,canvasSize); // Draw sky\n' +
    '      birdY -= birdDY -= 0.5; // Gravity\n' +
    '      context.drawImage(bird, birdX, birdY, birdSize * (524/374), birdSize); // Draw bird\n' +
    '      context.fillStyle = "green";\n' +
    '      pipeX -= 8; // Move pipe\n' +
    '      pipeX < -pipeWidth && // Pipe off screen?\n' +
    '        ((pipeX = canvasSize), (topPipeBottomY = pipeGap * Math.random())); // Reset pipe and randomize gap.\n' +
    '      context.fillRect(pipeX, 0, pipeWidth, topPipeBottomY); // Draw top pipe\n' +
    '      context.fillRect(pipeX, topPipeBottomY + pipeGap, pipeWidth, canvasSize); // Draw bottom pipe\n' +
    '      context.fillStyle = "black";\n' +
    '      context.fillText(score++, 9, 25); // Increase and draw score\n' +
    '      bestScore = bestScore < score ? score : bestScore; // New best score?\n' +
    "      context.fillText('Best: '+bestScore, 9, 50); // Draw best score\n" +
    '      (((birdY < topPipeBottomY || birdY > topPipeBottomY + pipeGap) && pipeX < birdSize * (524/374))// Bird hit pipe?\n' +
    '       || birdY > canvasSize) && // Bird falls off screen\n' +
    '      ((birdDY = 0), (birdY = 200), (pipeX = canvasSize), (score = 0)); // Bird died\n' +
    '    }, interval)\n' +
    '  </script>)");\n' +
    '\n' +
    '    BIOS::KEY::EKey key;\n' +
    '    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)\n' +
    '    {\n' +
    '        if (key == BIOS::KEY::EKey::Enter)\n' +
    '            BIOS::DBG::Print(R"(<script>birdDY=9</script>)");\n' +
    '    }\n' +
    '    return 0;\n' +
    '}\n',
  'gps.cpp': '#include <library.h>\n' +
    '\n' +
    'using namespace BIOS;\n' +
    'char msg[128];\n' +
    '\n' +
    'typedef char* string;\n' +
    '\n' +
    'void getToken(string& p, char* token, char delim)\n' +
    '{\n' +
    '    while(*p != 0)\n' +
    '    {\n' +
    '        if (*p == delim)\n' +
    '        {\n' +
    '            *token = 0;\n' +
    '            *p++;\n' +
    '            return;\n' +
    '        }\n' +
    '        *token++ = *p++;\n' +
    '    }\n' +
    '    *token = 0;\n' +
    '    return;\n' +
    '}\n' +
    '\n' +
    'int ord(char c) \n' +
    '{\n' +
    "    //_ASSERT(c >= '0' && c <= '9');\n" +
    "    return c - '0';\n" +
    '}\n' +
    '\n' +
    'bool parseDegree(char* degree, int& deg, int &min, int& dec)\n' +
    '{\n' +
    "    if (strlen(degree) == 9 && degree[4] == '.')\n" +
    '    {\n' +
    '        deg = ord(degree[0])*10 + ord(degree[1]);\n' +
    '        min = ord(degree[2])*10 + ord(degree[3]);\n' +
    '        dec = ord(degree[5])*1000 + ord(degree[6])*100 +\n' +
    '          ord(degree[7])*10 + ord(degree[8]);\n' +
    '        return true;\n' +
    '    }\n' +
    '    \n' +
    "    if (strlen(degree) == 10 && degree[5] == '.')\n" +
    '    {\n' +
    '        deg = ord(degree[0])*100 + ord(degree[1])*10 +\n' +
    '            ord(degree[2])*1;\n' +
    '        min = ord(degree[3])*10 + ord(degree[4]);\n' +
    '        dec = ord(degree[6])*1000 + ord(degree[7])*100 +\n' +
    '          ord(degree[8])*10 + ord(degree[9]);\n' +
    '        return true;\n' +
    '    }\n' +
    '    return false;\n' +
    '}\n' +
    '\n' +
    'bool parseTime(char* str, int& h, int& m, int& s)\n' +
    '{\n' +
    "    if (strlen(str) != 10 || str[6] != '.')\n" +
    '        return false;\n' +
    '    h = ord(str[0])*10 + ord(str[1]);\n' +
    '    m = ord(str[2])*10 + ord(str[3]);\n' +
    '    s = ord(str[4])*10 + ord(str[5]);\n' +
    '    return true;\n' +
    '}\n' +
    '\n' +
    'void process(char* p)\n' +
    '{\n' +
    '    // http://www.technoblogy.com/show?SJ0\n' +
    '    // $GNRMC,180159.212,A,4808.8024,N,01708.5613,E,1.95,46.83,020821,,,A*4D\n' +
    '    char token[32];\n' +
    "    getToken(p, token, ',');\n" +
    '    if (strcmp(token, "$GNRMC") != 0)\n' +
    '        return;\n' +
    '\n' +
    "    getToken(p, token, ','); // time\n" +
    '    int hour, min, sec;\n' +
    '    if (!parseTime(token, hour, min, sec))\n' +
    '        return;\n' +
    '\n' +
    "    getToken(p, token, ','); \n" +
    '    if (strcmp(token, "A") != 0) // has valid signal\n' +
    '    {\n' +
    '        DBG::Print("No signal");\n' +
    '        return;\n' +
    '    }\n' +
    '\n' +
    '    int latDeg, latMin, latMinDec;\n' +
    "    getToken(p, token, ','); // latitude coord\n" +
    '    if (!parseDegree(token, latDeg, latMin, latMinDec))\n' +
    '        return;\n' +
    "    getToken(p, token, ','); // latitude N/S\n" +
    '    int latSgn = strcmp(token, "S") == 0 ? -1 : 1;\n' +
    '\n' +
    '    int lonDeg, lonMin, lonMinDec;\n' +
    "    getToken(p, token, ','); // longitude coord\n" +
    '    if (!parseDegree(token, lonDeg, lonMin, lonMinDec))\n' +
    '        return;\n' +
    "    getToken(p, token, ','); // longitude E/W\n" +
    '    int lonSgn = strcmp(token, "W") == 0 ? -1 : 1;\n' +
    '\n' +
    '    DBG::Print("<script>onData({h:%d,m:%d,s:%d,"\n' +
    '      "lat:%s(%d+(%d.%04d)/60),"\n' +
    '      "lon:%s(%d+(%d.%04d)/60)})</script>",\n' +
    '      hour, min, sec, \n' +
    '      latSgn==-1 ? "-" : "", latDeg, latMin, latMinDec,\n' +
    '      lonSgn==-1 ? "-" : "", lonDeg, lonMin, lonMinDec);\n' +
    '      /*\n' +
    '    DBG::Print("%02d:%02d:%02d GMT lat=%d&deg; %d.%04d, lon=%d&deg; %d.%04d\\n", \n' +
    '        hour, min, sec,\n' +
    '        latDeg*latSgn, latMin, latMinDec,\n' +
    '        lonDeg*lonSgn, lonMin, lonMinDec);\n' +
    '        */\n' +
    '}\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::DBG::Print(R"(\n' +
    '<div id="time"></div>\n' +
    "<div style='position:relative; width:500px; height:250px; background:#ffff00'>\n" +
    "<div style='position:absolute; left:0px; top:0px; width:500px; height:250px; background:url(https://www.world-maps.org/images/wor32k6.jpg); background-size: 500px 250px; '><div>\n" +
    "<div id='marker' style='position:absolute; width:48px; height:48px; background:url(https://img.icons8.com/fluent/48/000000/marker-storm.png);'></div>\n" +
    '</div>\n' +
    ')");\n' +
    '\n' +
    'BIOS::DBG::Print(R"(<script>\n' +
    '  function onData(x) \n' +
    '  { \n' +
    '    console.log(x); \n' +
    '    var m = document.getElementById("marker");\n' +
    '    m.style.left = (x.lon+180)/360*500 - 24 + "px";\n' +
    '    m.style.top = (-x.lat+90)/180*250 - 48 + "px";\n' +
    '    document.getElementById("time").innerHTML = "GPS time: " + \n' +
    '        [x.h, x.m, x.s].map(v=>("0"+v).substr(-2)).join(":");\n' +
    '  }\n' +
    '</script>)");\n' +
    '\n' +
    '\n' +
    '\n' +
    '    //DBG::Print("<script>function onData(x) { console.log(x); }</script>");    \n' +
    '    //process((char*)"$GNRMC,180159.212,A,4808.8024,N,01708.5613,E,1.95,46.83,020821,,,A*4D");\n' +
    '    //return 123;\n' +
    '    \n' +
    '    BIOS::GPIO::UART::Setup(9600, BIOS::GPIO::UART::length8);\n' +
    '    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);\n' +
    '    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);\n' +
    '\n' +
    '    BIOS::KEY::EKey key;\n' +
    '    while ((key = KEY::GetKey()) != BIOS::KEY::EKey::Escape)\n' +
    '    {\n' +
    '        int i = 0;\n' +
    '        while (GPIO::UART::Available())\n' +
    '        {\n' +
    '            char ch = GPIO::UART::Read();\n' +
    "            if (ch == '\\x0d' || ch == '\\x0a')\n" +
    '            {\n' +
    '                msg[i] = 0;\n' +
    '                if (i > 0)\n' +
    '                    process(msg);\n' +
    '                i = 0;\n' +
    '            }\n' +
    '            else\n' +
    '            if ( i < COUNT(msg)-1)\n' +
    '                msg[i++] = ch;\n' +
    '        }\n' +
    '        msg[i] = 0;\n' +
    '\n' +
    '      SYS::DelayMs(100);\n' +
    '    }\n' +
    '    \n' +
    '    return 0;\n' +
    '}\n',
  'hello.cpp': '#include <library.h>\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::DBG::Print("Hello world!");\n' +
    '    return 724;\n' +
    '}\n',
  'label.cpp': '#include <library.h>\n' +
    '\n' +
    'using namespace BIOS;\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::KEY::EKey key;\n' +
    '    while ((key = KEY::GetKey()) != BIOS::KEY::EKey::Escape)\n' +
    '    {\n' +
    '        int x1 = rand() % BIOS::LCD::Width;\n' +
    '        int y1 = rand() % BIOS::LCD::Height;\n' +
    '        int x2 = rand() % BIOS::LCD::Width;\n' +
    '        int y2 = rand() % BIOS::LCD::Height;\n' +
    '        int c = rand() & 0xffff;\n' +
    '        if (x2 > x1 && y2 > y1)\n' +
    '        BIOS::LCD::Print(x1, y1, c, RGBTRANS, "Hello!");\n' +
    '    }\n' +
    '    \n' +
    '    return 0;\n' +
    '}',
  'melody.cpp': '// plays Axel F on internal piezo speaker of LA104\n' +
    '#include <library.h>\n' +
    '#include <math.h>\n' +
    '\n' +
    'void WriteTim8(int reg, uint16_t val)\n' +
    '{\n' +
    '  *((uint16_t*)(0x40013400 + reg)) = val;\n' +
    '}\n' +
    '\n' +
    'void SoundOn()\n' +
    '{\n' +
    '  WriteTim8(0x20, 0x3000);\n' +
    '  WriteTim8(0x00, 0x0081);\n' +
    '  WriteTim8(0x40, 150); // volume\n' +
    '}\n' +
    '\n' +
    'void SoundOff()\n' +
    '{\n' +
    '  WriteTim8(0x20, 0x0000);\n' +
    '  WriteTim8(0x00, 0x0080);\n' +
    '}\n' +
    '\n' +
    'void Sound(int f)\n' +
    '{\n' +
    '    WriteTim8(0x24, 0);\n' +
    '    WriteTim8(0x28, 15-1);\n' +
    '    int div = 72e6/15/f;\n' +
    '    WriteTim8(0x2c, div-1);\n' +
    '    //BIOS::DBG::Print("f=%d, div=%d, ", f, div);\n' +
    '    SoundOn();\n' +
    '}\n' +
    '\n' +
    'int C=0, Cis=1, D=2, Dis=3, E=4, F=5, Fis=6, G=7, Gis=8, A=9, Ais=10, B=11, None=12;\n' +
    'const int BPM = 120;\n' +
    '\n' +
    'const int Beat = 60000 / BPM;\n' +
    'int Whole = Beat*4;\n' +
    'int Half = Whole/2;\n' +
    'int Quarter = Whole/4;\n' +
    'int Eigth = Whole/8;\n' +
    'int Sixteenth = Whole/16;\n' +
    'int Thirtysecond = Whole/32;\n' +
    'int WholeDot = Whole*1.5;\n' +
    'int HalfDot = Half*1.5;\n' +
    'int QuarterDot = Quarter*1.5;\n' +
    'int EigthDot = Eigth*1.5;\n' +
    'int SixteenthDot = Sixteenth*1.5;\n' +
    'int ThirtysecondDot = Thirtysecond*1.5;\n' +
    '\n' +
    'int Frequency(int note, int octave)\n' +
    '{\n' +
    '  const int table[] = {4186, 4434, 4698, 4978, 5274, 5587, 5915, 6271, 6644, 7040, 7458, 7902};\n' +
    '  return table[note] >> (4-octave);\n' +
    '}\n' +
    '\n' +
    'void Play(int note, int octave, int length)\n' +
    '{\n' +
    '  if (note != None) \n' +
    '    Sound(Frequency(note, octave));\n' +
    '\n' +
    '  //BIOS::DBG::Print("len=%d, ", length);\n' +
    '  BIOS::SYS::DelayMs(length);\n' +
    '  SoundOff();\n' +
    '  BIOS::SYS::DelayMs(10);\n' +
    '}\n' +
    '\n' +
    'int main()\n' +
    '{\n' +
    '    // generated from http://x.valky.eu/klavirgen\n' +
    '    Play(G, 2, Quarter);\n' +
    '    Play(Ais, 2, EigthDot);\n' +
    '    Play(G, 2, Sixteenth);\n' +
    '    Play(None, 0, Sixteenth);\n' +
    '    Play(G, 2, Sixteenth);\n' +
    '    Play(C, 3, Eigth);\n' +
    '    Play(G, 2, Eigth);\n' +
    '    Play(F, 2, Eigth);\n' +
    '    Play(G, 2, Quarter);\n' +
    '    Play(D, 3, EigthDot);\n' +
    '    Play(G, 2, Sixteenth);\n' +
    '    Play(None, 0, Sixteenth);\n' +
    '    Play(G, 2, Sixteenth);\n' +
    '    Play(Dis, 3, Eigth);\n' +
    '    Play(D, 3, Eigth);\n' +
    '    Play(Ais, 2, Eigth);\n' +
    '    Play(G, 2, Eigth);\n' +
    '    Play(D, 3, Eigth);\n' +
    '    Play(G, 3, Eigth);\n' +
    '    Play(G, 2, Sixteenth);\n' +
    '    Play(F, 2, Sixteenth);\n' +
    '    Play(None, 0, Sixteenth);\n' +
    '    Play(F, 2, Sixteenth);\n' +
    '    Play(D, 2, Eigth);\n' +
    '    Play(A, 2, Eigth);\n' +
    '    Play(G, 2, Half);\n' +
    '\n' +
    '    return 0;\n' +
    '}\n',
  'stack.cpp': '#pragma GCC optimize ("-O0")\n' +
    '#include <library.h>\n' +
    '\n' +
    'using namespace BIOS;\n' +
    '\n' +
    'typedef int fix16_t;\n' +
    'void drawline_aa(fix16_t fx1, fix16_t fy1, fix16_t fx2, fix16_t fy2, int color);\n' +
    '\n' +
    'void main5(void)\n' +
    '{\n' +
    '    int x = 100, y = 100, c = 0;\n' +
    '    KEY::EKey key;\n' +
    '    while ((key = KEY::GetKey()) != KEY::EKey::Escape)\n' +
    '    {\n' +
    '        int x1 = rand() % LCD::Width;\n' +
    '        int y1 = 16+(rand() % (LCD::Height-32));\n' +
    '        int c = rand() & 0xffff;\n' +
    '        drawline_aa(x*256, y*256, x1*256, y1*256, c);\n' +
    '        x = x1;\n' +
    '        y = y1;\n' +
    '\n' +
    '        if ((c++ % 1000) == 0)\n' +
    '            DBG::Print("<b>%d</b> lines, ", c);\n' +
    '    }\n' +
    '}\n' +
    '\n' +
    'void main4(void) { main5(); }\n' +
    'void main3(void) { main4(); }\n' +
    'void main2(void) { main3(); }\n' +
    'void main1(void) { main2(); }\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    main1();\n' +
    '    return 0;\n' +
    '}\n' +
    '\n' +
    '\n' +
    '// https://github.com/PetteriAimonen/QuadPawn/blob/master/Runtime/drawing.c\n' +
    '\n' +
    '/* Antialiased line drawing */\n' +
    '\n' +
    '// Alpha-blend two colors together. Alpha is 0 to 255.\n' +
    '// The ratios have been biased a bit to make the result look\n' +
    '// better on a cheap TFT.\n' +
    'int blend(int fg, int bg, int alpha)\n' +
    '{\n' +
    '    int fg_per_2 = (fg & 0xF7DE) >> 1;\n' +
    '    int fg_per_4 = (fg & 0xE79C) >> 2;\n' +
    '    int fg_per_8 = (fg & 0xC718) >> 3;\n' +
    '    \n' +
    '    int bg_per_2 = (bg & 0xF7DE) >> 1;\n' +
    '    int bg_per_4 = (bg & 0xE79C) >> 2;\n' +
    '    int bg_per_8 = (bg & 0xC718) >> 3;\n' +
    '    \n' +
    '    if (alpha > 224)\n' +
    '        return fg; // 100% blend\n' +
    '    else if (alpha > 192)\n' +
    '        return (fg - fg_per_8 + bg_per_8); // 88% blend\n' +
    '    else if (alpha > 128)\n' +
    '        return (fg - fg_per_4 + bg_per_4); // 75% blend\n' +
    '    else if (alpha > 64)\n' +
    '        return (fg_per_2 + bg_per_2); // 50% blend\n' +
    '    else if (alpha > 32)\n' +
    '        return (fg_per_4 + bg - bg_per_4); // 25% blend\n' +
    '    else\n' +
    '        return bg; // 0% blend\n' +
    '}\n' +
    '\n' +
    '\n' +
    '// Draws antialiased lines\n' +
    "// Xiaolin Wu's algorithm, using x/256 fixed point values\n" +
    'void drawline_aa(fix16_t x1, fix16_t y1, fix16_t x2, fix16_t y2, int color)\n' +
    '{\n' +
    '    bool reverse_xy = false;\n' +
    '    \n' +
    '    auto swap = [](int *x, int *y) {\n' +
    '        int temp = *x;\n' +
    '        *x = *y;\n' +
    '        *y = temp;\n' +
    '    };\n' +
    '    \n' +
    '    // plot the pixel at (x, y) with brightness c\n' +
    '    auto plot = [&](int x, int y, int c) {\n' +
    '        if (reverse_xy)\n' +
    '            swap(&x, &y);\n' +
    '        \n' +
    '        uint16_t oldcolor = BIOS::LCD::GetPixel(x >> 8, y >> 8);\n' +
    '        BIOS::LCD::PutPixel(x >> 8, y >> 8, blend(color, oldcolor, c));\n' +
    '    };\n' +
    '    \n' +
    '    // Integer part of x\n' +
    '    auto ipart = [](int x) -> int {\n' +
    '        return x & (~0xFF);\n' +
    '    };\n' +
    '    \n' +
    '    auto round = [&](int x) -> int {\n' +
    '        return ipart(x + 128);\n' +
    '    };\n' +
    '    \n' +
    '    // Fractional part of x\n' +
    '    auto fpart = [](int x) -> int {\n' +
    '        return x & 0xFF;\n' +
    '    };\n' +
    '    \n' +
    '    // Remaining fractional part of x\n' +
    '    auto rfpart = [&](int x) -> int {\n' +
    '        return 256 - fpart(x);\n' +
    '    };\n' +
    '\n' +
    '    int dx = x2 - x1;\n' +
    '    int dy = y2 - y1;\n' +
    '    if (abs(dx) < abs(dy))\n' +
    '    {\n' +
    '        swap(&x1, &y1);\n' +
    '        swap(&x2, &y2);\n' +
    '        swap(&dx, &dy);\n' +
    '        reverse_xy = true;\n' +
    '    }\n' +
    '    \n' +
    '    if (x2 < x1)\n' +
    '    {\n' +
    '        swap(&x1, &x2);\n' +
    '        swap(&y1, &y2);\n' +
    '    }\n' +
    '    \n' +
    '    int gradient = dy * 256 / dx;\n' +
    '    \n' +
    '    // handle first endpoint\n' +
    '    int xend = round(x1);\n' +
    '    int yend = y1 + gradient * (xend - x1) / 256;\n' +
    '    int xgap = rfpart(x1 + 128);\n' +
    '    int xpxl1 = xend;  // this will be used in the main loop\n' +
    '    int ypxl1 = ipart(yend);\n' +
    '    plot(xpxl1, ypxl1, rfpart(yend) * xgap / 256);\n' +
    '    plot(xpxl1, ypxl1 + 256, fpart(yend) * xgap / 256);\n' +
    '    int intery = yend + gradient; // first y-intersection for the main loop\n' +
    '    \n' +
    '    // handle second endpoint\n' +
    '    xend = round(x2);\n' +
    '    yend = y2 + gradient * (xend - x2) / 256;\n' +
    '    xgap = fpart(x2 + 128);\n' +
    '    int xpxl2 = xend;  // this will be used in the main loop\n' +
    '    int ypxl2 = ipart(yend);\n' +
    '    plot(xpxl2, ypxl2, rfpart(yend) * xgap / 256);\n' +
    '    plot(xpxl2, ypxl2 + 256, fpart(yend) * xgap / 256);\n' +
    '    \n' +
    '    // main loop\n' +
    '    for (int x = xpxl1 + 1; x <= xpxl2 - 1; x += 256)\n' +
    '    {\n' +
    '        plot(x, ipart(intery), rfpart(intery));\n' +
    '        plot(x, ipart(intery) + 256, fpart(intery));\n' +
    '        intery = intery + gradient;\n' +
    '    }\n' +
    '}\n' +
    '\n' +
    '/* Non-antialiased line drawing */\n' +
    '\n' +
    'void drawline(int x1, int y1, int x2, int y2, int color, int dots)\n' +
    '{\n' +
    '    // Algorithm from here: http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Simplification\n' +
    '    int dx = abs(x2 - x1);\n' +
    '    int dy = abs(y2 - y1);\n' +
    '    \n' +
    '    int sx = (x1 < x2) ? 1 : -1;\n' +
    '    int sy = (y1 < y2) ? 1 : -1;\n' +
    '    \n' +
    '    int err = dx - dy;\n' +
    '    int count = 0;\n' +
    '    for(;; count++)\n' +
    '    {\n' +
    '        if (!dots || (count >> (dots - 1)) & 1)\n' +
    '        {\n' +
    '            //__Point_SCR(x1, y1);\n' +
    '            //__LCD_SetPixl(color);\n' +
    '        }\n' +
    '        \n' +
    '        if (x1 == x2 && y1 == y2) break;\n' +
    '        \n' +
    '        int e2 = 2 * err;\n' +
    '        if (e2 > -dy)\n' +
    '        {\n' +
    '            err -= dy;\n' +
    '            x1 += sx;\n' +
    '        }\n' +
    '        else if (e2 < dx)\n' +
    '        {\n' +
    '            err += dx;\n' +
    '            y1 += sy;\n' +
    '        }\n' +
    '    }\n' +
    '}\n'
}
