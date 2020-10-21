#include <library.h>
using namespace BIOS;

#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/framework/Console.h"
#include "framework/shapes.h"
#include "framework/layout.h"
#include "framework/block.h"
#include "framework/menu.h"
#include "framework/scroller.h"
#include "framework/file.h"

#include "mfrc522/MFRC522.h"

#include "mainmenu.h"
#include "rfid.h"
#include "application.h"

CApplication app;

void mainInit()
{
#ifdef __APPLE__
    BIOS::FAT::Init();
    BIOS::OS::SetArgument((char*)"RFID/RFIDTOOL.ELF");
#endif
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);

    app.Create();
    app.WindowMessage( CWnd::WmPaint );
#ifdef TEST
    app.Test();
#endif
}

bool mainLoop()
{
    BIOS::KEY::EKey key;
    key = KEY::GetKey();
    if (key != BIOS::KEY::None)
        app.WindowMessage(CWnd::WmKey, key);
    app.WindowMessage(CWnd::WmTick);

    return key != KEY::EKey::Escape;
}

void mainFinish()
{
    app.Destroy();
    BIOS::FAT::SetSharedBuffer(nullptr);
}

#ifdef _ARM
/*
extern "C" { void* __dso_handle __attribute__ ((__weak__)); }
extern "C" { void* _fini __attribute__((weak)); }

int __cxa_atexit(void (*func) (void *), void * arg, void * dso_handle)
{
    
}
*/

//uint32_t dsoPtr[16] = {0};
//uint32_t finiPtr[16] = {0};
__attribute__((__section__(".entry")))
int _main(void)
{
//    __dso_handle = dsoPtr;
//    _fini = finiPtr;
#else
int _main(void)
{
#endif
	mainInit();
	while (mainLoop());
	mainFinish();
    return 0;
}

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
