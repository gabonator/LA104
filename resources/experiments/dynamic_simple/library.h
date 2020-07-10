typedef unsigned char u8;
typedef unsigned int ui32;
typedef unsigned int u32;
typedef unsigned short ui16;
typedef unsigned short uc16;
typedef unsigned short u16;

#undef IMPORT
//#define IMPORT(local, remote, ret, args) extern __attribute__((noinline, naked)) ret local args;
#define IMPORT(local, remote, ret, args) ret local args;

	IMPORT(Test1, , void, ());
	IMPORT(Test2, , void, ());
	IMPORT(Test3, , void, ());
	IMPORT(Test4, , void, ());
	IMPORT(PutPixel, BIOS::LCD::PutPixel, void, (int, int, ui16));
	IMPORT(Print, BIOS::LCD::Print, int, (int, int, ui16, ui16, const char*));
	IMPORT(GetKeys, BIOS::KEY::GetKeys, ui16, ());	
	IMPORT(Printf, BIOS::LCD::Printf, int, (int x, int y, unsigned short clrf, unsigned short clrb, const char * format, ...) );

