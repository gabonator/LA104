#include "streamer/streamer.h"

#if !defined(EMULATED) 

namespace BIOS
{
#if defined(DS203)
#include "native/ds203.h"
#elif defined(DS213)
#include "native/ds213.h"
#else
  namespace LOW
  {
    uint32_t SetParam(int attribute, int value)
    {
      _ASSERT(0);
      return 0;
    }
  }
#endif

  namespace ADC
  {
    int index;
    uint32_t lastSample;

    int Transfer(int begin, int samples)
    {
      if (isStreaming())
      {
        _ASSERT(begin == 0);
        samples = min(samples, streamerBuffer.size());
        TERMINAL::Print("{bulk:%d,bps:8}", samples);
        TERMINAL::BulkTransfer(streamerBuffer, samples);
        return 0;
      }

      int count = samples*3;
      index = 0;
      lastSample = -1;

      TERMINAL::Print("{bulk:%d,bps:24}", count);
      for (int i=0; i<begin; i++)
        BIOS::ADC::Get();

      bool ok = TERMINAL::BulkTransfer(count, [](uint8_t* buf, int bytes)
      {
        for (int i=0; i<bytes; i++)
        {
          switch (index)
          {
            case 0:
              lastSample = BIOS::ADC::Get();
              buf[i] = lastSample & 0xff;
              lastSample >>= 8;
              index++;
            break;
            case 1:
              buf[i] = lastSample & 0xff;
              lastSample >>= 8;
              index++;
            break;
            case 2:
              buf[i] = lastSample & 0xff;
              index = 0;
            break;
          }
        }
      });

      if (!ok)
      {
        BIOS::DBG::Print("Bulk transfer failed!");
      }

      for (int i=begin+samples; i<BIOS::ADC::NumSamples; i++)
        BIOS::ADC::Get();

//        BIOS::ADC::Restart();
      return 0;
    }

    bool Ready2()
    {
      if (isStreaming())
        return streamerBuffer.size() >= 2048;

      return BIOS::ADC::GetState() == BIOS::ADC::EState::Full || BIOS::ADC::GetState() == BIOS::ADC::EState::Triggered;
    }

    void ConfigureTrigger2(int time, int value, BIOS::ADC::ETriggerType type, BIOS::ADC::EInput source)
    {
      if ((int)type == (int)BIOS::ADC::ETriggerType::None+1)
      {
/*
        BIOS::ADC::ConfigureTimebase(500e-9f);
        BIOS::ADC::ConfigureTrigger(0, 128, BIOS::ADC::ETriggerType::None, BIOS::ADC::EInput::CH1);
        BIOS::LOW::SetParam(BIOS::LOW::FPGA_SP_PERCNT_L, 0); // TODO: return back when leaving streaming mode
        BIOS::LOW::SetParam(BIOS::LOW::FPGA_SP_PERCNT_H, 0);
        BIOS::ADC::Get();
        BIOS::ADC::Get();
*/      streamerBegin();
//        adcConfigure();
//        streamerInit();
//        streamerStart();
      } else
      {
        if (isStreaming())
        {
          streamerEnd();
//          streamerStop();
//          streamerDeinit();
//          adcReset();
        }
        ConfigureTrigger(time, value*4, type, source);
      }
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

    void ConfigureInput2(EInput input, ECouple couple, EResolution res, int offset)
    {
      BIOS::ADC::ConfigureInput(input, couple, res, offset*4);
    }

    uint32_t SetParam(int attribute, int value)
    {
      return BIOS::LOW::SetParam(attribute, value);
    }
  }
}
#endif