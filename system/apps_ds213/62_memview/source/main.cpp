#include <library.h>
#include "application.h"

CApplication app;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{    
    app.Create("Application", CWnd::WsVisible, CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), nullptr);
	app.SetFocus();
    app.WindowMessage( CWnd::WmPaint );

    BIOS::KEY::EKey key = BIOS::KEY::None;
    while (key != BIOS::KEY::Escape) //(app.IsRunning())
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

