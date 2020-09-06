CDeviceCC1101Interface gModem;

bool analyse(CArray<uint16_t>&);


#include "../../os_host/source/framework/BufferedIo.h"
uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];
CBufferedWriter mWriter;

// 500 420 480 460 940 940
namespace PULSE
{
    uint16_t arrSignalData[400];
    CArray<uint16_t> arrSignal(arrSignalData, COUNT(arrSignalData));

    void uniqueName(char* name)
    {
        static int counter = 0;
        sprintf(name, "dump%03d.txt", counter++);
    }

    void dump(char* fname, CArray<uint16_t>& data)
    {
        _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
        BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);
        mWriter.Open(fname);

        auto Print = [&](const char* format, ...)
        {
          char buf[32];
          
          va_list args;
          va_start( args, format );
          vsprintf( buf, format, args );
          va_end(args);

          mWriter << buf;
        };

        Print("%d: ", data.GetSize());
        for (int i=0; i<data.GetSize(); i++)
            Print("%d, ", data[i]);
        Print("\r\n");
        
        mWriter.Close();
        BIOS::FAT::SetSharedBuffer(nullptr);
    }

    void push(int v)
    {
        if (v==-1)
        {
            if (arrSignal.GetSize() > 100 && arrSignal.GetSize() < 500)
            {
                analyse(arrSignal);
            }
                
            arrSignal.RemoveAll();
            return;
        }

        if (arrSignal.GetSize() < arrSignal.GetMaxSize())
            arrSignal.Add(v);
    }

    void join(int interval)
    {
        // L:1000, H:0, L:1000, H:0, L:300, H:1000, L:0, H:1000, L:0, H:100, L:500, H:500, L:500, H:500
        // -> L:2300, H:2100, L:500, H:500, L:500, H:500

        static int interval1 = 0;
        static int interval2 = 0;
        static bool leading = true;
        static bool terminated = false;

        if (interval == -1)
        {
            interval1 = 0;
            interval2 = 0;
            leading = true;
            push(-1);
            return;
        }
        if (interval1 == 0)
            interval += interval2;

        if (interval > 12000) // 12 ms max
        {
            if (!terminated)
            {
                terminated = true;
                push(-1);
            }
            leading = true; // aj tak ho potom pretlaci cez interval2!!
        }

        if (interval1 != 0 && interval2 != 0)
        {
            if (leading)
                leading = false;
            else
            {
                push(interval2);
                terminated = false;
            }
        }

        interval2 = interval1;
        interval1 = interval;
    }
}

namespace CC1101
{
  int SetFrequency(int f)
  {
    gModem.SetFrequency(f);
    return gModem.GetFrequency();
  }

  int GetFrequency()
  {
    return gModem.GetFrequency();
  }

  int SetBandwidth(int bw)
  {
    gModem.SetBandwidth(bw);
    return gModem.GetBandwidth();
  }

  int GetBandwidth()
  {
    return gModem.GetBandwidth();
  }

  void DeltaBandwidth(int d)
  {
    gModem.DeltaBandwidth(d);
  }

  int SetGain(int g)
  {
    gModem.SetGain(g);
    return gModem.GetGain();
  }

  int GetGain()
  {
    return gModem.GetGain();
  }

    void DeltaGain(int d)
    {
      gModem.DeltaGain(d);
    }

  int SetDataRate(int dr)
  {
    gModem.SetDataRate(dr);
    return gModem.GetDataRate();
  }

  int GetDataRate()
  {
    return gModem.GetDataRate();
  }

  int SetOutputPower(int power)
  {
    gModem.SetOutputPower(power);
    return gModem.GetOutputPower();
  }

  int GetOutputPower()
  {
    return gModem.GetOutputPower();
  }

  int Init()
  {
    if (!gModem.Init())
      return false;

  // 433.91, 135khz, 0dB	
//  gModem.SetFrequency(433876000UL);
/*
  gModem.SetFrequency(434424000UL); //434424000 keyfob
  gModem.DeltaGain(-100); 
  gModem.DeltaBandwidth(-100);
  gModem.DeltaBandwidth(9); // 8:203khz, 10: 135khz, 12 -> 101khz
  gModem.DeltaGain(+5); // 5:-12db
  gModem.SetDataRate(4000);
*/
    gModem.SetRxState();
    return true;
  }

  int Start()
  {
    gModem.SetRxState();
    streamerBufferMaxCounter = 4000;
    streamerBegin();
    return true;
  }

  int Stop()
  {
    streamerEnd();
    gModem.SetIdleState();
    return true;
  }

  int Status()
  {
    if (streamerOverrun)
    {
      streamerOverrun = 0;
      return 'E' * 256 + 'O';
    }

    return 0;
  }
}

bool framerGet()
{
    return gModem.Read();
}

bool framerStart()
{
  if (!CC1101::Init())
    return false;

  CC1101::SetFrequency(433950000);
  CC1101::SetBandwidth(101000);
  CC1101::SetGain(-14);
  CC1101::SetDataRate(4000);
    
  CC1101::Start(); // start streaming
  return true;
}

void framerStop()
{
  CC1101::Stop();
}

void framerLoop()
{
    int n = streamerBuffer.size();

    for (int i=0; i<n; i++)
    {
        int v = streamerBuffer.pull()*20;
        PULSE::join(v);
    }
}
