#include <library.h>
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include "../../../os_host/source/framework/BufferedIo.h"

#include "file/shapes.h"
#include "file/layout.h"
#include "file/file.h"

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

bool DoFlashing(char* name)
{
    CBufferedReader fw;
    if ( !fw.Open(name) )
        return false;

    int count = 0xb720;
    volatile uint16_t* ptrSpi = (uint16_t*)0x40003C0C; // SPI3_DR_ADDRESS
//    volatile uint32_t* ptrSpiStatus = 0x40003C08; // SPI3 Status reg
    volatile uint32_t* bit_SPI3_txe = (volatile uint32_t *)0x42078104;
    volatile uint32_t* cdone = (volatile uint32_t*)0x4222010c;
    CONSOLE::Print("cdone = %d", *cdone);
    int counter; // volatile?

    // reset
    *((volatile uint32_t*)0x4221819c) = 0;
    *((volatile uint32_t*)0x42218198) = 0;
    counter = 7;
    while (counter--);

    *((volatile uint32_t*)0x42218198) = 1;
    counter = 0x1baf;
    while (counter--);
    CONSOLE::Print("cdone = %d", *cdone);

    CStream spiStream((uint8_t*)ptrSpi, 1);
    while (count--)
    {
      fw >> spiStream;
      while (bit_SPI3_txe == 0);
    }

    *((volatile uint32_t*)0x4221819c) = 1;

    fw.Close();
    BIOS::SYS::DelayMs(10);
    CONSOLE::Print("cdone = %d", *cdone);

    CONSOLE::Print("\n");
    return !!*cdone;
}

bool GetPin(uint32_t nPortBase, int nPin)
{
    uint32_t dwAddr = nPortBase;
    uint32_t dwIDR = ((uint32_t*)dwAddr)[2];
    return (dwIDR & (1<<nPin)) ? 1 : 0;
}                   

bool GetPB4()
{
    #define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */
    #define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
    #define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)

    return GetPin(GPIOB_BASE, 4);
}


CFileDialog mFile;
CFileFilterSuffix filter(".FPG");

__attribute__((__section__(".entry")))
int _main(void)
{    
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);
    //mReader.Open((char*)"avr/tinystar.hex");

    APP::Init("FPGA flasher");
    APP::Status("AG1KLPQ48");

    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
        if (mFile.ModalShow(nullptr, "Load firmware", &filter))
        {
            APP::Init("FPGA flasher");
            APP::Status("AG1KLPQ48");
            
            CONSOLE::Print("Flashing '%s'...\n", mFile.GetFilename());

            CONSOLE::Color(RGB565(b0b0b0));
            if (DoFlashing(mFile.GetFilename()))
            {
                CONSOLE::Color(RGB565(ffffff));
                CONSOLE::Print("Flashed successfully!\n");
            } else
            {
                CONSOLE::Color(RGB565(ffffff));
                CONSOLE::Print("Flashing failed!\n");
            }

            // Wait for key press
            while ((key = BIOS::KEY::GetKey()) == BIOS::KEY::None)
            {
                // display PB4_MISO
                bool b = GetPB4();
                BIOS::LCD::Bar(CRect(BIOS::LCD::Width-20, BIOS::LCD::Height-20,
                    BIOS::LCD::Width-2, BIOS::LCD::Height-2), 
                    b ? RGB565(ff0000) : RGB565(00ff00));
            }
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
