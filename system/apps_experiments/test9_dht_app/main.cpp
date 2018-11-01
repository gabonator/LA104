#include <library.h>
#include "shapes.h"
#include "DHT.h"

class CVisualMeasure
{
  CDHT m_Sensor;
  int _x, _y;

public:
  void Create()
  {
    m_Sensor.Create( BIOS::GPIO::P1 );
  }

  void UpdateDht()
  {
    char buf[32];
    int x = _x, y = _y;
    uint16_t clr = RGB565(000000);
    bool init = m_Sensor.Init();
    bool read = false;
    if (init) 
      read = m_Sensor.Read(); // no pause between init & read!

    BIOS::LCD::Print(x, y, RGB565(ffffff), RGB565(b0b0b0), init ? "Init ok!      " : "Init failed");

    if (!init)
      return;

    y += 16;
    uint8_t* buffer = m_Sensor.GetBuffer();
    x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGB565(b0b0b0), "ROM: ");
    if (!read)
    {
      x += BIOS::LCD::Print(x, y, RGB565(ffffff), RGB565(b0b0b0), "read failed!     ");
      return;
    } else
    {
      for (int i=0; i<5; i++)
      {
        sprintf(buf, "%02x", buffer[i]);
        x += BIOS::LCD::Print(x, y, clr, RGB565(b0b0b0), buf) + 4;
      }

      if (m_Sensor.CheckCrc())
        x += BIOS::LCD::Print(x, y, clr, RGB565(b0b0b0), "crc ok  ");
      else
        x += BIOS::LCD::Print(x, y, clr, RGB565(b0b0b0), "crc err  ");
    }

    x = _x; y += 16;
    sprintf(buf, "Temperature=%f" "\xf8" "C   ", m_Sensor.GetTemperature());
    BIOS::LCD::Print(x, y, RGB565(ffffff), RGB565(b0b0b0), buf);

    x = _x; y += 16;
    sprintf(buf, "Humidity=%f%%   ", m_Sensor.GetHumidity());
    BIOS::LCD::Print(x, y, RGB565(ffffff), RGB565(b0b0b0), buf);

  }

  void OnPaint()
  {
    CRect rc(0, 0, 320, 240);
    GUI::Background(rc, RGB565(404040), RGB565(101010));

    CRect rc1(0, 0, 320, 14);
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
    
    CRect rc2(20, 16+50, 320-20, 240-50);
    GUI::Window(rc2, RGB565(ffffff));
    
    int x = 0;
    x += BIOS::LCD::Print(x, 0, RGB565(000000), RGB565(b0b0b0), " DHT test");
    x += BIOS::LCD::Draw( x, 0, RGB565(b0b0b0), RGBTRANS, CShapes_sel_right);
    x += BIOS::LCD::Print (rc2.right/2 + rc2.left/2- 21*4, rc2.top + 2, RGB565(000000), RGBTRANS, "DHT measure report");

    _x = rc2.left + 8;
    _y = rc2.top + 2 + 16 + 8;
    UpdateDht();
  }

  void OnKey(BIOS::KEY::EKey key)
  {
    UpdateDht();
    CDelay::DelayMs(500);
  }

};

__attribute__((__section__(".entry"))) int main(void)
{ 
  CVisualMeasure app;
  app.Create();
  app.OnPaint();

  BIOS::KEY::EKey key;
  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
    app.OnKey(key);

  return 0;
}
