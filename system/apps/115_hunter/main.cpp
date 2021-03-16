#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"
#include "hunter.h"

using namespace BIOS;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    APP::Init("Hunter test");
    APP::Status("P1: Data");
    HunterInit();
    BIOS::KEY::EKey key;
    while ((key = KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
      CONSOLE::Print("Start zone 2 for 10 min\n"); 
      HunterStart(2,10); 
      SYS::DelayMs(10000); 
      CONSOLE::Print("Stop zone 2\n"); 
      HunterStop(2); // Start zone 2 for 10 minutes, wait 1 min and stop
      SYS::DelayMs(10000);
      CONSOLE::Print("Start program C\n"); 
      HunterProgram(3);
      SYS::DelayMs(10000);
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

