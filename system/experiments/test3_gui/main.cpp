#include <library.h>
#include "shapes.h"

class CVisualMeasure
{
public:
  void OnPaint()
  {
    CRect rc(0, 0, 320, 240);
    GUI::Background(rc, RGB565(404040), RGB565(101010));

    CRect rc1(0, 0, 320, 14);
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
    
    CRect rc2(20, 16+20, 300, 220);
    GUI::Window(rc2, RGB565(ffffff));

    // Config - farenheit celsius, graph resolution, collect on/off, load/save
    // Devices - bus scan
    // Graph - draw live graph
    // Stats - max/min/avg?
    
    int x = 0, y = 0;
    x += BIOS::LCD::Print(x, 0, RGB565(000000), RGB565(b0b0b0), " VisualMeasure");
    x += BIOS::LCD::Draw( x, 0, RGB565(b0b0b0), RGBTRANS, CShapes_sel_right);
    x += 8;
    x += BIOS::LCD::Draw( x, 0, RGB565(b0b0b0), RGBTRANS, CShapes_sel_left);
    x += BIOS::LCD::Print(x, 0, RGB565(000000), RGB565(b0b0b0), "Config");
    x += BIOS::LCD::Draw( x, 0, RGB565(b0b0b0), RGBTRANS, CShapes_sel_right);
    x += 8;
    x += BIOS::LCD::Print(x, 0, RGB565(b0b0b0), RGBTRANS, "Graph");
    x += 16;
    x += BIOS::LCD::Print(x, 0, RGB565(b0b0b0), RGBTRANS, "Stats");

    x += BIOS::LCD::Print (rc2.right/2 + rc2.left/2- 13*4, rc2.top + 2, RGB565(000000), RGBTRANS, "Configuration");

    int _x = rc2.left + 8;
    int _y = rc2.top + 2 + 16 + 8;
    x = _x; y = _y;
    BIOS::LCD::Draw( x, y, RGB565(000000), RGBTRANS, CShapes_check_box);
    x += BIOS::LCD::Draw( x, y, RGB565(00ff00), RGBTRANS, CShapes_check_on);
    x += 4;
    x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGBTRANS, "OneWire at P1");
    y = _y += 16;
    x = _x + 16;
    BIOS::LCD::Draw( x, y, RGB565(000000), RGBTRANS, CShapes_dotout);
    x += BIOS::LCD::Draw( x, y, RGB565(0000ff), RGBTRANS, CShapes_dot);
    x += 3;
    x += BIOS::LCD::Print(x, y, RGB565(000000), RGBTRANS, "#0000000000: DS18S20  25");
    x += BIOS::LCD::Draw( x, y, RGB565(00000), RGBTRANS, CShapes_degree);
    x += BIOS::LCD::Print(x, y, RGB565(000000), RGBTRANS, "C");
    y = _y += 16;
    x = _x + 16;
    BIOS::LCD::Draw( x, y, RGB565(000000), RGBTRANS, CShapes_dotout);
    x += BIOS::LCD::Draw( x, y, RGB565(00ff00), RGBTRANS, CShapes_dot);
    x += 3;
    x += BIOS::LCD::Print(x, y, RGB565(000000), RGBTRANS, "#0000000000: DS18S20  25");
    x += BIOS::LCD::Draw( x, y, RGB565(00000), RGBTRANS, CShapes_degree);
    x += BIOS::LCD::Print(x, y, RGB565(000000), RGBTRANS, "C");
    x = _x;
    y = _y += 16;
    BIOS::LCD::Draw( x, y, RGB565(000000), RGBTRANS, CShapes_check_box);
    x += BIOS::LCD::Draw( x, y, RGB565(00ff00), RGBTRANS, CShapes_check_on);
    x += 4;
    x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGBTRANS, "DHT at P2");

    y = _y += 16;
    x = _x + 16;
    BIOS::LCD::Draw( x, y, RGB565(000000), RGBTRANS, CShapes_dotout);
    x += BIOS::LCD::Draw( x, y, RGB565(ff0000), RGBTRANS, CShapes_dot);
    x += 3;
    x += BIOS::LCD::Print(x, y, RGB565(000000), RGBTRANS, "Temperature: 17");
    x += BIOS::LCD::Draw( x, y, RGB565(00000), RGBTRANS, CShapes_degree);
    x += BIOS::LCD::Print(x, y, RGB565(000000), RGBTRANS, "C");

    y = _y += 16;
    x = _x + 16;
    BIOS::LCD::Draw( x, y, RGB565(000000), RGBTRANS, CShapes_dotout);
    x += BIOS::LCD::Draw( x, y, RGB565(ffff00), RGBTRANS, CShapes_dot);
    x += 4;
    x += BIOS::LCD::Print(x, y, RGB565(000000), RGBTRANS, "Humidity: 65");
    x += BIOS::LCD::Print(x, y, RGB565(000000), RGBTRANS, "%");

    x = _x;
    y = _y += 16;
    BIOS::LCD::Draw( x, y, RGB565(000000), RGB565(ffffff), CShapes_check_box);
    x += BIOS::LCD::Draw( x, y, RGB565(00ff00), RGBTRANS, CShapes_check_on);
    x += 4;
    x += BIOS::LCD::Print(x, y, RGB565(000000), RGB565(ffffff), "BMP at P3 & P4");
    
    y = _y += 16;
    x = _x + 12;
    x += BIOS::LCD::Print(x, y, RGB565(000000), RGBTRANS, "Not found");
  }
};

__attribute__((__section__(".entry"))) int main(void)
{ 
  CVisualMeasure app;
  app.OnPaint();
  while (BIOS::KEY::GetKey() == BIOS::KEY::None);

  return 0;
}
