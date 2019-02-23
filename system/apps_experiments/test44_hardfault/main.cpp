#include <library.h>

void Function4()
{
  BIOS::DBG::Print("Illegal memory write\n");
  int* p = (int*)0xaaaaaaaa;
  *p = 4;
  BIOS::DBG::Print("read = %d\n", *p);
}

void Function3()
{
  BIOS::DBG::Print("Function3 (0x%08x){\n", Function3);
  Function4();
  __asm("nop");
  __asm("nop");
  BIOS::DBG::Print("}\n");
}

void Function2()
{
  BIOS::DBG::Print("Function2 (0x%08x){\n", Function2);
  Function3();
  __asm("nop");
  __asm("nop");
  BIOS::DBG::Print("}\n");
}

void Function1()
{
  BIOS::DBG::Print("Function1 (0x%08x){\n", Function1);
  Function2();
  __asm("nop");
  __asm("nop");
  BIOS::DBG::Print("}\n");
}

#ifndef __APPLE__
__attribute__((__section__(".entry")))
#endif
int _main(void)
{ 
  Function1();
  return 0;
}

