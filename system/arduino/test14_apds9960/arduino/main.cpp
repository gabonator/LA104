#include <Arduino.h>
using namespace Arduino;

void *__dso_handle = (void *)nullptr;
void *_fini = (void *)nullptr;
extern "C" int __aeabi_atexit (void *arg, void (*func) (void *), void *d)
{
  // __cxa_atexit -- register a function to be called by exit or when a shared library is unloaded
  BIOS::DBG::Print("__atexit: %08x %08x %08x\n", arg, func, d);
  return 1;
}

void setup();
void loop();

__attribute__((__section__(".entry"))) 
int main(void)
{ 
  BIOS::LCD::Clear(RGB565(202020));
  BIOS::DBG::Print("[setup]\n");
  setup();
  BIOS::DBG::Print("[loop]\n");
  while (BIOS::KEY::GetKey() != BIOS::KEY::Escape)
  {
    loop();
  }

  return 0;
}

