#include "../../../os_host/source/bios/Bios.h"

void _HandleAssertion(const char* file, int line, const char* cond)
{
    printf("Assertion failed in %s [%d]: %s\n", file, line, cond);
}

namespace BIOS
{
  namespace LCD
  {
    int Print (int x, int y, unsigned short clrf, unsigned short clrb, char* str) { printf("%s", str); return 0; }
    int Print (int x, int y, unsigned short clrf, unsigned short clrb, const char* str) { printf("%s", str); return 0; }
    int Printf (int x, int y, unsigned short clrf, unsigned short clrb, const char *format, ...) { printf("not implemented\n"); return 0; }
  }
}