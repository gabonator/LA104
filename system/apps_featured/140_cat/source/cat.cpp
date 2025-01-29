#include "cicoctx.h"
using namespace CicoContext;

void sub_17230();

void start()
{
    headerSize = 0x0200;
    loadAddress = 0x1000;
    cs = 0x1723;
    ds = 0x1713;
    es = 0x1713;
    ss = 0x1000;
    sp = 0x0100;
    load("/Users/gabrielvalky/Documents/git/Projects/CicoJit/gamelib/alleycat/dos", "CAT.EXE", 55067);
    assert(memoryAGet(0x1010, 0x430) == 5);
    sub_17230();
}
void sub_17230();
void sub_17930();
void sub_1793d();
void sub_17b15();
void sub_181b7();
void sub_181f9();
void sub_18250();
void sub_18299();
void sub_18354();
void sub_18375();
void sub_18413();
void sub_185da();
void sub_18608();
void sub_18618();
void sub_18649();
void sub_18838();
void sub_18a60();
void sub_18daa();
void sub_18f61();
void sub_19070();
void sub_19325();
void sub_19527();
void sub_19560();
void sub_198c0();
void sub_198e3();
void sub_1990a();
void sub_19911();
void sub_19918();
void sub_19922();
void sub_1992c();
void sub_19969();
void sub_19c60();
void sub_19c98();
void sub_19d54();
void sub_19dce();
void sub_19e6d();
void sub_19eb4();
void sub_19ee0();
void sub_19f65();
void sub_19fa0();
void sub_19fcd();
void sub_19ffa();
void sub_1a02d();
void sub_1a040();
void sub_1c5e0();
void sub_1c680();
void sub_1cd51();
void sub_1ce90();
bool sub_1cee0();
void sub_1d004();
void sub_1d06b();
void sub_17230()
{
    // no return
    push(ds);
    ax = 0x0000;
    push(ax);
    sub_1ce90();
    ax = 0x1010;
    ds = ax;
    sub_185da();
    memoryASet(ds, 0x0690, 0x04);
    //memoryASet16(ds, 0x6df8, 0x0000);
    memoryASet(ds, 0x069b, 0x00);
    sub_18649();
    sub_18618();
    ax = memoryAGet16(ds, 0x0693);
    ax += 0x0240;
    //memoryASet16(ds, 0x6e00, ax);
    ax = 0x0004;
    interrupt(0x10);
    al = 0x04;
    if (memoryAGet(ds, 0x0697) == 0xfd)
        goto loc_1726f;
    al = 0x06;
loc_1726f:
    memoryASet(ds, 0x0690, al);
    ah = 0x0b;
    bx = 0x0101;
    interrupt(0x10);
    memoryASet16(ds, 0x0416, 0x0000);
    memoryASet16(ds, 0x0004, 0x0000);
    sub_18f61();
    if (memoryAGet(ds, 0x0697) == 0xfd)
        goto loc_17295;
    dx = 0x03d9;
    al = 0x20;
    out(dx, al);
loc_17295:
    sub_1a040();
    sub_19911();
    sub_1990a();
    memoryASet(ds, 0x041a, 0x00);
    ax = 0xffff;
    memoryASet16(ds, 0x041d, ax);
    memoryASet16(ds, 0x041f, ax);
    memoryASet(ds, 0x0000, 0xff);
loc_172b1:
    sub_198c0();
    memoryASet16(ds, 0x0008, 0x0000);
    memoryASet16(ds, 0x0004, 0x0000);
    sub_18f61();
    sub_1cd51();
    if (!sub_1cee0())
        return;
    sub_1cd51();
    if (memoryAGet(ds, 0x041a) != 0x00)
        goto loc_172de;
loc_172d3:
    sub_1cd51();
    assert(0); //sub_1d115();
    memoryASet(ds, 0x041a, 0x01);
loc_172de:
    ax = 0; //memoryAGet16(ds, 0x6df8);
    memoryASet16(ds, 0x0008, ax);
    memoryASet(ds, 0x1f80, 0x03);
    sub_1990a();
    memoryASet16(ds, 0x0004, 0x0000);
    sub_18f61();
    sub_1cd51();
    memoryASet16(ds, 0x1c30, 0x0000);
    ah = 0;
    interrupt(0x1a);
    memoryASet16(ds, 0x0412, dx);
    memoryASet16(ds, 0x0414, 0x0000);
    memoryASet(ds, 0x0418, 0x00);
    memoryASet(ds, 0x0419, 0x00);
    memoryASet(ds, 0x041c, 0x00);
    memoryASet(ds, 0x041b, 0x00);
    sub_1cd51();
    if (memoryAGet(ds, 0x1f80) == 0x00)
        goto loc_172b1;
    if (memoryAGet(ds, 0x041b) != 0x00)
        goto loc_172de;
    if (memoryAGet(ds, 0x041c) != 0x00)
        goto loc_172d3;
    assert(0); //sub_19c30();
    assert(0); //sub_1c630();
    memoryASet(ds, 0x1f81, 0xff);
    sub_1cd51();
    memoryASet16(ds, 0x0004, 0x0000);
    if (memoryAGet(ds, 0x0419) == 0x00)
        goto loc_17367;
    assert(0); //sub_179d1();
    memoryASet(ds, 0x0550, 0x02);
    memoryASet(ds, 0x0576, 0x01);
    memoryASet(ds, 0x0578, 0x20);
    goto loc_17370;
loc_17367:
    memoryASet16(ds, 0x0579, 0x0000);
    sub_1793d();
loc_17370:
    sub_19070();
    sub_18a60();
    assert(0); //sub_19440();
    sub_19560();
    sub_19922();
    sub_1992c();
    assert(0); //sub_1caed();
loc_17385:
    if (memoryAGet(ds, 0x1f80) != 0x00)
        goto loc_1738f;
    goto loc_172b1;
loc_1738f:
    assert(0); //sub_18568();
    if (memoryAGet(ds, 0x041c) == 0x00)
        goto loc_1739c;
    goto loc_172d3;
loc_1739c:
    if (memoryAGet(ds, 0x041b) == 0x00)
        goto loc_173a6;
    goto loc_172de;
loc_173a6:
    assert(0); //sub_18430();
    sub_17b15();
    assert(0); //sub_19093();
    if (memoryAGet(ds, 0x1cb8) != 0x00)
        goto loc_173c1;
    memoryASet(ds, 0x040f, memoryAGet(ds, 0x040f) + 1);
    if (memoryAGet(ds, 0x040f) & 0x03)
        goto loc_17385;
loc_173c1:
    assert(0); //sub_195ab();
    sub_198e3();
    if (memoryAGet(ds, 0x0551) == 0x00)
        goto loc_17385;
    if (memoryAGet(ds, 0x1f80) != 0x00)
        goto loc_173e7;
    goto loc_172b1;
loc_173e7:
    ah = 0;
    interrupt(0x1a);
    memoryASet16(ds, 0x0410, dx);
    ax = memoryAGet16(ds, 0x0579);
    memoryASet16(ds, 0x0001, ax);
    al = memoryAGet(ds, 0x057b);
    memoryASet(ds, 0x0003, al);
    memoryASet(ds, 0x0419, 0x01);
    if (memoryAGet(ds, 0x0418) == 0x00)
        goto loc_17415;
    memoryASet(ds, 0x0418, 0x00);
    memoryASet16(ds, 0x0004, 0x0007);
    goto loc_17468;
loc_17415:
    sub_1a02d();
    if (!(dl & 0xa0))
        goto loc_1743a;
    bx = memoryAGet16(ds, 0x0008);
    bx &= 0x0003;
    if (bx == 0x0003)
        goto loc_1743a;
    cl = 0x02;
    bx <<= cl;
    dx &= 0x0003;
    bx += dx;
    al = memoryAGet(ds, bx + 1057);
    goto loc_1744c;
loc_1743a:
    sub_1a02d();
    dx &= 0x0007;
    if (dx >= 0x0005)
        goto loc_1743a;
    bx = dx;
    al = memoryAGet(ds, bx + 1069);
loc_1744c:
    ah = 0;
    if (ax != memoryAGet16(ds, 0x041d))
        goto loc_1745a;
    if (ax == memoryAGet16(ds, 0x041f))
        goto loc_17415;
loc_1745a:
    memoryASet16(ds, 0x0004, ax);
    cx = memoryAGet16(ds, 0x041d);
    memoryASet16(ds, 0x041f, cx);
    memoryASet16(ds, 0x041d, ax);
loc_17468:
    memoryASet16(ds, 0x0006, 0x0000);
    bx = memoryAGet16(ds, 0x0004);
    if (bx <= 0x0007)
        goto loc_17479;
    bx = 0;
loc_17479:
    bx <<= 1;
    assert(0); //    jumpIndirect(cs, memoryAGet16(cs, bx + 592));
}
void sub_17930()
{
    memoryASet16(ds, 0x057d, 0x0000);
    memoryASet16(ds, 0x0684, 0x0000);
}
void sub_1793d()
{
    cx = 0x0000;
    ah = 0x01;
    if (memoryAGet16(ds, 0x0579) < 0x00a0)
        goto loc_1794f;
    cx = 0x0128;
    ah = 0xff;
loc_1794f:
    memoryASet(ds, 0x056e, ah);
    memoryASet(ds, 0x0558, 0x03);
    memoryASet(ds, 0x0559, 0x0c);
    dl = 0xb4;
    memoryASet16(ds, 0x0579, cx);
    memoryASet(ds, 0x057b, dl);
    memoryASet(ds, 0x057c, 0xe6);
    sub_19ee0();
    memoryASet16(ds, 0x055f, ax);
    memoryASet16(ds, 0x0561, 0x0b03);
    sub_18354();
    memoryASet(ds, 0x0571, 0x00);
    memoryASet16(ds, 0x0572, 0x0002);
    memoryASet(ds, 0x0576, 0x01);
    memoryASet(ds, 0x055b, 0x00);
    memoryASet(ds, 0x0550, 0x00);
    memoryASet(ds, 0x055c, 0x00);
    memoryASet(ds, 0x055a, 0x00);
    memoryASet(ds, 0x0583, 0x00);
    memoryASet(ds, 0x0698, 0x00);
    memoryASet(ds, 0x0699, 0x00);
    memoryASet(ds, 0x0551, 0x00);
    memoryASet(ds, 0x0584, 0x00);
    memoryASet(ds, 0x0552, 0x00);
    memoryASet16(ds, 0x0554, 0x0000);
    memoryASet(ds, 0x0553, 0x00);
    memoryASet(ds, 0x127c, 0x00);
    sub_17930();
}
void sub_17b15()
{
    ah = 0;
    interrupt(0x1a);
    if (dx != memoryAGet16(ds, 0x057d))
        goto loc_17b2d;
    if (memoryAGet16(ds, 0x0684) == 0x0000)
        goto loc_17b2c;
    memoryASet16(ds, 0x0684, memoryAGet16(ds, 0x0684) - 1);
    if (memoryAGet16(ds, 0x0684) == 0)
        goto loc_17b3c;
loc_17b2c:
    return;
loc_17b2d:
    ax = 0x0020;
    if (memoryAGet(ds, 0x0697) != 0xfd)
        goto loc_17b39;
    ax >>= 1;
loc_17b39:
    memoryASet16(ds, 0x0684, ax);
loc_17b3c:
    if (memoryAGet16(ds, 0x0004) == 0x0002)
        goto loc_17b4d;
    cl = memoryAGet(ds, 0x0571);
    cl |= memoryAGet(ds, 0x056e);
    if (cl)
        goto loc_17b56;
loc_17b4d:
    push(dx);
    push(ax);
    sub_18608();
    ax = pop();
    dx = pop();
//    if (flags.zero) // gabo!
//        goto loc_17b2c;
loc_17b56:
    memoryASet16(ds, 0x057d, dx);
    memoryASet16(ds, 0x057f, ax);
    if (memoryAGet16(ds, 0x0004) != 0x0004)
        goto loc_17b6b;
    if (memoryAGet(ds, 0x39e1) != 0x00)
        goto loc_17b2c;
loc_17b6b:
    if (memoryAGet16(ds, 0x0004) != 0x0006)
        goto loc_17b79;
    if (memoryAGet(ds, 0x44bd) != 0x00)
        goto loc_17b2c;
loc_17b79:
    if (memoryAGet16(ds, 0x0004) == 0x0002)
        goto loc_17b83;
    goto loc_17ddc;
loc_17b83:
    si = memoryAGet16(ds, 0x0008);
    si <<= 1;
    ax = memoryAGet16(ds, 0x057d);
    ax -= memoryAGet16(ds, 0x05f1);
    if (ax < memoryAGet16(ds, si + 1417))
        goto loc_17c06;
    if (ax < memoryAGet16(ds, si + 1433))
        goto loc_17ba1;
    memoryASet(ds, 0x0552, 0x01);
loc_17ba1:
    memoryASet(ds, 0x05f5, memoryAGet(ds, 0x05f5) - 1);
    if (memoryAGet(ds, 0x05f5) != 0)
        goto loc_17be9;
    assert(0); //sub_1cbaf();
    memoryASet(ds, 0x05f5, 0x06);
    al = memoryAGet(ds, 0x05f4);
    if (memoryAGet(ds, 0x057b) < 0xb3)
        goto loc_17bc2;
    if (al >= 0xc8)
        goto loc_17bc2;
    al += 0x1e;
    memoryASet(ds, 0x05f4, al);
loc_17bc2:
    dl = memoryAGet(ds, 0x057b);
    flags.carry = dl < al;
    dl -= al;
    if (!flags.carry)
        goto loc_17bcc;
    dl = 0;
loc_17bcc:
    cx = memoryAGet16(ds, 0x0579);
    dl &= 0xf8;
    sub_19ee0();
    di = ax;
    si = 0x064e;
    ax = 0xb800;
    es = ax;
    bp = 0x000e;
    cx = 0x0503;
    sub_19f65();
loc_17be9:
    memoryASet(ds, 0x056e, 0x00);
    memoryASet(ds, 0x0571, 0x01);
    memoryASet(ds, 0x05f3, 0x01);
    memoryASet(ds, 0x0576, 0x20);
    bx = 0;
    ah = 0x0b;
    interrupt(0x10);
    goto loc_17cb6;
loc_17c06:
    si = memoryAGet16(ds, 0x0008);
    si <<= 1;
    bx = 0;
    if (ax < memoryAGet16(ds, si + 1449))
        goto loc_17c26;
    bl++;
    if (ax < memoryAGet16(ds, si + 1465))
        goto loc_17c26;
    bl = 0x05;
    if (ax < memoryAGet16(ds, si + 1481))
        goto loc_17c26;
    bl--;
loc_17c26:
    ah = 0x0b;
    interrupt(0x10);
    al = memoryAGet(ds, 0x056e);
    memoryASet(ds, 0x056f, al);
    al = memoryAGet(ds, 0x0571);
    memoryASet(ds, 0x0570, al);
    al = memoryAGet(ds, 0x0698);
    if (al != 0x00)
        goto loc_17c4a;
    if (memoryAGet16(ds, 0x0574) < 0x0010)
        goto loc_17c5e;
    memoryASet16(ds, 0x0574, memoryAGet16(ds, 0x0574) - 1);
    goto loc_17c67;
loc_17c4a:
    if (al != memoryAGet(ds, 0x056e))
        goto loc_17c5e;
    if (memoryAGet16(ds, 0x0574) >= 0x0030)
        goto loc_17c67;
    memoryASet16(ds, 0x0574, memoryAGet16(ds, 0x0574) + 0x0003);
    goto loc_17c67;
loc_17c5e:
    memoryASet(ds, 0x056e, al);
    memoryASet16(ds, 0x0574, 0x0020);
loc_17c67:
    ax = memoryAGet16(ds, 0x0574);
    cl = 0x03;
    ax >>= cl;
    bx = memoryAGet16(ds, 0x0008);
    bl <<= 1;
    if (ax <= memoryAGet16(ds, bx + 1644))
        goto loc_17c7e;
    ax = memoryAGet16(ds, bx + 1644);
loc_17c7e:
    memoryASet16(ds, 0x0572, ax);
    sub_181f9();
    al = memoryAGet(ds, 0x0699);
    if (al != 0x00)
        goto loc_17c9a;
    al = ~al;
    if (memoryAGet(ds, 0x0576) < 0x10)
        goto loc_17cae;
    memoryASet(ds, 0x0576, memoryAGet(ds, 0x0576) - 1);
    goto loc_17cb6;
loc_17c9a:
    if (al != memoryAGet(ds, 0x0571))
        goto loc_17cae;
    if (memoryAGet(ds, 0x0576) >= 0x40)
        goto loc_17cb6;
    memoryASet(ds, 0x0576, memoryAGet(ds, 0x0576) + 0x04);
    goto loc_17cb6;
loc_17cae:
    memoryASet(ds, 0x0571, al);
    memoryASet(ds, 0x0576, 0x20);
loc_17cb6:
    si = memoryAGet16(ds, 0x0008);
    dl = memoryAGet(ds, 0x057b);
    cl = 0x04;
    bl = memoryAGet(ds, 0x0576);
    bl >>= cl;
    if (bl <= memoryAGet(ds, si + 1660))
        goto loc_17cd0;
    bl = memoryAGet(ds, si + 1660);
loc_17cd0:
    al = memoryAGet(ds, 0x0571);
    if (al < 0x01)
        goto loc_17cfe;
    if (al != 0x01)
        goto loc_17ce4;
    dl += bl;
    if (dl < 0xb4)
        goto loc_17cfe;
    dl = 0xb3;
    goto loc_17cfe;
loc_17ce4:
    tl = dl;
    dl -= bl;
    if (tl < bl)
        goto loc_17ced;
    if (dl > 0x03)
        goto loc_17cfe;
loc_17ced:
    ax = memoryAGet16(ds, 0x09b8);
    if (ax != memoryAGet16(ds, 0x055d))
        goto loc_17cfc;
    ax = memoryAGet16(ds, 0x057d);
    memoryASet16(ds, 0x05f1, ax);
loc_17cfc:
    dl = 0x02;
loc_17cfe:
    memoryASet(ds, 0x057b, dl);
    cx = memoryAGet16(ds, 0x0579);
    sub_19ee0();
    memoryASet16(ds, 0x0563, ax);
    if (memoryAGet(ds, 0x05f3) == 0x00)
        goto loc_17d19;
    bx = 0x0010;
    goto loc_17d94;
loc_17d19:
    al = memoryAGet(ds, 0x056e);
    if (al != memoryAGet(ds, 0x056f))
        goto loc_17d2b;
    al = memoryAGet(ds, 0x0571);
    if (al == memoryAGet(ds, 0x0570))
        goto loc_17d30;
loc_17d2b:
    bx = 0x0018;
    goto loc_17d94;
loc_17d30:
    memoryASet16(ds, 0x0587, memoryAGet16(ds, 0x0587) + 1);
    bx = memoryAGet16(ds, 0x0587);
    al = memoryAGet(ds, 0x0698);
    al |= memoryAGet(ds, 0x0699);
    if (al)
        goto loc_17d43;
    bl >>= 1;
loc_17d43:
    if (memoryAGet(ds, 0x057b) < 0xb3)
        goto loc_17d51;
    if (memoryAGet(ds, 0x0571) == 0x01)
        goto loc_17d6c;
loc_17d51:
    if (memoryAGet(ds, 0x057b) > 0x04)
        goto loc_17d5f;
    if (memoryAGet(ds, 0x0699) != 0x00)
        goto loc_17d83;
loc_17d5f:
    al = memoryAGet(ds, 0x0576);
    ah = 0;
    ax >>= 1;
    if (ax >= memoryAGet16(ds, 0x0574))
        goto loc_17d83;
loc_17d6c:
    if (memoryAGet(ds, 0x056e) == 0x00)
        goto loc_17d83;
    bx &= 0x0006;
    if (memoryAGet(ds, 0x056e) == 0x01)
        goto loc_17d94;
    bl |= 0x08;
    goto loc_17d94;
loc_17d83:
    bx &= 0x0002;
    bl |= 0x10;
    if (memoryAGet(ds, 0x0571) != 0x01)
        goto loc_17d94;
    bl += 0x04;
loc_17d94:
    ax = memoryAGet16(ds, bx + 2470);
    memoryASet16(ds, 0x055d, ax);
    ax = memoryAGet16(ds, bx + 2496);
    memoryASet16(ds, 0x0565, ax);
    al = 0x30;
    cx = 0x02bc;
    if (memoryAGet(ds, 0x0697) < 0xfd)
        goto loc_17dc7;
    if (memoryAGet(ds, 0x0697) == 0xfd)
        goto loc_17db5;
    al = 0x08;
    cx = 0x03e8;
loc_17db5:
    if (memoryAGet(ds, 0x057b) > al)
        goto loc_17dc7;
loc_17dbb:
    sub_18608();
    if (!flags.zero)
        goto loc_17dbb;
loc_17dc0:
    sub_18608();
    if (flags.zero)
        goto loc_17dc0;
loc_17dc5:
    if (--cx)
        goto loc_17dc5;
loc_17dc7:
    sub_18413();
    ax = memoryAGet16(ds, 0x0563);
    memoryASet16(ds, 0x055f, ax);
    sub_18375();
    assert(0); //sub_1a6d0();
    if (!flags.carry)
        goto loc_17ddb;
    sub_18375();
loc_17ddb:
    return;
loc_17ddc:
    sub_18daa();
    if (!flags.carry)
        goto loc_17de2;
loc_17de1:
    return;
loc_17de2:
    if (memoryAGet(ds, 0x1cb8) != 0x00)
        goto loc_17de1;
    if (memoryAGet(ds, 0x0558) == 0x00)
        goto loc_17e4c;
    if (memoryAGet(ds, 0x0559) == 0x00)
        goto loc_17e03;
    if (memoryAGet(ds, 0x1cbf) != 0x00)
        goto loc_17e02;
    memoryASet(ds, 0x0559, memoryAGet(ds, 0x0559) - 1);
loc_17e02:
    return;
loc_17e03:
    memoryASet(ds, 0x0558, memoryAGet(ds, 0x0558) - 1);
    if (memoryAGet(ds, 0x0558) != 0)
        goto loc_17e15;
    memoryASet16(ds, 0x0572, 0x0008);
    sub_181f9();
    goto loc_17e4c;
loc_17e15:
    sub_18250();
    memoryASet16(ds, 0x055d, bx);
    al = memoryAGet(ds, 0x0558);
    ah = memoryAGet(ds, 0x056e);
    sub_181b7();
    if (memoryAGet(ds, 0x0558) == 0x02)
        goto loc_17e30;
    sub_18413();
loc_17e30:
    sub_18daa();
    if (flags.carry)
        goto loc_17e4b;
    sub_19325();
    if (flags.carry)
        goto loc_17e4b;
    dl = memoryAGet(ds, 0x057b);
    cx = memoryAGet16(ds, 0x0579);
    sub_19ee0();
    memoryASet16(ds, 0x055f, ax);
    sub_18375();
loc_17e4b:
    return;
loc_17e4c:
    if (memoryAGet(ds, 0x055c) < 0x01)
        goto loc_17e9a;
    if (memoryAGet(ds, 0x055c) != 0x01)
        goto loc_17e8f;
    memoryASet(ds, 0x055c, memoryAGet(ds, 0x055c) + 1);
    memoryASet16(ds, 0x0572, 0x0006);
    dl = memoryAGet(ds, 0x057b);
    cx = memoryAGet16(ds, 0x0579);
    sub_19ee0();
    memoryASet16(ds, 0x0563, ax);
    sub_18413();
    sub_18daa();
    if (flags.carry)
        goto loc_17e96;
    sub_19325();
    if (flags.carry)
        goto loc_17e96;
    ax = memoryAGet16(ds, 0x0563);
    memoryASet16(ds, 0x055f, ax);
    memoryASet16(ds, 0x0565, 0x0e03);
    memoryASet16(ds, 0x055d, 0x09da);
    sub_18375();
loc_17e8f:
    if (memoryAGet(ds, 0x0699) != 0x00)
        goto loc_17e97;
loc_17e96:
    return;
loc_17e97:
    goto loc_180a8;
loc_17e9a:
    if (memoryAGet(ds, 0x0571) != 0x00)
        goto loc_17ea4;
    goto loc_18053;
loc_17ea4:
    sub_181f9();
    if (!flags.carry)
        goto loc_17ec0;
    memoryASet(ds, 0x056e, 0x00);
    memoryASet(ds, 0x0576, 0x02);
    memoryASet(ds, 0x0571, 0x01);
    memoryASet(ds, 0x055b, 0x00);
    goto loc_17ef1;
loc_17ec0:
    al = memoryAGet(ds, 0x0578);
    flags.carry = memoryAGet(ds, 0x0577) < al;
    memoryASet(ds, 0x0577, memoryAGet(ds, 0x0577) - al);
    if (!flags.carry)
        goto loc_17ef1;
    if (memoryAGet(ds, 0x0571) == 0x01)
        goto loc_17ee6;
    if (memoryAGet(ds, 0x0576) <= 0x01)
        goto loc_17ede;
    memoryASet(ds, 0x0576, memoryAGet(ds, 0x0576) - 1);
    goto loc_17ef1;
loc_17ede:
    memoryASet(ds, 0x0571, 0x01);
    goto loc_17ef1;
loc_17ee6:
    if (memoryAGet(ds, 0x0576) >= 0x04)
        goto loc_17ef1;
    memoryASet(ds, 0x0576, memoryAGet(ds, 0x0576) + 1);
loc_17ef1:
    if (memoryAGet(ds, 0x055a) != 0x00)
        goto loc_17f17;
    if (memoryAGet(ds, 0x055b) == 0x00)
        goto loc_17f05;
    memoryASet(ds, 0x055b, memoryAGet(ds, 0x055b) - 1);
    if (memoryAGet(ds, 0x055b) != 0)
        goto loc_17f17;
loc_17f05:
    if (memoryAGet(ds, 0x0571) != 0x01)
        goto loc_17f17;
    sub_18838();
    if (!flags.carry)
        goto loc_17f17;
    al = memoryAGet(ds, 0x057c);
    goto loc_17f59;
loc_17f17:
    al = memoryAGet(ds, 0x057c);
    if (memoryAGet(ds, 0x0571) == 0x01)
        goto loc_17f36;
    flags.carry = al < memoryAGet(ds, 0x0576);
    al -= memoryAGet(ds, 0x0576);
    if (!flags.carry)
        goto loc_17f7f;
    al = 0;
    memoryASet(ds, 0x0571, 0x01);
    memoryASet(ds, 0x0576, 0x01);
    goto loc_17f7f;
loc_17f36:
    al += memoryAGet(ds, 0x0576);
    if (al <= 0xe6)
        goto loc_17f7f;
    if (memoryAGet16(ds, 0x0004) != 0x0007)
        goto loc_17f52;
    if (al < 0xf8)
        goto loc_17f7f;
    al = 0xf8;
    memoryASet(ds, 0x0551, 0x01);
    goto loc_17f7f;
loc_17f52:
    al = 0xe6;
    memoryASet(ds, 0x0550, 0x00);
loc_17f59:
    memoryASet(ds, 0x0571, 0x00);
    memoryASet(ds, 0x0584, 0x00);
    memoryASet16(ds, 0x0572, 0x0002);
    memoryASet(ds, 0x055b, 0x00);
    memoryASet(ds, 0x055a, 0x00);
    if (memoryAGet(ds, 0x055c) == 0x00)
        goto loc_17f7f;
    push(ax);
    assert(0); //sub_1ccf2();
    ax = pop();
loc_17f7f:
    memoryASet(ds, 0x057c, al);
    flags.carry = al < 0x32;
    al -= 0x32;
    if (!flags.carry)
        goto loc_17f88;
    al = 0;
loc_17f88:
    memoryASet(ds, 0x057b, al);
    dl = memoryAGet(ds, 0x057b);
    cx = memoryAGet16(ds, 0x0579);
    sub_19ee0();
    memoryASet16(ds, 0x0563, ax);
    if (memoryAGet(ds, 0x0583) != 0x00)
        goto loc_17fa3;
    sub_18413();
loc_17fa3:
    sub_18daa();
    if (flags.carry)
        goto loc_17ff4;
    sub_19325();
    if (flags.carry)
        goto loc_17ff4;
    ax = memoryAGet16(ds, 0x0563);
    memoryASet16(ds, 0x055f, ax);
    if (memoryAGet(ds, 0x0584) == 0x00)
        goto loc_17fd1;
    memoryASet16(ds, 0x0585, memoryAGet16(ds, 0x0585) + 0x0002);
    bx = memoryAGet16(ds, 0x0585);
    bx &= 0x000e;
    ax = memoryAGet16(ds, bx + 4034);
    bx = memoryAGet16(ds, bx + 4050);
    goto loc_17fd8;
loc_17fd1:
    ax = memoryAGet16(ds, 0x0569);
    bx = memoryAGet16(ds, 0x0567);
loc_17fd8:
    memoryASet16(ds, 0x055d, ax);
    memoryASet16(ds, 0x0565, bx);
    al = 0x32;
    tl = al;
    al -= memoryAGet(ds, 0x057c);
    if (al == 0)
        goto loc_1800e;
    if (tl < memoryAGet(ds, 0x057c))
        goto loc_1800e;
    cx = 0x0168;
loc_17fec:
    if (--cx)
        goto loc_17fec;
    flags.carry = bh < al;
    bh -= al;
    if (bh == 0)
        goto loc_17ff4;
    if (!flags.carry)
        goto loc_17ffa;
loc_17ff4:
    memoryASet(ds, 0x0583, 0x01);
    return;
loc_17ffa:
    memoryASet16(ds, 0x0565, bx);
    ah = bl;
    ah <<= 1;
    mul(ah);
    ax += memoryAGet16(ds, 0x0569);
    memoryASet16(ds, 0x055d, ax);
    goto loc_1804f;
loc_1800e:
    if (memoryAGet16(ds, 0x0004) != 0x0007)
        goto loc_1801e;
    al = memoryAGet(ds, 0x057b);
    flags.carry = al < 0xbb;
    al -= 0xbb;
    if (stop(/*inject_t::temp - sub al, 0xbb*/))
        goto loc_1804f;
    if (stop(/*inject_t::carry*/))
        goto loc_1802c;
loc_1801e:
    if (memoryAGet(ds, 0x0550) != 0x02)
        goto loc_1804f;
    al = memoryAGet(ds, 0x057b);
    tl = al;
    al -= 0x5e;
    if (tl < 0x5e)
        goto loc_1804f;
loc_1802c:
    flags.carry = bh < al;
    bh -= al;
    if (bh == 0)
        goto loc_18032;
    if (!flags.carry)
        goto loc_18046;
loc_18032:
    if (memoryAGet16(ds, 0x0004) != 0x0007)
        goto loc_1803f;
    memoryASet(ds, 0x0551, 0x01);
    return;
loc_1803f:
    sub_1793d();
    assert(0); //sub_1cbfb();
    return;
loc_18046:
    memoryASet16(ds, 0x0565, bx);
    memoryASet(ds, 0x0576, 0x02);
loc_1804f:
    sub_18375();
    return;
loc_18053:
    if (memoryAGet16(ds, 0x0004) == 0x0007)
        goto loc_18061;
    if (memoryAGet(ds, 0x057b) >= 0xb4)
        goto loc_180a8;
loc_18061:
    sub_18838();
    if (flags.carry)
        goto loc_18073;
    memoryASet(ds, 0x056e, 0x00);
    memoryASet(ds, 0x0571, 0x01);
    goto loc_180e1;
loc_18073:
    if (memoryAGet16(ds, 0x0004) != 0x0000)
        goto loc_180a8;
    sub_19527();
    if (flags.carry)
        goto loc_18086;
    memoryASet(ds, 0x056c, 0x00);
    goto loc_180a8;
loc_18086:
    if (memoryAGet(ds, 0x056c) != 0x00)
        goto loc_18090;
    assert(0); //sub_1cb4f();
loc_18090:
    memoryASet(ds, 0x0699, 0x01);
    memoryASet(ds, 0x056c, 0x01);
    sub_1a02d();
    dl &= 0x01;
    if (dl != 0)
        goto loc_180a4;
    dl = 0xff;
loc_180a4:
    memoryASet(ds, 0x0698, dl);
loc_180a8:
    al = memoryAGet(ds, 0x056e);
    memoryASet(ds, 0x056f, al);
    al = memoryAGet(ds, 0x0698);
    memoryASet(ds, 0x056e, al);
    al = memoryAGet(ds, 0x0699);
    memoryASet(ds, 0x0571, al);
    if (al != 0x00)
        goto loc_180c1;
    goto loc_18164;
loc_180c1:
    if (memoryAGet(ds, 0x0571) != 0x01)
        goto loc_180f9;
    if (memoryAGet(ds, 0x057b) < 0xb4)
        goto loc_180e1;
    memoryASet(ds, 0x0571, 0x00);
    memoryASet(ds, 0x0584, 0x00);
    memoryASet(ds, 0x0699, 0x00);
    goto loc_18164;
loc_180e1:
    ah = 0x01;
    al = 0x20;
    memoryASet(ds, 0x055b, 0x08);
    if (memoryAGet(ds, 0x0550) != 0x01)
        goto loc_18121;
    memoryASet(ds, 0x0550, 0x00);
    goto loc_18121;
loc_180f9:
    memoryASet(ds, 0x055b, 0x00);
    ax = memoryAGet16(ds, 0x0572);
    bl = al;
    if (al <= 0x02)
        goto loc_18109;
    al -= 0x02;
loc_18109:
    memoryASet16(ds, 0x0572, ax);
    ah = 0x08;
    al = bl;
    al ^= 0x0f;
    cl = 0x04;
    al <<= cl;
    if (memoryAGet(ds, 0x0550) != 0x01)
        goto loc_18121;
    memoryASet(ds, 0x0550, memoryAGet(ds, 0x0550) + 1);
loc_18121:
    memoryASet(ds, 0x0578, al);
    memoryASet(ds, 0x0576, ah);
    memoryASet(ds, 0x0577, 0x01);
    memoryASet(ds, 0x055c, 0x00);
    bl = memoryAGet(ds, 0x056e);
    bl++;
    bl <<= 1;
    if (memoryAGet(ds, 0x0571) == 0xff)
        goto loc_18144;
    bl += 0x06;
loc_18144:
    bh = 0;
    ax = memoryAGet16(ds, bx + 4010);
    memoryASet16(ds, 0x0569, ax);
    ax = memoryAGet16(ds, bx + 4022);
    memoryASet16(ds, 0x0567, ax);
    memoryASet(ds, 0x39e0, 0x00);
    if (memoryAGet(ds, 0x127c) == 0x00)
        goto loc_18163;
    assert(0); //sub_1cb28();
loc_18163:
    return;
loc_18164:
    if (memoryAGet16(ds, 0x0004) == 0x0000)
        goto loc_18175;
    if (memoryAGet16(ds, 0x0004) == 0x0007)
        goto loc_18175;
    assert(0); //sub_1a675();
loc_18175:
    sub_181f9();
    dl = memoryAGet(ds, 0x057b);
    cx = memoryAGet16(ds, 0x0579);
    sub_19ee0();
    memoryASet16(ds, 0x0563, ax);
    al = memoryAGet(ds, 0x056e);
    al |= memoryAGet(ds, 0x0571);
    if (al)
        goto loc_18193;
    sub_18299(); // sit
    return;
loc_18193:
    sub_18250();
    memoryASet16(ds, 0x055d, bx);
    sub_18413();
    sub_18daa();
    if (flags.carry)
        return;
    sub_19325();
    if (flags.carry)
        return;
    ax = memoryAGet16(ds, 0x0563);
    memoryASet16(ds, 0x055f, ax);
    memoryASet16(ds, 0x0565, 0x0b03);
    sub_18375();
}
void sub_181b7()
{
    cx = 0x0b03;
    cl -= al;
    memoryASet16(ds, 0x0565, cx);
    if (ah == 0xff)
        goto loc_181d6;
    ah = 0;
    al <<= 1;
    memoryASet16(ds, 0x055d, memoryAGet16(ds, 0x055d) + ax);
    memoryASet16(ds, 0x0579, 0x0000);
    goto loc_181e4;
loc_181d6:
    ah = 0;
    al <<= 1;
    al <<= 1;
    al <<= 1;
    ax += 0x0128;
    memoryASet16(ds, 0x0579, ax);
loc_181e4:
    push(ds);
    es = pop();
    si = memoryAGet16(ds, 0x055d);
    di = 0x000e;
    al = 0x03;
    sub_19fa0();
    memoryASet16(ds, 0x055d, 0x000e);
}
void sub_181f9()
{
    memoryASet16(ds, 0x05f6, 0x0008);
    memoryASet16(ds, 0x05f8, 0x0123);
    if (memoryAGet16(ds, 0x0004) != 0x0007)
        goto loc_18218;
    memoryASet16(ds, 0x05f6, 0x0024);
    memoryASet16(ds, 0x05f8, 0x010f);
loc_18218:
    ax = memoryAGet16(ds, 0x0579);
    if (memoryAGet(ds, 0x056e) < 0x01)
        goto loc_1824e;
    if (memoryAGet(ds, 0x056e) != 0x01)
        goto loc_18237;
    ax += memoryAGet16(ds, 0x0572);
    if (ax < memoryAGet16(ds, 0x05f8))
        goto loc_1824b;
    ax = memoryAGet16(ds, 0x05f8);
    ax--;
    memoryASet16(ds, 0x0579, ax);
    flags.carry = true;
    return;
loc_18237:
    tx = ax;
    ax -= memoryAGet16(ds, 0x0572);
    if (tx < memoryAGet16(ds, 0x0572))
        goto loc_18243;
    if (ax >= memoryAGet16(ds, 0x05f6))
        goto loc_1824b;
loc_18243:
    ax = memoryAGet16(ds, 0x05f6);
    memoryASet16(ds, 0x0579, ax);
    flags.carry = true;
    return;
loc_1824b:
    memoryASet16(ds, 0x0579, ax);
loc_1824e:
    flags.carry = false;
}
void sub_18250()
{
    al = memoryAGet(ds, 0x056e);
    if (al == memoryAGet(ds, 0x056f))
        goto loc_1825f;
    memoryASet16(ds, 0x0572, 0x0002);
loc_1825f:
    if (memoryAGet16(ds, 0x0572) >= 0x0008)
        goto loc_18275;
    memoryASet(ds, 0x0577, memoryAGet(ds, 0x0577) - 1);
    al = memoryAGet(ds, 0x0577);
    al &= 0x03;
    if (al != 0)
        goto loc_18275;
    memoryASet16(ds, 0x0572, memoryAGet16(ds, 0x0572) + 1);
loc_18275:
    bl = memoryAGet(ds, 0x056b);
    bl++;
    if (bl < 0x06)
        goto loc_18282;
    bl = 0x00;
loc_18282:
    memoryASet(ds, 0x056b, bl);
    if (memoryAGet(ds, 0x056e) != 0xff)
        goto loc_18290;
    bl += 0x06;
loc_18290:
    bl <<= 1;
    bh = 0;
    bx = memoryAGet16(ds, bx + 3962);
}
void sub_18354()
{
    ax = 0x1010;
    es = ax;
    di = 0x05fa;
    push(ds);
    si = memoryAGet16(ds, 0x055f);
    ax = 0xb800;
    ds = ax;
    cx = memoryAGet16(es, 0x0561);
    sub_19ffa();
    ds = pop();
    memoryASet(ds, 0x0583, 0x00);
}
void sub_18375()
{
    ax = 0xb800;
    es = ax;
    di = memoryAGet16(ds, 0x055f);
    bp = 0x05fa;
    si = memoryAGet16(ds, 0x055d);
    cx = memoryAGet16(ds, 0x0565);
    memoryASet16(ds, 0x0561, cx);
    memoryASet(ds, 0x0583, 0x00);
    sub_19f65();
}
void sub_18413()
{
    ax = 0xb800;
    es = ax;
    di = memoryAGet16(ds, 0x055f);
    si = 0x05fa;
    cx = memoryAGet16(ds, 0x0561);
    sub_19fcd();
}
void sub_185da()
{
    ax = 0xf000;
    es = ax;
    al = 0xfc; //memoryAGet(es, 0xfffe);
    memoryASet(ds, 0x0697, al);
}
void sub_18608()
{
    dx = 0x03da;
    in(al, dx);
    al &= 0x08;
    flags.zero = al == 0;
}
void sub_18618()
{
    push(ax);
    push(es);
    push(di);
    push(cx);
    ax = 0x1010;
    es = ax;
    flags.direction = false;
    di = 0x06b7;
    cx = 0x0016;
    al = 0x80;
    rep_stosb<MemAuto, DirAuto>();
    ax = memoryAGet16(es, 0x0693);
    ax -= 0x0070;
    memoryASet16(es, 0x0691, ax);
    ax = 0x0040;
    es = ax;
    al = 0x00; //memoryAGet(es, 0x0012);
    //memoryASet(cs, 0x13e7, al);
    cx = pop();
    di = pop();
    es = pop();
    ax = pop();
}
void sub_18649()
{
//    ax = 0;
//    es = ax;
//    ax = memoryAGet16(es, 0x0024);
//    bx = memoryAGet16(es, 0x0026);
//    cx = memoryAGet16(es, 0x0120);
//    dx = memoryAGet16(es, 0x0122);
//    memoryASet16(cs, 0x13df, ax);
//    memoryASet16(cs, 0x13e1, bx);
//    memoryASet16(cs, 0x13e3, cx);
//    memoryASet16(cs, 0x13e5, dx);
//    bx = 0x14b3;
//    if (memoryAGet(ds, 0x0697) != 0xfd)
//        goto loc_18680;
//    bx = 0x14fb;
//loc_18680:
//    flags.interrupts = false;
//    memoryASet16(es, 0x0024, bx);
//    memoryASet16(es, 0x0026, cs);
//    if (memoryAGet(ds, 0x0697) != 0xfd)
//        goto loc_186ad;
//    memoryASet16(es, 0x0120, 0x1554);
//    memoryASet16(es, 0x0122, cs);
//    ax = 0x0040;
//    es = ax;
//    al = memoryAGet(es, 0x0018);
//    al |= 0x01;
//    memoryASet(es, 0x0018, al);
//loc_186ad:
//    flags.interrupts = true;
}
void sub_18838()
{
    if (memoryAGet16(ds, 0x0004) != 0x0007)
        goto loc_18843;
    assert(0); //sub_1a32a();
    return;
loc_18843:
    if (memoryAGet16(ds, 0x0004) == 0x0000)
        goto loc_1884e;
    assert(0); //sub_188f6();
    return;
loc_1884e:
    al = memoryAGet(ds, 0x057b);
    al &= 0xf8;
    if (al == 0x60)
        goto loc_18860;
    assert(0); //sub_18887();
    if (flags.carry)
        return;
    assert(0); //sub_189dd();
    return;
loc_18860:
    if (memoryAGet(ds, 0x0550) >= 0x02)
        goto loc_18885;
    memoryASet(ds, 0x057b, al);
    al += 0x32;
    memoryASet(ds, 0x057c, al);
    if (memoryAGet(ds, 0x0550) == 0x01)
        goto loc_18883;
    memoryASet(ds, 0x0550, 0x01);
    ah = 0;
    interrupt(0x1a);
    memoryASet16(ds, 0x0556, dx);
loc_18883:
    flags.carry = true;
    return;
loc_18885:
    flags.carry = false;
}
void sub_18a60()
{
    memoryASet(ds, 0x1665, 0x00);
    memoryASet(ds, 0x1673, 0x00);
    memoryASet(ds, 0x1677, 0x00);
    memoryASet(ds, 0x1678, 0x00);
    memoryASet16(ds, 0x166c, 0x0009);
}
void sub_18daa()
{
    if (memoryAGet16(ds, 0x0004) != 0x0000)
        goto loc_18e11;
    dl = memoryAGet(ds, 0x1673);
    if (dl == 0x00)
        goto loc_18e11;
    cx = memoryAGet16(ds, 0x17df);
    tl = ch;
    ch = cl;
    cl = tl;
    si = 0x0010;
    ax = memoryAGet16(ds, 0x1671);
    bx = memoryAGet16(ds, 0x0579);
    dh = memoryAGet(ds, 0x057b);
    di = 0x0018;
    ch = 0x0e;
    assert(0); //sub_1a059();
    if (!flags.carry)
        return;
    sub_18413();
    assert(0); //sub_18b52();
    assert(0); //sub_1830d();
    if (memoryAGet(ds, 0x1675) != 0x00)
        goto loc_18e0f;
    memoryASet(ds, 0x1675, 0x01);
    assert(0); //sub_18396();
    dl = 0x01;
    if (memoryAGet(ds, 0x1674) == 0xff)
        goto loc_18dfb;
    dl = 0xff;
loc_18dfb:
    memoryASet(ds, 0x1674, dl);
    memoryASet16(ds, 0x17ea, 0x0060);
    memoryASet(ds, 0x17e9, 0x01);
    memoryASet(ds, 0x055c, 0x00);
loc_18e0f:
    flags.carry = true;
    return;
loc_18e11:
    flags.carry = false;
}
void sub_18f61()
{
    if (memoryAGet(ds, 0x0697) == 0xfd)
        goto loc_18f78;
    ah = 0x0b;
    bh = 0x01;
    si = memoryAGet16(ds, 0x0004);
    bl = memoryAGet(ds, si + 6227);
    interrupt(0x10);
    goto loc_18f97;
loc_18f78:
    si = memoryAGet16(ds, 0x0004);
    bl = 0x01;
    bh = memoryAGet(ds, si + 6203);
    assert(0); //sub_18f9e();
    bl = 0x02;
    bh = memoryAGet(ds, si + 6211);
    assert(0); //sub_18f9e();
    bl = 0x03;
    bh = memoryAGet(ds, si + 6219);
    assert(0); //sub_18f9e();
loc_18f97:
    ah = 0x0b;
    bx = 0;
    interrupt(0x10);
}
void sub_19070()
{
    memoryASet(ds, 0x1cbf, 0x00);
    memoryASet16(ds, 0x1ce1, 0x0000);
    memoryASet(ds, 0x1cc0, 0x00);
    memoryASet(ds, 0x1cc1, 0x00);
    memoryASet(ds, 0x1cb8, 0x00);
    memoryASet(ds, 0x1cc8, 0xb1);
    sub_1c680();
}
void sub_19325()
{
    if (memoryAGet(ds, 0x1cb8) != 0x00)
        goto loc_19364;
    al = memoryAGet(ds, 0x1cbf);
    al |= memoryAGet(ds, 0x1cc0);
    al |= memoryAGet(ds, 0x1cc1);
    if (!al)
        goto loc_19364;
    if (memoryAGet(ds, 0x057b) < 0xa3)
        goto loc_19364;
    if (memoryAGet(ds, 0x0558) != 0x00)
        goto loc_19364;
    ax = memoryAGet16(ds, 0x1cc6);
    ax += 0x0020;
    if (ax < memoryAGet16(ds, 0x0579))
        goto loc_19364;
    flags.carry = ax < 0x0038;
    ax -= 0x0038;
    if (!flags.carry)
        goto loc_1935a;
    ax = 0;
loc_1935a:
    if (ax > memoryAGet16(ds, 0x0579))
        goto loc_19364;
    assert(0); //sub_19366();
    return;
loc_19364:
    flags.carry = false;
}
void sub_19527()
{
    if (memoryAGet(ds, 0x1d59) != 0x00)
        goto loc_19530;
    flags.carry = false;
    return;
loc_19530:
    cx = memoryAGet16(ds, 0x1d64);
    tl = cl;
    cl = ch;
    ch = tl;
    ax = memoryAGet16(ds, 0x1d5c);
    dl = memoryAGet(ds, 0x1d5f);
    si = 0x0010;
    bx = memoryAGet16(ds, 0x0579);
    dh = memoryAGet(ds, 0x057b);
    di = 0x0018;
    ch = 0x0e;
    stop(/*74*/);
    assert(0); //sub_1a059();
    if (!flags.carry)
        return;
    memoryASet(ds, 0x1d58, 0x01);
}
void sub_19560()
{
    memoryASet16(ds, 0x1f6c, 0x0000);
    ax = 0;
    dl = 0x01;
    if (memoryAGet16(ds, 0x0579) > 0x00a0)
        goto loc_19577;
    ax = 0x012c;
    dl = 0xff;
loc_19577:
    memoryASet16(ds, 0x1f30, ax);
    memoryASet16(ds, 0x1f32, ax);
    memoryASet16(ds, 0x1f34, ax);
    memoryASet(ds, 0x1f3c, dl);
    memoryASet(ds, 0x1f3d, dl);
    memoryASet(ds, 0x1f3e, dl);
    memoryASet(ds, 0x1f48, 0x01);
    memoryASet(ds, 0x1f49, 0x01);
    memoryASet(ds, 0x1f4a, 0x01);
    memoryASet(ds, 0x1f50, 0x00);
    memoryASet(ds, 0x1f51, 0x00);
    memoryASet(ds, 0x1f52, 0x00);
}
void sub_198c0()
{
    push(ds);
    es = pop();
    cx = 0x0007;
    si = 0x1f82;
loc_198c8:
    lodsb<MemAuto, DirAuto>();
    bx = 0x0007;
    bx -= cx;
    if (--cx && al == memoryAGet(ds, bx + 8073))
        goto loc_198c8;
    if (al > memoryAGet(ds, bx + 8073))
        goto loc_198d7;
    return;
loc_198d7:
    si = 0x1f82;
    di = 0x1f89;
    cx = 0x0007;
    rep_movsb<MemAuto, MemAuto, DirAuto>();
}
void sub_198e3()
{
    al = memoryAGet(ds, 0x1f80);
    if (al != memoryAGet(ds, 0x1f81))
        goto loc_198ed;
    return;
loc_198ed:
    memoryASet(ds, 0x1f81, al);
    ah = 0;
    cl = 0x04;
    ax <<= cl;
    ax += 0x2720;
    si = ax;
    ax = 0xb800;
    es = ax;
    di = 0x1260;
    cx = 0x0801;
    sub_19fcd();
}
void sub_1990a()
{
    di = 0x1f82;
    sub_19918();
}
void sub_19911()
{
    di = 0x1f89;
    sub_19918();
}
void sub_19918()
{
    push(ds);
    es = pop();
    cx = 0x0007;
    al = 0;
    rep_stosb<MemAuto, DirAuto>();
}
void sub_19922()
{
    bx = 0x1f89;
    di = 0x12ca;
    sub_19969();
}
void sub_1992c()
{
    bx = 0x1f82;
    di = 0x143c;
    sub_19969();
}
void sub_19969()
{
    ax = 0xb800;
    es = ax;
    memoryASet16(ds, 0x1f90, di);
    memoryASet16(ds, 0x1f93, bx);
    memoryASet(ds, 0x1f92, 0x00);
loc_1997b:
    bx = memoryAGet16(ds, 0x1f93);
    al = memoryAGet(ds, bx);
    ah = 0;
    cl = 0x04;
    ax <<= cl;
    ax += 0x2720;
    si = ax;
    di = memoryAGet16(ds, 0x1f90);
    cx = 0x0801;
    sub_19fcd();
    memoryASet16(ds, 0x1f90, memoryAGet16(ds, 0x1f90) + 0x0002);
    memoryASet16(ds, 0x1f93, memoryAGet16(ds, 0x1f93) + 1);
    memoryASet(ds, 0x1f92, memoryAGet(ds, 0x1f92) + 1);
    if (memoryAGet(ds, 0x1f92) == 0x07)
        return;
    if (memoryAGet(ds, 0x1f92) != 0x03)
        goto loc_1997b;
    memoryASet16(ds, 0x1f90, memoryAGet16(ds, 0x1f90) + 0x0002);
    goto loc_1997b;
}
void sub_19c60()
{
    ax = 0xb800;
    es = ax;
    flags.direction = false;
    di = 0;
    ax = 0xaaaa;
    cx = 0x0fa0;
    rep_stosw<MemAuto, DirAuto>();
    di = 0x2000;
    cx = 0x0fa0;
    rep_stosw<MemAuto, DirAuto>();
    sub_19dce();
    bx = 0x28a0;
    ax = 0;
    sub_19d54();
    sub_19c98();
    ax = memoryAGet16(ds, 0x0008);
    push(ax);
    memoryASet16(ds, 0x0008, 0x0001);
    sub_19eb4();
    ax = pop();
    memoryASet16(ds, 0x0008, ax);
}
void sub_19c98()
{
    bx = 0; //memoryAGet16(ds, 0x6df8);
    bx &= 0x0003;
    bl <<= 1;
    si = memoryAGet16(ds, bx + 10961);
    di = 0x1902;
    cx = 0x0801;
    sub_19fcd();
}
void sub_19d54()
{
    cx = memoryAGet16(ds, bx);
    memoryASet16(ds, 0x2ac7, cx);
    memoryASet16(ds, 0x2acc, ax);
    bx += 0x0002;
loc_19d60:
    si = memoryAGet16(ds, bx);
    if (si != 0xffff)
        goto loc_19d69;
    return;
loc_19d69:
    di = memoryAGet16(ds, bx + 2);
    di += memoryAGet16(ds, 0x2acc);
    flags.direction = false;
    memoryASet(ds, 0x2ad0, ch);
    ch = 0;
    memoryASet16(ds, 0x2ace, cx);
loc_19d7b:
    cx = memoryAGet16(ds, 0x2ace);
    rep_movsb<MemAuto, MemAuto, DirAuto>();
    di -= memoryAGet16(ds, 0x2ace);
    di ^= 0x2000;
    if (di & 0x2000)
        goto loc_19d92;
    di += 0x0050;
loc_19d92:
    memoryASet(ds, 0x2ad0, memoryAGet(ds, 0x2ad0) - 1);
    if (memoryAGet(ds, 0x2ad0) != 0)
        goto loc_19d7b;
    bx += 0x0004;
    cx = memoryAGet16(ds, 0x2ac7);
    goto loc_19d60;
}
void sub_19dce()
{
    memoryASet16(ds, 0x2ac2, 0x103e);
loc_19dd4:
    memoryASet16(ds, 0x2ac2, memoryAGet16(ds, 0x2ac2) + 0x0002);
    di = memoryAGet16(ds, 0x2ac2);
    if (di >= 0x1090)
        goto loc_19e02;
loc_19de3:
    sub_1a02d();
    dx &= 0x0030;
    if (dl == memoryAGet(ds, 0x2ac4))
        goto loc_19de3;
    memoryASet(ds, 0x2ac4, dl);
    dx += 0x2904;
    si = dx;
    cx = 0x0801;
    sub_19fcd();
    goto loc_19dd4;
loc_19e02:
    di = 0x1180;
    ax = 0x5655;
    cx = 0x0500;
    flags.direction = false;
    rep_stosw<MemAuto, DirAuto>();
    di = 0x3180;
    cx = 0x0500;
    rep_stosw<MemAuto, DirAuto>();
    memoryASet16(ds, 0x2ac2, 0x2944);
loc_19e1c:
    memoryASet(ds, 0x2ac4, 0x09);
loc_19e21:
    sub_1a02d();
    dx &= 0x0776;
    dx += 0x12c0;
    di = dx;
    si = memoryAGet16(ds, 0x2ac2);
    cx = 0x0501;
    sub_19fcd();
    memoryASet(ds, 0x2ac4, memoryAGet(ds, 0x2ac4) - 1);
    if (memoryAGet(ds, 0x2ac4) != 0)
        goto loc_19e21;
    memoryASet16(ds, 0x2ac2, memoryAGet16(ds, 0x2ac2) + 0x000a);
    if (memoryAGet16(ds, 0x2ac2) < 0x296c)
        goto loc_19e1c;
    memoryASet(ds, 0x2ac4, 0x05);
loc_19e50:
    sub_1a02d();
    dx &= 0x003e;
    dx += 0x3a98;
    di = dx;
    si = 0x296c;
    cx = 0x0501;
    sub_19fcd();
    memoryASet(ds, 0x2ac4, memoryAGet(ds, 0x2ac4) - 1);
    if (memoryAGet(ds, 0x2ac4) != 0)
        goto loc_19e50;
}
void sub_19e6d()
{
    memoryASet16(ds, 0x2ac2, di);
    al = 0x03;
    if (di < 0x1720)
        goto loc_19e7b;
    al--;
loc_19e7b:
    memoryASet(ds, 0x2ac4, al);
    memoryASet16(ds, 0x2ac2, memoryAGet16(ds, 0x2ac2) + 0x01e0);
    si = 0x2976;
    cx = 0x0c05;
    sub_19fcd();
loc_19e8d:
    di = memoryAGet16(ds, 0x2ac2);
    memoryASet16(ds, 0x2ac2, memoryAGet16(ds, 0x2ac2) + 0x0140);
    si = 0x29ee;
    cx = 0x0804;
    sub_19fcd();
    memoryASet(ds, 0x2ac4, memoryAGet(ds, 0x2ac4) - 1);
    if (memoryAGet(ds, 0x2ac4) != 0)
        goto loc_19e8d;
    di = memoryAGet16(ds, 0x2ac2);
    si = 0x2a2e;
    cx = 0x0b04;
    sub_19fcd();
}
void sub_19eb4()
{
    bx = memoryAGet16(ds, 0x0008);
    bl = memoryAGet(ds, bx + 10930);
loc_19ebc:
    memoryASet16(ds, 0x2ac5, bx);
    di = memoryAGet16(ds, bx + 10886);
    if (di != 0x0000)
        goto loc_19eca;
    return;
loc_19eca:
    sub_19e6d();
    bx = memoryAGet16(ds, 0x2ac5);
    bx += 0x0002;
    goto loc_19ebc;
}
void sub_19ee0()
{
    al = dl;
    ah = 0x28;
    mul(ah);
    if (!(dl & 0x01))
        goto loc_19eee;
    ax += 0x1fd8;
loc_19eee:
    dx = cx;
    dx >>= 1;
    dx >>= 1;
    ax += dx;
    cl &= 0x03;
    cl <<= 1;
}
void sub_19f65()
{
    flags.direction = false;
    memoryASet(ds, 0x2ae0, cl);
    memoryASet(ds, 0x2ae2, ch);
    ch = 0;
loc_19f70:
    cl = memoryAGet(ds, 0x2ae0);
loc_19f74:
    bx = memoryAGet16(es, di);
    memoryASet16(ds, bp, bx);
    lodsw<MemAuto, DirAuto>();
    ax &= bx;
    stosw<MemAuto, DirAuto>();
    bp += 0x0002;
    if (--cx)
        goto loc_19f74;
    di -= memoryAGet16(ds, 0x2ae0);
    di -= memoryAGet16(ds, 0x2ae0);
    di ^= 0x2000;
    if (di & 0x2000)
        goto loc_19f99;
    di += 0x0050;
loc_19f99:
    memoryASet(ds, 0x2ae2, memoryAGet(ds, 0x2ae2) - 1);
    if (memoryAGet(ds, 0x2ae2) != 0)
        goto loc_19f70;
}
void sub_19fa0()
{
    flags.direction = false;
    memoryASet16(ds, 0x2ae9, si);
    memoryASet(ds, 0x2ae0, cl);
    memoryASet(ds, 0x2ae2, ch);
    al <<= 1;
    memoryASet(ds, 0x2aeb, al);
    ch = 0;
loc_19fb4:
    cl = memoryAGet(ds, 0x2ae0);
    rep_movsw<MemAuto, MemAuto, DirAuto>();
    cl = memoryAGet(ds, 0x2aeb);
    memoryASet16(ds, 0x2ae9, memoryAGet16(ds, 0x2ae9) + cx);
    si = memoryAGet16(ds, 0x2ae9);
    memoryASet(ds, 0x2ae2, memoryAGet(ds, 0x2ae2) - 1);
    if (memoryAGet(ds, 0x2ae2) != 0)
        goto loc_19fb4;
}
void sub_19fcd()
{
    flags.direction = false;
    memoryASet(ds, 0x2ae0, cl);
    memoryASet(ds, 0x2ae2, ch);
    ch = 0;
loc_19fd8:
    cl = memoryAGet(ds, 0x2ae0);
    rep_movsw<MemAuto, MemAuto, DirAuto>();
    di -= memoryAGet16(ds, 0x2ae0);
    di -= memoryAGet16(ds, 0x2ae0);
    di ^= 0x2000;
    if (di & 0x2000)
        goto loc_19ff3;
    di += 0x0050;
loc_19ff3:
    memoryASet(ds, 0x2ae2, memoryAGet(ds, 0x2ae2) - 1);
    if (memoryAGet(ds, 0x2ae2) != 0)
        goto loc_19fd8;
}
void sub_19ffa()
{
    flags.direction = false;
    memoryASet(es, 0x2ae0, cl);
    memoryASet(es, 0x2ae2, ch);
    ch = 0;
loc_1a007:
    cl = memoryAGet(es, 0x2ae0);
    rep_movsw<MemAuto, MemAuto, DirAuto>();
    si -= memoryAGet16(es, 0x2ae0);
    si -= memoryAGet16(es, 0x2ae0);
    si ^= 0x2000;
    if (si & 0x2000)
        goto loc_1a025;
    si += 0x0050;
loc_1a025:
    memoryASet(es, 0x2ae2, memoryAGet(es, 0x2ae2) - 1);
    if (memoryAGet(es, 0x2ae2) != 0)
        goto loc_1a007;
}
void sub_1a02d()
{
    dx = memoryAGet16(ds, 0x2ae5);
    dl ^= dh;
    dl >>= 1;
    flags.carry = dl & 1;
    dl >>= 1;
    memoryASet16(ds, 0x2ae5, rcr(memoryAGet16(ds, 0x2ae5), 0x0001));
    dx = memoryAGet16(ds, 0x2ae5);
}
void sub_1a040()
{
    al = 0x00;
    out(0x43, al);
    in(al, 0x40);
    ah = al;
    in(al, 0x40);
    if (ax != 0x0000)
        goto loc_1a055;
    ax = 0xfa59;
loc_1a055:
    memoryASet16(ds, 0x2ae5, ax);
}
void sub_1c5e0()
{
    if (memoryAGet(ds, 0x0000) == 0x00)
        return;
    ah = 0;
    interrupt(0x1a);
    if (dx == memoryAGet16(ds, 0x5322))
        return;
    memoryASet16(ds, 0x5322, dx);
    bx = memoryAGet16(ds, 0x5320);
    bl = memoryAGet(ds, bx + 21388);
    if (bl == 0x66)
        goto loc_1c60d;
    bh = 0;
    memoryASet16(ds, 0x5320, memoryAGet16(ds, 0x5320) + 1);
    if (bx != 0x0000)
        goto loc_1c611;
loc_1c60d:
    sub_1cd51();
    return;
loc_1c611:
    al = 0xb6;
    out(0x43, al);
    ax = memoryAGet16(ds, bx + 21284);
    out(0x42, al);
    al = ah;
    out(0x42, al);
    in(al, 0x61);
    al |= 0x03;
    out(0x61, al);
}
void sub_1c680()
{
    memoryASet(ds, 0x5b0f, 0x0c);
    memoryASet16(ds, 0x5b0c, 0x0001);
    memoryASet16(ds, 0x5b12, 0x01ff);
    memoryASet16(ds, 0x5b0a, 0x000f);
    memoryASet(ds, 0x5b0e, 0x01);
}
void sub_1cd51()
{
    in(al, 0x61);
    al &= 0xfc;
    out(0x61, al);
}
void sub_1ce90()
{
    interrupt(0x11);
    al &= 0x30;
    if (al != 0x30)
        goto loc_1cec5;
    ax = 0xb800;
    ds = ax;
    ax = 0x55aa;
    memoryASet16(ds, 0x0000, ax);
    ax = memoryAGet16(ds, 0x0000);
    if (ax != 0x55aa)
        goto loc_1cec6;
    si = 0x60f0;
    assert(0); //sub_1cece();
    ax = 0x0040;
    ds = ax;
    ax = memoryAGet16(ds, 0x0010);
    al &= 0xcf;
    al |= 0x10;
    memoryASet16(ds, 0x0010, ax);
    ax = 0x0004;
    interrupt(0x10);
loc_1cec5:
    return;
loc_1cec6:
    si = 0x6112;
    assert(0); //sub_1cece();
loc_1cecc:
    goto loc_1cecc;
}
bool sub_1cee0()
{
    flags.direction = false;
    memoryASet16(ds, 0x0004, 0x0000);
    sub_18a60();
    sub_19560();
    sub_19c60();
    ax = 0xb800;
    es = ax;
    si = 0x6152;
    cx = 0x1d0b;
    di = 0x00bd;
    sub_19fcd();
    si = 0x63d0;
    cx = 0x160e;
    di = 0x069e;
    sub_19fcd();
    si = 0x6638;
    cx = 0x0c03;
    di = 0x0a78;
    sub_19fcd();
    si = 0x6680;
    cx = 0x080e;
    di = 0x0ca8;
    sub_19fcd();
    si = 0x6760;
    cx = 0x0b0c;
    di = 0x1d6e;
    sub_19fcd();
    si = 0x6868;
    cx = 0x0804;
    di = 0x1dec;
    sub_19fcd();
    memoryASet16(ds, 0x6a8d, 0x0000);
    sub_1d06b();
    memoryASet16(ds, 0x0579, 0x0000);
    sub_1793d();
    memoryASet(ds, 0x057b, 0x60);
    memoryASet(ds, 0x057c, 0x92);
    sub_19922();
    sub_1992c();
    memoryASet(ds, 0x1f80, 0x09);
    memoryASet(ds, 0x1f81, 0xff);
    sub_198e3();
    sub_19070();
    memoryASet(ds, 0x0698, 0x00);
    memoryASet(ds, 0x0699, 0x00);
    memoryASet(ds, 0x6a8a, 0x00);
    ax = memoryAGet16(ds, 0x0693);
    memoryASet16(ds, 0x6150, ax);
loc_1cf84:
    ah = 0;
    interrupt(0x1a);
    memoryASet16(ds, 0x6a8b, dx);
    memoryASet16(ds, 0x5322, dx);
    memoryASet16(ds, 0x6a93, dx);
    dx -= 0x0030;
    memoryASet16(ds, 0x6a88, dx);
    memoryASet16(ds, 0x5320, 0x0000);
loc_1cfa1:
    if (sync())
        return false;
    ah = 0;
    interrupt(0x1a);
    ax = dx;
    ax -= memoryAGet16(ds, 0x6a93);
    if (ax < 0x0024)
        goto loc_1cfb9;
    memoryASet16(ds, 0x6a93, dx);
    push(dx);
    sub_1d06b();
    dx = pop();
loc_1cfb9:
    dx -= memoryAGet16(ds, 0x6a8b);
    ax = memoryAGet16(ds, 0x56da);
    if (memoryAGet(ds, 0x041a) == 0x00)
        goto loc_1cfd0;
    ax += 0x0048;
    if (dx >= ax)
        goto loc_1cf84;
    goto loc_1cfd7;
loc_1cfd0:
    ax += 0x0006;
//    if (dx > ax) // gabo infinite cat
//        return;
loc_1cfd7:
    sub_1c5e0();
    sub_1d004();
    if (memoryAGet(ds, 0x069b) == 0x00)
        goto loc_1cffa;
    dx = 0x0201;
    in(al, dx);
    al &= 0x10;
    if (al == 0)
        goto loc_1cff3;
    memoryASet(ds, 0x6a8a, 0x01);
    goto loc_1cffa;
loc_1cff3:
    if (memoryAGet(ds, 0x6a8a) != 0x00)
        return true;
loc_1cffa:
    ax = memoryAGet16(ds, 0x6150);
    if (ax == memoryAGet16(ds, 0x0693))
        goto loc_1cfa1;
    return true;
}
void sub_1d004()
{
    if (memoryAGet16(ds, 0x0579) > 0x0020)
        goto loc_1d012;
    memoryASet(ds, 0x0698, 0x01);
    goto loc_1d04c;
loc_1d012:
    if (memoryAGet16(ds, 0x0579) < 0x0120)
        goto loc_1d021;
    memoryASet(ds, 0x0698, 0xff);
    goto loc_1d04c;
loc_1d021:
    ah = 0;
    interrupt(0x1a);
    ax = dx;
    ax -= memoryAGet16(ds, 0x6a88);
    if (ax < 0x0012)
        goto loc_1d04c;
    memoryASet16(ds, 0x6a88, dx);
    sub_1a02d();
    memoryASet(ds, 0x0698, 0x00);
    if (dl > 0xa0)
        goto loc_1d04c;
    dl &= 0x01;
    if (dl != 0)
        goto loc_1d048;
    dl = 0xff;
loc_1d048:
    memoryASet(ds, 0x0698, dl);
loc_1d04c:
    sub_18608();
    if (flags.zero)
        return;
    memoryASet16(ds, 0x0572, 0x0004);
    sub_17b15();
}
void sub_1d06b()
{
    ax = 0xb800;
    es = ax;
    memoryASet16(ds, 0x6a8d, memoryAGet16(ds, 0x6a8d) + 0x0002);
    bx = memoryAGet16(ds, 0x6a8d);
    bx &= 0x0002;
    si = memoryAGet16(ds, bx + 27279);
    cx = 0x0c0a;
    di = 0x1d38;
    sub_19fcd();
}
void sub_18299()
{
    memoryASet16(ds, 0x0572, 0x0002);
    memoryASet(ds, 0x0577, 0x08);
    if (memoryAGet16(ds, 0x0561) != 0x0c02)
        goto loc_182b7;
    memoryASet(ds, 0x056d, memoryAGet(ds, 0x056d) + 1);
    if (memoryAGet(ds, 0x056d) & 0x07)
        return;
loc_182b7:
    sub_18413();
    sub_18daa();
    if (flags.carry)
        return;
    sub_19325();
    if (flags.carry)
        return;
    sub_1a02d();
    bl = dl;
    bx &= 0x000e;
    si = memoryAGet16(ds, bx + 3986);
    ax = 0xb800;
    es = ax;
    di = memoryAGet16(ds, 0x055f);
    bp = 0x05fa;
    memoryASet16(ds, 0x0561, 0x0c02);
    cx = 0x0602;
    sub_19f65();
    sub_1a02d();
    bl = dl;
    bx &= 0x0006;
    si = memoryAGet16(ds, bx + 4002);
    di = memoryAGet16(ds, 0x055f);
    di += 0x00f0;
    bp = 0x0612;
    cx = 0x0602;
    sub_19f65();
    memoryASet(ds, 0x0583, 0x00);
}
