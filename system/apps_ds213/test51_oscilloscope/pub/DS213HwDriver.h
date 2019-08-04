/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  Hw_Driver.h for DS213 DFU & APP                                Author : bure
*******************************************************************************/
#ifndef __HW_DRV_H
#define __HW_DRV_H

  typedef  u32  (*pFunc0)(void);
  typedef  u32  (*pFunc1)(u32 i);
  typedef  u32  (*pFunc2)(u32 i, u32 j);
  typedef  u32  (*pFunc3)(u32 i, u32 j, u32 k);
  typedef  u32  (*pFunc4)(u32 i, u32 j, u32 k, u32 n);
  
  typedef struct
  { //------------------------------ SYS 相关定义 ------------------------------
    
    u8*    pHwVerStr;       // pStr = Hw.pHwVerStr
    u8*    pDfuVerStr;      // pStr = Hw.pDfuVerStr
    u8*    pMcuTypStr;      // pStr = Hw.pMcuTypStr
    u8*    pAdcTypStr;      // pStr = Hw.pAdcTypStr
    u8*    pProductStr;     // pStr = Hw.pProductStr
    u8*    pFpgaTypStr;     // pStr = Hw.pFpgaTypStr
    u8*    pDiskTypStr;     // pStr = Hw.pDiskTypStr
    u8*    pLcdcTypStr;     // pStr = Hw.pLcdcTypStr
  
    u32    ProductSN;       // Word = Hw.ProductSN
    u32    LicenceOk;       // Bit  = Hw.LicenceOk
    u32    GetLicence;      // Word = Hw.GetLicence
    
    u32    App1Base;        // Word = Hw.App1Base
    u32    App2Base;        // Word = Hw.App2Base
    u32    FpgaBase;        // Word = Hw.FpgaBase
    u32    LogoBase;        // Word = Hw.LogoBase
    u32    MaxLimit;        // Word = Hw.MaxLimit

    pFunc1 pDelay_uS;       // Hw.pDelay_uS(u32 uS);
    pFunc1 pNIVC_Ptr;       // Hw.pNIVC_Ptr(u16 Ptr);
    pFunc1 pSysTickCfg;     // Hw.pSysTickCfg(u16 uS);
    pFunc1 pAfReMap;        // Hw.pAfReMap(u16 Var);
    pFunc1 pDevInit;        // Hw.pDevInit(u16 Dev);
  
    u32    Mcu_SN_Base;     // Word = Hw.Mcu_SN_Base  
    u32    McuFlashPage;    // Word = Hw.McuFlashPage
    pFunc0 pMcuFlashLock;   // Hw.pMcuFlashLock();
    pFunc0 pMcuFlashUnlock; // Hw.pMcuFlashUnlock();
    pFunc1 pMcuFlashErase;  // Hw.pMcuFlashErase(u32 Addr);
    pFunc2 pMcuFlashWr4B;   // Hw.pMcuFlashWr4B(u32 Addr, u32 Data);
    pFunc2 pMcuFlashWr2B;   // Hw.pMcuFlashWr2B(u32 Addr, u16 Data);
  
    //---------------------------- 电源控制相关定义 ----------------------------
  
    vu8*   pOut_PEn;        // *Hw.pOut_PEn = 0/1 
    vu8*   pOut_nPD;        // *Hw.pOut_nPD = 0/1
//    vu8*   pOut_LED;        // *Hw.pOut_LED = 0/1
    vu16*  pPwm_LED;        // *Hw.pPwm_LED = 500 or 1000
    vu8*   pSt_Vin;         // Bit  = *Hw.pSt_Vin
    vu8*   pSt_Chg;         // Bit  = *Hw.pSt_Chg
    vu16*  pAdc_Vbty;       // Data = *Hw.pAdc_Vbty
    u32    Vbty_Scale;      // Word = Hw.VBTY_SCALE
    pFunc1 pPowerDown;      // Hw.pPowerDown(u8 NewState); ENABLE/DISABLE
  
    //--------------------------- 蜂鸣器设备相关定义 ---------------------------
  
    vu16*  pBuz_Vol;        // *Hw.pBuz_Vol  = 0~100
    vu16*  pBuz_Dev;        // *Hw.pBuz_Dev(|= ENBL) or (&= ~ENBL)
  
    //--------------------------- 按键类设备相关定义 ---------------------------
  
    u8*   pK1_St;           // Bit = *Hw.pK1_St
    u8*   pK2_St;           // Bit = *Hw.pK2_St
    u8*   pK3_St;           // Bit = *Hw.pK3_St
    u8*   pK4_St;           // Bit = *Hw.pK4_St
    u8*   pEa_St;           // Bit = *Hw.pEa_St
    u8*   pEb_St;           // Bit = *Hw.pEb_St
    u8*   pEc_St;           // Bit = *Hw.pEc_St
    u8*   pEd_St;           // Bit = *Hw.pEd_St
  
    //---------------------------- 显示设备相关定义 ----------------------------
  
    vu16*  pPwm_Bkl;        // *Hw.pPwm_Bkl  = 0~100
    vu8*   pOut_nRST;       // *Hw.pOut_nRST = 0/1 
    vu8*   pOut_D_C;        // *Hw.pOut_D_C  = 0/1 
    vu16*  pLcd_Data_W;     // *Hw.pLcd_Data_W = Data
    vu16*  pLcd_Data_R;     // Data = *Hw.pLcd_Data_R
    u32    MaxLcdCol;       // Word = Hw.MaxLcdCol
    u32    MaxLcdRow;       // Word = Hw.MaxLcdRow
    
    u32    LogoX0;          // Word = Hw.LogoX0
    u32    LogoY0;          // Word = Hw.LogoY0
    uc16*  pFont_8x14;      // Data = *Hw.pFont_8x14
    pFunc4 pLCD_W_Block;    // Hw.pLCD_W_Block(u16 x1, u16 y1, u16 x2, u16 y2);
    pFunc4 pLCD_R_Block;    // Hw.pLCD_R_Block(u16 x1, u16 y1, u16 x2, u16 y2);
    pFunc2 pDispPosn8x14;   // Hw.pDispPosn8x14(u16 x, u16 y);
    pFunc4 pDispStr8x14;    // Hw.pDispStr8x14(u16 ForeGrnd, u16 BackGrnd, u8 Mode, u32 pStr);
    pFunc1 pClrScrn;        // Hw.pClrScrn(vu16 Color);
    pFunc2 pDispLogo;       // Hw.pDispLogo(u16 x0, u16 y0);
    
    pFunc0 pLCD_DmaWait;    // Hw.pLCD_DmaWait();
    pFunc2 pLCD_DmaFill;    // Hw.pLCD_DmaFill(u32 SrcAddr, u16 n);
    pFunc2 pLCD_DmaSend;    // Hw.pLCD_DmaSend(u32 SrcAddr, u16 n);
  
    //--------------------------- FPGA 控制相关定义 ----------------------------
  
    u32    Fpga_Spi;        // Hw.Fpga_Spi = &pDev
    vu8*   pOut_Clr;        // *Hw.pOut_Clr  = 0/1 
    vu8*   pOut_nCfg;       // *Hw.pOut_nCfg = 0/1 
    vu8*   pOut_nSS0;       // *Hw.pOut_nSS0 = 0/1 
    vu8*   pOut_nSS1;       // *Hw.pOut_nSS1 = 0/1 
    vu8*   pSt_Done0;       // Bit = *Hw.pSt_Done0
    vu8*   pSt_Done1;       // Bit = *Hw.pSt_Done1
  
    //--------------------------- SPI 控制相关定义 -----------------------------

    pFunc4 pSpiComm;        // Hw.pSpiComm(u32 DevBase, u8 Cmd, u32 BufBase, u32 n);
    pFunc3 pSpiWrBuf;       // Hw.pSpiWrBuf(u32 DevBase, u32 BufBase, u32 n);
    pFunc2 pSpiRdWrByte;    // Hw.pSpiRdWrByte(u32 DevBase, u8 Byte);
    pFunc1 pSpiComplete;    // Hw.pSpiComplete(u32 DevBase);
  
    //--------------------------- USB 控制相关定义 -----------------------------

    pFunc1 pUSB_Pwr;        // Hw.pUSB_Pwr(u8 NewState); ENABLE/DISABLE
    vu8*   pOut_SCD;        // *Hw.pOUT_SCD = 0/1 
    vu8*   pOut_SCP;        // *Hw.pOUT_SCP = 0/1 
    
    //-------------------------- DISK 控制相关定义 -----------------------------

    vu8*   pOut_nSS2;       // *Hw.pOut_nSS2 = 0/1 
    u32    Disk_Spi;        // Hw.Disk_Spi   = &pDev
    u32    DiskSecSize;     // Word = Hw.DiskSecSize
    u32    DiskSecAmnt;     // Word = Hw.DiskSecAmnt
    u32    DiskPageSize;    // Word = Hw.DiskPageSize
    pFunc3 pSpiFlashSecW;   // Hw.pSpiFlashSecW(u32 BufBase, u32 WrAddr, u16 SecSize);
    pFunc3 pSpiFlashRead;   // Hw.pSpiFlashRead(u32 BufBase, u32 RdAddr, u16 n);

    //------------------------- 信号输入控制相关定义 ---------------------------
  
    vu8*   pOut_A_Coupl;    // *Hw.pOut_A_Coupl  = 0/1 
    vu8*   pOut_B_Coupl;    // *Hw.pOut_B_Coupl  = 0/1 
    vu8*   pOut_A_Level;    // *Hw.pOut_A_Level  = 0/1 
    vu8*   pOut_B_Level;    // *Hw.pOut_B_Level  = 0/1 
    vu8*   pOut_A_Slct0;    // *Hw.pOut_A_Slct0  = 0/1 
    vu8*   pOut_B_Slct0;    // *Hw.pOut_B_Slct0  = 0/1 
    vu8*   pOut_A_Slct1;    // *Hw.pOut_A_Slct1  = 0/1 
    vu8*   pOut_B_Slct1;    // *Hw.pOut_B_Slct1  = 0/1 
    vu8*   pOut_A_Slct2;    // *Hw.pOut_A_Slct2  = 0/1 
    vu8*   pOut_B_Slct2;    // *Hw.pOut_B_Slct2  = 0/1 
    vu8*   pOut_A_Slct_B;   // *Hw.pOut_A_Slct_B = 0/1 
    vu8*   pOut_B_Slct_A;   // *Hw.pOut_B_Slct_A = 0/1 
    vu16*  pCh_A_Posn;      // *Hw.pCh_A_Posn = 0~1023 
    vu16*  pCh_B_Posn;      // *Hw.pCh_B_Posn = 0~1023
  
    //------------------------- 信号输出控制相关定义 ---------------------------
  
    vu16*  pFout_TIM_PSC;   // *Hw.pFout_TIM_PSC  = Data
    vu16*  pFout_TIM_ARR;   // *Hw.pFout_TIM_ARR  = Data
    vu16*  pFout_TIM_CCR;   // *Hw.pFout_TIM_CCR  = Data
    vu16*  pFout_DMA_PSC;   // *Hw.pFout_DMA_PSC  = Data
    vu16*  pFout_DMA_ARR;   // *Hw.pFout_DMA_ARR  = Data
    vu32*  pFout_DMA_CNT;   // *Hw.pFout_DMA_CNT  = Data
    vu32*  pFout_DMA_CMA;   // *Hw.pFout_DMA_CMA  = Data

    vu16*  pFout_mV_DAC;    // *Hw.pFout_mV_DAC  = Data
    u32    Fout_Scale;      // Word = Hw.FOUT_SCALE
    pFunc1 pFout_DMA;       // Hw.pFout_DMA(ENABLE/DISABLE);
    vu8*   pFout_SLCT;      // Hw.pOut_SLCT = 0/1 
    
    
  } HwDrvDef;

#endif
  
/********************************* END OF FILE ********************************/
