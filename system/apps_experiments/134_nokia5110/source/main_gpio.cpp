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

__attribute__((__section__(".entry")))
int main(void)
{
    APP::Init("Nokia 5110 display test"); //pcd8544
    CONSOLE::Print("P1 - RST\n");
    CONSOLE::Print("P2 - DC\n");
    CONSOLE::Print("P3 - DIN\n");
    CONSOLE::Print("P4 - CLK\n");
    CONSOLE::Print("GND - CE\n");
//enum {DC = BIOS::GPIO::P2, CE = -1, CLK = BIOS::GPIO::P4, DIN = BIOS::GPIO::P3, RST = BIOS::GPIO::P1, LOW=0, HIGH=1, OUTPUT=2};

    display.Init();
    int grow = 0;
    while (!BIOS::KEY::GetKey())
    {
      for (int i=0; i<504; i++)
        buf[i] = rand()&255;
      for (int i=0; i<grow; i++)
        buf[i] = 255;
      if (grow<504)
        grow++;
      else
        grow = 0;
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
