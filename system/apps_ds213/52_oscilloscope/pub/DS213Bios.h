/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  Bios.h for DS213 DFU & APP                                     Author : bure
*******************************************************************************/
#ifndef __BIOS_H
#define __BIOS_H

  #include "Platform.h"

  #include <stdlib.h>
  #include <string.h>
  #include "Func.h"
  #include "DS213HwDriver.h"

  extern HwDrvDef  Hw;

  void __BiosInit(u32 pDrv);
  void SetLicence(u16 x, u16 y);
  
  //-------------------- DevInit(u16 Dev)
  
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

//-------------------------------- 
  
  #define WHT        0xFFFF
  #define CYAN       0xFFE0
  #define YEL        0x07FF
  #define PRPL       0xF81F
  #define GRN        0x07E0
  #define GRAY       0x7BEF
  #define DARK       0x39E7
  #define BLUE       0xF800
  #define ORNG       0x051F
  #define RED        0x001F
  #define BLK        0x0000

  #define CHAR       0x00   //  Standard Display 
  #define INVR       0x01   //  Inverse Display  
  #define SYMB       0x02   //  Symbol Display   
  #define VOID       0x04   //  Dotted Display   
  
#endif
/********************************* END OF FILE ********************************/
