#include "Bios.h"
#include "stm32f10x_flash.h"

#define FALSE 0
#define TRUE 1
#include "flash/all.h"

namespace BIOS {
  namespace MEMORY {
    uint8_t gSharedBuffer[BIOS::MEMORY::SharedBufferSize];

    static_assert(sizeof(LinearFlashing) <= sizeof(BIOS::MEMORY::gSharedBuffer), "Shared buffer size mismatch");

    PVOID GetSharedBuffer()
    {
      return gSharedBuffer;
    }

    void LinearStart()
    {
    	// TODO: for some reason the variable gets corrupted after first flashing (or uninitialized)
      //BIOS::DBG::Print("FL0 (%08x, %08x)", g_pLinearFlashing, g_ADCMem);
    	memset( gSharedBuffer, 0, sizeof(LinearFlashing) );
    	linearFlashProgramStart((LinearFlashing*)gSharedBuffer);
    }

    bool LinearFinish()
    {
    	bool bOk = linearFlashProgramFinish((LinearFlashing*)gSharedBuffer);
    	memset( gSharedBuffer, 0, sizeof(LinearFlashing) );
    	return bOk;
    }

    bool LinearProgram( ui32 nAddress, unsigned char* pData, int nLength )
    {
    	linearFlashProgram((LinearFlashing*)gSharedBuffer, nAddress, (flashdata_t*)pData, nLength);
    	return true;
    }
  }
}
