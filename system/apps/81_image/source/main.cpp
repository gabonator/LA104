#include <library.h>
#include "../../os_host/source/framework/BufferedIo.h"
#include "../../os_host/source/framework/Serialize.h"

using namespace BIOS;

#include "file/shapes.h"
#include "file/layout.h"
#include "file/file.h"

#include "bitmap.h"

uint8_t mFileSystemBuffer[BIOS::FAT::SectorSize];
CFileDialog mFile;
CFileFilterSuffix filter(".BMP");

__attribute__((__section__(".entry")))
int main(void)
{    
    BIOS::FAT::SetSharedBuffer(mFileSystemBuffer);
    BIOS::KEY::EKey key = BIOS::KEY::None;

    while (key != BIOS::KEY::Escape)
    {
        if (mFile.ModalShow(nullptr, "Load bitmap", &filter))
        {
            if (!DrawImage(mFile.GetFilename(), 0, 0))
            {
                BIOS::DBG::Print("Unsupported image format!\n");
            }

            // Wait for key press
            while ((key = BIOS::KEY::GetKey()) == BIOS::KEY::None);
        } else
        {
            break;
        }
    }

    BIOS::FAT::SetSharedBuffer(nullptr);
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

