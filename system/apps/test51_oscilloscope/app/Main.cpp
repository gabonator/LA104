/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  DS213_APP main.c                                               Author : bure
********************************************************************************
                               
  ----------------------------------------------------------------------------
  18.07.25 APP V1.01: 
                      
  18.07.27 APP V1.02: 
                      
  18.07.28 APP V1.03: 
                      
                      
                      
                      
                      singleģʽ
                      
                      
                      
  18.08.08 APP V1.04: 
  18.08.10 APP V1.05: 
                      
                      
                      
                      <Vt
  18.08.14 APP V1.06: 
                      
                      
                      
  18.08.15 APP V1.07: 
                      ɾ
  18.08.15 APP V1.08: 
                      
                      
  18.08.17 APP V1.09: 
 
  18.08.27 APP V1.10: 
                      
                      
  18.08.29 APP V1.11: 
                      
 
  18.09.03 APP V1.12: 
                      
                      
 
  18.09.27 APP V1.13: 
                      0.5us
                      
                      
           APP V1.14: 

  18.11.09 APP V1.15: 
                      
                      ģ
 
  18.12.22 APP V1.20: 
                      

  19.02.25 APP V1.21: 
                      

  19.03.08 APP V1.22: 
                      
                      

*******************************************************************************/
#include "Sys.h"
#include "DS213Bios.h"
#include <Library.h>

__attribute__((__section__(".entry"))) int main(void);

HwDrvDef  Hw;
u8  APP_VERSION[12] = { "Gabo1.22" };
u8  PwrDownSt = 0;

extern void SysInt(void);
extern bool AppRunning;

/*******************************************************************************

*******************************************************************************/
int main(void)
{
  pFunc1 __BiosInit = (pFunc1)0x08000101; // Point to DFU BiosInit
  __BiosInit((u32)&Hw);                   //

  Hw.pDevInit(PWRCTRL);
  Hw.pDevInit(KEYnDEV);
  Hw.pDevInit(DISPDEV);
  Hw.pDevInit(BUZZDEV);
  Hw.pClrScrn(BLK);
  u16 x0 = Hw.LogoX0, y0 = Hw.LogoY0;
  Hw.pDispLogo(x0, y0);
  *Hw.pPwm_Bkl = 50;
  *Hw.pBuz_Vol = 50;
  Hw.pDevInit(FPGADEV);
  Hw.pDevInit(FPGACFG);

  static BIOS::OS::TInterruptHandler pCurrentSysTick = BIOS::OS::GetInterruptVector(BIOS::OS::ISysTick);
//  BIOS::OS::SetInterruptVector(BIOS::OS::ISysTick, [](){ SysInt(); pCurrentSysTick(); });
  BIOS::OS::SetInterruptVector(BIOS::OS::ISysTick, SysInt);
  BIOS::LCD::Clear(RGB565(000000));
	
  DispStr(12*8, 90, WHT, eCHAR, "Oscilloscope APP");
  DispStr(29*8, 90, WHT, eCHAR,  APP_VERSION );
  
  *Hw.pOut_nPD = 1;                                // 
  Hw.pDevInit(SGNL_IN);                            // 
  Hw.pDevInit(SGNLOUT);                            // 
  Hw.pDevInit(SO_ANLG);                            // 
  Hw.pDevInit(SO_DGTL);                            // 
  FPGA_CtrlRW(A_C_CH, CHIP_RST);                   // FPGA #0 ģ
  FPGA_CtrlRW(B_D_CH, CHIP_RST);                   // FPGA #1 ģ
  SmplStart();                                     // FPGA 
  TrackClr();                                      // 
  
  uc8 STR1[] = "Parameter record not found";
  uc8 STR2[] = "Reload parameter from disk";
  uc8* STR   = (LoadParam()) ? STR1 : STR2;        // 
  DispStr(12*8, 30, GRN, eCHAR, (u8*)STR);          // 

  if(STR == STR1){                                 // 
    RowPosi(12*8, 50);
    AddStr(TXT2C, eCHAR, (u8*)"Correction");
    Correction();                                  // 
  } else Delay_mS(2000);
  
  *Hw.pPwm_Bkl = Pop[SBKL].Val;                    // 
  *Hw.pBuz_Vol = Pop[SVOL].Val;                    // 
  
  //****************************** ϵͳ
  KeyAct = 0;
  while (AppRunning) {
    UpdtMainMenu();                                           // 
    DispSync();                                               // 
    if(PopUpdt){
      UpdtPopMenu();                                          // 
      PopUpdt = 0;
    } 
    //============================== 
    u16 Act = KeyAct & (ENCDBIT | K1_ACTn | K2_ACTn | K3_ACTn);

    if (!Act && (KeyAct & K4_ACTn))
    {
      AppRunning = false;
    }


    KeyAct = 0;
    if(Act){                                                  // 

      u32 Track = Menu[Item].Src;
      u32 Range = Menu[(Track == CH_A)? RNA : RNB].Val;
      u16 *pKn  = &Menu[Item].Val;
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ K1 Push ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & K1_ACTn){ 
        if(~nKeySt & K4){                                     // 
          u32 r = Snapshot();                                 // 
          RowPosi(Menu[TM2].X0, Menu[TM2].Y0);
          if(r == OK) AddStr(TXT2C, eCHAR, " Saveing OK  ");
          else        AddStr(WARNG, eCHAR, " Saveing Err ");
          Delay_mS(2000);
          Menu[TM2].Flg |= UPDT;                              // 
        } else {                                              // 
          Menu[RUN].Val = (Menu[RUN].Val)? HOLD : SMPL;       // 
          Menu[RUN].Flg |= UPDT;                              // 
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ K2 Push ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & K2_ACTn){
        if(~nKeySt & K4){                                     // 
          u32 r = SaveParam();                                // 
          RowPosi(Menu[TM2].X0, Menu[TM2].Y0);
          if(r == OK) AddStr(TXT2C, eCHAR, " File Saved  ");
          else        AddStr(WARNG, eCHAR, " Save Error  ");
          Delay_mS(2000);
          Menu[TM2].Flg |= UPDT;                              // 
        } else {                                              // 
          PopHide = (PopHide) ? 0 : 1;                        // 
          if(!PopHide) UpdtPopMenu();                         // 
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ K3 Push ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & K3_ACTn){
        if(~nKeySt & K4){                                     // 
          if(Hw.LicenceOk == 0) *Hw.pOut_PEn = 0;             // 
          else {
            RowPosi(12*8, 120);
            AddStr(TXT2C, eCHAR, (u8*)"Correction");
            Correction();                                     // 
            RowPosi(23*8, 120);
            AddStr(TXT2C, eCHAR, "Completed    ");
            Delay_mS(2000);
          }
        } else {                                              // 
          if(PopHide){                                        // 
            if(Item == V_T || Item == TRG){
              Menu[Item].Flg |= UPDT, Menu[Item].Src += 1;
              Menu[Item].Src &= 3;                            // ѡ
              Menu[V_T].Src = Menu[Item].Src;
              Menu[V_T].Flg |= UPDT;                          // 
              Menu[TRG].Src = Menu[Item].Src; 
              Menu[TRG].Flg |= UPDT;
              Menu[T_0].Src = Menu[Item].Src;
              Menu[T_0].Flg |= UPDT;
            } else if(Item == YNP || Item == TM1 || Item == TM2){
              Menu[Item].Src += 1;
              Menu[Item].Src &= 3;                            // ѡ
              Menu[Item].Flg |= UPDT;
            } else if(Item == VM1 || Item == VM2 || Item == CAL){
              Menu[Item].Src += 1;
              Menu[Item].Src &= 1;                            // ѡ
              Menu[Item].Flg |= UPDT;
            } else if(Item >= V_1 && Item <= T_2) Menu[Item].Flg ^= INVR;
          } else PopProcess();                                // 
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Indx Encoder ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & ENCD_1n || Act & ENCD_1p){
        u32 Inc   = (~nKeySt & K4) ? 10 : 1;                  // 
        if(PopHide){                                          // 
          Menu[Item].Flg |= UPDT;  
          if     (Item == V_T) pKn = &Vt[Track];
          else if(Item == YNP) pKn = &Yn[Track];
          else if(Item == CAL) pKn = &Diff[Track][Range];
          if((Item == VM1 || Item == VM2)&& *pKn == RMS){     // 
            Menu[T_1].Flg &= ~INVR, Menu[T_2].Flg &= ~INVR;   // 
          }
          if(Item != V_T || Track < CH_C){
            if(Act & ENCD_1n) *pKn =IndxInc(*pKn, Inc, Item); // Indx+
            else              *pKn =IndxDec(*pKn, Inc, Item); // Indx-
          }
          if(Item == OUT){                                    // OUT 
            Menu[FRQ].Flg |= UPDT;
            if(Menu[FRQ].Val > 10 && *pKn != DGTL) Menu[FRQ].Val = 10; 
          }
          if(Item == FRQ){                                    // FRQ 
            if(*pKn > 10 && Menu[OUT].Val != DGTL) *pKn = 10; 
          }
          if(Item == SYN && *pKn == SLOW){                    // SYN 
            if(Menu[TIM].Val > 5 ) Menu[TIM].Val = 5; 
            Menu[TIM].Flg |= UPDT;
          }
          if(Item == TIM && Menu[SYN].Val == SLOW){           // TIM 
            if(*pKn > 5) *pKn = 5; 
          }
        } else {                                              // 
          PopUpdt = 1;
          if(Act & ENCD_1n) PopValInc(Line, Inc);             // Line+
          else              PopValDec(Line, Inc);             // Line-
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Item+ Encoder ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & ENCD_2p){
        Menu[Item].Flg |= FLSH;                               // ǰ
        if(PopHide){                                          // 
          Item = (Item >= VM1) ? 0 : Item+1;                  // 
        } else {                                              // 
          Line = (Line == SPDT) ? 0 : Line+1;                 // 
          PopUpdt = 1;
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Item- Encoder ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & ENCD_2n){
        Menu[Item].Flg |= FLSH;                               // ǰ
        if(PopHide){                                          // 
          Item = (Item > 0) ? Item-1 : VM1;                   // 
        } else {                                              // 
          Line = (Line > 0) ? Line-1 : SPDT;                  // 
          PopUpdt = 1;
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      Beep_mS(20);
      if(Pop[SPDT].Val != 0) PD_Cnt = Pop[SPDT].Val*60;       // 
    }
    //==========================================================================
    if(*Hw.pSt_Vin == 1){
      PD_Cnt = Pop[SPDT].Val*60;                              // 
      
    }
    if(PD_Cnt == 0 && PwrDownEn && *Hw.pSt_Vin == 0){         // 
      Hw.pPowerDown(ENABLE);                                  // 
      *Hw.pPwm_Bkl = 0;                                       // 
      *Hw.pOut_Clr = 1;                                       // FPGA ʡ
      PwrDownSt = 1;
    } else if(PwrDownSt == 1){                                // 
      Hw.pPowerDown(DISABLE);                                 // 
      *Hw.pPwm_Bkl = Pop[SBKL].Val;                           // 
      *Hw.pOut_Clr = 0;                                       // FPGA 
      *Hw.pPwm_LED = LED_PWM_MAX;                             // 
      PwrDownSt = 0;
      PD_Cnt = Pop[SPDT].Val*60;                              // 
    }
  }

  BIOS::OS::SetInterruptVector(BIOS::OS::ISysTick, pCurrentSysTick);

	return 0;
  //****************************************************************************   
}

/******************************** END OF FILE *********************************/
