/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  Hw_Driver.h for DS213 DFU & APP                                Author : bure
*******************************************************************************/
#ifndef __HW_DRV_H
#define __HW_DRV_H

  typedef  uint32_t  (*pFunc0)(void);
  typedef  uint32_t  (*pFunc1)(uint32_t i);
  typedef  uint32_t  (*pFunc2)(uint32_t i, uint32_t j);
  typedef  uint32_t  (*pFunc3)(uint32_t i, uint32_t j, uint32_t k);
  typedef  uint32_t  (*pFunc4)(uint32_t i, uint32_t j, uint32_t k, uint32_t n);

  enum { 
    PWRCTRL, // 
    KEYnDEV, // 
    BUZZDEV, // 
    DISPDEV, // LCD 
    USB_DEV, // USB ͨ
    USB_SCI, // SCI ͨ
    USBDISK, // USB Disk 
    FPGADEV, // FPGA 
    FPGACFG, // FPGA 
    SGNL_IN, // 
    SGNLOUT, // 
    SO_ANLG, // ģ
    SO_DGTL, // 
  };
  
  typedef struct
  { //------------------------------ SYS 
    
    uint8_t*    pHwVerStr;       // pStr = Hw.pHwVerStr
    uint8_t*    pDfuVerStr;      // pStr = Hw.pDfuVerStr
    uint8_t*    pMcuTypStr;      // pStr = Hw.pMcuTypStr
    uint8_t*    pAdcTypStr;      // pStr = Hw.pAdcTypStr
    uint8_t*    pProductStr;     // pStr = Hw.pProductStr
    uint8_t*    pFpgaTypStr;     // pStr = Hw.pFpgaTypStr
    uint8_t*    pDiskTypStr;     // pStr = Hw.pDiskTypStr
    uint8_t*    pLcdcTypStr;     // pStr = Hw.pLcdcTypStr
  
    uint32_t    ProductSN;       // Word = Hw.ProductSN
    uint32_t    LicenceOk;       // Bit  = Hw.LicenceOk
    uint32_t    GetLicence;      // Word = Hw.GetLicence
    
    uint32_t    App1Base;        // Word = Hw.App1Base
    uint32_t    App2Base;        // Word = Hw.App2Base
    uint32_t    FpgaBase;        // Word = Hw.FpgaBase
    uint32_t    LogoBase;        // Word = Hw.LogoBase
    uint32_t    MaxLimit;        // Word = Hw.MaxLimit

    pFunc1 pDelay_uS;       // Hw.pDelay_uS(uint32_t uS);
    pFunc1 pNIVC_Ptr;       // Hw.pNIVC_Ptr(u16 Ptr);
    pFunc1 pSysTickCfg;     // Hw.pSysTickCfg(u16 uS);
    pFunc1 pAfReMap;        // Hw.pAfReMap(u16 Var);
    pFunc1 pDevInit;        // Hw.pDevInit(u16 Dev);
  
    uint32_t    Mcu_SN_Base;     // Word = Hw.Mcu_SN_Base  
    uint32_t    McuFlashPage;    // Word = Hw.McuFlashPage
    pFunc0 pMcuFlashLock;   // Hw.pMcuFlashLock();
    pFunc0 pMcuFlashUnlock; // Hw.pMcuFlashUnlock();
    pFunc1 pMcuFlashErase;  // Hw.pMcuFlashErase(uint32_t Addr);
    pFunc2 pMcuFlashWr4B;   // Hw.pMcuFlashWr4B(uint32_t Addr, uint32_t Data);
    pFunc2 pMcuFlashWr2B;   // Hw.pMcuFlashWr2B(uint32_t Addr, u16 Data);
  
    //---------------------------- 
  
    volatile uint8_t*   pOut_PEn;        // *Hw.pOut_PEn = 0/1 
    volatile uint8_t*   pOut_nPD;        // *Hw.pOut_nPD = 0/1
//    volatile uint8_t*   pOut_LED;        // *Hw.pOut_LED = 0/1
    volatile uint16_t*  pPwm_LED;        // *Hw.pPwm_LED = 500 or 1000
    volatile uint8_t*   pSt_Vin;         // Bit  = *Hw.pSt_Vin
    volatile uint8_t*   pSt_Chg;         // Bit  = *Hw.pSt_Chg
    volatile uint16_t*  pAdc_Vbty;       // Data = *Hw.pAdc_Vbty
    uint32_t    Vbty_Scale;      // Word = Hw.VBTY_SCALE
    pFunc1 pPowerDown;      // Hw.pPowerDown(uint8_t NewState); ENABLE/DISABLE
  
    //--------------------------- 
  
    volatile uint16_t*  pBuz_Vol;        // *Hw.pBuz_Vol  = 0~100
    volatile uint16_t*  pBuz_Dev;        // *Hw.pBuz_Dev(|= ENBL) or (&= ~ENBL)
  
    //--------------------------- 
  
    uint8_t*   pK1_St;           // Bit = *Hw.pK1_St
    uint8_t*   pK2_St;           // Bit = *Hw.pK2_St
    uint8_t*   pK3_St;           // Bit = *Hw.pK3_St
    uint8_t*   pK4_St;           // Bit = *Hw.pK4_St
    uint8_t*   pEa_St;           // Bit = *Hw.pEa_St
    uint8_t*   pEb_St;           // Bit = *Hw.pEb_St
    uint8_t*   pEc_St;           // Bit = *Hw.pEc_St
    uint8_t*   pEd_St;           // Bit = *Hw.pEd_St
  
    //---------------------------- 
  
    volatile uint16_t*  pPwm_Bkl;        // *Hw.pPwm_Bkl  = 0~100
    volatile uint8_t*   pOut_nRST;       // *Hw.pOut_nRST = 0/1 
    volatile uint8_t*   pOut_D_C;        // *Hw.pOut_D_C  = 0/1 
    volatile uint16_t*  pLcd_Data_W;     // *Hw.pLcd_Data_W = Data
    volatile uint16_t*  pLcd_Data_R;     // Data = *Hw.pLcd_Data_R
    uint32_t    MaxLcdCol;       // Word = Hw.MaxLcdCol
    uint32_t    MaxLcdRow;       // Word = Hw.MaxLcdRow
    
    uint32_t    LogoX0;          // Word = Hw.LogoX0
    uint32_t    LogoY0;          // Word = Hw.LogoY0
    uint16_t*  pFont_8x14;      // Data = *Hw.pFont_8x14
    pFunc4 pLCD_W_Block;    // Hw.pLCD_W_Block(u16 x1, u16 y1, u16 x2, u16 y2);
    pFunc4 pLCD_R_Block;    // Hw.pLCD_R_Block(u16 x1, u16 y1, u16 x2, u16 y2);
    pFunc2 pDispPosn8x14;   // Hw.pDispPosn8x14(u16 x, u16 y);
    pFunc4 pDispStr8x14;    // Hw.pDispStr8x14(u16 ForeGrnd, u16 BackGrnd, uint8_t Mode, uint32_t pStr);
    pFunc1 pClrScrn;        // Hw.pClrScrn(volatile uint16_t Color);
    pFunc2 pDispLogo;       // Hw.pDispLogo(u16 x0, u16 y0);
    
    pFunc0 pLCD_DmaWait;    // Hw.pLCD_DmaWait();
    pFunc2 pLCD_DmaFill;    // Hw.pLCD_DmaFill(uint32_t SrcAddr, u16 n);
    pFunc2 pLCD_DmaSend;    // Hw.pLCD_DmaSend(uint32_t SrcAddr, u16 n);
  
    //--------------------------- FPGA 
  
    uint32_t    Fpga_Spi;        // Hw.Fpga_Spi = &pDev
    volatile uint8_t*   pOut_Clr;        // *Hw.pOut_Clr  = 0/1 
    volatile uint8_t*   pOut_nCfg;       // *Hw.pOut_nCfg = 0/1 
    volatile uint8_t*   pOut_nSS0;       // *Hw.pOut_nSS0 = 0/1 
    volatile uint8_t*   pOut_nSS1;       // *Hw.pOut_nSS1 = 0/1 
    volatile uint8_t*   pSt_Done0;       // Bit = *Hw.pSt_Done0
    volatile uint8_t*   pSt_Done1;       // Bit = *Hw.pSt_Done1
  
    //--------------------------- SPI 

    pFunc4 pSpiComm;        // Hw.pSpiComm(uint32_t DevBase, uint8_t Cmd, uint32_t BufBase, uint32_t n);
    pFunc3 pSpiWrBuf;       // Hw.pSpiWrBuf(uint32_t DevBase, uint32_t BufBase, uint32_t n);
    pFunc2 pSpiRdWrByte;    // Hw.pSpiRdWrByte(uint32_t DevBase, uint8_t Byte);
    pFunc1 pSpiComplete;    // Hw.pSpiComplete(uint32_t DevBase);
  
    //--------------------------- USB 

    pFunc1 pUSB_Pwr;        // Hw.pUSB_Pwr(uint8_t NewState); ENABLE/DISABLE
    volatile uint8_t*   pOut_SCD;        // *Hw.pOUT_SCD = 0/1 
    volatile uint8_t*   pOut_SCP;        // *Hw.pOUT_SCP = 0/1 
    
    //-------------------------- DISK 

    volatile uint8_t*   pOut_nSS2;       // *Hw.pOut_nSS2 = 0/1 
    uint32_t    Disk_Spi;        // Hw.Disk_Spi   = &pDev
    uint32_t    DiskSecSize;     // Word = Hw.DiskSecSize
    uint32_t    DiskSecAmnt;     // Word = Hw.DiskSecAmnt
    uint32_t    DiskPageSize;    // Word = Hw.DiskPageSize
    pFunc3 pSpiFlashSecW;   // Hw.pSpiFlashSecW(uint32_t BufBase, uint32_t WrAddr, u16 SecSize);
    pFunc3 pSpiFlashRead;   // Hw.pSpiFlashRead(uint32_t BufBase, uint32_t RdAddr, u16 n);

    //------------------------- 
  
    volatile uint8_t*   pOut_A_Coupl;    // *Hw.pOut_A_Coupl  = 0/1 
    volatile uint8_t*   pOut_B_Coupl;    // *Hw.pOut_B_Coupl  = 0/1 
    volatile uint8_t*   pOut_A_Level;    // *Hw.pOut_A_Level  = 0/1 
    volatile uint8_t*   pOut_B_Level;    // *Hw.pOut_B_Level  = 0/1 
    volatile uint8_t*   pOut_A_Slct0;    // *Hw.pOut_A_Slct0  = 0/1 
    volatile uint8_t*   pOut_B_Slct0;    // *Hw.pOut_B_Slct0  = 0/1 
    volatile uint8_t*   pOut_A_Slct1;    // *Hw.pOut_A_Slct1  = 0/1 
    volatile uint8_t*   pOut_B_Slct1;    // *Hw.pOut_B_Slct1  = 0/1 
    volatile uint8_t*   pOut_A_Slct2;    // *Hw.pOut_A_Slct2  = 0/1 
    volatile uint8_t*   pOut_B_Slct2;    // *Hw.pOut_B_Slct2  = 0/1 
    volatile uint8_t*   pOut_A_Slct_B;   // *Hw.pOut_A_Slct_B = 0/1 
    volatile uint8_t*   pOut_B_Slct_A;   // *Hw.pOut_B_Slct_A = 0/1 
    volatile uint16_t*  pCh_A_Posn;      // *Hw.pCh_A_Posn = 0~1023 
    volatile uint16_t*  pCh_B_Posn;      // *Hw.pCh_B_Posn = 0~1023
  
    //------------------------- 
  
    volatile uint16_t*  pFout_TIM_PSC;   // *Hw.pFout_TIM_PSC  = Data
    volatile uint16_t*  pFout_TIM_ARR;   // *Hw.pFout_TIM_ARR  = Data
    volatile uint16_t*  pFout_TIM_CCR;   // *Hw.pFout_TIM_CCR  = Data
    volatile uint16_t*  pFout_DMA_PSC;   // *Hw.pFout_DMA_PSC  = Data
    volatile uint16_t*  pFout_DMA_ARR;   // *Hw.pFout_DMA_ARR  = Data
    volatile uint32_t*  pFout_DMA_CNT;   // *Hw.pFout_DMA_CNT  = Data
    volatile uint32_t*  pFout_DMA_CMA;   // *Hw.pFout_DMA_CMA  = Data

    volatile uint16_t*  pFout_mV_DAC;    // *Hw.pFout_mV_DAC  = Data
    uint32_t    Fout_Scale;      // Word = Hw.FOUT_SCALE
    pFunc1 pFout_DMA;       // Hw.pFout_DMA(ENABLE/DISABLE);
    volatile uint8_t*   pFout_SLCT;      // Hw.pOut_SLCT = 0/1 
    
    
  } HwDrvDef;

#endif
  
/********************************* END OF FILE ********************************/
