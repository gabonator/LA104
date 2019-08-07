#include <library.h>
#include "MainWnd.h"

CMainWnd app;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{    
    app.Create();
    app.WindowMessage( CWnd::WmPaint );

    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::Escape)
    {
        if (key != BIOS::KEY::None)
            app.WindowMessage(CWnd::WmKey, key);
        app.WindowMessage(CWnd::WmTick);

        if (BIOS::OS::HasArgument()) // was internally set
        {
            BIOS::OS::SetArgument(BIOS::OS::GetArgument()); // hack, keep flag set
            break;
        }
    }
    
    app.Destroy();
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
//    while (1);
}

