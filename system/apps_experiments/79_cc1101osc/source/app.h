#include "app/attack.h"

void streamerSetPrescaler(int psc);

namespace APP
{
  uint32_t Info()
  {
    return (uint32_t)"{platform:'LA104',application:'CC1101-remote control',version:'1.0'}";
  }
  uint32_t GetConfigPtr()
  {
    return (uint32_t)&appConfig;
  }
  void Do()
  {
    CheckAttack();
  }
  void Init()
  {
    InitAttack();
  }
  void End()
  {
    streamerEnd();
    gModem.SetIdleState();
  }
  void SetPrescaler(int psc)
  {
    streamerSetPrescaler(psc);
  }
}