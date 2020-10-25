//https://gist.github.com/iwalpola/6c36c9573fd322a268ce890a118571ca

/*
PD9 - \WR
PD10 - \RD
PD8 - D/C
PA2 - \CS
PA3 - \RST
*/

#define LCD_DAT_W(Data) { GPIOE->ODR = Data; \
                          GPIOD->BRR = GPIO_Pin_9; \
                          GPIOD->BSRR = GPIO_Pin_9; }

#define LCD_CMD_W(Cmd)  { GPIOD->BRR = GPIO_Pin_8; /*LCD_D_C = 0;*/ \
                          LCD_DAT_W(Cmd); \
                          GPIOD->BSRR = GPIO_Pin_8; /*LCD_D_C = 1;*/ }

#define LCD_PORT_IN()  GPIOE->CRL = GPIOE->CRH = 0x44444444
#define LCD_PORT_OUT() GPIOE->CRL = GPIOE->CRH = 0x33333333
                 
void Set_Pixel(uint_fast16_t Color)
{
  LCD_DAT_W(Color);
}

uint32_t sharedPixel;

uint32_t ReadPixel(void)
{
  // reading word from ILI934 
  // three reads to obtain two pixel information
  //
  // 1.   pixel: [BBBB BBBB GGGG GGGG]  
  // 1&2. pixel: [RRRR RRRR bbbb bbbb] 
  // 2.   pixel: [gggg gggg rrrr rrrr]

  // RGB565 model:
  // MSB             LSB
  // bbbb bggg gggr rrrr

  uint32_t Tmp;
  if (sharedPixel != -1)
  {
    // take .... .... BBBB B... blue component from previous read 
    Tmp = (sharedPixel & 0x00f8) << 8;

    uint32_t Tmp2 = GPIOE->IDR;  // GGGG GGGG RRRR RRRR

    GPIOD->BSRR = GPIO_Pin_10;   // LCD_nRD = 1; 
    sharedPixel = -1;
    Tmp |= (Tmp2 & 0xFC00) >> 5; // join GGGG GG.. .... .... green component
    Tmp |= (Tmp2 & 0x00F8) >> 3; // join .... .... RRRR R... red component
    GPIOD->BRR = GPIO_Pin_10;    // LCD_nRD = 0; 
    return Tmp;
  } else
  {   
    Tmp = GPIOE->IDR;            // BBBB BBBB GGGG GGGG

    GPIOD->BSRR = GPIO_Pin_10;   // LCD_nRD = 1; 
    GPIOD->BRR = GPIO_Pin_10;    // LCD_nRD = 0; 
    Tmp = (Tmp & 0xF800) | ((Tmp & 0x00FC) << 3);  
    // keep BBBB B... .... .... blue component 
    // take .... .... GGGG GG.. green component and shift to left 3 bits
    asm("nop");
    asm("nop");
    asm("nop");
    sharedPixel = GPIOE->IDR;    // RRRR RRRR bbbb bbbb

    GPIOD->BSRR = GPIO_Pin_10;   // LCD_nRD = 1; 
    Tmp |= (sharedPixel) >> 11;  // shift to red and join
    GPIOD->BRR = GPIO_Pin_10;    // LCD_nRD = 0; 
    return Tmp;
  }
}

void ReadStart(void)
{
    sharedPixel = -1;
    LCD_CMD_W(0x2E);
    LCD_PORT_IN();

    GPIOD->BRR = GPIO_Pin_10;//    LCD_nRD = 0; 
    GPIOD->BSRR = GPIO_Pin_10;//    LCD_nRD = 1;
    GPIOD->BRR = GPIO_Pin_10;//    LCD_nRD = 0; 
}

void ReadFinish(void)
{
    GPIOD->BSRR = GPIO_Pin_10;//    LCD_nRD = 1;
    LCD_PORT_OUT();
}

uint16_t Get_Pixel()
{
  return ReadPixel();
}

void Set_Block(int x1, int y1, int x2, int y2)
{
  x2--;
  y2--;

  if (x2==x1) x2++; // workaround for reading pixels in single column 

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

void Set_Posi(uint_fast16_t x, uint_fast16_t y)
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
