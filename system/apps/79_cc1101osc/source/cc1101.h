#include "device/cc1101.h"
#include "streamer/streamer.h"

class CDeviceCC1101InterfaceX
{
public:
  int SetFrequency(int) {return 0;}
  int GetFrequency() {return 0;}
  int SetBandwidth(int) {return 0;}
  int GetBandwidth() {return 0;}
  int SetGain(int) {return 0;}
  int GetGain() {return 0;}
  int SetDataRate(int) {return 0;}
  int GetDataRate() {return 0;}
  int Init() {return 0;}
  int SetIdleState() {return 0;}
  int SetRxState() {return 0;}
};

CDeviceCC1101Interface gModem;

#include "device/send.h"

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

  int Transfer()
  {
    TERMINAL::BulkTransfer(streamerBuffer, streamerBuffer.size());
    return 0;
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

  int Send(uint16_t* data, int length, int divisor)
  {
    CArray<uint16_t> pulse(data, length);
    pulse.SetSize(length);

    int ok = SendPulses(pulse, divisor);
    gModem.SetRxState();

    return ok;
  }

  int Calibrate()
  {
    gModem.Calibrate();
    return true;
  }

  // direct modem control methods
  int Strobe(int code)
  {
    return gModem.UserStrobe(code);
  }

  int Write(int address, int value)
  {
    gModem.UserWrite(address, value);
    return gModem.UserRead(address);
  }

  int Read(int address)
  {
    return gModem.UserRead(address);
  }
}