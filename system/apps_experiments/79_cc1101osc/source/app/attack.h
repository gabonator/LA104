#define __irq__ __attribute__((interrupt("IRQ"), optimize("O2")))

struct appConfig_t
{
  uint32_t enabled{0};
uint32_t statTime{0};
uint32_t statTicks{0};
/*
  uint32_t minimumEven{20};
  uint32_t maximumEven{28};
  uint32_t minimumOdd{20};
  uint32_t maximumOdd{28};
*/

/*
  uint32_t minimumFirst{7000/20};
  uint32_t maximumFirst{9200/20};
  uint32_t minimumOdd{300/20};
  uint32_t maximumOdd{460/20};
  uint32_t minimumEven{1800/20};
  uint32_t maximumEven{5000/20};
*/

  uint32_t minimumFirst{0/20};
  uint32_t maximumFirst{0/20};
  uint32_t minimumOdd{360/20};
  uint32_t maximumOdd{600/20};
  uint32_t minimumEven{380/20};
  uint32_t maximumEven{600/20};

  uint32_t pulses{10};
  uint32_t delay{0};
  uint32_t power{0xb0};
//  uint32_t scrambleBits{40};
//  uint32_t scrambleBitrate{2000};
//  uint32_t inhibit{200};
  uint32_t inhibit{200};

  uint32_t impulseOfs{0};
  uint32_t impulses[128];

} appConfig;


extern uint32_t streamerMeasTime;
extern uint32_t streamerMeasTicks;
extern uint32_t streamerMeasStartSamples;
extern int totalSamples;

// 10ms
uint16_t attackBuffer[] = {5000, 0}; //{8000, 0, 8000, 0, 8000, 0, 1000};

void InitAttack()
{
  if (!appConfig.enabled)
    return;

  if (!CC1101::Init())
    BIOS::DBG::Print("CC1101 Init failed\n");

  //gModem.SetFrequency(434424000UL); //434424000 keyfob
  gModem.SetFrequency(433876000UL); //conrad / oregon

  gModem.DeltaGain(-100); 
  gModem.DeltaBandwidth(-100);
  gModem.DeltaBandwidth(9); // 8:203khz, 10: 135khz, 12 -> 101khz
  gModem.DeltaGain(+5); // 5:-12db
  gModem.SetDataRate(4000);
  gModem.SetOutputPower(appConfig.power);

  CC1101::Start(); // start streaming
}


long performAttack = 0;
long waitAttack = 0;
volatile long postAttackPulses = 0;
long eventAt = 0;

int mcounter = 0;

void DoAttack();

void secondaryPush(int c)
{
  appConfig.impulses[appConfig.impulseOfs++] = c;
  if (appConfig.impulseOfs == COUNT(appConfig.impulses))
    appConfig.impulseOfs = 0;

  postAttackPulses++;

  if (waitAttack > 0)
  {
    waitAttack -= c;
    return;
  }
  if (c < 20 || c > 25)
  {
    mcounter = 0;
    return;
  }
  if (mcounter++ >= appConfig.pulses)
  {
//    BIOS::SYS::Beep(10);

    streamerFlag = 0x1000;

    // since start
    eventAt = streamerLastTimestamp - ((long)streamerMeasTicks + (long)totalSamples - (long)streamerMeasStartSamples)/20;
    eventAt += 20; // 10ms
//    streamerLastTimestamp , streamerLastInterval

//    postAttackPulses = 0;
    waitAttack = appConfig.inhibit*20;
//    performAttack = BIOS::SYS::GetTick();
    mcounter = 0;
//    DoAttack();
  }
}

void CheckAttack()
{
  if (appConfig.enabled == 2)
  {
    int l = COUNT(appConfig.impulses);
    BIOS::DBG::Print("Dump: ");
int ii = postAttackPulses;
    for (int i=0; i<l; i++)
    {
      BIOS::DBG::Print("%d, ", appConfig.impulses[(ii+i)%l]);
    }
    BIOS::DBG::Print("               ---\n");
    appConfig.enabled = 1;
  }
  appConfig.statTime = streamerMeasTime;
  appConfig.statTicks = streamerMeasTicks;
  if (eventAt != 0)
  {
    BIOS::SYS::Beep(100);
    if (BIOS::SYS::GetTick() - eventAt >= 0)
    {
      streamerFlag = 0x0000;
      eventAt = 0; 
    }
  }
/*
"[21,25,20,26,23,24,21,22,23,26,22,23,23,25,20,26,20,25,22,24,22,24,22,25,24,22,22,24,22,25,21,25,22,25,21,25,21,25,22,24,22,23,7,46,
 9,104,39,53,42,52,43,52,19,26,44,50,21,25,45,25,22,49,22,25,21,22,23,26,22,25,44,49,21,25,22,26,20,25,21,25,45,49,47,48,22,25,45,49,
 45,48,22,26,22,23,23,23,22,26,20,25,21,25,22,24,22,25,22,24,22,24,22,25,22,24,21,25,22,25,21,25,20,26,20,25,23,25,20,26,46,49,20,26,
 20,25,21,25,45,26,22,48,22,25,46,46,22,26,45,50,45,48,47,24,25,45,22,26,20,25,21,25,21,27,20,25,21,25,21,25,22,25,21,


 25,4,18,1,49,1,
 2,3,74,2,31,41,29,18,51,20,27,43,51,19,27,21,25,21]"
*/
return;

  long now = BIOS::SYS::GetTick();
  if (performAttack != 0)
  {
    long delay = now - performAttack;
    int lostPulses = postAttackPulses;
    DoAttack();
    BIOS::DBG::Print("Attack delay %d, lost %d\n", delay, lostPulses);
    performAttack = 0;
  }
  return;

  static long lastUsed = 0;
  if (!appConfig.enabled)
    return;

//  long now = BIOS::SYS::GetTick();
  if (lastUsed != 0 && now-lastUsed<appConfig.inhibit)
    return;

  _ASSERT(appConfig.pulses <= COUNT(streamerSecondary));

  auto CheckPulses = [](int shift) -> bool 
  {
    int ind = (streamerSecondaryIndex + 64 - 1)%64;
    int pulse = streamerSecondary[ind];
                  
    if (appConfig.minimumFirst != 0)
    {
      if (pulse < appConfig.minimumFirst)
        return false;  
      if (pulse > appConfig.maximumFirst)
        return false; 
      ind = (ind + 63) & 63;
    }

//    BIOS::SYS::Beep(10);

    for (int i=0; i<appConfig.pulses; i++) 
    {
      pulse = streamerSecondary[ind];
      ind = (ind + 63) & 63;

      if ((i&1)==shift)
      {
        if (pulse < appConfig.minimumOdd)
          return false;  
        if (pulse > appConfig.maximumOdd)
          return false; 
      } else
      {
        if (pulse < appConfig.minimumEven)
          return false; 
        if (pulse > appConfig.maximumEven)
          return false; 
      }
    }
    return true;
  };

  if (!CheckPulses(0) && !CheckPulses(1))
     return;

  lastUsed = now;
  DoAttack();
  BIOS::SYS::DelayMs(appConfig.inhibit);
  for (int i=0; i<COUNT(streamerSecondary); i++)
  {
    streamerSecondary[i] = 0;
  }
}

void DoAttack()
{
  streamerFlag = 0x1000;

  BIOS::SYS::Beep(100);
//  BIOS::SYS::DelayMs(appConfig.delay);
  CArray<uint16_t> pulse(attackBuffer, COUNT(attackBuffer));
  pulse.SetSize(COUNT(attackBuffer));

  int prevDataRate = gModem.GetDataRate();
  gModem.SetDataRate(1000000UL/500);

  // pause dma streaming??
  streamerFlag = 0x2000;

  SendPulses(pulse, 500);
  streamerFlag = 0x4000;

  gModem.SetRxState(); 
  gModem.SetDataRate(prevDataRate);
  streamerFlag = 0x0000;
}
