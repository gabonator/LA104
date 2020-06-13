CDeviceCC1101Interface gModem;

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

  int SetGain(int g)
  {
    gModem.SetGain(g);
    return gModem.GetGain();
  }

  int GetGain()
  {
    return gModem.GetGain();
  }

  int SetDataRate(int dr)
  {
    gModem.SetDataRate(dr);
    return gModem.GetDataRate();
  }

  int GetDataRate(int dr)
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
  gModem.SetFrequency(434424000UL); //434424000 keyfob
  gModem.DeltaGain(-100); 
  gModem.DeltaBandwidth(-100);
  gModem.DeltaBandwidth(9); // 8:203khz, 10: 135khz, 12 -> 101khz
  gModem.DeltaGain(+5); // 5:-12db
  gModem.SetDataRate(4000);

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



bool framerStart()
{
  if (!CC1101::Init())
    return false;
  CC1101::Start(); // start streaming
  return true;
}

void framerStop()
{
  CC1101::Stop();
}