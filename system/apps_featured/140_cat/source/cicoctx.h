#pragma once
#include <stdint.h>
#ifndef _ARM
#include <assert.h>
#else
void _HandleAssertion1(const char*, int, const char*);
#define _STR(x) #x
#define assert(e) {if(!(e)) _HandleAssertion1(__FILE__, __LINE__, _STR(e)); }
#endif

namespace CicoContext
{

class cicocontext_t {
public:
    struct register_t
    {
      union {
        uint16_t r16;
        struct {
          uint8_t l;
          uint8_t h;
        } r8;
      };
    } a, b, c, d, temp;

    uint16_t _si, _di, _bp;
    uint16_t _cs, _ds, _ss, _es, _sp;

    int _headerSize;
    int _loadAddress;
    bool interrupts, direction, carry, zero;

    void memoryASet8(int seg, uint16_t ofs, uint8_t v);
    void memoryASet16(int seg, uint16_t ofs, uint16_t v);
    uint8_t memoryAGet8(int seg, uint16_t ofs);
    uint16_t memoryAGet16(int seg, uint16_t ofs);

    void memoryBiosSet8(int seg, int ofs, uint8_t v);
    void memoryBiosSet16(int seg, int ofs, uint16_t v);
    uint8_t memoryBiosGet8(int seg, int ofs);
    uint16_t memoryBiosGet16(int seg, int ofs);

    uint8_t memoryVideoGet8(int seg, int ofs);
    void memoryVideoSet8(int seg, int ofs, uint8_t data);
    uint16_t memoryVideoGet16(int seg, int ofs);
    void memoryVideoSet16(int seg, int ofs, uint16_t data);
    void callInterrupt(int i);
    void out(int port, uint8_t val);
    void in(uint8_t& val, int port);
    void out(int port, uint16_t val);
    void push(const uint16_t& r);
    uint16_t pop();
    bool stop(const char* msg = nullptr);
    void callIndirect(int a);
    void cbw();
    void div(uint8_t r);
    void div(uint16_t r);
    void idiv(uint8_t r);
    void idiv(uint16_t r);
    void mul(uint8_t r);
    void mul(uint16_t r);
    void imul(uint8_t r);
    void imul(uint16_t r);
    uint8_t rcl(uint8_t r, int n);
    uint16_t rcl(uint16_t r, int n);
    uint8_t rcr(uint8_t r, int n);
    uint16_t rcr(uint16_t r, int n);
    uint8_t rol(uint8_t r, int n);
    uint16_t rol(uint16_t r, int n);
    uint8_t ror(uint8_t r, int n);
    uint16_t ror(uint16_t r, int n);

    uint16_t sar(uint16_t a, uint8_t b);
    uint8_t sar(uint8_t a, uint8_t b);
    bool sync();
    void syncKeyb();
    void cmc();
    void aaa();
    void load(const char* path, const char* file, int size);
};

#ifndef _HOST

#define ax ctx.a.r16
#define bx ctx.b.r16
#define cx ctx.c.r16
#define dx ctx.d.r16
#define ah ctx.a.r8.h
#define bh ctx.b.r8.h
#define ch ctx.c.r8.h
#define dh ctx.d.r8.h
#define al ctx.a.r8.l
#define bl ctx.b.r8.l
#define cl ctx.c.r8.l
#define dl ctx.d.r8.l
#define si ctx._si
#define di ctx._di
#define ds ctx._ds
#define cs ctx._cs
#define es ctx._es
#define ss ctx._ss
#define sp ctx._sp
#define bp ctx._bp
#define flags ctx
#define tl ctx.temp.r8.l
#define tx ctx.temp.r16
#define headerSize ctx._headerSize
#define loadAddress ctx._loadAddress

#define memoryASet ctx.memoryASet8
#define memoryASet16 ctx.memoryASet16
#define memoryAGet ctx.memoryAGet8
#define memoryAGet16 ctx.memoryAGet16

#define memory16(seg, ofs) *((uint16_t*)&ctx.memory[(seg)*16+ofs])
#define memorySet(seg, ofs, val) ctx.memory[(seg)*16+ofs] = val
#define memorySet16(seg, ofs, val) memory16(seg, ofs) = val
#define memoryGet(seg, ofs) ctx.memory[(seg)*16+ofs]
#define memoryGet16(seg, ofs) memory16(seg, ofs)
#define memory(seg, ofs) ctx.memory[(seg)*16+ofs]

#define memoryVideoGet ctx.memoryVideoGet8
#define memoryVideoSet ctx.memoryVideoSet8
#define memoryVideoGet16 ctx.memoryVideoGet16
#define memoryVideoSet16 ctx.memoryVideoSet16

#define memoryBiosSet ctx.memoryBiosSet8
#define memoryBiosSet16 ctx.memoryBiosSet16
#define memoryBiosGet ctx.memoryBiosGet8
#define memoryBiosGet16 ctx.memoryBiosGet16

#define out ctx.out
#define in ctx.in
#define push(x) ctx.push((uint16_t)x)
#define pop ctx.pop
#define interrupt ctx.callInterrupt
#define stop ctx.stop
#define callIndirect ctx.callIndirect
#define load ctx.load


#define cbw ctx.cbw
#define div ctx.div
#define idiv ctx.idiv
#define mul ctx.mul
#define imul ctx.imul
#define sar ctx.sar
#define rol ctx.rol
#define ror ctx.ror
#define rcl ctx.rcl
#define rcr ctx.rcr
#define sync ctx.sync
#define syncKeyb ctx.syncKeyb
#define cmc ctx.cmc
#define aaa ctx.aaa

extern cicocontext_t ctx;

struct MemAuto
{
    static uint8_t Get8(int seg, int nAddr) { return memoryAGet(seg, nAddr); }
    static void Set8(int seg, int nAddr, uint8_t nData) { memoryASet(seg, nAddr, nData); }
    static uint16_t Get16(int seg, int nAddr) { return memoryAGet16(seg, nAddr); }
    static void Set16(int seg, int nAddr, uint16_t nData) { memoryASet16(seg, nAddr, nData); }
};

//struct MemData
//{
//    static uint8_t Get8(int seg, int nAddr) { return memoryGet(seg, nAddr); }
//    static void Set8(int seg, int nAddr, uint8_t nData) { memorySet(seg, nAddr, nData); }
//    static uint16_t Get16(int seg, int nAddr) { return memoryGet16(seg, nAddr); }
//    static void Set16(int seg, int nAddr, uint16_t nData) { memorySet16(seg, nAddr, nData); }
//};
//
//struct MemVideo
//{
//    static uint8_t Get8(int seg, int nAddr) { return memoryVideoGet(seg, nAddr); }
//    static void Set8(int seg, int nAddr, uint8_t nData) { memoryVideoSet(seg, nAddr, nData); }
//    static uint16_t Get16(int seg, int nAddr) { return memoryVideoGet16(seg, nAddr); }
//    static void Set16(int seg, int nAddr, uint16_t nData) { memoryVideoSet16(seg, nAddr, nData); }
//};
//
//struct MemBios
//{
//    static uint8_t Get8(int seg, int nAddr) { return memoryBiosGet(seg, nAddr); }
//    static void Set8(int seg, int nAddr, uint8_t nData) { memoryBiosSet(seg, nAddr, nData); }
//    static uint16_t Get16(int seg, int nAddr) { return memoryBiosGet16(seg, nAddr); }
//    static void Set16(int seg, int nAddr, uint16_t nData) { memoryBiosSet16(seg, nAddr, nData); }
//};
//
struct DirAuto
{
    static void Assert()
    {
    }
    template<class T>
    static T Move(T& i)
    {
        if (flags.direction)
            return i--;
        else
            return i++ & 0xffff;
    }
};

struct DirForward
{
    static void Assert()
    {
    }
    template<class T>
    static T Move(T& i)
    {
        assert(!flags.direction);
        return i++;
    }
};

struct DirBackward
{
    static void Assert()
    {
    }
    template<class T>
    static T Move(T& i)
    {
        assert(flags.direction);
        return i--;
    }
};

template <typename DST, typename SRC, typename DIR> void movsw()
{
    DIR::Assert();
    DST::Set8(es, DIR::Move(di), SRC::Get8(ds, DIR::Move(si)));
    DST::Set8(es, DIR::Move(di), SRC::Get8(ds, DIR::Move(si)));
}
template <typename DST, typename SRC, typename DIR> void movsb ()
{
    DIR::Assert();
    DST::Set8(es, DIR::Move(di), SRC::Get8(ds, DIR::Move(si)));
}

template <class DST, class DIR> void stosw()
{
    DIR::Assert();
    DST::Set8(es, DIR::Move(di), al);
    DST::Set8(es, DIR::Move(di), ah);
}

template <class DST, class DIR> void stosb()
{
    DIR::Assert();
    DST::Set8(es, DIR::Move(di), al);
}

template <typename DST, typename DIR> void rep_stosw()
{
    if (cx)
    {
        while (cx--)
            stosw<DST, DIR>();
        cx = 0;
    }
}
template <typename DST, typename DIR> void rep_stosb()
{
    if (cx)
    {
        while (cx--)
            stosb<DST, DIR>();
        cx = 0;
    }
}
template <typename DST, typename SRC, typename DIR> void rep_movsw()
{
    if (!cx)
        return;
    while (cx--)
        movsw<DST, SRC, DIR>();
    cx = 0;
}
template <typename DST, typename SRC, typename DIR> void rep_movsb()
{
    if (cx == 0) return;
    assert(cx);
    while (cx--)
        movsb<DST, SRC, DIR>();
    cx = 0;
}
template <typename SRC, typename DIR> void lodsw()
{
    DIR::Assert();
    al = SRC::Get8(ds, DIR::Move(si));
    ah = SRC::Get8(ds, DIR::Move(si));
}

template <typename SRC, typename DIR> void lodsb()
{
    DIR::Assert();
    al = SRC::Get8(ds, DIR::Move(si));
}
template <typename SRC, typename DIR> void rep_lodsb()
{
    assert(0);
}

template <typename SRC, typename DIR> void lodsb_es()
{
    DIR::Assert();
    al = SRC::Get8(es, DIR::Move(si));
}

template <typename SRC, typename DIR> void lodsb_ss()
{
    DIR::Assert();
    al = SRC::Get8(ss, DIR::Move(si));
}

template <class DST, class SRC, class DIR> void cmpsb()
{
    int a = SRC::Get8(ds, DIR::Move(si));
    int b = SRC::Get8(es, DIR::Move(di));
    flags.zero = a==b ? 1 : 0;
}

template <class DST, class SRC, class DIR> void cmpsw()
{
    int a = SRC::Get16(ds, DIR::Move(si));
    DIR::Move(si);
    int b = SRC::Get16(es, DIR::Move(di));
    DIR::Move(di);
    flags.carry = b < a;
    flags.zero = a==b ? 1 : 0;
}

template <class DST, class SRC, class DIR> void repe_cmpsw()
{
    flags.zero = 1;
    while (cx-- && flags.zero == 1 )
        cmpsw<DST, SRC, DIR>();
}

template <class SRC, class DIR> void repne_scasb(uint8_t value)
{
    assert(flags.direction == 0);
    flags.zero = 0;
    while (cx != 0 && flags.zero == 0 )
    {
        flags.zero = value - SRC::Get8(es, di) == 0;
        DIR::Move(di);
        cx--;
    }
}

#else

extern cicocontext_t ctx;

void cicocontext_t::mul(uint8_t r)
{
    int v = r * ctx.a.r8.l;
    ctx.a.r16 = v & 0xffff;
}
void cicocontext_t::mul(uint16_t r)
{
    int v = r * ctx.a.r16;
    ctx.a.r16 = v & 0xffff;
    ctx.d.r16 = v >> 16;
}
void cicocontext_t::imul(uint16_t r)
{
    int v = (int16_t)ctx.a.r16 * (int16_t)r;
    ctx.a.r16 = v & 0xffff;
    ctx.d.r16 = v >> 16;
}
void cicocontext_t::imul(uint8_t r)
{
    int v = (char)r * (short)ctx.a.r16;
    ctx.a.r16 = v & 0xffff;
}
void cicocontext_t::idiv(uint16_t r)
{
    int32_t n = (ctx.d.r16 << 16) | ctx.a.r16;
    ctx.a.r16 = n / (int16_t)r;
    ctx.d.r16 = n % (int16_t)r;
}
void cicocontext_t::idiv(uint8_t r)
{
    int32_t dw = (ctx.d.r16 << 16) | ctx.a.r16;
    uint16_t result = dw / r;
    uint16_t remainder = dw % r;
    ctx.a.r16 = result;
    ctx.d.r16 = remainder;
}
void cicocontext_t::div(uint16_t r)
{
    uint16_t result = ctx.a.r16 / r;
    uint16_t remain = ctx.a.r16 % r;
    ctx.a.r16 = result;
    ctx.d.r16 = remain;
}
void cicocontext_t::div(uint8_t r)
{
    uint16_t result = ctx.a.r16 / r;
    uint16_t remain = ctx.a.r16 % r;
    ctx.a.r8.l = result;
    ctx.a.r8.h = remain; //gabo!
}

uint8_t cicocontext_t::rcr(uint8_t r, int i)
{
    assert(i == 1);
    int newCarry = !!(r & 0x1);
    r >>= 1;
    r |= ctx.carry ? 0x80 : 0;
    ctx.carry = newCarry;
    return r;
}
uint8_t cicocontext_t::rcl(uint8_t r, int i)
{
    assert(i == 1);
    int newCarry = !!(r & 0x80);
    r <<= 1;
    r |= ctx.carry;
    ctx.carry = newCarry;
    return r;
}
uint16_t cicocontext_t::rcl(uint16_t r, int i)
{
    while (i > 0)
    {
        int newCarry = !!(r & 0x8000);
        r <<= 1;
        r |= ctx.carry ? 0x0001 : 0x0000;
        ctx.carry = newCarry;
        i--;
    }
    return r;
}
uint16_t cicocontext_t::rcr(uint16_t r, int i)
{
    while (i > 0)
    {
        int newCarry = !!(r & 0x0001);
        r >>= 1;
        r |= ctx.carry ? 0x8000 : 0x0000;
        ctx.carry = newCarry;
        i--;
    }
    return r;
}

uint16_t cicocontext_t::sar(uint16_t a, uint8_t b)
{
    int16_t sa = (short)a;
    sa >>= b;
    return sa;
}
uint8_t cicocontext_t::sar(uint8_t a, uint8_t b)
{
    int8_t sa = (char)a;
    sa >>= b;
    return sa;
}
void cicocontext_t::cbw()
{
    ctx.a.r8.h = (ctx.a.r8.l & 0x80) ? 0xff : 0;
}
uint8_t cicocontext_t::ror(uint8_t r, int l)
{
    return (r >> l) | (r << (8-l));
}
uint16_t cicocontext_t::ror(uint16_t r, int l)
{
    return (r >> l) | (r << (16-l));
}
uint8_t cicocontext_t::rol(uint8_t r, int l)
{
    return (r << l) | (r >> (8-l));
}
uint16_t cicocontext_t::rol(uint16_t r, int l)
{
    return (r << l) | (r >> (16-l));
}

#endif
}
