#include <library.h>
#include "../../os_host/source/framework/Console.h"
#include "../../os_host/source/framework/SimpleApp.h"

// https://www.avdweb.nl/arduino/hardware-interfacing/nokia-5110-lcd
class CNokia5110
{
//  unsigned char CircuitsTodayLogo [504];

public:
enum {DC = BIOS::GPIO::P2, 
      CE = -1, //GND
      CLK = BIOS::GPIO::P4, 
      DIN = BIOS::GPIO::P3, 
      RST = BIOS::GPIO::P1, 
      LOW=0, HIGH=1, OUTPUT=2};

void pinMode(int pin, int mode)
{
  if (pin == -1)
    return;
  BIOS::GPIO::PinMode((BIOS::GPIO::EPin)pin, BIOS::GPIO::EMode::Output);
}

void digitalWrite(int pin, int level)
{
  if (pin == -1)
    return;
  BIOS::GPIO::DigitalWrite((BIOS::GPIO::EPin)pin, level);
} 
 
void Cmd(unsigned char c){
  unsigned int i;
  digitalWrite(DC,LOW);
  digitalWrite(CE,LOW);
  for(i=0;i<8;i++){
    digitalWrite(CLK,LOW);
    if(c&0x80)
      digitalWrite(DIN,HIGH);
    else 
      digitalWrite(DIN,LOW);
    digitalWrite(CLK,HIGH);
    c <<= 1;
  }
  digitalWrite(CE,HIGH);
}
 
void Data(unsigned char d){
 unsigned int i;
  digitalWrite(DC,HIGH);
  digitalWrite(CE,LOW);
  for(i=0;i<8;i++){
    digitalWrite(CLK,LOW);
    if(d&0x80)
      digitalWrite(DIN,HIGH);
    else 
      digitalWrite(DIN,LOW);
    digitalWrite(CLK,HIGH);
    d <<= 1;
  }
  digitalWrite(CE,HIGH);
}

void dotPixel(unsigned char x,unsigned char y){
  Cmd(0x40|(y&0x07));
  Cmd(0x80|(x&0x7f));
}

void allClear(void)
{
  int pixel;
  dotPixel(0,0);                  
  for (pixel=504;pixel>0;pixel--) 
    Data(0);
//    Data(rand()&255);
//    Data((pixel & 1) ? 0x55 : 0xaa);
  //{Data(0x00);}  
}

void Init(void)
{
  pinMode(RST, OUTPUT);
  pinMode(CE, OUTPUT);
  pinMode(DC, OUTPUT);
  pinMode(DIN, OUTPUT);
  pinMode(CLK, OUTPUT);

  digitalWrite(RST,LOW);  
  BIOS::SYS::DelayMs(5);
  digitalWrite(RST,HIGH);  
  digitalWrite(CE,HIGH); 

  Cmd(0x21);    
  Cmd(0xb2);    
  Cmd(0x13);  
  Cmd(0x20);    
  Cmd(0x09);    
  allClear();   
  Cmd(0x08);    
  Cmd(0x0C);  
  dotPixel(0,0);    
}

void setPointer(unsigned char row,unsigned char col){
  if((row>6) || (row<1) || (col<1) || (col>14))
    return;
  dotPixel(((col-1)*6),(row-1));
}

void logoDisp(unsigned char logo[])
{
  unsigned int i;
  
  dotPixel(0,0);
  for(i=0;i<504;i++)
  Data(logo[i]);  
}
};

CNokia5110 display;
uint8_t buf[504];
uint16_t temp[84];

__attribute__((__section__(".entry")))
int main(void)
{
    APP::Init("Nokia 5110 display test"); //pcd8544
    CONSOLE::Print("P1 - RST\n");
    CONSOLE::Print("P2 - DC\n");
    CONSOLE::Print("P3 - DIN\n");
    CONSOLE::Print("P4 - CLK\n");
    CONSOLE::Print("GND - CE\n");
    CRect rcWindow(140, 40, 140+84, 40+48);
    rcWindow.Inflate(1, 1, 1, 1);
    BIOS::LCD::Rectangle(rcWindow, RGB565(b0b0b0));
    rcWindow.Deflate(1, 1, 1, 1);
    BIOS::LCD::Bar(rcWindow, RGB565(404040));

    BIOS::LCD::Print(rcWindow.left, rcWindow.top-2  , RGB565(ffffff), RGBTRANS, "Hello 1234");
    BIOS::LCD::Print(rcWindow.left, rcWindow.top+10, RGB565(ffffff), RGBTRANS, "ABCDEFGHIJ");
    BIOS::LCD::Print(rcWindow.left, rcWindow.top+22, RGB565(ffffff), RGBTRANS, "abcdefghij");
    BIOS::LCD::Print(rcWindow.left, rcWindow.top+34, RGB565(ffffff), RGBTRANS, "!@#$%^&*()");

    int animt = 0;
    int animi = 0;
//enum {DC = BIOS::GPIO::P2, CE = -1, CLK = BIOS::GPIO::P4, DIN = BIOS::GPIO::P3, RST = BIOS::GPIO::P1, LOW=0, HIGH=1, OUTPUT=2};

    display.Init();
    while (!BIOS::KEY::GetKey())
    {
      CRect invert;
      if (animt == 0)
      {
        invert = CRect(rcWindow.left+animi, rcWindow.top, rcWindow.left+animi+1, rcWindow.bottom);
        if (++animi == 84)
        {
          display.Init();
          animi = 0;
          animt = 1;
        }
      } else { 
        invert = CRect(rcWindow.left, rcWindow.top + animi, rcWindow.right, rcWindow.top + animi+1);
        if (++animi == 48)
        {
          display.Init();
          animi = 0;
          animt = 0;
        }
      }
      int l = invert.Width() + invert.Height()-1;
      _ASSERT(l <= COUNT(temp));
      BIOS::LCD::BufferBegin(invert);
      BIOS::LCD::BufferRead(temp, l);
      BIOS::LCD::BufferEnd();
      for (int i=0; i<l; i++)
        temp[i] = (temp[i] & 0x8000) ? RGB565(404040) : RGB565(ffffff);

      BIOS::LCD::BufferBegin(invert);
      BIOS::LCD::BufferWrite(temp, l);
      BIOS::LCD::BufferEnd();
      int i = 0;
      for (int y=0; y<6; y++)
        for (int x=0; x<84; x++, i++)
        {
          CRect pix(rcWindow.left+x, rcWindow.top+y*8, rcWindow.left+x+1, rcWindow.top+y*8+8);
          BIOS::LCD::BufferBegin(pix);
          BIOS::LCD::BufferRead(temp, l);
          BIOS::LCD::BufferEnd();
          uint8_t b = 0;
          for (int j=0; j<8; j++)
            if ((temp[7-j] & 0x8000))
              b |= 1<<j;
          buf[i] = b;
        }

      display.logoDisp(buf);
//      BIOS::SYS::DelayMs(20);
    }

    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::EMode::Input);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::EMode::Input);
    BIOS::GPIO::PinMode(BIOS::GPIO::P3, BIOS::GPIO::EMode::Input);
    BIOS::GPIO::PinMode(BIOS::GPIO::P4, BIOS::GPIO::EMode::Input);
    return 0;

}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}
