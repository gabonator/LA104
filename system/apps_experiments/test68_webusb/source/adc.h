#if !defined(EMULATED) && defined(DS203)
namespace BIOS
{
  namespace ADC
  {
    constexpr int maxBulkSamples = 12;
    char buffer[maxBulkSamples*5+1]; // stack?

    int Transfer()
    {
	constexpr int samples = 128; //BIOS::ADC::NumSamples;

	TERMINAL::Print("{hex:%d}", samples*5);
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
        BIOS::ADC::Restart();
    }
  }
}
#endif