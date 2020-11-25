#include <library.h>
#include "avrisp.h"
#include "../../../os_host/source/framework/Console.h"
#include "../../../os_host/source/framework/SimpleApp.h"
#include "../../../os_host/source/framework/BufferedIo.h"
#include "ihex.h"

#include "file/shapes.h"
#include "file/layout.h"
#include "file/file.h"

uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];

static int bufferBase = -1;
static uint8_t buffer[64];
static int bufferLen = 0;

bool HexProcess(uint32_t address, uint8_t* data, int length, bool(*commitHandler)(uint32_t address, uint8_t* data, int length))
{
    int base = address & ~63;
    if (bufferBase != base)
    {
        // commit
        if (bufferLen > 0)
        {
            if (!commitHandler(bufferBase, buffer, bufferLen))
                return false;
            bufferLen = 0;
        }
        bufferBase = base;
    }
    
    for (int i=0; i<length; i++)
    {
        buffer[bufferLen++] = data[i];
        if (bufferLen == 64)
        {
            if (!commitHandler(bufferBase, buffer, bufferLen))
                return false;
            bufferLen = 0;
            bufferBase += 64;
        }
    }
    
    return true;
}

bool HexLoad(char* strFile, bool(*commitHandler)(uint32_t offset, uint8_t* data, int len) )
{
    CBufferedReader fw;
    if ( !fw.Open( strFile ) )
        return false;

    IHexRecord irec;
    uint16_t addressOffset = 0x00;
    uint32_t address = 0x0;
    int ihexError;

    while ((ihexError = Read_IHexRecord(&irec, fw)) == IHEX_OK)
    {
        switch(irec.type)
        {
            case IHEX_TYPE_00:    /**< Data Record */
                address = (((uint32_t) addressOffset) << 16)+ irec.address;
                if (!HexProcess(address, irec.data, irec.dataLen, commitHandler))
                {
                    fw.Close();
                    return false;
                }
            break;

            case IHEX_TYPE_04:    /**< Extended Linear Address Record */
                addressOffset = (((uint16_t) irec.data[0]) << 8 ) + irec.data[1];
                //CONSOLE::Print("adr %08x\n", addressOffset);
            break;

            case IHEX_TYPE_01:    /**< End of File Record */
            case IHEX_TYPE_05:    /**< Start Linear Address Record */
            break;

            case IHEX_TYPE_02:    /**< Extended Segment Address Record */
            case IHEX_TYPE_03:    /**< Start Segment Address Record */
                fw.Close();
            return false;
        }

        if ( irec.type == IHEX_TYPE_01 )
            break;
    }
    
    HexProcess((((uint32_t) -1) << 16), 0, 0, commitHandler);

    fw.Close();
    return true;
}

bool DoFlashing(char* name)
{
    uint8_t params84[20] = {0x14, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x03, 0xff, 0xff, 0xff, 0xff, 0x00, 0x40, 0x02, 0x00, 0x00, 0x00, 0x20, 0x00};
    // only device code, page size, eeprom size
    memcpy(buffer, params84, sizeof(params84));
    constexpr uint8_t signature84[] = {0x1e, 0x93, 0x0c};
    set_parameters();

    start_pmode();
    uint8_t signature[3];
    signature[0] = spi_transaction(0x30, 0x00, 0x00, 0x00);
    signature[1] = spi_transaction(0x30, 0x00, 0x01, 0x00);
    signature[2] = spi_transaction(0x30, 0x00, 0x02, 0x00);
    
    if (signature[0] != signature84[0] || signature[1] != signature84[1] || signature[2] != signature84[2])
    {
        CONSOLE::Color(RGB565(ff0000));
        CONSOLE::Print("Wrong chip signature %02x%02x%02x, should be %02x%02x%02x\n",
                       signature[0], signature[1], signature[2],
                       signature84[0], signature84[1], signature84[2]);
        return false;
    }
    
    CONSOLE::Print("Chip signature: %02x%02x%02x\n", signature[0], signature[1], signature[2]);

    if(spi_transaction(0xac, 0x80, 0x00, 0x00) != 0x00)
    {
        CONSOLE::Color(RGB565(ff0000));
        CONSOLE::Print("Universal read failed\n");
        end_pmode();
        return false;
    }

    CONSOLE::Print("Write:  ");

    bufferLen = 0;
    bufferBase = -1;
    bool ok = HexLoad(name, [](uint32_t address, uint8_t* data, int len) -> bool
    {
        CONSOLE::Print("W");
        for (int i=len; i<64; i++)
            data[i] = 0xff;

        for (int i=0; i<len; i+=2)
        {
          flash(LOW, (address+i)/2, data[i]);
          flash(HIGH, (address+i)/2, data[i+1]);
        }
        
        commit(address/2);
        return true;
    });
    
    CONSOLE::Print("\n");

    if (!ok)
    {
        end_pmode();
        return false;
    }
                   
    CONSOLE::Print("Verify: ");

    bufferLen = 0;
    bufferBase = -1;
    ok = HexLoad(name, [](uint32_t address, uint8_t* data, int len) -> bool
    {
        CONSOLE::Print("R");
        for (int i=0; i<len; i+=2)
        {
            uint8_t low = flash_read(LOW, (address+i)/2);
            uint8_t high = flash_read(HIGH, (address+i)/2);
            if (low != data[i] || high != data[i+1])
            {
                CONSOLE::Color(RGB565(ff0000));
                CONSOLE::Print("Verify error at [0x%04x] == 0x%02x%02x != 0x%02x%02x\n", (address+i)/2,
                               low, high, data[i], data[i+1]);
                return false;
            }
        }
        return true;
    });
    CONSOLE::Print("\n");

    end_pmode();
    return ok;
}

CFileDialog mFile;
CFileFilterSuffix filter(".HEX");

#ifndef __APPLE__
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
#ifdef __APPLE__
    BIOS::OS::SetArgument((char*)"avr/avrflash.elf");
#endif
    
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);
    //mReader.Open((char*)"avr/tinystar.hex");

    APP::Init("AVRISP");
    APP::Status("ATTiny84, 1MHz internal oscillator");

    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
        if (mFile.ModalShow(nullptr, "Load firmware", &filter))
        {
            APP::Init("AVRISP");
            APP::Status("ATTiny84, 1MHz internal oscillator");
            
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

