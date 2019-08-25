#include "Bios.h"
#include "stm32f10x_flash.h"

#define FALSE 0
#define TRUE 1
#include "flash/all.h"

namespace BIOS {
  namespace MEMORY {
    uint8_t* gSharedBuffer{nullptr};

    static_assert(sizeof(LinearFlashing) <= BIOS::MEMORY::SharedBufferSize, "Shared buffer size mismatch");

    void SetSharedBuffer(void* buffer)
    {
      gSharedBuffer = (uint8_t*)buffer;
    }

    void* GetSharedBuffer()
    {
      return gSharedBuffer;
    }

    void LinearStart()
    {
        _ASSERT(gSharedBuffer);
    	// TODO: for some reason the variable gets corrupted after first flashing (or uninitialized)
      //BIOS::DBG::Print("FL0 (%08x, %08x)", g_pLinearFlashing, g_ADCMem);
    	memset( gSharedBuffer, 0, sizeof(LinearFlashing) );
    	linearFlashProgramStart((LinearFlashing*)gSharedBuffer);
    }

    bool LinearFinish()
    {
        _ASSERT(gSharedBuffer);
    	bool bOk = linearFlashProgramFinish((LinearFlashing*)gSharedBuffer);
    	memset( gSharedBuffer, 0, sizeof(LinearFlashing) );
    	return bOk;
    }

    bool LinearProgram( ui32 nAddress, unsigned char* pData, int nLength )
    {
        _ASSERT(gSharedBuffer);
        int result = linearFlashProgram((LinearFlashing*)gSharedBuffer, nAddress, (flashdata_t*)pData, nLength);
    	return result == 0;
    }
  }
}
