#include "streamer/streamer.h"

#if !defined(EMULATED) && defined(DS203)
namespace BIOS
{
  namespace LOW
  {
    enum {
      CH_A_OFFSET=       0,     // A channel vertical displacement Value = 0 ~ 200
      CH_B_OFFSET=       1,     // B-channel vertical displacement Value = 0 ~ 200
      BACKLIGHT=         2,     // Backlight Brightness Value = 0 ~ 100
      BEEP_VOLUME=       3,     // Buzzer Volume Value = 0 ~ 100
      BETTERY_DT=        4,     // battery voltage detection Value = 1: start
      ADC_MODE=          5,     // ADC operating mode Value = 1 / 0
      FIFO_CLR=          6,     // FIFO Pointer Reset Value = 1 / 0: W_PTR / R_PTR
      //#define R_PTR           0       // FIFO read address pointer is reset ( Note: Seems to be used for something else in FPGA V2.50)
      W_PTR=           1,       // FIFO write & read address pointer is reset
      T_BASE_PSC=        7,     // Time Base Prescale Value = 0 ~ 65535
      T_BASE_ARR=        8,     // time base divider value Value = 0 ~ 65535
      CH_A_COUPLE=       9,     // A channel coupling Value = 1 / 0: AC / DC
      CH_A_RANGE=       10,     // A channel input range Value = 0 ~ 5
      CH_B_COUPLE=      11,     // B-channel coupling Value = 1 / 0: AC / DC
      CH_B_RANGE=       12,     // B channel input range Value = 0 ~ 5
      ANALOG_ARR=       13,     // analog output divider value Value = 0 ~ 65535
      ANALOG_PTR=       14,     // analog output pointer Value = 0 ~ 65535
      ANALOG_CNT=       15,     // number of points per cycle synthesis Value = 0 ~ 65535
      DIGTAL_PSC=       16,     // pulse output prescaler Value = 0 ~ 65535
      DIGTAL_ARR=       17,     // pulse output divider value Value = 0 ~ 65535
      DIGTAL_CCR=       18,     // pulse output duty cycle value Value = 0 ~ 65535
      KEY_IF_RST=       19,     // timer interrupt flag is reset Value = 0
      STANDBY=          20,     // waiting to enter the power-down Value = 0
      FPGA_RST=         31,     // FPGA Reset Value = 0

      TRIGG_MODE=       32+0,  // trigger mode Value = Mode
      V_THRESHOLD=      32+1,  // voltage trigger threshold Value = 0 ~ 200
      T_THRESHOLD=      32+2,  // pulse width trigger time threshold Value = 0 ~ 65535
      ADC_CTRL=         32+4,  // ADC work status Set Value = 1 / 0 EN / DN
      A_POSITION=       32+5,  // CH_A zero point Value = 0 ~ 200
      B_POSITION=       32+6,  // CH_B zero point Value = 0 ~ 200
      REG_ADDR=         32+7,  // the address determines which set of registers in the FPGA the data read by the MCU

      // FPGA register addresses for Set_Param
      FPGA_SP_TRIGGMODE=    1024+0, // Note: resets other settings to defaults
      FPGA_SP_VTHRESHOLD=   1024+1,
      FPGA_SP_TTHRESHOLD_L= 1024+2, // Time threshold, bits 0..7
      FPGA_SP_TTHRESHOLD_H= 1024+3, // Time threshold, bits 8..15
      FPGA_SP_CTRLREG=      1024+4, // Control register
      FPGA_SP_SELECT=       1024+5, // Read selector for reading trigger results
      FPGA_SP_DEPTH_L=      1024+6, // Sample count, bits 0..7
      FPGA_SP_DEPTH_H=      1024+7, // Sample count, bits 8..11
      FPGA_SP_PERCNT_L=     1024+8, // Presample count, bits 0..7
      FPGA_SP_PERCNT_H=     1024+9, // Presample count, bits 8..11
      FPGA_SP_DELAY_1=     1024+10, // Trigger delay, bits 0..7
      FPGA_SP_DELAY_2=     1024+11, // Trigger delay, bits 8..15
      FPGA_SP_DELAY_3=     1024+12, // Trigger delay, bits 16..23
      FPGA_SP_DELAY_4=     1024+13, // Trigger delay, bits 24..31
    };

    uint32_t SetParam(int attribute, int value)
    {
      #define BiosBase(addr) (void*)(0x08004200+addr)
      if (attribute == 9999)
      {
        return BIOS::ADC::Get();
      } else
      if (attribute == 9990)
      {
        streamerPause();
        return 0;
      } else
      if (attribute == 9991)
      {
        streamerResume();
        return 0;
      } else
      if (attribute & 1024)
      {
        attribute &= 1023;
        uint32_t (*__Set_Param)(uint8_t, uint8_t) = (uint32_t (*)(uint8_t, uint8_t)) BiosBase(0x51);
        __Set_Param(attribute, value);
        return 0;
      } else
      {
        if (value == -1)
        {
          uint32_t (*__Get)(uint8_t) = (uint32_t (*)(uint8_t)) BiosBase(0x55);
          return __Get(attribute);
        }

        void (*__Set)(uint8_t, uint32_t) = (void (*)(uint8_t, uint32_t)) BiosBase(0x4D);
        __Set(attribute, value);
        return 0;
      }
    }
  }

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

      return BIOS::ADC::Ready();
    }

    void ConfigureTrigger2(int time, int value, ETriggerType type, EInput source)
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
        ConfigureTrigger(time, value, type, source);
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

    uint32_t SetParam(int attribute, int value)
    {
      return BIOS::LOW::SetParam(attribute, value);
    }
  }
}
#endif