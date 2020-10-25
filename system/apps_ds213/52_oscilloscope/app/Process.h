/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  DS213_APP Process.h                                            Author : bure
*******************************************************************************/
#ifndef __Process_h
#define __Process_h

#include "Platform.h"

  #define CH_A       0
  #define CH_B       1
  #define CH_C       2
  #define CH_D       3
  #define FINISH     0x08 //0x80 // FINISH==FULL???

  typedef enum {
    RECOD1 = 2, RECOD2, RECOD3, RECOD4,
  } track3_mode;

  typedef enum {
    A_ADD_B = 2, A_SUB_B, C_AND_D, C_OR_D, INV_A, INV_B,
  } track_mode;

  extern u8  Recod[], Trigg, ScrnF;
  extern u16 RmsA, RmsB, MaxA, MaxB, MinA, MinB;
  
  void DispSync(void);
  void CtrlUpdate(u8 Item); 

#endif
  
/******************************** END OF FILE *********************************/
