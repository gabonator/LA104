#include <library.h>
                                  
extern const uint32_t testdata[1024];

uint32_t buffer[1024];
uint8_t sharedBuffer[BIOS::MEMORY::SharedBufferSize];

#define FLASH_SIZE (*(uint16_t*)0x1FFFF7E0)

__attribute__((__section__(".entry")))
int _main(void)
{    
    BIOS::LCD::Clear(0x000000);

    BIOS::DBG::Print("Official flash size: %d kB\n", FLASH_SIZE);

    for (int i=0; i<COUNT(buffer); i++)
      buffer[i] = testdata[i] + i;

    BIOS::DBG::Print("Writing to flash address %08x...\n", ((uint32_t)testdata) + 0x00040000);
    BIOS::MEMORY::SetSharedBuffer(sharedBuffer);
    BIOS::MEMORY::LinearStart();
    BIOS::MEMORY::LinearProgram(((uint32_t)testdata) + 0x00040000, (uint8_t*)buffer, sizeof(buffer));
    BIOS::MEMORY::LinearFinish();
    BIOS::MEMORY::SetSharedBuffer(nullptr);

    for (int i=0; i<10; i++)
    {
      BIOS::DBG::Print("[%08x]=", (testdata+i));
      BIOS::DBG::Print("%08x, ", *(testdata+i));
      BIOS::DBG::Print("[%08x]=", (testdata+0x00040000/4+i));
      BIOS::DBG::Print("%08x\n", *(testdata+0x00040000/4+i));
    }

/*


    int i;
    for (i = 0; i < sizeof(testdata)/4; i++)
    {
        if (testdata[i] != 0xF00F0000)
        {
            BIOS::DBG::Print("Fail at %x: %x\n", &testdata[i], testdata[i]);
            BIOS::SYS::DelayMs(1000);
            return 1;
        }
    }    

    BIOS::DBG::Print("Test complete: extra flash at 0x%08x works.\n", (uint32_t)testdata);
*/
    while (BIOS::KEY::GetKey() != BIOS::KEY::F2) 
    {
    }
    return 0;
}
