namespace Platform
{
    typedef int32_t time_t;

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
/*
        char buf[128];
        CONSOLE::Print("Warning: ");
        va_list args;
        va_start( args, msg );
        vsprintf( buf, msg, args );
        va_end(args);
        CONSOLE::Print(buf);
        CONSOLE::Print("\n");
*/
    }

    void log_e(const char* msg, ...)
    {
/*
        char buf[128];
        CONSOLE::Print("Error: ");
        va_list args;
        va_start( args, msg );
        vsprintf( buf, msg, args );
        va_end(args);
        CONSOLE::Print(buf);
        CONSOLE::Print("\n");
*/
    }

    void log_i(const char* msg, ...)
    {
/*
        char buf[128];
        CONSOLE::Print("Info: ");
        va_list args;
        va_start( args, msg );
        vsprintf( buf, msg, args );
        va_end(args);
        CONSOLE::Print(buf);
        CONSOLE::Print("\n");
*/
    }
}

