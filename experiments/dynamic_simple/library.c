#include "library.h"

#undef IMPORT
#define IMPORT(local, remote, ret, args) ret local args

IMPORT(Test1, , void, ()) {}
IMPORT(Test2, , void, ()) {}
IMPORT(Test3, , void, ()) {}
IMPORT(Test4, , void, ()) {}
IMPORT(PutPixel, BIOS::LCD::PutPixel, void, (int a, int b, ui16 c)) {}
IMPORT(Print, BIOS::LCD::Print, int, (int a, int b, ui16 c, ui16 d, const char* e)) {return 0;}
IMPORT(GetKeys, BIOS::KEY::GetKeys, ui16, ()) {return 0;}
IMPORT(Printf, BIOS::LCD::Printf, int, (int x, int y, unsigned short clrf, unsigned short clrb, const char * format, ...) ){return 0;}
