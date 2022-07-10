//int main( int argc, char** argv );
#include <tuple>
#include "testcases.h"

#ifndef __APPLE__
///
#include "lib/RTC.h"

/*
void setup()
{
     if (!RTC.begin())
     {
       BIOS::DBG::Print("rtc error\n");
       return;
     }

  BIOS::DBG::Print("r%da%dt%de%d, ", RTC.isRunning(), RTC.isAlarmEnabled(),
    RTC.isAlarmTriggered(), RTC.isTimerEnabled());
  
  BIOS::DBG::Print("date: %04d%02d%02d ", RTC.getYear(), RTC.getMonth(), RTC.getDay());
  BIOS::DBG::Print("time: %02d:%02d:%02d\n", RTC.getHours(), RTC.getMinutes(), RTC.getSeconds());
  if (sc && !RTC.isRunning())
  {
    BIOS::DBG::Print("enabling\n");
    RTC.setDateTime((char*)__DATE__, (char*)__TIME__);
    RTC.startClock();
    sc = false;
//    RTC.enableTimer();
  }
}*/
#else
class PCF8563 {
public:
    bool begin() { return true; }
    int getYear() { return 2022; }
    int getMonth() { return 1; }
    int getDay() { return 1; }
    int getHours() { return 12; }
    int getMinutes() { return 30; }
    int getSeconds() { return 45; }
    void setDateTime(char*, char*) {}
    void setYear(int) { }
    void setMonth(int) { }
    void setDay(int) { }
    void setHours(int) { }
    void setMinutes(int) { }
    void setSeconds(int) { }
    void stopClock() {}
    void startClock() {}
};
#endif

PCF8563 RTC;

///
#ifndef __APPLE__
__attribute__((__section__(".persistent")))
#endif
char mPersistentConfig[256];

bool memError = false;
#ifdef CA4
uint8_t char_0[] = {63, 164, 129, 191, 130, 1, 11, 253, 204, 1, 7, 255, 207, 0, 4, 255, 194, 90, 67, 250, 196, 0, 2, 254, 129, 0, 9, 254, 129, 0, 1, 127, 15, 253, 5, 47, 16, 62, 3, 125, 17, 125, 3, 125, 17, 62, 4, 63, 16, 63, 4, 191, 15, 255, 5, 253, 193, 1, 8, 253, 193, 1, 3, 254, 208, 1, 5, 253, 206, 1, 8, 253, 201, 6, 15, 85, 67, 0, 63};
uint8_t char_1[] = {63, 0, 62, 42, 24, 127, 24, 254, 25, 191, 24, 253, 65, 0, 20, 254, 65, 0, 2, 255, 210, 2, 2, 255, 211, 1, 1, 255, 211, 1, 63, 0, 63, 0, 61};
uint8_t char_2[] = {58, 47, 12, 5, 8, 255, 129, 0, 7, 254, 65, 0, 3, 255, 194, 0, 6, 255, 193, 1, 3, 255, 195, 0, 5, 254, 194, 0, 2, 191, 196, 0, 7, 191, 5, 63, 129, 191, 11, 63, 4, 63, 1, 254, 129, 0, 6, 62, 4, 63, 2, 254, 65, 0, 5, 125, 4, 63, 3, 255, 65, 0, 4, 125, 4, 63, 4, 255, 65, 0, 3, 62, 4, 63, 5, 255, 65, 0, 1, 253, 5, 63, 6, 255, 193, 65, 195, 2, 2, 63, 7, 255, 199, 0, 2, 63, 8, 254, 197, 1, 3, 63, 9, 253, 194, 2, 5, 21, 63, 0, 11};
uint8_t char_3[] = {62, 25, 9, 1, 9, 253, 129, 0, 6, 127, 8, 254, 193, 2, 6, 253, 193, 1, 4, 255, 129, 1, 6, 253, 194, 0, 2, 255, 15, 255, 5, 63, 16, 63, 4, 31, 7, 47, 5, 62, 3, 125, 8, 63, 5, 125, 3, 125, 8, 63, 5, 125, 4, 63, 7, 127, 5, 63, 4, 127, 6, 255, 193, 1, 196, 0, 1, 253, 129, 0, 205, 0, 3, 255, 199, 249, 197, 1, 5, 255, 197, 130, 197, 1, 7, 254, 195, 2, 3, 1, 12, 85, 63, 0, 6};
uint8_t char_4[] = {35, 189, 24, 253, 193, 1, 19, 253, 194, 2, 18, 253, 196, 0, 16, 125, 65, 255, 65, 0, 14, 125, 2, 254, 193, 0, 13, 125, 3, 253, 193, 1, 12, 125, 5, 255, 129, 0, 10, 125, 6, 253, 193, 1, 9, 125, 8, 255, 129, 0, 2, 255, 211, 0, 1, 255, 211, 1, 1, 255, 211, 1, 1, 85, 65, 190, 78, 0, 6, 125, 24, 125, 25, 5, 63, 0, 5};
uint8_t char_5[] = {62, 25, 3, 1, 15, 253, 65, 64, 195, 26, 11, 254, 193, 2, 65, 255, 197, 6, 4, 253, 194, 1, 65, 255, 200, 0, 1, 255, 7, 254, 129, 255, 195, 0, 1, 63, 8, 63, 3, 253, 5, 47, 8, 126, 4, 63, 3, 125, 9, 125, 4, 63, 4, 31, 8, 125, 4, 63, 4, 63, 8, 126, 4, 63, 4, 127, 7, 253, 5, 63, 4, 253, 129, 0, 1, 253, 193, 0, 1, 63, 5, 254, 194, 254, 195, 0, 2, 63, 6, 255, 199, 1, 3, 63, 7, 253, 197, 1, 4, 42, 9, 165, 66, 0, 63, 0};
uint8_t char_6[] = {63, 233, 197, 26, 13, 254, 204, 1, 7, 255, 206, 2, 5, 254, 194, 254, 194, 229, 196, 2, 3, 253, 129, 0, 1, 190, 4, 254, 65, 0, 1, 127, 7, 126, 4, 254, 5, 63, 8, 47, 4, 63, 3, 125, 9, 62, 4, 62, 3, 125, 9, 62, 4, 125, 4, 47, 8, 63, 4, 62, 4, 127, 7, 254, 5, 63, 4, 254, 129, 0, 1, 254, 129, 128, 195, 2, 3, 255, 201, 192, 196, 0, 4, 255, 199, 0, 196, 1, 6, 254, 196, 2, 2, 46, 12, 85, 63, 0, 6};
uint8_t char_7[] = {63, 0, 11, 42, 24, 63, 24, 63, 4, 5, 16, 63, 4, 255, 193, 6, 11, 63, 4, 255, 197, 2, 7, 63, 4, 255, 200, 1, 4, 63, 7, 233, 199, 1, 2, 63, 11, 249, 196, 2, 1, 63, 14, 253, 195, 194, 194, 0, 13, 253, 195, 252, 19, 253, 197, 0, 17, 254, 195, 0, 19, 255, 193, 0, 20, 254, 25, 21, 58};
uint8_t char_8[] = {62, 249, 129, 1, 17, 253, 197, 0, 129, 191, 9, 254, 199, 244, 197, 1, 4, 255, 130, 253, 193, 253, 198, 0, 2, 255, 5, 253, 195, 229, 194, 2, 2, 63, 6, 254, 193, 0, 65, 63, 4, 47, 7, 63, 5, 62, 3, 125, 8, 63, 5, 125, 3, 125, 8, 63, 5, 125, 4, 47, 7, 127, 5, 63, 4, 63, 6, 255, 193, 0, 129, 63, 4, 254, 65, 0, 129, 255, 195, 254, 193, 1, 3, 255, 193, 254, 194, 252, 198, 0, 3, 253, 198, 193, 197, 2, 7, 255, 195, 1, 1, 105, 14, 89, 63, 0, 6};
uint8_t char_9[] = {63, 0, 3, 233, 130, 1, 8, 189, 4, 253, 198, 0, 5, 254, 193, 64, 203, 1, 3, 253, 193, 2, 197, 230, 196, 0, 2, 191, 4, 254, 65, 0, 1, 253, 129, 0, 1, 63, 4, 127, 7, 253, 4, 125, 5, 63, 8, 62, 3, 125, 5, 47, 8, 125, 4, 47, 4, 63, 8, 61, 4, 63, 4, 63, 8, 63, 4, 254, 4, 189, 7, 190, 6, 255, 129, 64, 195, 1, 1, 254, 65, 0, 2, 253, 196, 254, 200, 2, 5, 253, 206, 1, 8, 249, 202, 2, 14, 149, 129, 21, 63, 0};
uint8_t char_dot[] = {63, 0, 20, 5, 23, 191, 24, 191, 24, 191, 24, 21, 63, 0, 63, 0, 63, 0, 63, 0, 63, 0, 23};
uint8_t char_colon[] = {63, 0, 19, 21, 10, 5, 10, 191, 9, 254, 11, 191, 9, 254, 11, 191, 9, 254, 12, 5, 9, 5, 63, 0, 63, 0, 63, 0, 63, 0, 63, 0, 9};
uint8_t char_slash[] = {29, 109, 24, 253, 194, 6, 19, 254, 197, 1, 18, 254, 197, 6, 18, 249, 198, 2, 18, 249, 197, 6, 18, 249, 198, 1, 18, 249, 194, 2, 21, 185, 63, 0, 63, 0, 63, 0, 63, 0, 14};
#endif

uint8_t char_0[] = {27, 33, 129, 53, 227, 54, 129, 2, 7, 50, 237, 3, 4, 61, 239, 5, 2, 59, 226, 20, 8, 42, 227, 4, 1, 63, 225, 1, 12, 63, 225, 0, 226, 2, 13, 57, 225, 18, 226, 0, 14, 63, 97, 57, 225, 1, 13, 57, 225, 2, 226, 5, 13, 61, 225, 0, 161, 63, 161, 1, 9, 41, 226, 6, 1, 57, 228, 37, 100, 44, 193, 63, 226, 0, 2, 49, 238, 14, 6, 42, 234, 21, 11, 9, 69, 1, 28};
uint8_t char_1[] = {31, 0, 31, 0, 14, 63, 193, 0, 17, 60, 225, 3, 17, 57, 226, 1, 17, 59, 225, 6, 3, 9, 45, 63, 225, 5, 2, 63, 242, 1, 244, 3, 148, 1, 31, 0, 31, 0, 31, 0, 10};
uint8_t char_2[] = {1, 1, 20, 63, 161, 1, 9, 36, 65, 0, 3, 63, 226, 3, 8, 62, 226, 1, 2, 63, 227, 4, 7, 63, 227, 1, 1, 63, 228, 4, 7, 41, 226, 5, 1, 63, 161, 58, 226, 3, 8, 61, 225, 0, 226, 5, 65, 63, 225, 2, 7, 57, 225, 2, 226, 5, 1, 59, 226, 1, 7, 63, 97, 56, 225, 5, 2, 60, 226, 1, 6, 63, 65, 56, 225, 5, 3, 61, 226, 1, 4, 58, 225, 1, 226, 5, 4, 62, 226, 3, 2, 57, 226, 0, 226, 5, 5, 62, 227, 53, 227, 3, 1, 63, 161, 0, 5, 60, 230, 5, 2, 63, 161, 0, 6, 49, 228, 3, 3, 9, 33, 0, 9, 1, 27};
uint8_t char_3[] = {26, 43, 129, 0, 7, 20, 6, 57, 226, 6, 7, 58, 225, 14, 4, 63, 226, 6, 7, 59, 227, 1, 1, 61, 225, 14, 10, 42, 226, 5, 1, 63, 161, 0, 12, 61, 225, 9, 226, 1, 5, 59, 225, 0, 4, 63, 66, 63, 7, 58, 225, 0, 4, 63, 97, 57, 225, 1, 5, 58, 225, 2, 4, 63, 33, 56, 225, 4, 5, 61, 225, 6, 3, 61, 225, 0, 193, 63, 97, 0, 2, 59, 228, 28, 193, 63, 161, 0, 33, 63, 225, 38, 129, 62, 225, 30, 230, 6, 3, 59, 231, 2, 129, 63, 226, 6, 5, 58, 229, 2, 3, 18, 10, 25, 97, 2, 31, 0, 1};
uint8_t char_4[] = {5, 49, 193, 5, 17, 57, 227, 2, 15, 57, 228, 4, 14, 57, 225, 52, 226, 14, 13, 57, 225, 2, 129, 63, 225, 3, 11, 57, 225, 2, 1, 58, 226, 6, 10, 57, 225, 2, 3, 61, 226, 2, 8, 57, 225, 2, 4, 59, 226, 5, 7, 58, 225, 3, 5, 57, 227, 2, 1, 63, 242, 2, 244, 2, 165, 63, 193, 45, 170, 1, 4, 57, 225, 2, 17, 57, 225, 2, 18, 18, 31, 0, 3};
uint8_t char_5[] = {26, 51, 129, 0, 1, 26, 33, 0, 9, 57, 226, 5, 2, 61, 226, 46, 97, 2, 4, 57, 227, 5, 2, 62, 232, 5, 1, 61, 225, 22, 4, 59, 233, 0, 226, 6, 6, 60, 225, 0, 65, 51, 226, 8, 226, 1, 6, 57, 225, 2, 2, 60, 225, 16, 226, 0, 7, 63, 97, 0, 1, 60, 225, 8, 226, 1, 7, 63, 129, 0, 1, 60, 225, 0, 226, 4, 6, 59, 225, 2, 2, 60, 225, 0, 161, 63, 97, 0, 3, 57, 226, 1, 2, 60, 225, 0, 33, 63, 225, 30, 65, 43, 227, 5, 3, 60, 225, 0, 1, 58, 233, 0, 3, 60, 225, 0, 2, 49, 230, 4, 5, 42, 161, 0, 4, 25, 130, 10, 31, 0};
uint8_t char_6[] = {27, 43, 193, 63, 227, 46, 129, 1, 7, 59, 236, 13, 5, 60, 232, 54, 229, 3, 2, 58, 226, 20, 34, 59, 225, 5, 1, 41, 227, 2, 1, 62, 225, 2, 4, 57, 225, 4, 2, 59, 226, 0, 226, 3, 6, 59, 225, 0, 2, 59, 225, 8, 226, 1, 6, 58, 225, 1, 3, 63, 66, 63, 33, 0, 5, 58, 225, 2, 3, 63, 97, 57, 225, 3, 6, 61, 225, 0, 2, 58, 225, 2, 193, 63, 65, 0, 3, 59, 226, 0, 1, 58, 226, 0, 65, 63, 225, 38, 97, 52, 227, 2, 1, 63, 226, 4, 2, 58, 232, 4, 2, 63, 225, 5, 4, 49, 230, 2, 3, 55, 97, 0, 6, 17, 97, 10, 31, 0, 1};
uint8_t char_7[] = {31, 0, 7, 51, 193, 0, 17, 60, 225, 0, 17, 60, 225, 0, 226, 38, 97, 1, 10, 60, 225, 0, 231, 3, 8, 60, 225, 0, 233, 21, 6, 60, 225, 0, 2, 25, 129, 62, 229, 3, 3, 60, 225, 0, 7, 51, 229, 2, 1, 60, 225, 0, 9, 41, 228, 5, 129, 63, 13, 41, 228, 62, 225, 0, 13, 58, 229, 0, 15, 60, 227, 0, 16, 58, 226, 0, 17, 25, 97, 0, 21};
uint8_t char_8[] = {26, 52, 226, 30, 6, 1, 6, 57, 230, 6, 1, 50, 227, 5, 4, 63, 231, 12, 231, 0, 1, 60, 225, 14, 2, 49, 229, 45, 227, 5, 1, 63, 193, 0, 4, 63, 226, 1, 2, 62, 225, 8, 226, 2, 5, 59, 225, 3, 3, 57, 225, 18, 226, 0, 5, 58, 225, 1, 4, 63, 97, 57, 225, 1, 5, 58, 225, 2, 4, 63, 65, 56, 225, 3, 5, 61, 225, 5, 3, 60, 225, 1, 193, 63, 33, 0, 2, 59, 227, 21, 65, 60, 225, 6, 1, 58, 226, 28, 97, 61, 225, 46, 231, 2, 2, 61, 231, 1, 230, 2, 4, 59, 229, 2, 2, 33, 129, 3, 8, 26, 97, 2, 31, 0, 1};
uint8_t char_9[] = {26, 26, 5, 43, 193, 55, 161, 3, 5, 57, 226, 0, 1, 49, 231, 14, 3, 57, 227, 1, 1, 63, 233, 0, 1, 62, 225, 13, 2, 60, 226, 2, 2, 50, 226, 5, 1, 63, 161, 0, 2, 63, 225, 1, 5, 62, 225, 8, 226, 2, 3, 63, 129, 0, 5, 57, 225, 18, 226, 1, 3, 63, 65, 0, 6, 63, 97, 57, 225, 2, 3, 62, 97, 0, 6, 63, 65, 56, 225, 6, 3, 60, 193, 0, 5, 60, 225, 1, 97, 63, 193, 2, 2, 63, 161, 0, 3, 59, 225, 5, 2, 62, 227, 28, 97, 63, 225, 37, 161, 63, 226, 1, 3, 61, 238, 1, 5, 41, 235, 4, 10, 17, 65, 27, 97, 18, 33, 0, 26};
uint8_t char_dot[] = {31, 0, 12, 36, 129, 0, 17, 63, 225, 0, 17, 63, 225, 0, 17, 27, 97, 0, 31, 0, 31, 0, 31, 0, 31, 0, 31, 0, 31, 0, 31, 0, 5};
uint8_t char_colon[] = {31, 0, 12, 36, 129, 0, 6, 33, 129, 3, 6, 63, 225, 0, 6, 58, 225, 6, 6, 63, 225, 0, 6, 58, 225, 6, 6, 27, 97, 0, 7, 27, 65, 0, 31, 0, 31, 0, 31, 0, 31, 0, 31, 0, 31, 0, 26};
uint8_t char_slash[] = {65, 47, 97, 1, 16, 58, 228, 20, 15, 42, 230, 29, 33, 0, 12, 25, 193, 63, 228, 20, 15, 34, 230, 30, 33, 0, 12, 25, 161, 63, 228, 21, 15, 34, 193, 63, 225, 5, 18, 43, 129, 0, 31, 0, 31, 0, 31, 0, 31, 0, 31, 0, 9};

namespace printer {
template <int N> int PrintBig(int x, int y, uint16_t c1, uint16_t c0, char* text)
{
    uint8_t* fontBase = (uint8_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::CharRom);
    int oldx = x;
    while (*text)
    {
        uint8_t* charBase = fontBase + (*text++) * 14;
        for (ui8 _y=0; _y<14; _y++)
        {
            ui8 col = *charBase++;
            for (ui8 _x=0; _x<8; _x++, col <<= 1)
                BIOS::LCD::Bar(x + _x*N, y + _y*N, x + _x*N+N, y + _y*N+N, col & 128 ? c1 : c0);
        }
        x += 9*N;
    }
    return x - oldx;
}
/*
template <> int PrintBig<2>(int x, int y, uint16_t c1, uint16_t c0, char* text)
{
    uint8_t* fontBase = (uint8_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::CharRom);
    int oldx = x;
    while (*text)
    {
        uint8_t ch = *text++;
        if (ch == '_')
        {
            x += 9*2;
            continue;
        }
        
        uint8_t* charBase = fontBase + ch * 14;
        for (ui8 _y=0; _y<14; _y++)
        {
            int colp = charBase[-1];
            int col = charBase[0];
            int coln = charBase[+1];
            int m = 128;

            BIOS::LCD::BufferBegin(CRect(x, y + _y*2, x + 8*2+2, y + _y*2 + 2));
            for (ui8 _x=0; _x<8; _x++, m >>= 1)
            {
                // AdvMAME2x
                bool A = colp & m;
                bool B = col & (m >> 1);
                bool P = col & m;
                bool C = col & (m << 1);
                bool D = coln & m;

                bool topleft = ((C==A) && (C!=D) && (A!=B)) ? A : P;
                bool topright = ((A==B) && (A!=C) && (B!=D)) ? B : P;
                bool bottomleft = ((D==C) && (D!=B) && (C!=A)) ? C : P;
                bool bottomright = ((B==D) && (B!=A) && (D!=C)) ? D : P;
                
                uint16_t pixels[4] = {bottomleft ? c1 : c0, topleft ? c1 : c0,
                    bottomright ? c1 : c0, topright ? c1 : c0};
                BIOS::LCD::BufferWrite(pixels, 4);
            }
            BIOS::LCD::BufferEnd();

            charBase++;
        }
        x += 9*2;
    }
    return x - oldx;
}
*/
#ifdef CA4
template <> int PrintBig<2>(int x, int y, uint16_t c1, uint16_t c0, char* text)
{
    uint16_t pal[4] = {RGB565(000000), RGB565(404040), RGB565(808080), RGB565(b0b0b0)};
    int oldx = x;
    while (*text)
    {
        uint8_t ch = *text++;
        if (ch == '_')
        {
            x += 9*2;
            continue;
        }

        BIOS::LCD::BufferBegin(CRect(x, y, x + 16, y + 22));
        uint8_t* fo[] = {char_0, char_1, char_2, char_3, char_4, char_5, char_6, char_7, char_8, char_9, char_dot};
        uint8_t* buf = nullptr;
        if (ch == ' ')
        {
            x += 9*2;
            continue;
        }
        if (ch >= '0' && ch <= '9')
            buf = fo[ch - '0'];
        else
            buf = fo[10];
        int remain = 16*22;
        do {
            uint8_t dr = *buf++;
            for (int i=0; i<(dr&63); i++, remain--)
                BIOS::LCD::BufferWrite(pal[dr>>6]);

            if (remain <= 0)
                break;

            uint8_t ds = *buf++;
            BIOS::LCD::BufferWrite(pal[ds&3]);
            if (--remain <= 0)
                break;
            ds >>= 2;
            BIOS::LCD::BufferWrite(pal[ds&3]);
            if (--remain <= 0)
                break;
            ds >>= 2;
            BIOS::LCD::BufferWrite(pal[ds&3]);
            if (--remain <= 0)
                break;
            ds >>= 2;
            BIOS::LCD::BufferWrite(pal[ds&3]);
            if (--remain <= 0)
                break;
            ds >>= 2;
        } while (1);
        BIOS::LCD::BufferEnd();

        x += 9*2;
    }
    return x - oldx;
}
#endif
//    template <int N> int PrintBig(int x, int y, uint16_t c1, uint16_t c0, char* text)

template <> int PrintBig<2>(int x, int y, uint16_t c1, uint16_t c0, char* text)
{
    uint16_t pal[8];
    const int r0 = Get565R(c0);
    const int g0 = Get565G(c0);
    const int b0 = Get565B(c0);
    const int r1 = Get565R(c1);
    const int g1 = Get565G(c1);
    const int b1 = Get565B(c1);
    for (int i=0; i<8; i++)
        pal[i] = RGB565RGB(r0 + (r1-r0)*i/8, g0 + (g1-g0)*i/8, b0 + (b1-b0)*i/8);
    
    int oldx = x;
    while (*text)
    {
        uint8_t ch = *text++;
        if (ch == '_')
        {
            x += 9*2-3;
            continue;
        }

        BIOS::LCD::BufferBegin(CRect(x, y, x + 16, y + 22));
        uint8_t* fo[] = {char_0, char_1, char_2, char_3, char_4, char_5, char_6, char_7, char_8, char_9, char_dot};
        uint8_t* buf = nullptr;
        if (ch == ' ')
        {
            x += 9*2-3;
            continue;
        }
        if (ch >= '0' && ch <= '9')
            buf = fo[ch - '0'];
        else if (ch == ':')
            buf = char_colon;
        else
            buf = fo[10];
        int remain = 16*22-22;
        do {
            uint8_t dr = *buf++;
            for (int i=0; i<(dr&31) && remain; i++, remain--)
                BIOS::LCD::BufferWrite(pal[dr>>5]);

            if (remain <= 0)
                break;

            uint8_t ds = *buf++;
            BIOS::LCD::BufferWrite(pal[ds&7]);
            if (--remain <= 0)
                break;
            ds >>= 3;
            BIOS::LCD::BufferWrite(pal[ds&7]);
            if (--remain <= 0)
                break;
        } while (1);
        BIOS::LCD::BufferEnd();

        x += 9*2-3;
    }
    return x - oldx;
}

}

class CApplication : public CWnd
{
	int mPos{0};
    uint32_t mStartupTimestamp;
    uint32_t mEpochShift;
    char mTopTime[32]{""};
    char mTopTimeH[32]{""};
    char mBottomTime[32]{""};
    char mBottomTimeH[32]{""};
    
public:
    // https://stackoverflow.com/questions/7960318/math-to-convert-seconds-since-1970-into-date-and-vice-versa
    template <class Int> const Int
    days_from_civil(Int y, unsigned m, unsigned d) noexcept
    {
        static_assert(std::numeric_limits<unsigned>::digits >= 18,
                 "This algorithm has not been ported to a 16 bit unsigned integer");
        static_assert(std::numeric_limits<Int>::digits >= 20,
                 "This algorithm has not been ported to a 16 bit signed integer");
        y -= m <= 2;
        const Int era = (y >= 0 ? y : y-399) / 400;
        const unsigned yoe = static_cast<unsigned>(y - era * 400);      // [0, 399]
        const unsigned doy = (153*(m + (m > 2 ? -3 : 9)) + 2)/5 + d-1;  // [0, 365]
        const unsigned doe = yoe * 365 + yoe/4 - yoe/100 + doy;         // [0, 146096]
        return era * 146097 + static_cast<Int>(doe) - 719468;
    }
    
    template <class Int> const std::tuple<Int, unsigned, unsigned>
    civil_from_days(Int z) noexcept
    {
        static_assert(std::numeric_limits<unsigned>::digits >= 18,
                 "This algorithm has not been ported to a 16 bit unsigned integer");
        static_assert(std::numeric_limits<Int>::digits >= 20,
                 "This algorithm has not been ported to a 16 bit signed integer");
        z += 719468;
        const Int era = (z >= 0 ? z : z - 146096) / 146097;
        const unsigned doe = static_cast<unsigned>(z - era * 146097);          // [0, 146096]
        const unsigned yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;  // [0, 399]
        const Int y = static_cast<Int>(yoe) + era * 400;
        const unsigned doy = doe - (365*yoe + yoe/4 - yoe/100);                // [0, 365]
        const unsigned mp = (5*doy + 2)/153;                                   // [0, 11]
        const unsigned d = doy - (153*mp+2)/5 + 1;                             // [1, 31]
        const unsigned m = mp + (mp < 10 ? 3 : -9);                            // [1, 12]
        return std::tuple<Int, unsigned, unsigned>(y + (m <= 2), m, d);
    }
    
    void Create( const char* pszId, ui16 dwFlags, const CRect& rc, CWnd* pParent )
    {
        mEpochShift = days_from_civil(uint32_t(2020), 1, 1);
        const uint32_t secondsPerDay = 24*60*60;
        uint32_t days = days_from_civil(uint32_t(2022), 7, 10) - mEpochShift;
        uint32_t secondsToday = (16*60+1)*60+0;
        mStartupTimestamp = days*secondsPerDay + secondsToday;
        
        //BIOS::DBG::Print(built);
        CWnd::Create(pszId, dwFlags, rc, pParent);
		
		SetTimer(100);
    }
    
    void SetDateTime(uint32_t& timestamp, int year, int month, int day, int hour, int minute, int second)
    {
        timestamp = days_from_civil(uint32_t(year), month, day) - mEpochShift;
        timestamp *= 24*60*60;
        timestamp += (hour*60+minute)*60+second;
    }
    
    void GetDateTime(uint32_t timestamp, int& year, int& month, int &day, int& hour, int& minute, int& second)
    {
        const uint32_t secondsPerDay = 24*60*60;
        uint32_t days = timestamp / secondsPerDay;
        uint32_t remain = timestamp % secondsPerDay;
        second = remain % 60;
        remain /= 60;
        minute = remain % 60;
        remain /= 60;
        hour = remain;
        std::tie(year, month, day) = civil_from_days(days + mEpochShift);
    }
    
    void RelDateTime(uint32_t& timestamp, int dyear, int dmonth, int dday, int dhour, int dminute, int dsecond)
    {
        int year, month, day, hour, minute, second;
        GetDateTime(timestamp, year, month, day, hour, minute, second);
        year += dyear;
        month += dmonth;
        day += dday;
        hour += dhour;
        minute += dminute;
        second += dsecond;
        SetDateTime(timestamp, year, month, day, hour, minute, second);
    }
    
    void RelDevice(int dyear, int dmonth, int dday, int dhour, int dminute, int dsecond)
    {
        if (dyear != 0)
            RTC.setYear(RTC.getYear()+dyear);
        if (dmonth != 0)
            RTC.setMonth(RTC.getMonth()+dmonth);
        if (dday != 0)
            RTC.setDay(RTC.getDay()+dday);
        if (dhour != 0)
            RTC.setHours(RTC.getHours()+dhour);
        if (dminute != 0)
            RTC.setMinutes(RTC.getMinutes()+dminute);
        if (dsecond != 0)
            RTC.setSeconds(RTC.getSeconds()+dsecond);
    }
    
    void SystemToDevice()
    {
        uint32_t timestamp = mStartupTimestamp + BIOS::SYS::GetTick() / 1000;
        int year, month, day, hour, minute, second;
        GetDateTime(timestamp, year, month, day, hour, minute, second);
        RTC.stopClock();
        RTC.setYear(year);
        RTC.setMonth(month);
        RTC.setDay(day);
        RTC.setHours(hour);
        RTC.setMinutes(minute);
        RTC.setSeconds(second);
        RTC.startClock();
    }
    
    void DeviceToSystem()
    {
        uint32_t timestamp;
        int year = RTC.getYear(), month = RTC.getMonth(), day = RTC.getDay();
        int hour = RTC.getHours(), minute = RTC.getMinutes(), second = RTC.getSeconds();
        SetDateTime(timestamp, year, month, day, hour, minute, second);
        mStartupTimestamp = timestamp - BIOS::SYS::GetTick() / 1000;
    }
    void GetTime(int& year, int& month, int &day, int& hour, int& minute, int& second)
    {
        year = RTC.getYear();
        month = RTC.getMonth();
        day = RTC.getDay();
        hour = RTC.getHours();
        minute = RTC.getMinutes();
        second = RTC.getSeconds();
    }
    void RunTests()
    {
        while (!BIOS::KEY::GetKey())
        {
            int index = random() % (COUNT(testCases)/2);
            const char* date1 = testCases[index*2];
            const char* date2 = testCases[index*2 + 1];
            
            int year1 = atoi(date1);
            int month1 = atoi(date1+5);
            int day1 = atoi(date1+8);
            int hour1 = atoi(date1+11);
            int minute1 = atoi(date1+14);
            int second1 = atoi(date1+17);
            
            int year2 = atoi(date2);
            int month2 = atoi(date2+5);
            int day2 = atoi(date2+8);
            int hour2 = atoi(date2+11);
            int minute2 = atoi(date2+14);
            int second2 = atoi(date2+17);
            int year, month, day, hour, minute, second;

            RTC.stopClock();
            RTC.setYear(year1);
            RTC.setMonth(month1);
            RTC.setDay(day1);
            RTC.setHours(hour1);
            RTC.setMinutes(minute1);
            RTC.setSeconds(second1);
            RTC.startClock();
            
            GetTime(year, month, day, hour, minute, second);
            RedrawTimes();
            if (year != year1 || month != month1 || day != day1)
            {
                BIOS::DBG::Print("%s -> %s ", date1, date2);
                BIOS::DBG::Print("(%04d%02d%02d %02d%02d%02d) ", year, month, day, hour, minute, second);
                BIOS::DBG::Print("Setting date failed\n");
                break;
            }
            if (hour != hour1 || minute != minute1 || second != second1)
            {
                BIOS::DBG::Print("%s -> %s ", date1, date2);
                BIOS::DBG::Print("(%04d%02d%02d %02d%02d%02d) ", year, month, day, hour, minute, second);
                BIOS::DBG::Print("Setting time failed\n");
                break;
            }
            int maxCycles = 1000/50;
            while (RTC.getSeconds() == second1)
            {
                BIOS::SYS::DelayMs(50);
                if (--maxCycles <= 0)
                {
                    BIOS::DBG::Print("%s -> %s ", date1, date2);
                    BIOS::DBG::Print("(%04d%02d%02d %02d%02d%02d) ", year, month, day, hour, minute, second);
                    BIOS::DBG::Print("Clock not working\n");
                    break;
                }
            }
            if (--maxCycles <= 0)
                break;

            GetTime(year, month, day, hour, minute, second);
            RedrawTimes();

            if (year != year2 || month != month2 || day != day2)
            {
                BIOS::DBG::Print("%s -> %s ", date1, date2);
                BIOS::DBG::Print("(%04d%02d%02d %02d%02d%02d) ", year, month, day, hour, minute, second);
                BIOS::DBG::Print("Flipping date failed\n");
                break;
            }
            if (hour != hour2 || minute != minute2 || second != second2)
            {
                BIOS::DBG::Print("%s -> %s:", date1, date2);
                BIOS::DBG::Print("(%04d%02d%02d %02d%02d%02d) ", year, month, day, hour, minute, second);
                BIOS::DBG::Print("Flipping time failed\n");
                break;
            }
        }
    }

	void OnKey(int key) override
	{
		if (key == BIOS::KEY::EKey::Down)
		{
            switch (mPos)
            {
                case 0: RelDateTime(mStartupTimestamp, -1, 0, 0, 0, 0, 0); break;
                case 1: RelDateTime(mStartupTimestamp, 0, -1, 0, 0, 0, 0); break;
                case 2: RelDateTime(mStartupTimestamp, 0, 0, -1, 0, 0, 0); break;
                case 3: RelDateTime(mStartupTimestamp, 0, 0, 0, -1, 0, 0); break;
                case 4: RelDateTime(mStartupTimestamp, 0, 0, 0, 0, -1, 0); break;
                case 5: RelDateTime(mStartupTimestamp, 0, 0, 0, 0, 0, -1); break;

                case 8: RelDevice(-1, 0, 0, 0, 0, 0); break;
                case 9: RelDevice(0, -1, 0, 0, 0, 0); break;
                case 10: RelDevice(0, 0, -1, 0, 0, 0); break;
                case 11: RelDevice(0, 0, 0, -1, 0, 0); break;
                case 12: RelDevice(0, 0, 0, 0, -1, 0); break;
                case 13: RelDevice(0, 0, 0, 0, 0, -1); break;
            }
		}
		if (key == BIOS::KEY::EKey::Up)
		{
            switch (mPos)
            {
                case 0: RelDateTime(mStartupTimestamp, +1, 0, 0, 0, 0, 0); break;
                case 1: RelDateTime(mStartupTimestamp, 0, +1, 0, 0, 0, 0); break;
                case 2: RelDateTime(mStartupTimestamp, 0, 0, +1, 0, 0, 0); break;
                case 3: RelDateTime(mStartupTimestamp, 0, 0, 0, +1, 0, 0); break;
                case 4: RelDateTime(mStartupTimestamp, 0, 0, 0, 0, +1, 0); break;
                case 5: RelDateTime(mStartupTimestamp, 0, 0, 0, 0, 0, +1); break;
                case 8: RelDevice(+1, 0, 0, 0, 0, 0); break;
                case 9: RelDevice(0, +1, 0, 0, 0, 0); break;
                case 10: RelDevice(0, 0, +1, 0, 0, 0); break;
                case 11: RelDevice(0, 0, 0, +1, 0, 0); break;
                case 12: RelDevice(0, 0, 0, 0, +1, 0); break;
                case 13: RelDevice(0, 0, 0, 0, 0, +1); break;
            }
		}
		if (key == BIOS::KEY::EKey::Left && mPos > 0)
		{
			mPos--;
            RedrawTimes();
            RedrawButtons();
		}
		if (key == BIOS::KEY::EKey::Right && mPos < 14)
		{
			mPos++;
            RedrawTimes();
            RedrawButtons();
		}
		if (key == BIOS::KEY::EKey::Enter) // && mPos == 8)
        {
            if (mPos == 6)
                SystemToDevice();
            if (mPos == 7)
                DeviceToSystem();
            if (mPos == 14)
                RunTests();
            //RTC.setDateTime((char*)__DATE__, (char*)__TIME__);
            RedrawTimes();
		}
	}

    void FindDifference(char* format, char *cache)
    {
        if (strlen(format) != strlen(cache))
        {
            strcpy(cache, format);
            return;
        }
        while (*format)
        {
            if (*format == 0)
                break;
            if (*format == *cache)
                *format = '_';
            else
                *cache = *format;
            format++;
            cache++;
        }
    }
    void ClearRange(char* dst, char* src, int from, int len, bool invert)
    {
        int i;
        for (i=0; i<strlen(dst); i++)
        {
            if (invert)
            {
                if (i >= from && i < from + len)
                    dst[i] = '_';
                else
                    dst[i] = src[i] & 127;
            } else {
                if (i < from || i >= from + len)
                    dst[i] = '_';
                else
                    dst[i] = src[i] & 127;
            }
        }
        dst[i] = 0;
    }
    void SetRange(char* str, int from, int len)
    {
        for (int i=from; i<from+len; i++)
            str[i] |= 0x80;
    }
    void Filter(char* dst, char* src, bool b)
    {
        int i;
        for (i=0; src[i]; i++)
        {
            if (b)
                dst[i] = src[i] & 0x80 ? (src[i] & 0x7f) : '_';
            else
                dst[i] = src[i] & 0x80 ? '_' : src[i];
        }
        dst[i] = 0;
    }
    

    void RedrawTimes()
    {
        uint32_t timestamp = mStartupTimestamp + BIOS::SYS::GetTick() / 1000;
        int year, month, day, hour, minute, second;
        GetDateTime(timestamp, year, month, day, hour, minute, second);
        char format[32], temp[32];
        int rangeFrom = 0, rangeLen = 0;
        switch (mPos)
        {
            case 0: rangeFrom = 0; rangeLen = 4; break;
            case 1: rangeFrom = 5; rangeLen = 2; break;
            case 2: rangeFrom = 8; rangeLen = 2; break;
            case 3: rangeFrom = 12; rangeLen = 2; break;
            case 4: rangeFrom = 15; rangeLen = 2; break;
            case 5: rangeFrom = 18; rangeLen = 2; break;
            default: rangeFrom = 0; rangeLen = 0; break;
        }

        sprintf(format, "%04d/%02d/%02d  %02d:%02d:%02d", year, month, day, hour, minute, second);
        SetRange(format, rangeFrom, rangeLen);
        FindDifference(format, mTopTime);
        Filter(temp, format, true);
        printer::PrintBig<2>(8, 40+16, RGB565(404040), RGB565(ffffff), temp);
        Filter(temp, format, false);
        printer::PrintBig<2>(8, 40+16, RGB565(ffffff), RGB565(404040), temp);
        

//        sprintf(format, "(\x18 device to system)   (\x19 system to device)");

// 
     if (!RTC.begin())
     {
//       BIOS::DBG::Print("rtc error\n");
       sprintf(format, "..../../..  ..:..:..");
     } else {
        sprintf(format, "%04d/%02d/%02d  %02d:%02d:%02d", 
          RTC.getYear(), RTC.getMonth(), RTC.getDay(), RTC.getHours(), RTC.getMinutes(), RTC.getSeconds());
     }
        
        switch (mPos)
        {
            case 8: rangeFrom = 0; rangeLen = 4; break;
            case 9: rangeFrom = 5; rangeLen = 2; break;
            case 10: rangeFrom = 8; rangeLen = 2; break;
            case 11: rangeFrom = 12; rangeLen = 2; break;
            case 12: rangeFrom = 15; rangeLen = 2; break;
            case 13: rangeFrom = 18; rangeLen = 2; break;
            default: rangeFrom = 0; rangeLen = 0; break;
        }
        SetRange(format, rangeFrom, rangeLen);
        FindDifference(format, mBottomTime);
        Filter(temp, format, true);
        printer::PrintBig<2>(8, 80+40+16, RGB565(404040), RGB565(ffffff), temp);
        Filter(temp, format, false);
        printer::PrintBig<2>(8, 80+40+16, RGB565(ffffff), RGB565(404040), temp);
    }
    
    void OnPaint() override
    {
        
        //GUI::Background(m_rcClient, RGB565(404040), RGB565(101010));
		BIOS::LCD::Bar(m_rcClient, RGB565(404040));
        
        CRect rc1(m_rcClient);
        rc1.bottom = 14;
        GUI::Background(rc1, RGB565(4040b0), RGB565(404040));

        BIOS::LCD::Print(8, 0, RGB565(ffffff), RGBTRANS, "RTC Monitor");
		//ShowPage(mAddress);
		//ShowTitle();
        
        RedrawTimes();
        RedrawButtons();
        
        BIOS::LCD::Printf(8, 40, RGB565(b0b0b0), RGB565(404040), "System Date            System Time");
        BIOS::LCD::Printf(8, 120, RGB565(b0b0b0), RGB565(404040), "Device Date            Device Time");
    }
    
    void RedrawButtons()
    {
        BIOS::LCD::Print(47, 92, mPos == 6 ? RGB565(d0d0d0) : RGB565(404040), mPos == 6 ? RGB565(ffffff) : RGB565(808080), " \x19 Copy ");
        BIOS::LCD::Print(47-1, 92, mPos == 6 ? RGB565(404040) : RGB565(f0f0f0), RGBTRANS, " \x19 Copy ");
        BIOS::LCD::Print(227, 92, mPos == 7 ? RGB565(d0d0d0) : RGB565(404040), mPos == 7 ? RGB565(ffffff) : RGB565(808080), " \x18 Copy ");
        BIOS::LCD::Print(227-1, 92, mPos == 7 ? RGB565(404040) : RGB565(f0f0f0), RGBTRANS, " \x18 Copy ");

        BIOS::LCD::Print(47, 170, mPos == 14 ? RGB565(d0d0d0) : RGB565(404040), mPos == 14 ? RGB565(ffffff) : RGB565(808080), " Run tests ");
        BIOS::LCD::Print(47-1, 170, mPos == 14 ? RGB565(404040) : RGB565(f0f0f0), RGBTRANS, " Run tests ");
    }
	
	void ShowTitle()
	{
		CRect rc1(m_rcClient);
		rc1.left = 80;
		rc1.bottom = 14;
		GUI::Background(rc1, RGB565(4040b0), RGB565(404040));

	}
	
    void OnTimer() override
    {
		//ShowPage(mAddress, true);
        //Invalidate();
        RedrawTimes();
//setup();
    }
	
	void Redraw()
	{
	}
	
};
