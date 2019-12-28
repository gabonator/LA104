#if !defined(EMULATED) && defined(DS203)
namespace BIOS
{
  namespace DAC
  {
    void SetWave(uint16_t* buffer, int length)
    {
      if (buffer)
        BIOS::DAC::SetMode(BIOS::DAC::EMode::Buffer, buffer, length);
      else 
        BIOS::DAC::SetMode(BIOS::DAC::EMode::Square, nullptr, 0);
    }
  }
  namespace ADC
  {
    constexpr int maxBulkSamples = 12;
    char buffer[maxBulkSamples*5+1]; // stack?

    int Transfer(int begin, int samples)
    {
	TERMINAL::Print("{hex:%d}", samples*5);
	for (int i=0; i<begin; i++)
          BIOS::ADC::Get();

	int index = 0;
	while (index < samples)
        {
          int count = min(samples-index, maxBulkSamples);
          for (int i=0; i<count; i++)
          {
            uint32_t sample = BIOS::ADC::Get();
            sprintf(buffer+i*5, "%05x", sample & 0x000fffff);
          }
          TERMINAL::Write((uint8_t*)buffer, count*5);
          index += count;
        } 

	for (int i=begin+samples; i<BIOS::ADC::NumSamples; i++)
          BIOS::ADC::Get();

//        BIOS::ADC::Restart();
    }

    int ConfigureTimebase2(float f)
    {
	const static float arrTime[] =
		{100e-9f, 200e-9f, 500e-9f,
		1e-6f, 2e-6f, 5e-6f,
		10e-6f, 20e-6f, 50e-6f, 100e-6f, 200e-6f, 500e-6f,
		1e-3f, 2e-3f, 5e-3f,
		10e-3f, 20e-3f, 50e-3f, 100e-3f, 200e-3f, 500e-3f,
		1.0f};

      for (int i=0; i<COUNT(arrTime); i++)
      {
        float base = arrTime[i];
        float thresh = base * 0.1f;
        if (base * 0.9f <= f && f <= base*1.1f)
        {
          // align to constant
          f = base;
          BIOS::ADC::ConfigureTimebase(f);
          return 0;
        }
      }
      _ASSERT(0);
      return 0;
    }

  }
}
#endif