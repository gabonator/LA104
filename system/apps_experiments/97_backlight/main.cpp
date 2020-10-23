#include <library.h>
#include "../../os_host/source/framework/Console.h"

#include "bios.h"

// 2954 - battery low
// 4100..4250..4360..4400..4450..4480..4510 charging
using namespace BIOS;

int __Bios(uint32_t a, uint32_t b)
{
  typedef uint32_t(*Tfunc)(uint32_t, uint32_t);
  static Tfunc ___Bios = (Tfunc)(0x08000100|1);

  return ___Bios(a, b);
}

int average[64] = {0};
int avi = -1;

int val = 50;
void loop(BIOS::KEY::EKey key)
{
  int d = 0;
  if (key == BIOS::KEY::EKey::Right)
    d += 1;
  if (key == BIOS::KEY::EKey::Left)
    d -= 1;
  if (key == BIOS::KEY::EKey::Up)
    d += 10;
  if (key == BIOS::KEY::EKey::Down)
    d -= 10;
  if (d!=0)
  {
    val += d;
    val = max(0, min(val, 100));
    CONSOLE::Print("Set backlight = %d%%\n", val);
    __Bios(DISPDEV, val);
  }

  int power = __Bios(PWRCTRL, VIN_ST);
  int battery = __Bios(PWRCTRL, VBTYmV);
  if (avi==-1)
  {
    for (int i=0; i<COUNT(average); i++) 
      average[i] = battery;
    avi = 0;
  }
  average[avi++] = battery;
  if (avi == COUNT(average)) 
    avi = 0;
  int sum = 0;
  for (int i=0; i<COUNT(average); i++) 
    sum += average[i];
  CONSOLE::Print("battery=%d, avg=%d, power=%d\n", battery, sum / COUNT(average), power);
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    CONSOLE::window.left = 8;
    CONSOLE::cursor = CONSOLE::window.TopLeft();

    CRect rcClient(0, 0, LCD::Width, LCD::Height);
    LCD::Bar(rcClient, RGB565(0000b0));
    
    CRect rc1(rcClient);
    rc1.bottom = 14;
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));    
    LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "Battery / display test");

    CRect rc2(rcClient);
    rc2.top = rc2.bottom-14;
    GUI::Background(rc2, RGB565(404040), RGB565(202020));
    LCD::Print(8, rc2.top, RGB565(808080), RGBTRANS, "...");

    KEY::EKey key;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
      loop(key);
      BIOS::SYS::DelayMs(100);
    }
    
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    CONSOLE::Color(RGB565(ffff00));
    CONSOLE::Print("Assertion failed in ");
    CONSOLE::Print(file);
    CONSOLE::Print(" [%d]: %s\n", line, cond);
#ifdef __APPLE__
    //kill(getpid(), SIGSTOP);
#endif
    while (1);
}

