namespace BIOS
{
  namespace ADC
  {
    void Init() 
    {
    }

    bool Ready() 
    {
	return false;
    }

    BIOS::ADC::ERunState GetState() 
    {
	return BIOS::ADC::ERunState::Start;
    }

    void Restart() 
    {
    }

    BIOS::ADC::TSample::SampleType Get() 
    {
	return 0;
    }

    int GetPointer() 
    { 
	return 0;
    }

    void Enable(bool bEnable) 
    {
    }

    bool Enabled()
    { 
      return false; 
    }

    void ConfigureInput(BIOS::ADC::EInput input, BIOS::ADC::ECouple couple, BIOS::ADC::EResolution res, int offset) 
    {
    }

    void ConfigureTimebase(float timePerDiv) 
    {
    }

    void ConfigureTrigger(int time, int value, BIOS::ADC::ETriggerType type, BIOS::ADC::EInput source)
    {
    }

    // Configure buffer length!!!
  }
}
