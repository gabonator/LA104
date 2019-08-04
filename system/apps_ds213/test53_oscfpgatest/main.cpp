#include <library.h>

#include "platform.h"
#include "DS213HwDriver.h"
HwDrvDef  Hw;

#include "Fpga.h"
#include "fpga2.h"

using namespace BIOS;

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
  pFunc1 __BiosInit = (pFunc1)0x08000101; // Point to DFU BiosInit
  __BiosInit((u32)&Hw);                   //

    CRect rcClient(0, 0, LCD::Width, LCD::Height);
    LCD::Bar(rcClient, RGB565(0000b0));

//SetRangeA(0);
//SmplStart();
SetPreSmplDpth(1);
    int Xposi = 0;
    BIOS::KEY::EKey key;

*Hw.pOut_Clr = 1;
*Hw.pOut_Clr = 0;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
EVERY(5000)
{
//SetRangeA(0);
//SmplStart();
}
      FPGA_DataWr(A_C_CH, SMPL_RPTR, Xposi);
      FPGA_DataWr(B_D_CH, SMPL_RPTR, Xposi);

      uint32_t status1 = FPGA_CtrlRW(CH_A, SMPL_ST);
      uint32_t status2 = FPGA_CtrlRW(CH_B, SMPL_ST);
      int ScrnF = 0;
  if((status1|status2) & SCRNF) ScrnF = 1;
      LCD::Bar(rcClient, RGB565(0000b0));
      BIOS::LCD::Printf(0, 0, RGB565(ffffff), RGB565(b0b0b0), "status=%x/%x/%d  ", status1, status2, ScrnF);

      int i=0;
//      if ((status1 & SCRNF)  || (status2 & SCRNF))
//        i = 1;
      for (; (i>>3)<400; i++)
      {
        uint32_t ac = FPGA_SmplRd(A_C_CH);
        uint32_t bd = FPGA_SmplRd(B_D_CH);
if (ac & 0x10)
{
  i--;
  continue;
}
        BIOS::LCD::PutPixel(i>>3, ac>>8, RGB565(ff0000));
        BIOS::LCD::PutPixel(i>>3, bd>>8, RGB565(ffff00));
//if (i>10 && i<20)
//BIOS::LCD::Printf(0, (i-10)*14, RGB565(ffffff), RGB565(b00000), "%x", ac);
//for (volatile int j=0; j<200; j++);
/*
        if ((i%10) == 0)
        {
          BIOS::LCD::Printf(0, j, RGB565(ffffff), RGB565(b0b0b0), "ac=%x, bd=%x    ", ac, bd);
          j+=14;
        }*/
      }

//if (status1== 0x2e && status2 == 0x0e) //3e, 1e
//   SmplStart();

// if (ScrnF)
//    SmplStart();
//      BIOS::SYS::DelayMs(5);

  /*
      if((status1 & SCRNF) || (status2 & SCRNF))
      {
        int MaxA, MinA, MaxB, MinB;
        MaxA = FPGA_ByteRd(A_C_CH, VMAX), MinA = FPGA_ByteRd(A_C_CH, VMIN);
        MaxB = FPGA_ByteRd(B_D_CH, VMAX), MinB = FPGA_ByteRd(B_D_CH, VMIN);
      }
*/
BIOS::SYS::DelayMs(50);
    }
    
    return 0;
}
