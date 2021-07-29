#include <library.h>

namespace Sampler
{
	const int Samples =  BIOS::ADC::NumSamples;
	BIOS::ADC::TSample::SampleType memory[Samples];
	
	void Copy()
	{
//		BIOS::ADC::Restart(); //TODO: order?
		for (int i=0; i<Samples; i++)
		{
			memory[i] = BIOS::ADC::Get();
		}
	}
	
  BIOS::ADC::TSample::SampleType& GetAt(int i)
  {
    return memory[i % Samples];
  }
}
