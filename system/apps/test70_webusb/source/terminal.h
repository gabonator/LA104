namespace TERMINAL
{
  void Write(uint8_t* p, int n)
  {
    for (int i=0; i<10; i++)
    {
      if (cdc_transmit(p, n))
        return;
      BIOS::SYS::DelayMs(5);
    }
    BIOS::DBG::Print("TX-Fail");
  }

  void Print(const char* msg, ...)
  {
    char buf[64];
    char* bbuf = buf; 

    va_list args;

    va_start( args, msg );
    vsprintf( bbuf, msg, args );

    _ASSERT(strlen(buf) < sizeof(buf)-8);
    Write((uint8_t*)buf, strlen(buf));
  }
}