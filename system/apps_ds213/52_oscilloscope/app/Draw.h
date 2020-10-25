/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  DS213_APP Draw.h                                               Author : bure
*******************************************************************************/
#ifndef __DRAW_H
#define __DRAW_H

#include "Platform.h"

  typedef enum {
    TRCK1, TRCK2, TRCK3, TRCK4, DARK1, DARK2, DARK3, DARK4, 
    BLANK, WARNG, CURSR, TXT2C, GRNDC, GRIDC, XATTR, 
    FOUTC = 4, TXT1C = 10, NORMO = 3, N_2_W = 16,
  } color_num;                                      // For Palette[Cnum]

  typedef enum {
    BKGRN, CNUM1, CNUM2, CNUM3, 
  } color_pop;                                      // For HueTab[Cnum]

  typedef enum {
    SWAV, LWAV, SBUF, SCSV, SVOL = 4, SBKL = 5, SPDT = 6,
  } uimenu_pop;                                     // For Pop[Line]

  //--------------------------- 

  #define WHT        0xFFFF
  #define CYAN       0xFFE0
  #define CYAN_      0xBDE0
  #define YEL        0x07FF
  #define YEL_       0x05F7
  #define PRPL       0xF81F
  #define PRPL_      0xB817
  #define GRN        0x07E0
  #define GRN_       0x05E0
  #define GRAY       0x7BEF
  #define DARK       0x39E7
  #define BLUE       0xF800
  #define ORNG       0x051F
  #define RED        0x001F
  #define BLK        0x0000

  #define X_SIZE     360
  #define X_BASE     0
  #define Y_SIZE     200
  #define Y_BASE     0 
  #define MIN_X      11
  #define MIN_Y      14
  #define MAX_X      (X_SIZE + MIN_X)
  #define MAX_Y      (Y_SIZE + MIN_Y)

  //----------------------------------------------------------------------------
  
  extern u8   Track[], YnHide[];
  extern uc16 Palette[], BmpColor[];
  extern u32  PopBuf[];
  extern u8   PopHide;
  extern u16  Nx0, Ny0, Bx1, Bx2, By1, By2, FpsCnt;

  void PopProcess(void);
  void UpdtPopMenu(void);
  void PopSTR(u8 Hue, u8 Inv, u8 *s);
  void RowPosi(u16 x, u16 y);
  void AddStr(u8 ColorNo, u8 Mode, u8 *Str);
  void PrintStr(u8 ColorNo, u8 Mode, u8 *Str);
  void PrintClk(u16 x, u16 y, u8 Phase);
  void UpdateTag(void);
  void DisplayWaveForm(void);
  void Litimg(void);

#endif

/********************************* END OF FILE ********************************/
