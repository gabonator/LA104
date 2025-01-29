#define _HOST
#include "cicoctx.h"
#include <stdio.h>

void start();
void SetPixel(int x, int y, int c);
uint32_t GetTick();
void Load(uint8_t*, int, int);
bool Sync();
void PlayTone(int);

uint16_t stack[32];
int stackPos = 0;

enum {
    cacheLineRows = 64,
    cacheLineSize = 64,
    cacheLineMask = cacheLineSize-1
};

struct cacheLine_t {
    enum {empty, read, write} type;
    uint16_t base{3};
    uint8_t data[cacheLineSize];
    uint32_t access{0};
};

cacheLine_t cache[cacheLineRows];
cacheLine_t* cacheLast = &cache[0];
uint32_t cacheTime = 0;

uint8_t memoryRead(uint16_t ofs)
{
    if (cacheLast->base == (ofs & ~cacheLineMask))
    return cacheLast->data[ofs & cacheLineMask];
    cacheLast->access = cacheTime++;

    for (int i=0; i<cacheLineRows; i++)
        if (cache[i].base == (ofs & ~cacheLineMask))
        {
            cacheLast = &cache[i];
            return cacheLast->data[ofs & cacheLineMask];
        }
    for (int i=0; i<cacheLineRows; i++)
    {
        if (cache[i].type == cacheLine_t::empty)
        {
            cache[i].type = cacheLine_t::read;
            cache[i].base = ofs & ~cacheLineMask;
            Load(cache[i].data, cache[i].base, cacheLineSize);
            cacheLast = &cache[i];
            return cacheLast->data[ofs & cacheLineMask];
        }
    }
    // no empty
    int j = -1;
    int minUsed = cacheTime++;
    for (int i=0; i<cacheLineRows; i++)
    {
        if (&cache[i] != cacheLast && cache[i].type == cacheLine_t::read)
        {
            if (cache[i].access < minUsed)
            {
                j = i;
                minUsed = cache[j].access;
            }
        }
    }
    assert(j>=0);
    cache[j].base = ofs & ~cacheLineMask;
    Load(cache[j].data, cache[j].base, cacheLineSize);
    cacheLast = &cache[j];
    return cacheLast->data[ofs & cacheLineMask];
}

void memoryWrite(uint16_t ofs, uint8_t v)
{
    if (cacheLast->base == (ofs & ~cacheLineMask))
    {
        cacheLast->type = cacheLine_t::write;
        cacheLast->data[ofs & cacheLineMask] = v;
        return;
    }
    cacheLast->access = cacheTime++;

    for (int i=0; i<cacheLineRows; i++)
        if (cache[i].base == (ofs & ~cacheLineMask))
        {
            cacheLast = &cache[i];
            cacheLast->type = cacheLine_t::write;
            cacheLast->data[ofs & cacheLineMask] = v;
            return;
        }
    
    memoryRead(ofs);
    assert(cacheLast->base == (ofs & ~cacheLineMask));
    cacheLast->type = cacheLine_t::write;
    cacheLast->data[ofs & cacheLineMask] = v;
}

namespace CicoContext
{
    cicocontext_t ctx;
    bool cicocontext_t::stop(const char* msg)
    {
        assert(0);
        return false;
    }
    uint8_t cicocontext_t::memoryAGet8(int seg, uint16_t ofs)
    {
        if (seg == 0xb800)
            return memoryVideoGet8(seg, ofs);

        assert(seg == 0x1010);
        return memoryRead(ofs);
    }
    void cicocontext_t::memoryASet8(int seg, uint16_t ofs, uint8_t v)
    {
        if (seg >= 0xb800)
        {
            memoryVideoSet8(seg, ofs, v);
            return;
        }

        assert(seg == 0x1010);
        memoryWrite(ofs, v);
    }
    uint16_t cicocontext_t::memoryAGet16(int seg, uint16_t ofs)
    {
        if (seg == 0xb800)
                return memoryVideoGet16(seg, ofs);

        assert(seg == 0x1010);
        return memoryRead(ofs)+memoryRead(ofs+1)*256;
    }
    void cicocontext_t::memoryASet16(int seg, uint16_t ofs, uint16_t v)
    {
        if (seg == 0xb800)
        {
            memoryVideoGet16(seg, ofs);
            return;
        }

        assert(seg == 0x1010);
        memoryWrite(ofs, v);
        memoryWrite(ofs+1, v>>8);
    }
    void cicocontext_t::memoryVideoSet8(int seg, int ofs, uint8_t data)
    {
#ifdef DS213
        int page = ofs & 0x2000;
        ofs &= 0x1fff;
        int y = (ofs / 80)*2;
        int x = (ofs % 80)*5;
        if (page)
            y ++;
        assert(y < 200);
        SetPixel(x++, y, data >> 6);
        SetPixel(x++, y, (data >> 4) & 3);
        SetPixel(x++, y, (data >> 4) & 3);
        SetPixel(x++, y, (data >> 2) & 3);
        SetPixel(x++, y, (data >> 0) & 3);
#else
        int page = ofs & 0x2000;
        ofs &= 0x1fff;
        int y = (ofs / 80)*2;
        int x = (ofs % 80)*4;
        if (page)
            y ++;
        assert(y < 200);
        SetPixel(x++, y, data >> 6);
        SetPixel(x++, y, (data >> 4) & 3);
        SetPixel(x++, y, (data >> 2) & 3);
        SetPixel(x++, y, (data >> 0) & 3);
#endif
    }
    void cicocontext_t::memoryVideoSet16(int seg, int ofs, uint16_t data)
    {
        memoryVideoSet8(seg, ofs, data & 0xff);
        memoryVideoSet8(seg, ofs+1, data >> 8);
    }
    uint8_t cicocontext_t::memoryVideoGet8(int seg, int ofs)
    {
        return 0;
    }
    uint16_t cicocontext_t::memoryVideoGet16(int seg, int ofs)
    {
        return 0xaaaa;
    }
    void cicocontext_t::callInterrupt(int i)
    {
        if (i == 0x11)
        {
            ctx.a.r16 = 0;
            return;
        }
        if (i == 0x1A)
        {
            ctx.c.r16 = 0;
            ctx.d.r16 = GetTick() / 60;
            return;
        }
        if (i == 0x10)
            return;
        assert(0);
    }
    void cicocontext_t::load(const char* path, const char* file, int size)
    {
    }
    void cicocontext_t::out(int port, uint8_t val)
    {
        static uint16_t port42buf = 0;
        
        if (port == 0x61)
        {
            if ((val & 3) == 0)
                PlayTone(0);
            else
                PlayTone(1193180 / port42buf);
        }
        if (port == 0x42)
            port42buf = (port42buf>>8) | (val<<8);
    }
    void cicocontext_t::out(int port, uint16_t val)
    {
    }
    void cicocontext_t::in(uint8_t& val, int port)
    {
        if (port == 0x3da)
        {
            static int retrace = 0;
            retrace++;
            val = (retrace & 1) ? 9 : 0;
            return;
        }
    }
    bool cicocontext_t::sync()
    {
        return Sync();
    }
    void cicocontext_t::push(const uint16_t& r)
    {
        assert(stackPos < sizeof(stack)/sizeof(stack[0]));
        stack[stackPos++] = r;
    }
    uint16_t cicocontext_t::pop(void)
    {
        assert(stackPos > 0);
        return stack[--stackPos];
    }
};

