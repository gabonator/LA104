#pragma once
#include "Printable.h"

class Stream
{
public:
	
    virtual int available() { BIOS::KEY::GetKey(); return BIOS::GPIO::UART::Available(); }
	
	virtual size_t write(uint8_t) { { assert(0); }return 0; }
	
	virtual void flush() { assert(0); }
	
	virtual int peek() { { assert(0); }return 0; }
	
	virtual int read() { { assert(0); }return 0; }


void println() { assert(0); }
void print() { assert(0); }
void print(int, int) { assert(0); }
void print(char*) { assert(0); }
void print(const char*) { assert(0); }
void print(const Printable&) { assert(0); }
void setTimeout(int) {  }
size_t readBytes(int n) { assert(0); return 0; }
    size_t readBytes(uint8_t* b, int n) { *b = BIOS::GPIO::UART::Read(); return 1; }
};
