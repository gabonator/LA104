#include <library.h>
#include "shapes.h"

class CVisualMeasure
{
  int pwm[4] = {0};
  int focus = 0;
  int _x, _y;

public:
  CVisualMeasure()
  {
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Pwm);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Pwm);
    BIOS::GPIO::PinMode(BIOS::GPIO::P3, BIOS::GPIO::Pwm);
    BIOS::GPIO::PinMode(BIOS::GPIO::P4, BIOS::GPIO::Pwm);
  }

  void OnPaint()
  {
    CRect rc(0, 0, 320, 240);
    GUI::Background(rc, RGB565(404040), RGB565(101010));

    CRect rc1(0, 0, 320, 14);
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));
    
    CRect rc2(50, 16+50, 320-50, 240-50);
    GUI::Window(rc2, RGB565(ffffff));
    
    int x = 0;
    x += BIOS::LCD::Print(x, 0, RGB565(000000), RGB565(b0b0b0), " PWM control application");
    x += BIOS::LCD::Draw( x, 0, RGB565(b0b0b0), RGBTRANS, CShapes_sel_right);
    x += BIOS::LCD::Print (rc2.right/2 + rc2.left/2- 13*4, rc2.top + 2, RGB565(000000), RGBTRANS, "PWM outputs");

    _x = rc2.left + 8;
    _y = rc2.top + 2 + 16 + 8;
    Redraw();
  }

  void Redraw()
  {
    RedrawPwm(0);
    RedrawPwm(1);
    RedrawPwm(2);
    RedrawPwm(3);
  }

  void RedrawPwm(int i)
  {
    DrawPwm(i, _x, _y + 16*i, focus==i, pwm[i]);
  }

  void DrawPwm(int channel, int x, int y, bool focus, int value)
  {
    char test[32];
    sprintf(test, " P%d: %3d%% ", channel+1, value);
    BIOS::LCD::Print(x, y, RGB565(ffffff), focus ? RGB565(0000b0) : RGB565(b0b0b0), test);

    CRect rcBar(x + 110, y+1, x+110+80, y+8);
    BIOS::LCD::Bar(rcBar, RGB565(000000));
    rcBar.Deflate(2, 2, 2, 2); // Hard fault when 1,1,1,1 ????

    int p = value*(rcBar.right - rcBar.left)/100;
    if (p>0)
    {
      rcBar.right = rcBar.left + p;
      BIOS::LCD::Bar(rcBar, RGB565(ffffff));
    }
  }

  void OnKey(BIOS::KEY::EKey key)
  {
    switch (key)
    {
      case BIOS::KEY::Up: 
        if (focus > 0)
        {
          focus--;
          Redraw();
        } 
      break;
      case BIOS::KEY::Down: 
        if (focus < 3)
        {
          focus++;
          Redraw();
        } 
      break;
      case BIOS::KEY::Left: 
        if (pwm[focus] > 0)
        {
          pwm[focus]-=2;
          if (pwm[focus] < 0)
            pwm[focus] = 0;
          RedrawPwm(focus);
          UpdatePwm(focus, pwm[focus]);
        } 
      break;
      case BIOS::KEY::Right: 
        if (pwm[focus] < 100)
        {
          pwm[focus]+=2;
          if (pwm[focus] > 100)
            pwm[focus] = 100;
          RedrawPwm(focus);
          UpdatePwm(focus, pwm[focus]);
        } 
      break;
      default:      
      break;
    }
  }

  void UpdatePwm(int ch, int value)
  {
    float fPerc = value / 100.0f;
    // apply exponential curve
    float fExp = fPerc*fPerc;
    int nValue = (int)(fExp * BIOS::GPIO::AnalogRange);

    switch (ch)
    {
      case 0: BIOS::GPIO::AnalogWrite(BIOS::GPIO::P1, nValue); break;
      case 1: BIOS::GPIO::AnalogWrite(BIOS::GPIO::P2, nValue); break;
      case 2: BIOS::GPIO::AnalogWrite(BIOS::GPIO::P3, nValue); break;
      case 3: BIOS::GPIO::AnalogWrite(BIOS::GPIO::P4, nValue); break;
    }
  }
};

__attribute__((__section__(".entry"))) int main(void)
{ 
  CVisualMeasure app;
  app.OnPaint();
  BIOS::KEY::EKey key;
  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
    app.OnKey(key);

  return 0;
}
