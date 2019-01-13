#include "Bios.h"
#include "../../lib/STM32F10x_StdPeriph_Driver/inc/misc.h"
//      NVIC_SetVectorTable(NVIC_VectTab_RAM, (uint32_t)(g_pfnRamVectorTable - NVIC_VectTab_RAM));

namespace BIOS
{
  namespace OS
  {
    char strGlobalArgument[64];
    bool bArgumentChanged{false};

    void SetArgument(char* argument)
    {
      _ASSERT(strlen(argument) < sizeof(strGlobalArgument)-1);
      strcpy(strGlobalArgument, argument);
      bArgumentChanged = true;
    }

    char* GetArgument()
    {
      return strGlobalArgument;
    }

    bool HasArgument()
    {
      if (!bArgumentChanged)
        return false;

      bArgumentChanged = false;     
      return true;
    }

    extern "C" void (*g_pfnVectors[])(void);
    extern "C" TInterruptHandler g_pfnRamVectorTable[];

    void CopyVectorTable()
    {
      if ((uint32_t)g_pfnVectors[0] == (uint32_t)g_pfnRamVectorTable[0])
        return;

      memcpy(g_pfnRamVectorTable, g_pfnVectors, 76*4);

      NVIC_SetVectorTable(NVIC_VectTab_RAM, (uint32_t)(g_pfnRamVectorTable - NVIC_VectTab_RAM));
    }

    TInterruptHandler GetInterruptVector(EInterruptVector i)
    {
      CopyVectorTable();
      return g_pfnRamVectorTable[i];
    }

    void SetInterruptVector(EInterruptVector i, TInterruptHandler handler)
    {
      CopyVectorTable();
      g_pfnRamVectorTable[i] = handler;
    }

    void EnableInterrupts(uint32_t mask)
    {
      __asm volatile (
      "	msr primask, %0\n"
      :: "r" (mask) 
      );
    }

    uint32_t DisableInterrupts()
    {
      uint32_t mask;
      __asm volatile (			   \
      "	mrs %0, primask\n"
      : "=r" (mask) 
      );
      return mask;
    }
  }
}
