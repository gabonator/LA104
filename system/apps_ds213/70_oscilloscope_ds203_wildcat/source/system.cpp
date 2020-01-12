#include <library.h>

extern "C" {
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
int main(void);
}

__attribute__((__section__(".entry")))
int _main(void) 
{
  BIOS::OS::SetInterruptVector(BIOS::OS::ITIM2_IRQ, TIM2_IRQHandler);
  BIOS::OS::SetInterruptVector(BIOS::OS::ITIM3_IRQ, TIM3_IRQHandler);
  BIOS::OS::SetInterruptVector(BIOS::OS::ITIM4_IRQ, TIM4_IRQHandler);
  main();
}
/*
extern "C" {

void __Set(uint8_t Object, uint32_t Value)
{
  _ASSERT(0);
}

uint32_t  __Get(uint8_t Object)
{
  _ASSERT(0);
}

 uint8_t   __ReadDiskData(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t Lenght)
 {
   _ASSERT(0);
 }

 uint8_t   __ProgDiskPage(uint8_t* pBuffer, uint32_t ProgAddr)
 {
   _ASSERT(0);
 }
 
 uint32_t  __GetDev_SN(void)
 {
   return 0x11;
 }



void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}

  void __Point_SCR(uint16_t x0, uint16_t y0)
  {
  }
  void __LCD_SetPixl(uint16_t Color)
  {
  }
  uint16_t  __LCD_GetPixl(void)
  {
    return 0;
  }

  uint8_t __OpenFileWr(uint8_t* Buffer, char* FileName, uint16_t* Cluster, uint32_t* pDirAddr)
  {
    return 0;
  }

  uint8_t __OpenFileRd(uint8_t* Buffer, char* FileName, uint16_t* Cluster, uint32_t* pDirAddr)
  {
    return 0;
  }

  uint8_t __CloseFile(uint8_t* Buffer, uint32_t Lenght, uint16_t* Cluster, uint32_t* pDirAddr)
  {
    return 0;
  }

  uint8_t __ReadFileSec(uint8_t* Buffer, uint16_t* Cluster)
  {
    return 0;
  }

  void __Set_Param(uint8_t RegAddr, uint8_t Parameter)
  {
  }

 void __LCD_DMA_Ready(void) {}                    // Wait LCD data DMA ready
 void __Row_Copy(const uint16_t *S_Buffer, uint16_t *T_Buffer) {} // Copy one row base data to buffer
 void __Row_DMA_Ready(void) {}                    // Wait row base data DMA ready
 uint32_t  __Read_FIFO(void) {}                        // Read data from FIFO & Ptr+1

 void __Display_Str(uint16_t x0, uint16_t y0, uint16_t Color, uint8_t Mode, char *s) {}
 void __LCD_Set_Block(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2) {}

 void __Clear_Screen(uint16_t Color) {}
 void __LCD_Copy(const uint16_t *pBuffer, uint16_t NumPixel) {} // Send a row data to LCD

 uint8_t __ProgFileSec(uint8_t* Buffer, uint16_t* Cluster) {}

}
*/