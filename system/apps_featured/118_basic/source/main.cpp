#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include "../../../os_host/source/framework/BufferedIo.h"

#include "../../../apps/120_ps2keyboard/keyboard.h"

#include "file/shapes.h"
#include "file/layout.h"
#include "file/file.h"
#include "font8x8.h"

#include <Arduino.h>
#include "terminal-basic/basic_interpreter.hpp"
#include "terminal-basic/version.h"

#include "terminal-basic/basic_gfx.hpp"
static BASIC::GFXModule gfxModule;

uint16_t colors[] = {
    RGB565(000000), RGB565(0000b0), RGB565(00b000), RGB565(00b0b0),
    RGB565(b00000), RGB565(b00b00), RGB565(b08000), RGB565(b0b0b0),
    RGB565(808080), RGB565(0000ff), RGB565(00ff00), RGB565(00ffff),
    RGB565(ff0000), RGB565(ff00ff), RGB565(ffff00), RGB565(ffffff),
};

void BASIC::GFXModule::_init()
{
}


bool BASIC::GFXModule::command_boxc(Interpreter&) { _ASSERT(0); return false; }
    bool BASIC::GFXModule::command_box(Interpreter&){ _ASSERT(0); return false; }
    bool BASIC::GFXModule::command_circlec(Interpreter&) { _ASSERT(0); return false; }
    bool BASIC::GFXModule::command_circle(Interpreter& i) {
        INT x,y,r, c;
        /*
        if (i.getToken() == Token::COMMA)
        {
            Value v3;
            _lexer.getNext();
            fExpression(v3);
        }*/

        if (!getIntegerFromStack(i, c))
            return false;
        if (!getIntegerFromStack(i, r))
            return false;
        if (!getIntegerFromStack(i, y))
            return false;
        if (!getIntegerFromStack(i, x))
            return false;
        
        c = colors[c];

        const auto drawCircle = [](int xc, int yc, int x, int y, int c)
        {
            BIOS::LCD::PutPixel(xc+x, yc+y, c);
            BIOS::LCD::PutPixel(xc-x, yc+y, c);
            BIOS::LCD::PutPixel(xc+x, yc-y, c);
            BIOS::LCD::PutPixel(xc-x, yc-y, c);
            BIOS::LCD::PutPixel(xc+y, yc+x, c);
            BIOS::LCD::PutPixel(xc-y, yc+x, c);
            BIOS::LCD::PutPixel(xc+y, yc-x, c);
            BIOS::LCD::PutPixel(xc-y, yc-x, c);
        };
         
        // Function for circle-generation
        // using Bresenham's algorithm
        const auto circleBres = [&](int xc, int yc, int r, int c)
        {
            int x = 0, y = r;
            int d = 3 - 2 * r;
            drawCircle(xc, yc, x, y, c);
            while (y >= x)
            {
                x++;
                if (d > 0)
                {
                    y--;
                    d = d + 4 * (x - y) + 10;
                }
                else
                    d = d + 4 * x + 6;
                drawCircle(xc, yc, x, y, c);
            }
        };
        circleBres(x, y, r, c);
        return true;
    }
    bool BASIC::GFXModule::command_color(Interpreter& i) {
        INT f, b;
        if (!getIntegerFromStack(i, b))
            return false;
        if (!getIntegerFromStack(i, f))
            return false;
        _ASSERT(f>=0 && f<=15);
        _ASSERT(b>=0 && b<=15);
        CONSOLE::colorFront = colors[f];
        CONSOLE::colorBack = colors[b];
        return true;
    }
    bool BASIC::GFXModule::command_cursor(Interpreter&) { _ASSERT(0); return false; }
    bool BASIC::GFXModule::command_ellipsec(Interpreter&) { _ASSERT(0); return false; }
    bool BASIC::GFXModule::command_ellipse(Interpreter&) { _ASSERT(0); return false; }
    bool BASIC::GFXModule::command_linec(Interpreter&) { _ASSERT(0); return false; }
    bool BASIC::GFXModule::command_lineto(Interpreter&) { _ASSERT(0); return false; }
    bool BASIC::GFXModule::command_line(Interpreter& i) {
        const auto& line = [](int x, int y, int x2, int y2, int c)
        {
            int dx,dy,p;
            dx=x2-x;
            dy=y2-y;
            p = 2 * (dy) - (dx);
            while(x <= x2)
            {
                BIOS::LCD::PutPixel(x, y, c);
                if(p < 0)
                {
                    x=x+1;
                    y=y;
                    p = p + 2 * (dy);
                }
                else
                {
                    x=x+1;
                    y=y+1;
                    p = p + 2 * (dy - dx);
                }
            }
        };
        int args[6] = {0};
        int index=5;
        Parser::Value v(Integer(0));
        while (i.popValue(v) && index >= 0)
            args[index--] = Integer(v);

        index++;
        bool block = index == 0 && args[index+5] == 111;
        bool blockfill = index == 0 && args[index+5] == 222;
        CRect rc(args[index], args[index+1], args[index+2], args[index+3]);
        if (rc.left > rc.right)
        {
            int t = rc.left;
            rc.left = rc.right;
            rc.right = t;
        }
        if (blockfill)
        {
            if (rc.Height() > 0 && rc.Width() > 0)
                BIOS::LCD::Bar(rc, colors[args[index+4]]);
        }
        else if (block)
        {
            if (rc.Height() > 0 && rc.Width() > 0)
                BIOS::LCD::Rectangle(rc, colors[args[index+4]]);
        }
        else
            line(args[index], args[index+1], args[index+2], args[index+3], colors[args[index+4]]);
        return true;
        
    }
    bool BASIC::GFXModule::command_pointc(Interpreter&) { _ASSERT(0); return false; }
    bool BASIC::GFXModule::command_point(Interpreter&) { _ASSERT(0); return false; }
    bool BASIC::GFXModule::command_screen(Interpreter& i) {
        INT a, b;
        if (!getIntegerFromStack(i, b))
            return false;
        if (!getIntegerFromStack(i, a))
            return false;
        BIOS::LCD::Bar(CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), RGB565(000000));
        return true;
        
    }
    bool BASIC::GFXModule::command_play(Interpreter& i) {
        const char *str;
        if (!i.popString(str))
            return false;
        // TODO: play str
        return true;
    }
    bool BASIC::GFXModule::command_sound(Interpreter& i)
    {
        INT freq, dur;
        if (!getIntegerFromStack(i, dur))
            return false;
        if (!getIntegerFromStack(i, freq))
            return false;
        return true;
    }
    bool BASIC::GFXModule::command_paint(Interpreter& i)
    {
        
        INT x, y, border, color;
        if (!getIntegerFromStack(i, border))
            return false;
        if (!getIntegerFromStack(i, color))
            return false;
        if (!getIntegerFromStack(i, y))
            return false;
        if (!getIntegerFromStack(i, x))
            return false;
        typedef void(*_paint_t)(int, int, int, int);
        static _paint_t _paint = [](int x, int y, int c, int o)
        {
            if (BIOS::LCD::GetPixel(x, y) != o)
                return;
            BIOS::LCD::PutPixel(x, y, c);
            _paint(x-1, y, c, o);
            _paint(x+1, y, c, o);
            _paint(x, y-1, c, o);
            _paint(x, y+1, c, o);
        };
        int p = BIOS::LCD::GetPixel(x, y);
        _paint(x, y, color, p);
        return true;
    }

const char* startup = nullptr;
#if 0
const char* startup1  = R"-(
940 REM The IBM Personal Computer Music
950 REM Version 1.10 (C)Copyright IBM Corp 1981, 1982
960 REM Licensed Material - Program Property of IBM
975 REM DEF SEG: POKE 106,0
980 SAMPLES$ = "NO"
990 GOTO 1010
1000 SAMPLES$ = "YES"
1010 SCREEN 0,1:COLOR 15,0,0:CLS:LOCATE 5,19,0:PRINT "IBM"
1020 LOCATE 7,12:PRINT "Personal Computer"
1030 COLOR 10,0:LOCATE 10,9:PRINT CHR$(213)+STRING$(21,205)+CHR$(184)
1040 LOCATE 11,9:PRINT CHR$(179)+"        MUSIC        "+CHR$(179)
1050 LOCATE 12,9:PRINT CHR$(179)+STRING$(21,32)+CHR$(179)
1060 LOCATE 13,9:PRINT CHR$(179)+"    Version 1.10     "+CHR$(179)
1070 LOCATE 14,9:PRINT CHR$(212)+STRING$(21,205)+CHR$(190)
1080 COLOR 15,0:LOCATE 17,4:PRINT "(C) Copyright IBM Corp 1981, 1982"
1090 COLOR 14,0:LOCATE 23,7:PRINT "Press space bar to continue"
1150 SCREEN 0,1:COLOR 15,1:CLS:COLOR 15,1
1160 LOCATE 15,7:PRINT " ------- selections -------"
1170 LOCATE 16,7:PRINT " A-MARCH  E-HUMOR  I-SAKURA"
1180 LOCATE 17,7:PRINT " B-STARS  F-BUG    J-BLUE  "
1190 LOCATE 18,7:PRINT " C-FORTY  G-POP    K-SCALES"
1191 LOCATE 19,7:PRINT " D-HAT    H-DANDY  ESC KEY-EXIT"
1200 COLOR 15,0
1210 FOR I=0 TO 15:FOR J=0 TO 8
1220 LOCATE 5+J,5+I*2:PRINT CHR$(219);CHR$(221);
1230 NEXT J:NEXT I
1240 FOR I=0 TO 12:FOR J=0 TO 4
1250 IF I=2 OR I=6 OR I=9 OR I=13 THEN 1270
1260 LOCATE 5+J,8+I*2:PRINT CHR$(32);CHR$(222);
1270 NEXT J:NEXT I
1280 FOR J=0 TO 9
1290 LOCATE 4+J,4:COLOR 4,0:PRINT CHR$(221);:LOCATE 4+J,36:COLOR 15,0
1300 PRINT CHR$(221);:COLOR 4,1:PRINT CHR$(221);
1310 NEXT J
1320 COLOR 4,1:LOCATE 4,4
1330 FOR I=0 TO 32:PRINT CHR$(219);:NEXT I
1340 PRINT CHR$(221);:LOCATE 13,4
1350 FOR I=0 TO 32:PRINT CHR$(219);:NEXT I
1360 PRINT CHR$(221);:COLOR 0,7:DIM M(88),O(70)
1370 FOR I=7 TO 88:M(I) =  36.8*(2^(1/12))^(I-6):NEXT I
1380 FOR I=0 TO 6:M(I) = 32767:NEXT I
1390 O(0) = 0
1400 O(39)=5:O(40)=7:O(41)=8:O(42)=9
1410 O(43)=10:O(44)=11:O(45)=13:O(46)=14
1420 O(47)=15:O(48)=16:O(49)=17:O(50)=18
1430 O(51)=19:O(52)=21:O(53)=22:O(54)=23
1440 O(55)=24:O(56)=25:O(57)=27:O(58)=28
1450 O(59)=29:O(60)=30:O(61)=31:O(62)=32
1460 O(63)=33:O(64)=35:O(65)=36:O(66)=37
1470 O(67)=38:O(68)=39:O(69)=40:O(70)=42
1480 GOTO 1630
1490 READ J,K
1500 CMD$ = INKEY$:IF CMD$="" THEN 1540
1510 IF CMD$=CHR$(27) THEN RETURN
1520 POKE 106,0
1530 REM
1540 IF J = -1  THEN RETURN
1550 Q = O(J)
1560 IF J>64 OR J<39 THEN 1590
1570 REM IF SCREEN(5,Q)<>32 THEN COLOR 0,7:LOCATE 11,Q:PRINT CHR$(14);:COLOR 15,0:GOTO 1590
1571 COLOR 0,7:LOCATE 11,Q:PRINT CHR$(14);:COLOR 15,0:GOTO 1590
1580 COLOR 15,0:LOCATE 7,Q:PRINT CHR$(14);:COLOR 0,7
1590 SOUND M(J),K:IF J=0 AND K=1 THEN 1600:'SKIP NEXT FOR STACCATTO
1595 SOUND 32767,1
1600 IF J>64 OR J<39 THEN 1490
1620 COLOR 15,0:LOCATE 11,Q:PRINT CHR$(219);:GOTO 1490
1630 LOCATE 21,5:PRINT "                                ";
1640 LOCATE 21,5:PRINT "ENTER SELECTION ==>";
1650 IF INKEY$ <> "" THEN 1650
1660 CMD$=INKEY$:IF CMD$="" THEN 1660
1670 IF CMD$=CHR$(27)  THEN 1850
1680 IF CMD$="A" OR CMD$="a" THEN S$= "MARCH ":RESTORE 4000:GOTO 1770
1690 IF CMD$="B" OR CMD$="b" THEN S$= "STARS ":RESTORE 4100:GOTO 1770
1700 IF CMD$="C" OR CMD$="c" THEN S$= "FORTY ":RESTORE 3700:GOTO 1770
1710 IF CMD$="D" OR CMD$="d" THEN S$= "HAT   ":RESTORE 4300:GOTO 1770
1720 IF CMD$="E" OR CMD$="e" THEN S$= "HUMOR ":RESTORE 3500:GOTO 1770
1730 IF CMD$="F" OR CMD$="f" THEN S$= "BUG   ":RESTORE 3200:GOTO 1770
1740 IF CMD$="G" OR CMD$="g" THEN S$= "POP   ":RESTORE 3600:GOTO 1770
1750 IF CMD$="H" OR CMD$="h" THEN S$= "DANDY ":RESTORE 3900:GOTO 1770
1755 IF CMD$="I" OR CMD$="i" THEN S$= "SAKURA":RESTORE 4500:GOTO 1770
1757 IF CMD$="J" OR CMD$="j" THEN S$= "BLUE  ":RESTORE 3300:GOTO 1770
1761 IF CMD$="K" OR CMD$="k" THEN S$= "SCALES":RESTORE 4400:GOTO 1770
1769 GOTO 1640
1770 PRINT " ";CMD$;"-";S$
1780 READ D
1800 READ S$:LOCATE 23,1+(40.5-LEN(S$))/2
1805 COLOR 15,4:PRINT S$;:COLOR 0,7
1810 GOSUB 1490
1820 S$=STRING$(39," ")
1821 LOCATE 23,1
1823 COLOR 4,1:PRINT S$:COLOR 0,7
1830 GOTO 1630
1840 END
1850 IF SAMPLES$="YES" THEN CHAIN "SAMPLES",1000
1860 SCREEN 0,1:COLOR 7,0,0:CLS:END
3000 REM The IBM Personal Computer Music Scroll
3010 REM Version 1.00 (C)Copyright IBM Corp 1981
3020 REM Licensed Material - Program Property of IBM
3200 DATA -2,"La Cucaracha - Mexican Folk Song"
3210 DATA 42,1,0,1,42,1,0,1,42,1,0,1,47,1,0,5,51,1,0,3,42,1,0,1,42,1,0,1
3220 DATA 42,1,0,1,47,1,0,5,51,1,0,5,30,1,0,1,30,1,0,1,35,1,0,3,47,1,0,1
3230 DATA 47,1,0,1,46,1,0,1,46,1,0,1,44,1,0,1,44,1,0,1,42,8,0,2,42,1,0,1
3240 DATA 42,1,0,1,42,1,0,1,46,1,0,5,49,1,0,3,42,1,0,1,42,1,0,1,42,1,0,1
3250 DATA 46,1,0,5,49,1,0,5,37,1,0,1,37,1,0,1,30,1,0,3,54,2,56,2,54,2,52,2
3260 DATA 51,2,49,2,47,8
3270 DATA -1,-1
3300 DATA -2,"Blue Danube Waltz by J.S.Strauss"
3310 DATA 42,4,46,4,49,4,49,4,0,4,61,2,0,2,61,2,0,6,58,2,0,2,58,2,0,6,42,4,42,4
3320 DATA 46,4,49,4
3330 DATA 49,4,0,4,61,2,0,2,61,2,0,6,59,2,0,2,59,2,0,6,41,4,41,4,44,4,51,4,51,4
3340 DATA 0,4,63,2,0,2,63,2,0,6,59,2,0,2
3350 DATA 59,2,0,6,41,4,41,4,44,4,51,4,51,4,0,4,63,2,0,2,63,2,0,6,58,2,0,2,58,2
3360 DATA 0,6,42,4
3370 DATA 42,4,46,4,49,4,54,4,0,4,66,2,0,2,66,2,0,6,61,2,0,2,61,2,0,6,42,4
3380 DATA 42,4,46,4,49,4,54,4,0,4,66,2,0,2
3390 DATA 66,2,0,6,63,2,0,2,63,2,0,6,44,4,44,4,47,4,51,2,0,2,51,14,0,2,48,4
3400 DATA 49,4,58,16
3410 DATA 54,4,46,4,46,8,44,4,51,8,49,4,42,4,0,2,42,2,42,4,0,8,49,2,0,2,47,2
3420 DATA 0,6,49,2,0,2
3430 DATA 47,2,0,6,49,4,58,16,56,4,49,2,0,2,46,2,0,6,49,2,0,2,46,2,0,6,49,4
3440 DATA 56,16,54,4,49,2,0,2,47,2,0,6,49,2,0,2,47,2,0,6,49,4,58,16
3450 DATA 56,4,49,4,54,4,56,4,58,4,61,8,59,4,58,2,58,2,58,4,56,2,0,2,54,4,0,8
3460 DATA -1,-1
3500 DATA -2,"Humoresque by Dvorak"
3510 DATA 47,3,0,2,49,1,47,3,0,2,49,1,51,3,0,2,54,1,56,3,0,2,54,1
3520 DATA 59,3,0,2,58,1,61,3,0,2,59,1,58,3,0,2,61,1,59,3,0,2,56,1
3530 DATA 54,3,0,2,54,1,56,3,0,2,54,1,59,3,0,2,56,1,54,3,0,2,51,1
3540 DATA 49,24,47,3,0,2,49,1,47,3,0,2,49,1,51,3,0,2,54,1,56,3,0,2,54,1
3550 DATA 56,3,0,2,58,1,61,3,0,2,59,1,58,3,0,2,61,1,59,3,0,2,56,1
3560 DATA 54,3,0,2,54,1,59,3,0,2,47,1,49,6,54,6,47,18
3570 DATA -1,-1
3600 DATA -2,"Pop! Goes the Weasle - Anonymous"
3610 DATA 47,2,0,2,47,2,49,2,0,2,49,2,51,2,54,2,51,2,47,2,0,2,42,2
3620 DATA 47,2,0,2,47,2,49,2,0,2,49,2,51,6,47,2,0,2,42,2,47,2,0,2,47,2,49,2
3630 DATA 0,2,49,2,51,2,54,2,51,2,47,2,0,4,56,2,0,4,49,2,0,2,52,2,51,6,47,2
3640 DATA 0,4,59,2,0,2,59,2,56,2,0,2,59,2,58,2,61,2,58,2,54,2,0,4,59,2,0,2
3650 DATA 59,2,56,2,0,2,59,2,58,6,54,2,0,2,51,2,52,2,0,2,51,2,52,2,0,2,54,2
3660 DATA 56,2,0,2,58,2,59,2,0,4,56,2,0,4,49,2,0,2,52,2,51,6,47,2
3670 DATA -1,-1
RUN
)-";
const char* startup3  = R"-(
900 BF = 222: B = 111
901 LINE (1,1)-(320,200),2,BF
940 REM The IBM Personal Computer Art
950 REM Version 1.10 (C)Copyright IBM Corp 1981, 1982
960 REM Licensed Material - Program Property of IBM
970 REM Author - Glenn Stuart Dardick
980 SAMPLES$ = "NO"
990 GOTO 1010
1000 SAMPLES$ = "YES"
1010 SCREEN 0,1:COLOR 15,0,0:CLS:LOCATE 5,19:PRINT "IBM"
1020 LOCATE 7,12,0:PRINT "Personal Computer"
1030 COLOR 10,0:LOCATE 10,9,0:PRINT CHR$(213)+STRING$(21,205)+CHR$(184)
1040 LOCATE 11,9,0:PRINT CHR$(179)+"         ART         "+CHR$(179)
1050 LOCATE 12,9,0:PRINT CHR$(179)+STRING$(21,32)+CHR$(179)
1060 LOCATE 13,9,0:PRINT CHR$(179)+"    Version 1.10     "+CHR$(179)
1070 LOCATE 14,9,0:PRINT CHR$(212)+STRING$(21,205)+CHR$(190)
1080 COLOR 15,0:LOCATE 17,4,0:PRINT "(C) Copyright IBM Corp 1981, 1982"
1090 COLOR 14,0:LOCATE 23,7,0:PRINT "Press space bar to continue"
1100 IF INKEY$<>"" THEN GOTO 1100
1110 CMD$ = INKEY$
1120 IF CMD$ = "" THEN GOTO 1110
1130 IF CMD$ = CHR$(27) THEN GOTO 1530
1140 IF CMD$ = " " THEN GOTO 1160
1150 GOTO 1110
1160 GOTO 1300
1180 WIDTH 80:CLS:LOCATE 3,1
1190 PRINT "HOLD IT!"
1200 PRINT "YOU'RE NOT USING THE COLOR/GRAPHICS MONITOR ADAPTER!"
1210 PRINT "THIS PROGRAM USES GRAPHICS AND REQUIRES THAT ADAPTER."
1220 PRINT "PRESS THE SPACE BAR TO CONTINUE."
1230 DEF SEG
1240 IF INKEY$<>"" THEN GOTO 1240
1250 CMD$ = INKEY$
1260 IF CMD$ = "" THEN GOTO 1250
1270 IF CMD$ = CHR$(27) THEN GOTO 1530
1280 IF CMD$ = " " THEN GOTO 1530
1290 GOTO 1250
1300 REM
1301 REM
1302 PLAY "mf"
1303 GOTO 1305
1304 RESUME 1305
1305 REM
1310 SCREEN 1,0:COLOR 8,0:CLS
1320 LINE (1,1)-(320,200),2,BF
1330 LINE (30,30)-(290,170),0,BF
1340 LINE (30,30)-(1,1),1
1350 LINE (290,30)-(320,1),1
1360 LINE (30,170)-(1,200),1
1370 LINE (290,170)-(320,200),1
1380 LINE (100,179)-(216,193),0,BF
1390 LOCATE 24,16
1391 PRINT " The City ";
1392 LOCATE 2,10
1400 PRINT "press ESC key to exit";
1410 IX1=RND*250+35
1420 IX2=RND*250+35
1430 IX2 = (IX1-IX2)/3 + IX2
1440 IY1=RND*110+55
1450 IY2=165
1460 C = RND*15+1:LINE (IX1,IY1)-(IX2,IY2),C,BF
1470 LINE (IX1,IY1)-(IX2,IY2),8,B
1480 LINE (IX1+1,IY1+1)-(IX2-1,IY2-1),7,B
1490 IA = ABS((IX1-IX2)*(IY1-IY2))
1500 IS = (36400!-IA)/360 + 37
1510 SOUND IS,2
1520 K$=INKEY$
1522 IF K$ = "" THEN GOTO 1410
1524 IF K$ <> CHR$(27) THEN GOTO 1410
1530 SCREEN 0,1:IF SAMPLES$ = "YES" THEN CHAIN "SAMPLES",1000
1540 COLOR 7,0,0:CLS:END
RUN
)-";
#endif

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

class Output : public VT100::Print
{
public:
    Output() : VT100::Print() {}
    void clear() override {}
    
protected:
public:
    void _putch(char c)
    {
        if (c == '\n')
        {
            CONSOLE::cursor.x = CONSOLE::window.left;
            CONSOLE::cursor.y += 8;
            return;
        }
        if (CONSOLE::cursor.y+14 >= CONSOLE::window.bottom)
        {
            CONSOLE::cursor.y -= 14;
            CONSOLE::Scroll(CONSOLE::window, 14);
            BIOS::LCD::Bar(CONSOLE::window.left, CONSOLE::cursor.y, CONSOLE::window.right, CONSOLE::cursor.y + 14, CONSOLE::colorBack);
        }
        
        BIOS::LCD::BufferBegin(CRect(CONSOLE::cursor.x, CONSOLE::cursor.y,
                                     CONSOLE::cursor.x+8, CONSOLE::cursor.y+8));
        uint8_t uc = c;
        const uint8_t* base = font8x8 + uc*8+7;
        uint16_t buf[8];
        for (int y=0; y<8; y++)
        {
            for (int x=0; x<8; x++)
            {
                buf[x] = (base[-x] & (128>>y)) ? CONSOLE::colorFront : CONSOLE::colorBack;
            }
            BIOS::LCD::BufferWrite(buf, COUNT(buf));
        }
        BIOS::LCD::BufferEnd();
        CONSOLE::cursor.x += 8;
    }
    uint8_t getCursorX() override { _ASSERT(0); return 0; }
    void setCursor(uint8_t, uint8_t) override {_ASSERT(0);}
    void setCursorX(uint8_t) override {_ASSERT(0);}
    void writeChar(uint8_t c) override {_ASSERT(0);}
    
    virtual void addAttribute(VT100::TextAttr) override {assert(0);}
    virtual void resetAttributes() override {assert(0);}

    virtual size_t write(uint8_t c) override {
        static enum {WaitEsc, Begin, Value, Value2, Code} state = WaitEsc;
        static int value = 0, value2 = 0;
        if (state == WaitEsc)
        {
            if (c == 27)
            {
                state = Begin;
                return 0;
            } else
            {
                _putch(c);
                //CONSOLE::Putch(c);
                //CONSOLE::cursor.x -= 4;
            }
            return 0;
        }
        if (state == Begin)
        {
            if (c == '[')
            {
                value2 = -1;
                value = 0;
                state = Value;
                return 0;
            } else
                _ASSERT(0);
        }
        if (state == Value)
        {
            if (c >= '0' && c <= '9')
            {
                value *= 10;
                value += c - '0';
                return 0;
            }
            state = Code;
        }
        if (state == Value2)
        {
            if (c >= '0' && c <= '9')
            {
                value2 *= 10;
                value2 += c - '0';
                return 0;
            }
            state = Code;
        }
        if (state == Code)
        {
            switch (c)
            {
                case ';':
                    value2 = 0;
                    state = Value2;
                    return 0;
                case 'm':
                    CONSOLE::Color(value == 1 ? RGB565(ffff50) : RGB565(ffffff));
                    break;
                case 'C': // right
                    CONSOLE::cursor.x += value*8;
                    break;
                case 'D': // left
                    CONSOLE::cursor.x -= value*8;
                    CONSOLE::cursor.x = max(CONSOLE::cursor.x, CONSOLE::window.left);
                    break;
                case 'J': // 2J cls
                    BIOS::LCD::Bar(0, 0, BIOS::LCD::Width, BIOS::LCD::Height, CONSOLE::colorBack);
                    break;
                case 'H': // home?
                    CONSOLE::cursor = CONSOLE::window.TopLeft();
                    break;
                case 'f': // go to
                    if (value2 != -1)
                        CONSOLE::cursor.x = value2*8;
                    //if (value > 17)
                     //   value = 17;
                    CONSOLE::cursor.y = value*8;
                    break;
                default:
                    _ASSERT(0);
            }
            state = WaitEsc;
            return 0;
        }
        return 0;
    }

private:
    void scroll() {}
};

uint32_t HAL_random_generate(uint32_t max) { return rand() % max; }
void HAL_random_seed(uint32_t seed) { srand(seed); }
uint32_t HAL_time_gettime_ms() { return BIOS::SYS::GetTick(); }
void HAL_time_sleep_ms(uint32_t ms) {
    BIOS::SYS::DelayMs(ms); }
void HAL_update() { }

const char* replPreset[] = {"PRINT \"Hello!\"", "10 FOR I = 1 TO 10", "20 PRINT I, I*I",
    "30 NEXT I", "LIST", "PRINT RND()*1000", "RUN", "LOAD"};


class CTokenizer
{
  const char* mString;

public:
  CTokenizer(const char* str)
  {
    mString = str;
  }

  bool GetToken(char* out, int maxLength)
  {
      if (!*mString)
          return false;
      
    for (int i=0; i<maxLength; i++)
    {
      out[i] = mString[i];
      if (mString[i] == 0)
      {
        mString += i;
        return true;
      }
      if (mString[i] == '\n')
      {
        out[i] = 0;
        mString += i;
        mString++;
        return true;
      }
    }
      _ASSERT(0);
    mString += maxLength;
    out[maxLength] = 0;
      return true;
  }
};

class CBasic
{
    Stream _stream;
    Print _print;
    Output _output;
    bool mRunning{false};

public:
    BASIC::Interpreter mBasic;

    CBasic() : mBasic(_stream, _output, BASIC::SINGLE_PROGSIZE)
    {
    }
    void Init()
    {
        mBasic.addModule(&gfxModule);
        mBasic.newProgram();
        /*
        for(int i=128; i<256; i++)
        {
            CONSOLE::cursor.x = (i%16)*10;
            CONSOLE::cursor.y = (i/16)*10;
            _output._putch(i);
        }
        return;*/
        
        if (startup)
        {
            CONSOLE::Print("\n");
            mBasic._inputPosition = 0;
            CTokenizer token(startup);
            while (token.GetToken((char*)mBasic._inputBuffer, PROGSTRINGSIZE-1))
            {
                mBasic.exec();
            }
//            strcpy((char*)mBasic._inputBuffer, "RUN");
//            mBasic.exec();
//            strcpy(mBasic._program._text, startup);
//            mBasic._program._textEnd = mBasic._program._variablesEnd = mBasic._program._arraysEnd = strlen(startup);
        }
    }
    void SetBuffer(const char* text)
    {
        mBasic._inputPosition = 0;
        //mBasic._inputL strlen(text);
        strcpy((char*)mBasic._inputBuffer, text);
    }
    char* GetBuffer()
    {
        return (char*)mBasic._inputBuffer;
    }
    void SetLastChar(char c)
    {
        mBasic._inputBuffer[0] = c;
    }
    void Eval()
    {
        mBasic.exec();
    }
    void Step()
    {
        if (mBasic._state == BASIC::Interpreter::EXECUTE)
            mBasic.step();
    }
    bool IsRunning()
    {
        if (mBasic._state == BASIC::Interpreter::EXECUTE)
        {
            mRunning = true;
            return true;
        }
        return false;
    }
    bool FinishedRunning()
    {
        if (mBasic._state != BASIC::Interpreter::EXECUTE && mRunning)
        {
            mRunning = false;
            return true;
        }
        return false;
    }
};

class CRepl : public CWnd
{
    int mReplPresetIndex{0};
    CBasic mBasic;
    CFileDialog mFile;
    CFileFilterSuffix mFilter{".BAS"};
    CBufferedReader mReader;
    bool mBanner{true};
    
public:
    void Create()
    {
        CWnd::Create("main", CWnd::WsVisible | CWnd::WsTick,
                     CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
        
        Clear();
        
        mBasic.SetBuffer(replPreset[mReplPresetIndex]);
        ShowQuery();
        
        SetTimer(500);
        mBasic.Init();
    }
    
    void Clear()
    {
        APP::Init("BASIC Interpreter");
        CONSOLE::Clear();
        CONSOLE::Color(RGB565(b0b0b0));
        if (mBanner)
        {
            CONSOLE::Print("TerminalBasic %s\n", VERSION);
            CONSOLE::Color(RGB565(ffffff));
            CONSOLE::Print("READY\n");
            mBanner = false;
        }
        APP::Status("");
    }
    
    void PrintAnswer(const char* msg, size_t len)
    {
        CONSOLE::Color(RGB565(b0b0b0));
        for (int i=0; i<len; i++)
            if (msg[i] != '\r')
                CONSOLE::Putch(msg[i]);
    }
    
    void PrintError(const char* msg, size_t len)
    {
        CONSOLE::Color(RGB565(ffff00));
        for (int i=0; i<len; i++)
            if (msg[i] != '\r')
                CONSOLE::Putch(msg[i]);
    }
    
    void OnChar(char c)
    {
        char *buf = mBasic.GetBuffer();
        int len = strlen(buf);
        if (c == 13)
        {
            OnKey(BIOS::KEY::Enter);
            return;
        }
        if (c == 8)
        {
            if (len > 0)
            {
                buf[len-1] = 0;
                CONSOLE::Print("\x08 \x08");
            }
            return;
        } else
        {
            buf[len++] = c;
            buf[len] = 0;
            CONSOLE::Putch(c);
        }
    }
    
    virtual void OnKey(BIOS::KEY::EKey key)
    {
        if (key == BIOS::KEY::EKey::Up && mReplPresetIndex > 0)
        {
            mReplPresetIndex--;
            mBasic.SetBuffer(replPreset[mReplPresetIndex]);
            ShowQuery();
        }
        if (key == BIOS::KEY::EKey::Down && mReplPresetIndex < COUNT(replPreset)-1)
        {
            mReplPresetIndex++;
            mBasic.SetBuffer(replPreset[mReplPresetIndex]);
            ShowQuery();
        }
        if (key == BIOS::KEY::EKey::Enter)
        {
            ShowQuery(false);
            HideCursor();
            CONSOLE::Print("\n");
            if (strcmp(mBasic.GetBuffer(), "mini.loadScript()") == 0)
            {
                LoadScript();
            } else
            {
                CONSOLE::Color(RGB565(ffffff));
                mBasic.Eval();
            }
            if (!mBasic.IsRunning())
                ShowQuery();
        }
    }
     
    void RunScript(const char* text)
    {
        /*
        strcpy((char*)BIOS::FAT::GetSharedBuffer(), text);
        vstr_t line;
        line.alloc = BufferSectorSize;
        line.len = strlen(text);
        line.buf = (char*)BIOS::FAT::GetSharedBuffer();
        mReader.Close();
                                
        gc_reset();
        mPython.Eval(line, true);
         */
        _ASSERT(0);
    }

    void LoadScript()
    {
        if (mFile.ModalShow(nullptr, "Load python script", &mFilter))
        {
            CONSOLE::Clear();
            APP::Status(mFile.GetFilename());
            
            if (mReader.Open(mFile.GetFilename()))
            {
                /*
                vstr_t line;
                line.alloc = BufferSectorSize;
                line.len = mReader.GetFileSize();
                line.buf = (char*)mReader.GetBuffer();
                mReader.Close();
                                
                //gc_collect();
                usingKeys = false;
                gc_reset();
                mPython.Eval(line, true);
                
                if (!usingKeys)
                {
                    CONSOLE::Color(RGB565(aaffaa));
                    CONSOLE::Print("Press any key to continue...");
                    BIOS::KEY::EKey key;
                    while ((key = BIOS::KEY::GetKey()) == BIOS::KEY::None);
                    CONSOLE::Print("\n");
                } else {
                    Clear();
                }
                */
                _ASSERT(0);
            } else {
                Clear();
                const char* message = "cannot open file\n";
                PrintError(message, strlen(message));
            }
        } else {
            Clear();
            const char* message = "loadScript() failed\n";
            PrintError(message, strlen(message));
        }
        SetFocus();
    }

    void ListScript()
    {
        if (mFile.ModalShow(nullptr, "Load python script", &mFilter))
        {
            CONSOLE::Clear();
            CONSOLE::Color(RGB565(b0b0b0));
            ShowQuery(false);
            PrintAnswer("\n", 1);
            if (mReader.Open(mFile.GetFilename()))
            {
                PrintAnswer((char*)mReader.GetBuffer(), mReader.GetFileSize());
            }
        } else {
            Clear();
            const char* message = "loadScript() failed\n";
            PrintError(message, strlen(message));
        }
        SetFocus();
    }

    void ShowQuery(bool active = true)
    {
        HideCursor();
        if (CONSOLE::cursor.x > CONSOLE::window.left)
            BIOS::LCD::Bar(CONSOLE::window.left , CONSOLE::cursor.y, CONSOLE::cursor.x, CONSOLE::cursor.y+14, CONSOLE::colorBack);
        CONSOLE::cursor.x = CONSOLE::window.left;
        CONSOLE::Color(active ? RGB565(00ff00) : RGB565(aaffaa));
        CONSOLE::Print(">>> ");
        CONSOLE::Color(active ? RGB565(ffffff) : RGB565(aaffaa));
        CONSOLE::Print(mBasic.GetBuffer());
        if (active)
            ShowCursor();
    }
    
    void BlinkCursor()
    {
        static bool on = false;
        if (mBasic.IsRunning())
            return;
        if (!HasFocus())
            return;

        if (on)
            ShowCursor();
        else
            HideCursor();
        on = !on;
    }

    void ShowCursor()
    {
        using namespace CONSOLE;
        BIOS::LCD::Draw(cursor.x, cursor.y, colorFront, colorBack, CShapes_cursor);
    }

    void HideCursor()
    {
        CONSOLE::Print(" \x08");
    }
    
    virtual void OnTimer() override
    {
        BlinkCursor();
    }
    virtual void OnTick()
    {
        while (BIOS::GPIO::UART::Available())
        {
            if (mBasic.IsRunning()) // TODO: input?
                mBasic.SetLastChar(BIOS::GPIO::UART::Read());
            else
                OnChar(BIOS::GPIO::UART::Read());
        }

        for (int i=0; i<20; i++)
            mBasic.Step();
        if (mBasic.FinishedRunning())
            ShowQuery();
    }
    virtual void WindowMessage(int nMsg, int nParam =0) override
    {
        if (nMsg == CWnd::WmTick)
            OnTick();
        CWnd::WindowMessage(nMsg, nParam);
    }
};

CRepl mRepl;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
#ifdef __APPLE__
    BIOS::OS::SetArgument((char*)"devel/basic.elf");
#endif
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);

    CKeyboard keyboard;
    keyboard.Attach();
    mRepl.Create();
    mRepl.SetFocus();
    BIOS::KEY::EKey key;
#ifdef __APPLE__
    while (1)
    {
        key = BIOS::KEY::GetKey();
#else
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
#endif
        char ch = keyboard.Get();
        if (ch)
          mRepl.OnChar(ch);

        if (key != BIOS::KEY::EKey::None)
        {
            mRepl.OnKey(key);
        }
        mRepl.WindowMessage(CWnd::WmTick);
    }
    keyboard.Detach();

    BIOS::FAT::SetSharedBuffer(nullptr);
    return 0;
}


#ifndef __APPLE__
extern "C" void __cxa_pure_virtual(void)
{
  _ASSERT(!"Pure virtual call");
}
#endif

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
    while (1);
}

