examples = 
{
  '00_bars.cpp': '#include <library.h>\n' +
    '\n' +
    'using namespace BIOS;\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::KEY::EKey key;\n' +
    '    while ((key = KEY::GetKey()) != BIOS::KEY::EKey::Escape)\n' +
    '    {\n' +
    '        int x1 = rand() % BIOS::LCD::Width;\n' +
    '        int y1 = (rand() % (BIOS::LCD::Height-28)) + 14;\n' +
    '        int x2 = rand() % BIOS::LCD::Width;\n' +
    '        int y2 = (rand() % (BIOS::LCD::Height-28)) + 14;\n' +
    '        int c = rand() & 0xffff;\n' +
    '        if (x2 > x1 && y2 > y1)\n' +
    '            BIOS::LCD::Bar(x1, y1, x2, y2, c);\n' +
    '    }\n' +
    '    \n' +
    '    return 0;\n' +
    '}\n',
  '01_hello.cpp': '#include <library.h>\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::DBG::Print("Hello world!");\n' +
    '    return 724;\n' +
    '}\n',
  '02_counter.cpp': '#include <library.h>\n' +
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
  '03_chart.cpp': '#include <library.h>\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::DBG::Print(R"(<script src="https://code.highcharts.com/highcharts.js"></script>\n' +
    '    <div id="container" style="width:500px; height:300px; background:#808080;"></div>\n' +
    '    <script>\n' +
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
    '\n' +
    '    while (BIOS::KEY::GetKey() == BIOS::KEY::EKey::None)\n' +
    '    {\n' +
    '        BIOS::SYS::DelayMs(3000);\n' +
    '        BIOS::DBG::Print(R"(<script>addPoint(%d)</script>)", rand() % 100);\n' +
    '    }\n' +
    '    return 0;\n' +
    '}\n',
  '04_colorpicker.cpp': '#include <library.h>\n' +
    '\n' +
    'uint32_t color = RGB32(255, 0, 0);\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::DBG::Print(R"(\n' +
    '    <input type="color" value="#ff0000">\n' +
    '    <script>\n' +
    '      document\n' +
    `        .querySelector("input[type='color']")\n` +
    '        .addEventListener("input", updateColor, false);\n' +
    '        \n' +
    '      function updateColor()\n' +
    '      {\n' +
    '        var html = event.target.value;\n' +
    '        var color = parseInt("0x"+html.substr(1));\n' +
    '        BIOS.memWrite(colorPtr, [color >> 16, color >> 8, color]);\n' +
    '      }\n' +
    '    </script>)");\n' +
    '    BIOS::DBG::Print(R"(<script>colorPtr = 0x%08x;</script>)", &color);\n' +
    '\n' +
    '    int padding = 8;\n' +
    '    CRect rect(padding, 14 + padding, \n' +
    '        BIOS::LCD::Width - padding, BIOS::LCD::Height - 14 - padding);\n' +
    '        \n' +
    '    while (BIOS::KEY::GetKey() == BIOS::KEY::EKey::None)\n' +
    '    {\n' +
    '        BIOS::LCD::Bar(rect, RGB32TO565(color));\n' +
    '        BIOS::SYS::DelayMs(20);\n' +
    '    }\n' +
    '    return 0;\n' +
    '}\n' +
    '\n',
  '05_flappybird.cpp': '// Play flappy bird in browser, press F1 on LA104 to jump\n' +
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
    '    game = setInterval(() => {\n' +
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
    '    BIOS::DBG::Print(R"(<script>clearInterval(game)</script>)");\n' +
    '    return 0;\n' +
    '}\n',
  '06_drawing.cpp': '#include <library.h>\n' +
    '\n' +
    'void drawline(int x1, int y1, int x2, int y2, int color);\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::DBG::Print(R"(\n' +
    '<canvas id="my_canvas_id" width=320 height=240></script>\n' +
    '<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>\n' +
    '<script>\n' +
    "var can = document.getElementById('my_canvas_id');\n" +
    "var ctx = can.getContext('2d');\n" +
    'imgData = ctx.getImageData(0, 0, can.width, can.height);\n' +
    '\n' +
    'for (var y=0; y<240; y++)\n' +
    '  for (var x=0; x<320; x++)\n' +
    '  {\n' +
    '    var i = ((Math.floor(x/16)+Math.floor(y/16))&1) ? 10 : 20; \n' +
    '    imgData.data[(y*320+x)*4] = 0; \n' +
    '    imgData.data[(y*320+x)*4+1] = 0;\n' +
    '    imgData.data[(y*320+x)*4+2] = 0; \n' +
    '    imgData.data[(y*320+x)*4+3] = i; \n' +
    '  }\n' +
    '\n' +
    'ctx.putImageData(imgData, 0, 0);\n' +
    'mousePressed = false;\n' +
    '\n' +
    "$('#my_canvas_id').mousedown(function (e) {\n" +
    '  mousePressed = true;\n' +
    '  Draw(e.pageX - $(this).offset().left, e.pageY - $(this).offset().top, false);\n' +
    '})\n' +
    '.mousemove(function (e) {\n' +
    '  if (mousePressed) {\n' +
    '      Draw(e.pageX - $(this).offset().left, e.pageY - $(this).offset().top, true);\n' +
    '  }\n' +
    '})\n' +
    '.mouseup(function (e) {\n' +
    '  mousePressed = false;\n' +
    '  promise = promise.then( () => BIOS.memWrite(ptrTo, [0xff, 0xff, 0xff, 0xff]) );\n' +
    '})\n' +
    '.mouseleave(function (e) {\n' +
    '  mousePressed = false;\n' +
    '  promise = promise.then( () => BIOS.memWrite(ptrTo, [0xff, 0xff, 0xff, 0xff]) );\n' +
    '});\n' +
    '\n' +
    'promise = Promise.resolve();\n' +
    'function Draw(x, y, isDown) \n' +
    '{\n' +
    '  if (isDown) \n' +
    '  {\n' +
    '    ctx.beginPath();\n' +
    '    ctx.strokeStyle = "#000000";\n' +
    '    ctx.lineWidth = 2;\n' +
    '    ctx.lineJoin = "round";\n' +
    '    ctx.moveTo(window.lastX, window.lastY);\n' +
    '    ctx.lineTo(x, y);\n' +
    '    ctx.closePath();\n' +
    '    ctx.stroke();\n' +
    '    promise = promise.then( () => BIOS.memWrite(ptrTo, [x&255, x>>8, 0, 0, y&255, y>>8, 0, 0]) );\n' +
    '  }\n' +
    '  lastX = x; \n' +
    '  lastY = y;\n' +
    '}\n' +
    '</script>\n' +
    ')");\n' +
    '\n' +
    '    CPoint from(-1, -1), to(-1, -1);\n' +
    '    BIOS::DBG::Print(R"(<script>ptrTo = 0x%08x;</script>)", &to);\n' +
    '\n' +
    '    while (BIOS::KEY::GetKey() == BIOS::KEY::EKey::None)\n' +
    '    {\n' +
    '        if (from != to)\n' +
    '        {\n' +
    '            if (from.x != -1 && to.x != -1)\n' +
    '                drawline(from.x, from.y, to.x, to.y, RGB565(ffffff));\n' +
    '\n' +
    '            from = to;\n' +
    '        }\n' +
    '    }\n' +
    '    return 0;\n' +
    '}\n' +
    '\n' +
    'void drawline(int x1, int y1, int x2, int y2, int color)\n' +
    '{\n' +
    '    // http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Simplification\n' +
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
    '        BIOS::LCD::PutPixel(x1, y1, color);\n' +
    '\n' +
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
    '\n',
  '07_3d.cpp': '#include <library.h>\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::DBG::Print(R"(<canvas id="c" style="width:400px; height:400px;"></canvas>\n' +
    '\n' +
    '<script type="module">\n' +
    "import * as THREE from 'https://threejsfundamentals.org/threejs/resources/threejs/r132/build/three.module.js';\n" +
    'rot = {x:0, y:0, z:0};\n' +
    '\n' +
    'function main() {\n' +
    '  const k = 0.05;\n' +
    '\n' +
    "  const canvas = document.querySelector('#c');\n" +
    '  const renderer = new THREE.WebGLRenderer({canvas});\n' +
    '  const camera = new THREE.PerspectiveCamera(75, 3, 0.1, 5);\n' +
    '  camera.position.z = 1.5;\n' +
    '  const scene = new THREE.Scene();\n' +
    '  const geometry = new THREE.BoxGeometry(1, 0.3, 1);\n' +
    '\n' +
    '  const cubes = [];\n' +
    '  const loader = new THREE.TextureLoader();\n' +
    '  const materials = [\n' +
    "    new THREE.MeshBasicMaterial({map: loader.load('https://threejsfundamentals.org/threejs/resources/images/flower-1.jpg')}),\n" +
    "    new THREE.MeshBasicMaterial({map: loader.load('https://threejsfundamentals.org/threejs/resources/images/flower-2.jpg')}),\n" +
    "    new THREE.MeshBasicMaterial({map: loader.load('https://threejsfundamentals.org/threejs/resources/images/flower-3.jpg')}),\n" +
    "    new THREE.MeshBasicMaterial({map: loader.load('https://threejsfundamentals.org/threejs/resources/images/flower-4.jpg')}),\n" +
    "    new THREE.MeshBasicMaterial({map: loader.load('https://threejsfundamentals.org/threejs/resources/images/flower-5.jpg')}),\n" +
    "    new THREE.MeshBasicMaterial({map: loader.load('https://threejsfundamentals.org/threejs/resources/images/flower-6.jpg')}),\n" +
    '  ];\n' +
    '  \n' +
    '  const cube = new THREE.Mesh(geometry, materials);\n' +
    '  scene.add(cube);\n' +
    '  cubes.push(cube);\n' +
    '\n' +
    '  function resizeRendererToDisplaySize(renderer) {\n' +
    '    const canvas = renderer.domElement;\n' +
    '    const width = canvas.clientWidth;\n' +
    '    const height = canvas.clientHeight;\n' +
    '    const needResize = canvas.width !== width || canvas.height !== height;\n' +
    '    if (needResize) {\n' +
    '      renderer.setSize(width, height, false);\n' +
    '    }\n' +
    '    return needResize;\n' +
    '  }\n' +
    '\n' +
    '  function render(time) {\n' +
    '    if (resizeRendererToDisplaySize(renderer)) {\n' +
    '      const canvas = renderer.domElement;\n' +
    '      camera.aspect = canvas.clientWidth / canvas.clientHeight;\n' +
    '      camera.updateProjectionMatrix();\n' +
    '    }\n' +
    '\n' +
    '    cubes.forEach((cube, ndx) => {\n' +
    '      cube.rotation.x = cube.rotation.x * (1-k) + rot.x * k;\n' +
    '      cube.rotation.y = cube.rotation.y * (1-k) + rot.y * k;\n' +
    '      cube.rotation.z = cube.rotation.z * (1-k) + rot.z * k;\n' +
    '    });\n' +
    '\n' +
    '    renderer.render(scene, camera);\n' +
    '    requestAnimationFrame(render);\n' +
    '  }\n' +
    '\n' +
    '  requestAnimationFrame(render);\n' +
    '}\n' +
    '\n' +
    'main();\n' +
    '</script>\n' +
    ')");\n' +
    '\n' +
    '    int x = 1, z = 0;\n' +
    '    BIOS::KEY::EKey key;\n' +
    '    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)\n' +
    '    {\n' +
    '        switch (key)\n' +
    '        {\n' +
    '            case BIOS::KEY::F1: x=0; z=0; break;\n' +
    '            case BIOS::KEY::Left: z--; break;\n' +
    '            case BIOS::KEY::Right: z++; break;\n' +
    '            case BIOS::KEY::Up: x--; break;\n' +
    '            case BIOS::KEY::Down: x++; break;\n' +
    '        }\n' +
    '        BIOS::SYS::DelayMs(100);\n' +
    '        BIOS::DBG::Print(R"(<script>rot = {y:Math.sin(%d/1000), x:%d/2, z:%d/2}</script>)", \n' +
    '            BIOS::SYS::GetTick(), x, z);\n' +
    '    }\n' +
    '    return 0;\n' +
    '}\n',
  '08_cat.cpp': '#include <library.h>\n' +
    '\n' +
    'uint16_t buffer[BIOS::LCD::Width];\n' +
    'uint8_t check = 0;\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::DBG::Print(R"(<script>ptrCheck=0x%08x;</script>)", &check);\n' +
    '    BIOS::DBG::Print(R"(<canvas id="canvas" width=%d height=%d>)", \n' +
    '        BIOS::LCD::Width, BIOS::LCD::Height);\n' +
    '        \n' +
    '    BIOS::DBG::Print(R"(\n' +
    '<script>\n' +
    "var can = document.getElementById('canvas');\n" +
    "var ctx = can.getContext('2d');\n" +
    'var img = new Image;\n' +
    'var imgData;\n' +
    'var promise = Promise.resolve();\n' +
    'img.crossOrigin = "Anonymous";\n' +
    'img.onload = function() {  \n' +
    '  var s = Math.max(can.width/img.width, can.height/img.height);\n' +
    '  var w = img.width*s;\n' +
    '  var h = img.height*s;\n' +
    '  ctx.drawImage(img, (can.width-w)/2, (can.height-h)/2, w, h);\n' +
    '  imgData = ctx.getImageData(0, 0, can.width, can.height);\n' +
    '  promise = promise.then(()=>BIOS.memWrite(ptrCheck, [0])); // ready\n' +
    '};\n' +
    'img.src = "https://cataas.com/cat?"+Math.random();\n' +
    '\n' +
    'window.transfer = function(left, top, right, bottom, ptr, check) \n' +
    '{\n' +
    '    var buf = [];\n' +
    '    for (var y=top; y<bottom; y++)\n' +
    '        for (var x=left; x<right; x++)\n' +
    '        {\n' +
    '            if (x > can.width)\n' +
    '            {\n' +
    '                buf.push(0);\n' +
    '                buf.push(0);\n' +
    '            } else\n' +
    '            {\n' +
    '                var r = imgData.data[(y*can.width+x)*4]; \n' +
    '                var g = imgData.data[(y*can.width+x)*4+1]; \n' +
    '                var b = imgData.data[(y*can.width+x)*4+2]; \n' +
    '                var rgb565 = (r>>3) | ((g>>2)<<5) | ((b>>3)<<11);\n' +
    '                buf.push(rgb565 & 255);\n' +
    '                buf.push(rgb565 >> 8);\n' +
    '            }\n' +
    '        }\n' +
    '\n' +
    '    promise = promise.then(() => BIOS.memWrite(ptr, buf))\n' +
    '        .then(() => BIOS.memWrite(check, [1]));\n' +
    '}\n' +
    '</script>)");\n' +
    '    \n' +
    '    check = 4;\n' +
    '    int line = 14;    \n' +
    '    CRect rect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height);\n' +
    '    while (BIOS::KEY::GetKey() == BIOS::KEY::EKey::None)\n' +
    '    {\n' +
    '        if (check == 0)\n' +
    '        {\n' +
    '            check = 3;\n' +
    '            rect.top = line;\n' +
    '            rect.bottom = line+1;\n' +
    '            BIOS::DBG::Print(R"(<script>transfer(%d, %d, %d, %d, 0x%08x, 0x%08x)</script>)", \n' +
    '                rect.left, rect.top, rect.right, rect.bottom, \n' +
    '                buffer, &check);\n' +
    '                \n' +
    '            line++;\n' +
    '        }\n' +
    '        if (check == 1)\n' +
    '        {\n' +
    '            BIOS::LCD::BufferBegin(rect);\n' +
    '            BIOS::LCD::BufferWrite(buffer, rect.Width()*rect.Height());\n' +
    '            BIOS::LCD::BufferEnd();\n' +
    '\n' +
    '            if (line >= BIOS::LCD::Height-14)\n' +
    '                break;\n' +
    '\n' +
    '            check = 0;\n' +
    '        }\n' +
    '    }\n' +
    '\n' +
    '    return 0;\n' +
    '}\n' +
    '\n',
  '09_bouncetext.cpp': '#include <library.h>\n' +
    '\n' +
    'int x = BIOS::LCD::Width/2, y = BIOS::LCD::Height/2;\n' +
    'int dx = 1, dy = 1;\n' +
    'char text[20] = "Hello";\n' +
    'volatile struct {uint8_t r, g, b; } color = {255, 0, 0};\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    CRect inner(8, 14+8, BIOS::LCD::Width-8, BIOS::LCD::Height-14-8);\n' +
    '    BIOS::LCD::Bar(inner, RGB565(404040));\n' +
    '    BIOS::LCD::Rectangle(inner, RGB565(b0b0b0));\n' +
    '    while (!BIOS::KEY::GetKey())\n' +
    '    {\n' +
    '        x += dx;\n' +
    '        y += dy;\n' +
    '        \n' +
    '        CRect draw(x, y, x+strlen(text)*8, y+14);\n' +
    '        BIOS::LCD::Print(x, y, RGB565RGB(color.r, color.g, color.b), \n' +
    '            RGB565(404040), text);\n' +
    '\n' +
    '        if (draw.right+dx >= inner.right)       dx = -abs(dx);\n' +
    '        if (draw.left+dx <= inner.left)         dx = +abs(dx);\n' +
    '        if (draw.bottom+dy >= inner.bottom)     dy = -abs(dy);\n' +
    '        if (draw.top+dy <= inner.top)           dy = +abs(dy);\n' +
    '            \n' +
    '        BIOS::SYS::DelayMs(5);\n' +
    '        BIOS::LCD::Bar(draw, RGB565(404040));\n' +
    '    }\n' +
    '    return 0;\n' +
    '}',
  '10_breakpoint.cpp': '#include <library.h>\n' +
    '\n' +
    'int x = BIOS::LCD::Width/2, y = BIOS::LCD::Height/2;\n' +
    'int dx = 1, dy = 1;\n' +
    'char text[20] = "Hello";\n' +
    'volatile struct {uint8_t r, g, b; } color = {255, 0, 0};\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    CRect inner(8, 14+8, BIOS::LCD::Width-8, BIOS::LCD::Height-14-8);\n' +
    '    BIOS::LCD::Bar(inner, RGB565(404040));\n' +
    '    BIOS::LCD::Rectangle(inner, RGB565(b0b0b0));\n' +
    '    while (!BIOS::KEY::GetKey())\n' +
    '    {\n' +
    '        x += dx;\n' +
    '        y += dy;\n' +
    '        \n' +
    '        CRect draw(x, y, x+strlen(text)*8, y+14);\n' +
    '        BIOS::LCD::Print(x, y, RGB565RGB(color.r, color.g, color.b), \n' +
    '            RGB565(404040), text);\n' +
    '\n' +
    '        if (draw.right+dx >= inner.right)       \n' +
    '        {\n' +
    '            BIOS::SYS::Beep(10);\n' +
    '            dx = -abs(dx);\n' +
    '        }\n' +
    '        if (draw.left+dx <= inner.left)\n' +
    '        {\n' +
    '            BIOS::SYS::Beep(10);\n' +
    '            dx = +abs(dx);\n' +
    '        }\n' +
    '        if (draw.bottom+dy >= inner.bottom)\n' +
    '        {\n' +
    '            BIOS::SYS::Beep(10);\n' +
    '            dy = -abs(dy);\n' +
    '        }\n' +
    '        if (draw.top+dy <= inner.top)  \n' +
    '        {\n' +
    '            BIOS::SYS::Beep(10);\n' +
    '            dy = +abs(dy);\n' +
    '        }\n' +
    '            \n' +
    '        BIOS::SYS::DelayMs(5);\n' +
    '        BIOS::LCD::Bar(draw, RGB565(404040));\n' +
    '    }\n' +
    '    return 0;\n' +
    '}',
  'bounceballs.cpp': '#include <library.h>\n' +
    '\n' +
    'class Ball \n' +
    '{\n' +
    '    int x;\n' +
    '    int y;\n' +
    '    int vx;\n' +
    '    int vy;\n' +
    '    uint16_t gradient[25];\n' +
    '    CRect backRect;\n' +
    '\n' +
    'public:\n' +
    '    Ball()\n' +
    '    {\n' +
    '        x = (random()%(BIOS::LCD::Width-40))+20;\n' +
    '        y = (random()%(BIOS::LCD::Height-40))+20;\n' +
    '        vx = (random() & 3)+1;\n' +
    '        vy = (random() & 3)+1;\n' +
    '        \n' +
    '        for (int i=0; i<COUNT(gradient); i++)\n' +
    '        {\n' +
    '            switch (x%3)\n' +
    '            {\n' +
    '                case 0: \n' +
    '                    gradient[i] = RGB565RGB(50-i*2, 120-i*5, 255);\n' +
    '                    break;\n' +
    '                case 1: \n' +
    '                    gradient[i] = RGB565RGB(50-i*2, 255, 120-i*5);\n' +
    '                    break;\n' +
    '                case 2: \n' +
    '                    gradient[i] = RGB565RGB(255, 50-i*2, 120-i*5);\n' +
    '                    break;\n' +
    '            }\n' +
    '        }\n' +
    '    }\n' +
    '    \n' +
    '    void Draw()\n' +
    '    {\n' +
    '        static constexpr int curve[11] = {10, 10, 10, 9, 9, 8, 8, 7, 6, 4, 0};\n' +
    '        \n' +
    '        backRect = CRect(x-10, y-10, x+11, y+11);\n' +
    '        const int r = 10;\n' +
    '        for (int _y=-10; _y<=10; _y++)\n' +
    '        {\n' +
    '            int px = curve[abs(_y)];\n' +
    '            int ofsx = 10-px;\n' +
    '            BIOS::LCD::BufferBegin(CRect(x-10+ofsx, y+_y, x+21, y+_y+1));\n' +
    '            BIOS::LCD::BufferWrite(gradient+(_y+10)/2, px*2);\n' +
    '            BIOS::LCD::BufferEnd();\n' +
    '        }\n' +
    '    }\n' +
    '    \n' +
    '    void Hide()\n' +
    '    {\n' +
    '        BIOS::LCD::Bar(backRect, RGB565(404040));\n' +
    '    }\n' +
    '    \n' +
    '    void Move()\n' +
    '    {\n' +
    '        x += vx;\n' +
    '        y += vy;\n' +
    '        if (x + vx + 10 >= BIOS::LCD::Width-1 || x + vx <= 10)\n' +
    '        {\n' +
    '            vx = -vx;\n' +
    '            BIOS::SYS::Beep(5);\n' +
    '        }\n' +
    '        if (y + vy + 10 > BIOS::LCD::Height-16 || y + vy <= 10+16)\n' +
    '        {\n' +
    '            vy = -vy;\n' +
    '            BIOS::SYS::Beep(5);\n' +
    '        }\n' +
    '    }\n' +
    '};\n' +
    '\n' +
    'Ball ballsData[16];\n' +
    'CArray<Ball> balls(ballsData, COUNT(ballsData));\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    balls.SetSize(balls.GetMaxSize());\n' +
    '    while (!BIOS::KEY::GetKey())\n' +
    '    {\n' +
    '        for (int i=0; i<balls.GetSize(); i++)\n' +
    '            balls[i].Draw();\n' +
    '            \n' +
    '        BIOS::SYS::DelayMs(10);\n' +
    '        \n' +
    '        for (int i=0; i<balls.GetSize(); i++)\n' +
    '            balls[i].Move();\n' +
    '            \n' +
    '        for (int i=0; i<balls.GetSize(); i++)\n' +
    '            balls[i].Hide();\n' +
    '    }\n' +
    '    return 72;\n' +
    '}\n' +
    '\n' +
    'void _HandleAssertion(const char* file, int line, const char* cond)\n' +
    '{\n' +
    '    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\\n", file, line, cond);\n' +
    '}\n',
  'caliperfun.cpp': '#include <library.h>\n' +
    '\n' +
    'using namespace BIOS;\n' +
    'using namespace BIOS::GPIO;\n' +
    '\n' +
    'int ReceiveWord(uint32_t& data)\n' +
    '{\n' +
    '    int counter;\n' +
    '    data = 0;\n' +
    '    for (int i=0; i<23; i++)\n' +
    '    {\n' +
    '        // low: 6us\n' +
    '        counter = 0;\n' +
    '        while (!GPIO::DigitalRead(P2) && counter < 10)\n' +
    '           counter ++; \n' +
    '        if (2 > counter || counter > 5)\n' +
    '            return i*1000 + 100 + counter;\n' +
    '    \n' +
    '        // high: 6us (sample in middle)\n' +
    '        counter = 2;\n' +
    '        GPIO::DigitalRead(P2);\n' +
    '        GPIO::DigitalRead(P2);\n' +
    '        \n' +
    '        data >>= 1;\n' +
    '        data |= GPIO::DigitalRead(P1) ? 0 : 1 << 22;\n' +
    '    \n' +
    '        while (GPIO::DigitalRead(P2) && counter < 10)\n' +
    '            counter ++;\n' +
    '        if (2 > counter || counter > 5)\n' +
    '            return i*1000 + 500 + counter;\n' +
    '    }\n' +
    '    \n' +
    '    if (data & (1<<21))\n' +
    '        data |= ~((1<<22)-1);\n' +
    '\n' +
    '    counter = 0;\n' +
    '    while (!GPIO::DigitalRead(P2) && counter < 10)\n' +
    '       counter ++; \n' +
    '    if (2 > counter || counter > 5)\n' +
    '        return 40000 + counter;\n' +
    '    \n' +
    '    return 0;\n' +
    '}\n' +
    '\n' +
    'int ReceivePacket(uint32_t& data1, uint32_t& data2)\n' +
    '{\n' +
    '    // 50us\n' +
    '    int counter = 0;\n' +
    '    while (GPIO::DigitalRead(P2) && counter < 100)\n' +
    '       counter ++; \n' +
    '    if (34-10 > counter || counter > 34+10)\n' +
    '        return false;\n' +
    '        \n' +
    '    if (ReceiveWord(data1) != 0)\n' +
    '        return false;\n' +
    '        \n' +
    '    // 100us\n' +
    '    counter = 0;\n' +
    '    while (GPIO::DigitalRead(P2) && counter < 600)\n' +
    '        counter ++; \n' +
    '    if (76-10 > counter || counter > 76+10)\n' +
    '        return false;\n' +
    '        \n' +
    '    if (ReceiveWord(data2) != 0)\n' +
    '        return false;\n' +
    '\n' +
    '    // 60us\n' +
    '    counter = 0;\n' +
    '    while (GPIO::DigitalRead(P2) && counter < 600)\n' +
    '        counter ++; \n' +
    '    if (41-10 > counter || counter > 41+10)\n' +
    '        return false;\n' +
    '\n' +
    '    return true;\n' +
    '}\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::DBG::Print(R"(\n' +
    "<div id='measurement' style='font-size:20px;'></div>\n" +
    "<br><div id='bar' style='background:gray; width:200px; height:16px;'></div>\n" +
    '<script>\n' +
    'ticksGood = 0;\n' +
    'metric = [{t:"M4",d:6.9,c:0}, {t:"M5",d:7.8,c:0}, {t:"M6",d:9.9,c:0},\n' +
    '  {t:"M8",d:12.8,c:0}];\n' +
    '\n' +
    'window.onMeas = (len, beep) =>\n' +
    '{\n' +
    '    var mm = len/1024*2.54;\n' +
    '    var found = metric.find(m=>(mm >= m.d-0.2 && mm <= m.d+0.2));\n' +
    '    var table = "<table border=1 width=200>";\n' +
    '    table += "<thead><tr><th>Size</th><th>Count</th></tr></thead>"\n' +
    '    for (var i in metric)\n' +
    '        table += "<tr><td>"+metric[i].t+"</td><td>"+metric[i].c+"</td></tr>";\n' +
    '    table += "</table>";\n' +
    '\n' +
    `    $('#measurement').html(mm.toFixed(1) + "mm " +\n` +
    '        (found ? found.t : "") + "<br>" +\n' +
    '        table);\n' +
    '    \n' +
    "    $('#bar').width(len/100);\n" +
    `    $('#bar').css("background", found ? "green" : "gray");\n` +
    '    if (found)\n' +
    '    {\n' +
    '        if (ticksGood++ == 5)\n' +
    '        {\n' +
    '            BIOS.memWrite(beep, [1]);\n' +
    '            found.c++;\n' +
    '        }\n' +
    '    }\n' +
    '    else\n' +
    '        ticksGood = 0;\n' +
    '    \n' +
    '}\n' +
    '</script>)");\n' +
    '\n' +
    '    PinMode(EPin::P1, EMode::Input);\n' +
    '    PinMode(EPin::P2, EMode::Input);\n' +
    '    PinMode(EPin::P3, EMode::Output);\n' +
    '    DigitalWrite(EPin::P3, 1);\n' +
    '    bool beep = false;\n' +
    '    BIOS::KEY::EKey key;\n' +
    '    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)\n' +
    '    {\n' +
    '        if (beep)\n' +
    '        {\n' +
    '            BIOS::SYS::Beep(200);\n' +
    '            beep = false;\n' +
    '        }\n' +
    '        if (GPIO::DigitalRead(P2))\n' +
    '        {\n' +
    '            OS::DisableInterrupts();\n' +
    '            uint32_t data1, data2;\n' +
    '            int valid = ReceivePacket(data1, data2);\n' +
    '            OS::EnableInterrupts(0);\n' +
    '            \n' +
    '            if (valid)\n' +
    '                BIOS::DBG::Print("<script>window.onMeas(%d, 0x%08x);</script>", data2, &beep); \n' +
    '        }\n' +
    '    }\n' +
    '    return 0;\n' +
    '}\n',
  'calipershort.cpp': '#include <library.h>\n' +
    '\n' +
    'using namespace BIOS;\n' +
    'using namespace BIOS::GPIO;\n' +
    '\n' +
    'void ReceiveWord(uint32_t& data)\n' +
    '{\n' +
    '    data = 0;\n' +
    '    for (int i=0; i<23; i++)\n' +
    '    {\n' +
    '        while (!GPIO::DigitalRead(P2));\n' +
    '        GPIO::DigitalRead(P2);\n' +
    '        GPIO::DigitalRead(P2);\n' +
    '        data >>= 1;\n' +
    '        data |= GPIO::DigitalRead(P1) ? 0 : 1 << 22;\n' +
    '        while (GPIO::DigitalRead(P2));\n' +
    '    }\n' +
    '    \n' +
    '    if (data & (1<<21))\n' +
    '        data |= ~((1<<22)-1);\n' +
    '\n' +
    '    while (!GPIO::DigitalRead(P2));\n' +
    '}\n' +
    '\n' +
    'int ReceivePacket(uint32_t& data1, uint32_t& data2)\n' +
    '{\n' +
    '    int counter = 0;\n' +
    '    while (GPIO::DigitalRead(P2) && counter < 100)\n' +
    '       counter ++; \n' +
    '    if (34-10 > counter || counter > 34+10)\n' +
    '        return false;\n' +
    '\n' +
    '    ReceiveWord(data1);\n' +
    '    while (GPIO::DigitalRead(P2));\n' +
    '    ReceiveWord(data2);\n' +
    '    while (GPIO::DigitalRead(P2));\n' +
    '\n' +
    '    return true;\n' +
    '}\n' +
    '\n' +
    'int main(void)\n' +
    '{\n' +
    '    BIOS::DBG::Print(R"(\n' +
    "<div id='measurement' style='font-size:20px;'></div>\n" +
    "<br><div id='bar' style='background:gray; width:200px; height:16px;'></div>\n" +
    '<script>\n' +
    'ticksGood = 0;\n' +
    'metric = [{t:"M4",d:6.9,c:0}, {t:"M5",d:7.8,c:0}, {t:"M6",d:9.9,c:0},\n' +
    '  {t:"M8",d:12.8,c:0}];\n' +
    '\n' +
    'onMeas = (len, beep) =>\n' +
    '{\n' +
    '    var mm = len/1024*2.54;\n' +
    '    var found = metric.find(m=>(mm >= m.d-0.2 && mm <= m.d+0.2));\n' +
    '    var table = "<table border=1 width=200>";\n' +
    '    table += "<thead><tr><th>Size</th><th>Count</th></tr></thead>"\n' +
    '    for (var i in metric)\n' +
    '        table += "<tr><td>"+metric[i].t+"</td><td>"+metric[i].c+"</td></tr>";\n' +
    '    table += "</table>";\n' +
    '\n' +
    `    $('#measurement').html(mm.toFixed(1) + "mm " +\n` +
    '        (found ? found.t : "") + "<br>" +\n' +
    '        table);\n' +
    '    \n' +
    "    $('#bar').width(len/100);\n" +
    `    $('#bar').css("background", found ? "green" : "gray");\n` +
    '    if (found)\n' +
    '    {\n' +
    '        if (ticksGood++ == 5)\n' +
    '        {\n' +
    '            BIOS.memWrite(beep, [1]);\n' +
    '            found.c++;\n' +
    '        }\n' +
    '    }\n' +
    '    else\n' +
    '        ticksGood = 0;\n' +
    '    \n' +
    '}\n' +
    '</script>)");\n' +
    '\n' +
    '    PinMode(EPin::P1, EMode::Input);\n' +
    '    PinMode(EPin::P2, EMode::Input);\n' +
    '    PinMode(EPin::P3, EMode::Output);\n' +
    '    DigitalWrite(EPin::P3, 1);\n' +
    '    bool beep = false;\n' +
    '    BIOS::KEY::EKey key;\n' +
    '    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)\n' +
    '    {\n' +
    '        if (beep)\n' +
    '        {\n' +
    '            BIOS::SYS::Beep(200);\n' +
    '            beep = false;\n' +
    '        }\n' +
    '        if (GPIO::DigitalRead(P2))\n' +
    '        {\n' +
    '            OS::DisableInterrupts();\n' +
    '            uint32_t data1, data2;\n' +
    '            int valid = ReceivePacket(data1, data2);\n' +
    '            OS::EnableInterrupts(0);\n' +
    '            \n' +
    '            if (valid)\n' +
    '                BIOS::DBG::Print("<script>onMeas(%d, 0x%08x);</script>", data2, &beep); \n' +
    '        }\n' +
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
    '}\n'
}

document.getElementById('input-file')
      .addEventListener('change', getFile)

    function getFile(event) {
	    const input = event.target
      if ('files' in input && input.files.length > 0) {
	      placeFileContent(
          html_editor,
          input.files[0])

          <!-- dirty hack for now just to remove the drop down menu -->
          document.getElementById('_examples').setAttribute('class','dropdown-menu')
      }
    }

    function placeFileContent(target, file) {
	    readFileContent(file).then(content => {
     	target.setValue(content)
      }).catch(error => console.log(error))
    }

    function readFileContent(file) {
	    const reader = new FileReader()
      return new Promise((resolve, reject) => {
        reader.onload = event => resolve(event.target.result)
        reader.onerror = error => reject(error)
        reader.readAsText(file)
      })
  }
