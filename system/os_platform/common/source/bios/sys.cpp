#include "Bios.h"
#include <math.h>

char gArgument[128] = "";
bool hasArgument = false;

void BIOS::SYS::DelayMs(int intervalMs)
{
  gHal->Delay(intervalMs);
  //assert(0);
}

uint32_t BIOS::SYS::GetTick()
{
  return gHal->GetTick();
}

int BIOS::SYS::Execute(uint32_t addr)
{
  assert(0);
  return 0;
}

void BIOS::SYS::Beep(int intervalMs)
{
}

void BIOS::OS::SetArgument(char* argument)
{
    _ASSERT(strlen(argument) < COUNT(gArgument)-1);
    strcpy(gArgument, argument);
    hasArgument = true;
}

char* BIOS::OS::GetArgument()
{
    return gArgument;
}

BIOS::OS::TInterruptHandler BIOS::OS::GetInterruptVector(BIOS::OS::EInterruptVector)
{
    return nullptr;
}

void BIOS::OS::SetInterruptVector(BIOS::OS::EInterruptVector, BIOS::OS::TInterruptHandler)
{
}

bool BIOS::OS::HasArgument()
{
	bool aux = hasArgument;
	hasArgument = false;
	return aux;
}

///// TODO: move to adc.cpp
#if defined(DS203) || defined(DS213)
//void BIOS::ADC::Init() {}
//bool BIOS::ADC::Ready() { return true; }
BIOS::ADC::EState BIOS::ADC::GetState() { return BIOS::ADC::EState::Full; }

int _adc_i = 0;
void BIOS::ADC::Restart(int) { _adc_i = 0;}
BIOS::ADC::TSample::SampleType BIOS::ADC::Get()
{
	int i = _adc_i++;
	
	BIOS::ADC::TSample s;
	 s.CH1 = (int)(sin(i*0.1)*110+128 + (random()%10));
	 s.CH2 = (i%60)*2 + (random()%5);
	 s.CH3 = 0;
	 s.CH4 = 0;
	return s.value;
	
}

int BIOS::ADC::GetPointer() { return 0; }
bool BIOS::ADC::Enable(bool bEnable) { return true; }
bool BIOS::ADC::Enabled() { return true; }

void BIOS::ADC::ConfigureInput(EInput input, ECouple couple, EResolution res, int offset) {}
void BIOS::ADC::ConfigureTimebase(float timePerDiv) {}
void BIOS::ADC::ConfigureTrigger(int time, int value, ETriggerType type, EInput source) {}
#endif


// move to private.cpp

extern unsigned char font[256*14];
//extern "C" volatile char lastChar;

namespace BIOS
{
  namespace SYS
  {
    uintptr_t GetAttribute(BIOS::SYS::EAttribute eAttribute)
    {
      static uint32_t dummy = 0;
      static uint32_t dummy2 = 0;
      switch (eAttribute)
      {
        case EAttribute::CharRom: return (uintptr_t)font;
        case EAttribute::ScreenWidth: return 320;
        case EAttribute::ScreenHeight: return 240;
        case EAttribute::DiskSectorSize: return BIOS::FAT::SectorSize;
        case EAttribute::GpioStatus: return (uintptr_t)&dummy;
        case EAttribute::GpioI2cSpeed: return (uintptr_t)&dummy2;
//        case EInternal::LastChar: return (uintptr_t)&lastChar;
        default:
          break;
      }
      BIOS::DBG::Print("attr not defined (%d)", eAttribute);
//      _ASSERT(0);
      return 0;
    }
  }
}
