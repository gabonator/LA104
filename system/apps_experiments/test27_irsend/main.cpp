#include <library.h>
#include "ir/ir_tx.h"

class CSamsungGenerator
{
public:
  enum 
  {
    VolumeUp =     0xE0E0E01F,
    VolumeDown =   0xE0E0D02F,
    ChannelUp =    0xE0E048B7,
    ChannelDown =  0xE0E008F7,
    Power =        0xE0E040BF
  };

public:
  void GetSequence(uint32_t code, CArray<uint16_t>& sequence)
  {
    constexpr int SAMSUNG_BITS =         32;
    constexpr int SAMSUNG_HDR_MARK =   4500; //5000;// 4500
    constexpr int SAMSUNG_HDR_SPACE =  4500; //5000;// 4500
    constexpr int SAMSUNG_BIT_MARK =    560;
    constexpr int SAMSUNG_ONE_SPACE =  1690;
    constexpr int SAMSUNG_ZERO_SPACE =  560;
    //	constexpr int SAMSUNG_RPT_SPACE =  2250;

    sequence.RemoveAll();

    // Set IR carrier frequency
    enableIROut(38);

    // Header
    mark(sequence, SAMSUNG_HDR_MARK);
    space(sequence, SAMSUNG_HDR_SPACE);

    // Data
    for (unsigned long  mask = 1UL << (SAMSUNG_BITS - 1);  mask;  mask >>= 1) 
    {
    	if (code & mask) {
    		mark(sequence, SAMSUNG_BIT_MARK);
    		space(sequence, SAMSUNG_ONE_SPACE);
    	} else {
    		mark(sequence, SAMSUNG_BIT_MARK);
    		space(sequence, SAMSUNG_ZERO_SPACE);
    	}
    }

    // Footer
    mark(sequence, SAMSUNG_BIT_MARK);
    space(sequence, 10);  // Always end with the LED off
  }

private:
  void enableIROut(int frequencyKhz)
  {
  }

  void mark(CArray<uint16_t>& sequence, int duration)
  {
    _ASSERT(sequence.GetSize() + 1 < sequence.GetMaxSize());
    sequence.Add(duration);
  }

  void space(CArray<uint16_t>& sequence, int duration)
  {
    _ASSERT(sequence.GetSize() + 1 < sequence.GetMaxSize());
    sequence.Add(duration);
  }
};

class CInfrared
{
  static IrCode irCode;

public:
  void Init()
  {
    ir_tx_setup();
    BIOS::OS::SetInterruptVector(BIOS::OS::ITIM1_UP_IRQ, ir_irq);
    ir_tx_start();
  }

  void Send(CArray<uint16_t>& sequence)
  {
    irCode.repeatCount = 1;
    irCode.gap = 0;
    irCode.codeLength = sequence.GetSize();
    irCode.code = sequence.GetData();
    ir_tx_send(&irCode);
  }
};

uint16_t gSequenceData[80];
CArray<uint16_t> gSequence;

IrCode CInfrared::irCode;
CSamsungGenerator samsung;
CInfrared infrared;

void Send(const char* id, int type)
{
  BIOS::DBG::Print("<%s> ", id);
  samsung.GetSequence(type, gSequence);
  infrared.Send(gSequence);
}

__attribute__((__section__(".entry"))) 
int main(void)
{ 
  gSequence.Init(gSequenceData, COUNT(gSequenceData));

  BIOS::LCD::Clear(RGB565(202020));
  BIOS::OS::TInterruptHandler pOldTimerHandler = BIOS::OS::GetInterruptVector(BIOS::OS::ITIM1_UP_IRQ);
  BIOS::DBG::Print("Init... ");
  infrared.Init();
  BIOS::DBG::Print("Ok. ");

  BIOS::KEY::EKey key;
  while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
  {
    switch (key)
    {
      case BIOS::KEY::Up: Send("volume+", CSamsungGenerator::VolumeUp); break;
      case BIOS::KEY::Down: Send("volume-", CSamsungGenerator::VolumeDown); break;
      case BIOS::KEY::Left: Send("ch-", CSamsungGenerator::ChannelDown); break;
      case BIOS::KEY::Right: Send("ch+", CSamsungGenerator::ChannelUp); break;
      case BIOS::KEY::F1: Send("power", CSamsungGenerator::Power); break;
      default: break;
    }
  }

  ir_tx_stop();
  BIOS::OS::SetInterruptVector(BIOS::OS::ITIM1_UP_IRQ, pOldTimerHandler);
  BIOS::DBG::Print("Exiting. ");

  return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}
