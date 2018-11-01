#ifndef __FILES_H
#define __FILES_H

#include "STM32F10x.h"


#define  SAVE_ICO_X0    299
#define  SAVE_ICO_Y0    225  
                           
#define  FILE_ICO_X0    299
#define  FILE_ICO_Y0    225 


u8  Save_Bmp(u16 FileNum);
u8  Save_Buf(u16 FileNo);
u8  Save_Csv(s16 FileNo);
u8  ReadParameter(void);
u8  SaveParameter(void);
u8  RestoreParameter(void);
u16 Load_File_Num(u8 Tpye);

void File_Num(void);
void PrintClk(u16 x0, u16 y0, u8 Phase);
void Clear_File_ICO(u16 x, u16 y);
void DispFileInfo(u8 Info);


#endif
/*********************************  END OF FILE  ******************************/
