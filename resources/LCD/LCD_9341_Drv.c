/********************* (C) COPYRIGHT 2017 e-Design Co.,Ltd. ********************
 Brief   : ILI9341 LCD Drivers                                    Author : bure
********************************************************************************
   case BLOCK_X:
	   SetBlockX(Var);
	   return SUCC;
   case BLOCK_Y:
	   SetBlockY(Var);
	   return SUCC;
   case PIXEL_X:
	   SetPixelX((u16)Var);
	   return SUCC;
   case PIXEL_Y:
	   SetPixelY((u16)Var);
	   return SUCC;
   case WrPIXEL:
	   LCD_DAT_W(Var);
	   return SUCC;
   case RdPIXEL:
	   return ReadPixel();

    
*******************************************************************************/
void LCD_Init(u8 Ctrl);
void SetBlockX(u32 x1_x2);
void SetBlockY(u32 y1_y2);
void SetBlock(u16 x1, u16 x2, u16 y1, u16 y2);
void SetPosi(u16 x, u16 y);
void SetPixel(u16 Color);
void SetPixelX(u16 x);
void SetPixelY(u16 y);
u16  ReadPixel(void);
//equal _Bios(WrPIXEL, Data);
#define LCD_DAT_W(Data) {*(u16*)LCD_DATA_ODR = (u16)Data;                      \
                         LCD_nWR = 0;                                          \
                         LCD_nWR = 1;}

#define LCD_CMD_W(Cmd)  {LCD_D_C = 0;                                          \
                         LCD_DAT_W(Cmd);                                       \
                         LCD_D_C = 1;}

#define LCD_PXL_R(Var)  {LCD_CMD_W(0x2D);                                      \
                         LCD_nRD = 0;                                          \
                         Var = *(u16*)LCD_DATA_IDR;                            \
                         LCD_nRD = 1;}


/*******************************************************************************
  Init_LCD : ILI9341 LCD 显示模块初始化
  ----------------------------------------------------------------------------
  Ctrl:  Bit7/6/5 = Y/X/V Mirror  Bit3 = RGB Ctrl
*******************************************************************************/
void LCD_Init(u8 Ctrl)
{
//--------------------------------------------------------
    LCD_nCS = 0;
    LCD_nRD = 1;
    LCD_D_C = 1;
    LCD_nRST = 1;
    Delay_uS(1000);          // Delay 1ms
    LCD_nRST = 0;
    Delay_uS(1000);          // Delay 1ms
    LCD_nRST = 1;
    Delay_uS(150000);        // Delay 150 ms


//  *****ILI9341V + HSD28 东莞一众*********
/**/
    LCD_CMD_W(0x3A);
    LCD_DAT_W(0x55);

//  LCD_CMD_W(0xF6);       // 接口控制
//  LCD_DAT_W(0x01);
//  LCD_DAT_W(0x33);       // 0x11修复读显示问题

    LCD_CMD_W(0xB5);
    LCD_DAT_W(0x04);
    LCD_DAT_W(0x04);
    LCD_DAT_W(0x0A);
    LCD_DAT_W(0x14);

    LCD_CMD_W(0x35);
    LCD_DAT_W(0x00);

    LCD_CMD_W(0xCF);
    LCD_DAT_W(0x00);
    LCD_DAT_W(0xEA);
    LCD_DAT_W(0XF0);

    LCD_CMD_W(0xED);
    LCD_DAT_W(0x64);
    LCD_DAT_W(0x03);
    LCD_DAT_W(0X12);
    LCD_DAT_W(0X81);
    LCD_CMD_W(0xE8);
    LCD_DAT_W(0x85);
    LCD_DAT_W(0x00);
    LCD_DAT_W(0x78);

    LCD_CMD_W(0xCB);
    LCD_DAT_W(0x39);
    LCD_DAT_W(0x2C);
    LCD_DAT_W(0x00);
    LCD_DAT_W(0x33);
    LCD_DAT_W(0x06);

    LCD_CMD_W(0xF7);
    LCD_DAT_W(0x20);

    LCD_CMD_W(0xEA);
    LCD_DAT_W(0x00);
    LCD_DAT_W(0x00);

    LCD_CMD_W(0xC0);         // Power control
    LCD_DAT_W(0x21);         // VRH[5:0]

    LCD_CMD_W(0xC1);         // Power control
    LCD_DAT_W(0x10);         // BT[3:0]

    LCD_CMD_W(0xC5);         // VCM control
    LCD_DAT_W(0x4F);
    LCD_DAT_W(0x38);

    LCD_CMD_W(0xC7);         // VCM control2
    LCD_DAT_W(0Xa5);

    LCD_CMD_W(0x36);         // Memory Access Control
    LCD_DAT_W(Ctrl + 0x04);  // 04:BGR,08:RGB

    LCD_CMD_W(0xB1);
    LCD_DAT_W(0x00);
    LCD_DAT_W(0x13);

    LCD_CMD_W(0xB6);         // Display Function Control
    LCD_DAT_W(0x0A);
    LCD_DAT_W(0xA2);

    LCD_CMD_W(0xF2);         // 3Gamma Function Disable
    LCD_DAT_W(0x02);

    LCD_CMD_W(0xE0);         // Set Gamma
    LCD_DAT_W(0x0F);
    LCD_DAT_W(0x27);
    LCD_DAT_W(0x24);
    LCD_DAT_W(0x0C);
    LCD_DAT_W(0x10);
    LCD_DAT_W(0x08);
    LCD_DAT_W(0x55);
    LCD_DAT_W(0X87);
    LCD_DAT_W(0x45);
    LCD_DAT_W(0x08);
    LCD_DAT_W(0x14);
    LCD_DAT_W(0x07);
    LCD_DAT_W(0x13);
    LCD_DAT_W(0x08);
    LCD_DAT_W(0x00);

    LCD_CMD_W(0XE1);         // Set Gamma
    LCD_DAT_W(0x00);
    LCD_DAT_W(0x0F);
    LCD_DAT_W(0x12);
    LCD_DAT_W(0x05);
    LCD_DAT_W(0x11);
    LCD_DAT_W(0x06);
    LCD_DAT_W(0x25);
    LCD_DAT_W(0x34);
    LCD_DAT_W(0x37);
    LCD_DAT_W(0x01);
    LCD_DAT_W(0x08);
    LCD_DAT_W(0x07);
    LCD_DAT_W(0x2B);
    LCD_DAT_W(0x34);
    LCD_DAT_W(0x0F);

//  =======================================

    LCD_CMD_W(0x11);         // Exit Sleep
    Delay_uS(120000);
    LCD_CMD_W(0x29);         // Display on
}
/*******************************************************************************
  Set Block : 指定显示块   输入: X、Y坐标
*******************************************************************************/
void SetBlock(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_CMD_W(0x2A);
    LCD_DAT_W(y1 >> 8);
    LCD_DAT_W(y1 & 255);
    LCD_DAT_W(y2 >> 8);
    LCD_DAT_W(y2 & 255);

    LCD_CMD_W(0x2B);
    LCD_DAT_W(x1 >> 8);
    LCD_DAT_W(x1 & 255);
    LCD_DAT_W(x2 >> 8);
    LCD_DAT_W(x2 & 255);

    LCD_CMD_W(0x2C);      // Ready to write GRAM
}
/*******************************************************************************
  SetBlockX: 指定 X 显示区域
*******************************************************************************/
void SetBlockX(u32 x1_x2) //equal _Bios(BLOCK_X, x1_x2);
{
    LCD_CMD_W(0x2B);
    LCD_DAT_W((u8)(x1_x2 >> 24));
    LCD_DAT_W((u8)(x1_x2 >> 16));
    LCD_DAT_W((u8)(x1_x2 >>  8));
    LCD_DAT_W((u8)(x1_x2));
    LCD_CMD_W(0x2C);      // Ready to write GRAM
}
/*******************************************************************************
  SetBlockY: 指定 Y 显示区域 
*******************************************************************************/
void SetBlockY(u32 y1_y2) //equal _Bios(BLOCK_Y, y1_y2);
{
    LCD_CMD_W(0x2A);
    LCD_DAT_W((u8)(y1_y2 >> 24));
    LCD_DAT_W((u8)(y1_y2 >> 16));
    LCD_DAT_W((u8)(y1_y2 >>  8));
    LCD_DAT_W((u8)(y1_y2));
    LCD_CMD_W(0x2C);      // Ready to Write GRAM
}
/*******************************************************************************
  Set Position : 指定显示位置   输入: X、Y坐标
*******************************************************************************/
void SetPosi(u16 x, u16 y)
{
    LCD_CMD_W(0x2A);
    LCD_DAT_W(0x00);
    LCD_DAT_W(y & 255);
    LCD_DAT_W(0x00);
    LCD_DAT_W(239);

    LCD_CMD_W(0x2B);
    LCD_DAT_W(x >> 8);
    LCD_DAT_W(x & 255);
    LCD_DAT_W(0x01);
    LCD_DAT_W(0x3F);

    LCD_CMD_W(0x2C);
}

void SetPixelX(u16 x) //equal _Bios(PIXEL_X, x);
{
    LCD_CMD_W(0x2B);
    LCD_DAT_W(x >> 8);
    LCD_DAT_W(x & 255);
    LCD_DAT_W(0x01);
    LCD_DAT_W(0x3F);

    LCD_CMD_W(0x2C);
}
void SetPixelY(u16 y) //equal _Bios(PIXEL_Y, y);
{
    LCD_CMD_W(0x2A);
    LCD_DAT_W(0x00);
    LCD_DAT_W(y & 255);
    LCD_DAT_W(0x00);
    LCD_DAT_W(239);

    LCD_CMD_W(0x2C);

}
void SetPixel(u16 Color) 
{
    LCD_DAT_W(Color);
}

u16 ReadPixel(void) //equal _Bios(RdPIXEL, 0);
{
    u16 Tmp;

    LCD_CMD_W(0x2E);
    LCD_PORT_IN();
    LCD_nRD = 0; 
    Delay_uS(1);
    LCD_nRD = 1;
    Delay_uS(1); 
    LCD_nRD = 0; 
    Delay_uS(1);
    Tmp = (GPIOE->IDR);
    LCD_nRD = 1; 
    Tmp = (Tmp & 0xF800) + ((Tmp << 3) & 0x07E0);
    LCD_nRD = 0; 
    Delay_uS(1);
    Tmp |= (GPIOE->IDR) >> 11;
    LCD_nRD = 1; 
    LCD_PORT_OUT();

    return Tmp;
}
/******************************** END OF FILE *********************************/
