#pragma once
#include <stdint.h>
#include <stddef.h>
#include <Arduino.h>
#include <assert.h>
#include <library.h>

class Print
{
public:
//virtual size_t write(uint8_t) { assert(0); return 0; }
    size_t print(char c) { write(c); }
    size_t print(int i) { char buf[32]; sprintf(buf, "%d", i); print(buf); }
    size_t print(long i) { char buf[32]; sprintf(buf, "%d", (int)i); print(buf); }
    size_t print(int, int) { assert(0); return 0; }
    size_t print(char* msg){ while (*msg) write(*msg++); }
    size_t print(const char*) { assert(0); return 0; }
    size_t println() { write('\n'); return 0; }
    
    virtual size_t write(uint8_t c) { assert(0); }
};

