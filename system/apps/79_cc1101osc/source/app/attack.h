#define __irq__ __attribute__((interrupt("IRQ"), optimize("O2")))

struct appConfig_t
{
  const char* structure{R"(["structure", "enabled", "timer", "samples", "pulses", "minimumFirst", "maximumFirst", "minimumOdd", "maximumOdd",
"minimumEven", "maximumEven", "reqPulses", "delay", "power", "inhibit", "duration"])"};
  uint32_t enabled{0};

  uint32_t timer{0};
  uint32_t samples{0};
  uint32_t pulses{0};

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

  uint32_t reqPulses{10};
  uint32_t delay{0};
  uint32_t power{0xb0};
//  uint32_t scrambleBits{40};
//  uint32_t scrambleBitrate{2000};
//  uint32_t inhibit{200};
  uint32_t inhibit{200};
  uint32_t duration{495};

  uint32_t impulseOfs{0};
  uint32_t impulses[128];

} appConfig;


//extern uint32_t streamerMeasTime;
//extern uint32_t streamerMeasTicks;
//extern uint32_t streamerMeasStartSamples;
extern volatile int totalSamples;
extern volatile int totalPulses;

uint32_t streamerCurrent();

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

char strThreadMessage[512] = {0};

void secondaryPush(int c)
{
  if (appConfig.enabled == 0)
    return;

/*
  if (appConfig.enabled == 3)
  {
    for (int i=0; i<20; i++)
      appConfig.impulses[i] = appConfig.impulses[i+1];
    appConfig.impulses[20] = c;
  }
  postAttackPulses++;

  if (waitAttack > 0)
  {
    waitAttack -= c;
    return;
  }
*/
  if (eventAt)
    return;
  if (c != 0 && (c < 18 || c > 28))
  {
/*
    if (appConfig.enabled == 3 && mcounter>5)
    {
      sprintf(strThreadMessage, "c:%d,x:%d,d:[", mcounter, c);
      char* p = strThreadMessage + strlen(strThreadMessage);
      for (int i=0;i<21; i++)
      {
        sprintf(p, "%d,", appConfig.impulses[i]);
        p += strlen(p);
      }
      strcat(p, "]\n");
    }
*/
    mcounter = 0;
    return;
  }
  if (mcounter++ >= appConfig.reqPulses)
  {
//    BIOS::SYS::Beep(10);

    streamerFlag = 0x1000;
    eventAt = totalSamples+appConfig.duration; // 49500 -> 1s, 4950 -> 100ms
    // since start
//    eventAt = streamerLastTimestamp - ((long)streamerMeasTicks + (long)totalSamples - (long)streamerMeasStartSamples)/20;
//    eventAt += 20; // 10ms
//    streamerLastTimestamp , streamerLastInterval

//    postAttackPulses = 0;
//    waitAttack = appConfig.inhibit*20;
//    performAttack = BIOS::SYS::GetTick();
    mcounter = 0;
//    DoAttack();
  }
}

uint32_t streamerCCR();
uint32_t streamerCurrent();

void CheckAttack()
{
  if (appConfig.enabled==8)
  {
    int ccr = streamerCCR(); // 0..1023
    ccr = (1024 - ccr)&511;
    int add = totalSamples;
    int sum = (ccr + add) / 10;
    sum %= BIOS::LCD::Width;
    int x0 = 0;
    int x1 = max(0,sum-ccr/10);
    int x2 = sum;
    BIOS::LCD::Bar(x0, 40, x1, 45, RGB565(00D000));
    BIOS::LCD::Bar(x1, 40, x2, 45, RGB565(b00000));
    BIOS::LCD::Bar(x2, 40, BIOS::LCD::Width, 45, RGB565(808080));

    int xx = (ccr/5) % BIOS::LCD::Width;
    BIOS::LCD::Bar(0, 60, xx, 65, RGB565(ff0000));
    BIOS::LCD::Bar(xx, 60, BIOS::LCD::Width, 65, RGB565(000000));
    return;
  }

  appConfig.timer = BIOS::SYS::GetTick();
  appConfig.samples = totalSamples;
  appConfig.pulses = totalPulses;
/*
  if (strThreadMessage[0])
  {
    BIOS::DBG::Print(strThreadMessage);
    strThreadMessage[0] = 0;
    appConfig.enabled = 1;
  }

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
*/
  if (eventAt != 0)
  {
    streamerFlag = 0x2000;

    while ((long)streamerCurrent() - eventAt < 0);

//    if ((long)streamerCurrent() - eventAt >= 0)
    {
      streamerFlag = 0x0000;
      eventAt = 0; 
    }
    gModem.FixWrite(0x36);
    //gModem.SetIdleState(); 
    BIOS::SYS::Beep(100);
    BIOS::SYS::DelayMs(3);
    gModem.FixWrite(0x34);
    //gModem.SetRxState(); 
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
