#include "Bios.h"

namespace BIOS {
  namespace MEMORY {
    PVOID GetSharedBuffer()
    {
      return nullptr;
    }

    void LinearStart()
    {
        assert(0);
    }

    bool LinearFinish()
    {
        assert(0);
    	return false;
    }

    bool LinearProgram( uint32_t nAddress, unsigned char* pData, int nLength )
    {
        assert(0);
    	return false;
    }
  }
}
