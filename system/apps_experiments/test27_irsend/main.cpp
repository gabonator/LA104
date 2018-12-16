// based on https://github.com/joeferner/stm32-wifi-ir
// and https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Samsung.cpp
// resources: http://elektrolab.wz.cz/katalog/samsung_protocol.pdf
// (nec protocol, different timing) https://www.sbprojects.net/knowledge/ir/nec.php
// https://codebender.cc/sketch:81232#IR%20Send%20Samsung%20TV%20Power.ino
// http://irdb.tk/codes/

#include <library.h>
#include "ir/ir_tx.h"

uint16_t code[80];
int codeLen = 0;

void enableIROut(int carrier)
{
  codeLen = 0;
}

void mark(int interval)
{
  code[codeLen++] = interval;
  if (codeLen > (int)(sizeof(code)/sizeof(code[0])))
    while (1);
}

void space(int interval)
{
  code[codeLen++] = interval;
  if (codeLen > (int)(sizeof(code)/sizeof(code[0])))
    while (1);
}

void sendSAMSUNG (unsigned long data)
{
	constexpr int SAMSUNG_BITS =         32;
	constexpr int SAMSUNG_HDR_MARK =   4500; //5000;// 4500
	constexpr int SAMSUNG_HDR_SPACE =  4500; //5000;// 4500
	constexpr int SAMSUNG_BIT_MARK =    560;
	constexpr int SAMSUNG_ONE_SPACE =  1690;
	constexpr int SAMSUNG_ZERO_SPACE =  560;
//	constexpr int SAMSUNG_RPT_SPACE =  2250;

	// Set IR carrier frequency
	enableIROut(38);

	mark(10000); //wtf??

	// Header
	mark(SAMSUNG_HDR_MARK);
	space(SAMSUNG_HDR_SPACE);

	// Data
	for (unsigned long  mask = 1UL << (SAMSUNG_BITS - 1);  mask;  mask >>= 1) {
		if (data & mask) {
			mark(SAMSUNG_BIT_MARK);
			space(SAMSUNG_ONE_SPACE);
		} else {
			mark(SAMSUNG_BIT_MARK);
			space(SAMSUNG_ZERO_SPACE);
		}
	}

	// Footer
	mark(SAMSUNG_BIT_MARK);
	space(10);  // Always end with the LED off
}

// in milliseconds
uint16_t S_pup[69]={
  10000, 4600,4350,700,1500,700,1500,700,1550,700,450,650,400,700,450,650,450,700,400,700,
  1500,700,1550,650,1550,700,450,650,450,700,400,700,400,700,400,700,400,700,1550,700,400,
  700,400,700,1550,650,450,700,400,700,400,700,1550,650,450,650,1600,650,1550,650,450,700,
  1500,700,1500,700,1550,650};

void send()
{
  sendSAMSUNG(0xE0E0D02F); //0xE0E048B7);

  static IrCode irCode;
  irCode.repeatCount = 1;
  irCode.gap = 100000;
  irCode.codeLength = 69; //codeLen;
  irCode.code = S_pup;//code;
  ir_tx_send(&irCode);
}

__attribute__((__section__(".entry"))) 
int main(void)
{ 
  BIOS::LCD::Clear(RGB565(202020));
  BIOS::OS::TInterruptHandler pOldTimerHandler = BIOS::OS::GetInterruptVector(BIOS::OS::ITIM1_UP_IRQ);
  BIOS::DBG::Print("Init... ");
  ir_tx_setup();
  BIOS::OS::SetInterruptVector(BIOS::OS::ITIM1_UP_IRQ, ir_irq);
  ir_tx_start();
  BIOS::DBG::Print("Ok. ");

  while (BIOS::KEY::GetKey() != BIOS::KEY::Escape)
  {
    BIOS::DBG::Print("Sending... ");
    send();
    BIOS::SYS::DelayMs(3000);
  }

  ir_tx_stop();
  BIOS::OS::SetInterruptVector(BIOS::OS::ITIM1_UP_IRQ, pOldTimerHandler);
  BIOS::DBG::Print("Exiting. ");

  return 0;
}
