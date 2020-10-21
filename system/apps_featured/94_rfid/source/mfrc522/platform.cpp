#include "platform.h"

SPISettings::SPISettings(int a, int b, int c) 
{
}

void Serial_print(char *s) { BIOS::DBG::Print(s); }
void Serial_println(char *s) { BIOS::DBG::Print("%s\n", s); }
void Serial_print(const char *s) { BIOS::DBG::Print(s); }
void Serial_println(const char *s) { BIOS::DBG::Print("%s\n", s); }
void Serial_print(int n) { BIOS::DBG::Print("%d", n); }
void Serial_print(int n, int f) { BIOS::DBG::Print("%02x", n); }
void Serial_println(int n, int f) { BIOS::DBG::Print("%02x\n", n); }
void Serial_println() { BIOS::DBG::Print("\n"); }
long millis() 
{
  return BIOS::SYS::GetTick(); 
}
