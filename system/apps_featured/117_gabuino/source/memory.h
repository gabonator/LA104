void EventLoop();

namespace MEMORY
{
  uint8_t* writePtr = nullptr;
  int writeCount = 0;
  bool running = false;

  int Write(uint32_t address, uint32_t count)
  {
    writePtr = (uint8_t*)address;
    writeCount = count;
    return 1;
  }

  void HandleWrite(uint8_t* buf, int count)
  {
//    BIOS::DBG::Print("c=%d,wc=%d,b=%s", count, writeCount, buf);
    writeCount -= count;
    _ASSERT(writeCount >= 0);
    while (count--)
      *writePtr++ = *buf++;
  }

  int Read(uint32_t ptr, int n)
  {
    uint8_t* p = (uint8_t*)ptr;

    TERMINAL::Print("{bulk:%d,bps:8}", n);
    if (!TERMINAL::BulkTransfer(p, n))
      return 0;
    return 1;
  }

  int Done()
  {
    return 1;
  }

  void Zero(uint32_t ptr, uint32_t count)
  {
    uint8_t* p = (uint8_t*)ptr;
    while (count--)
      *p++ = 0;
  }

  int Exec(uint32_t ptr)
  {
    typedef int(*TFunc)();
    running = true;
    int retval = ((TFunc)ptr)();
    running = false;
    return retval;
  }

  int Stop()
  {
    running = false;
    return 0;
  }

  int GetKey2()
  {
//    EventLoop();
    if (running)
      return BIOS::KEY::GetKey();
    else
      return BIOS::KEY::EKey::Escape;
  }

  uint32_t GetProcAddress2(char* name)
  {
    if (strcmp(name, "_ZN4BIOS3KEY6GetKeyEv") == 0)
      return (uint32_t)MEMORY::GetKey2;
    return BIOS::SYS::GetProcAddress(name);
  }
/*
  int Trace()
  {
    static uint32_t *SP;
    asm("mrs %0, msp" : "=r"(SP) : :);

    uintptr_t estack = BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::EndStack)
    uint32_t *p = sp+6;
    while (i < 10 && p < &estack)
    {
        if ((*p >> 24) == 0x08)
            callers[i++] = *p;        
        p++;
    }

    return 0;
  }   

  void SaveStack()
  {
  }
*/
}

