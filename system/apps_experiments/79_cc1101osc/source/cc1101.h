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

  int Init()
  {
    if (!gModem.Init())
      return false;

  // 433.91, 135khz, 0dB	
  gModem.SetFrequency(433876000UL);
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
    return 0;
  }
}