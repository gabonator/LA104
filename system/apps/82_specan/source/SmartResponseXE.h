#pragma once
#include <library.h>

typedef uint8_t byte;
typedef uint8_t u8;

char SRXEGetKey();
enum {FONT_LARGE, FONT_MEDIUM};
enum {LOW = 0, HIGH = 1};
enum {OUTPUT, INPUT};

void SRXEWriteString(int x, int y, const char* text, int font, int clrf, int clrb);
void delay(int);
void SRXESleep();
void SRXEHorizontalLine(int x, int y, int w, int a, int b);
void SRXEVerticalLine(int x, int y, int w, int c);
//void itoa(int n, char* buf, int base);

void pinMode(int, int);
void digitalWrite(int, int);
int digitalRead(int);