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
    while (app.IsRunning())
    {
	key = BIOS::KEY::GetKey();
	
        if (key != BIOS::KEY::None)
            app.WindowMessage(CWnd::WmKey, key);
		
        app.WindowMessage(CWnd::WmTick);
    }
    
    app.Destroy();
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
//    while (1);
}

