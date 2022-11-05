namespace Platform
{
    typedef int32_t time_t;
    void (*redirect)(const char*){nullptr};

    time_t millis()
    {
        return BIOS::SYS::GetTick();
    }

    void delay(int ms)
    {
        BIOS::SYS::DelayMs(ms);
    }

    void log_w(const char* msg, ...)
    {
        char buf[128];
        va_list args;
        va_start( args, msg );
        vsprintf( buf, msg, args );
        va_end(args);
        if (redirect)
        {
          redirect(buf);
          return;
        }
        BIOS::DBG::Print("Warning: ");
        BIOS::DBG::Print(buf);
        BIOS::DBG::Print("\n");
    }

    void log_e(const char* msg, ...)
    {
        char buf[128];
        va_list args;
        va_start( args, msg );
        vsprintf( buf, msg, args );
        va_end(args);
        if (redirect)
        {
          redirect(buf);
          return;
        }
        BIOS::DBG::Print("Error: ");
        BIOS::DBG::Print(buf);
        BIOS::DBG::Print("\n");
    }

    void log_i(const char* msg, ...)
    {
        char buf[128];
        va_list args;
        va_start( args, msg );
        vsprintf( buf, msg, args );
        va_end(args);
        if (redirect)
        {
          redirect(buf);
          return;
        }
        BIOS::DBG::Print("Info: ");
        BIOS::DBG::Print(buf);
        BIOS::DBG::Print("\n");
    }

}
