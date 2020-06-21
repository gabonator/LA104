namespace BIOS
{
namespace DBG
{
void Print(const char * format, ...)
{
	char buf[1024];

        va_list args;
        
        va_start( args, format );
        vsprintf( buf, format, args );
	std::cout << buf;
}


}
}

extern "C" {

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in ");
    BIOS::DBG::Print(file);
    BIOS::DBG::Print(" [%d]: %s\n", line, cond);
#ifdef __APPLE__
    //kill(getpid(), SIGSTOP);
#endif
    while (1);
}
}