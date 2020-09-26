#include <library.h>
#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/framework/Serialize.h"

using namespace BIOS;

#include "bitmap.h"

uint8_t mFileSystemBuffer[BIOS::FAT::SectorSize];

__attribute__((__section__(".entry")))
int main(void)
{    
    BIOS::FAT::SetSharedBuffer(mFileSystemBuffer);
    DrawImage((char*)"logo.bmp", 0, 0);
    BIOS::FAT::SetSharedBuffer(nullptr);
    while (BIOS::KEY::GetKey() != BIOS::KEY::Escape)
    {
    }
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

