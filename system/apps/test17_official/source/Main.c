/**********************(C) COPYRIGHT 2017 e-Design Co.,Ltd.*********************
 * FileName    : Main.c
 * Description : 
 * Author      : SNAKE
 * History     : 
 *              Beta 1.04  
 *              APP V1.01  
 *******************************************************************************/
#include "MyDefine.h"

void Hardware_Init(void);
void Show_Startup_Info(void);
void FPGA_Startup_Wave(void);
void Set_SysParam(void);
void SysTick_Handler(void);

bool appRunning = true;
void ExitApplication()
{
  BIOS::DBG::Print("Exiting app");
  appRunning = false;
}

BIOS::OS::TInterruptHandler pOldSysTick = nullptr;

void Interrupt_Init()
{
  pOldSysTick = BIOS::OS::GetInterruptVector(BIOS::OS::ISysTick);

  BIOS::OS::SetInterruptVector(BIOS::OS::ISysTick, [](){
    pOldSysTick();
    SysTick_Handler();
  });
}

void Interrupt_Deinit()
{
  BIOS::OS::SetInterruptVector(BIOS::OS::ISysTick, pOldSysTick);
}

//===============================APP
char   APP_VERSION[] = "APP V1.01x";                 // 


/*******************************************************************************
 * FunctionName : main
 * Description  : 
*******************************************************************************/
int main(void)
{
    u8  M_Flag = 0;
    u8  File_ST = 0;
    Interrupt_Init();
    Hardware_Init();                             // Ӳ
    Show_Startup_Info();                         // 
    ReadParameter();                             // 
    Clr_Scrn(BLK);                               // 
    FPGA_Startup_Wave();
    Set_SysParam();
    ShowAllMenu();
    File_Num();                                  // 

    while(appRunning)
    {
        ShutdownTest();                          // 
        ShortcutBMP();                           // K1
        if(Menu.flag)KeyQuickAct();              // T1

        // shadowing original key value!
        int gKeyActv = BIOS::KEY::GetKey();

        if(gKeyActv)
        {
            switch(gKeyActv)
            {
            case BIOS::KEY::F1:
                gKeyActv = 0;
                {
                    Menu.flag = 1;
                    Item.flag = 0;
                    SubItem.flag = 0;
                    Process();
                    ShowSmplStatus(STOP);
                    if(gBeepFlag)
                    {
                        gBeepFlag = 0;
                        BIOS::SYS::Beep(50);
                    }
                }
                break;

            case BIOS::KEY::F2:
                if(Menu.flag)
                {
                    ShowPopItem(Menu.index);
                    Menu.flag = 0;
                    Item.flag = 1;
                }
                else if(Item.flag)
                {
                    if(Menu.index == FILE_CTRL)
                    {
                        if(gItemIndexNum[FILE_CTRL] == EXIT)
                        {
                            ExitApplication();
                        }
                    }
                    else if(Menu.index == IN_TYPE)
                    {
                        if(gItemParam[INTYPE]!=IN_USER)
                        {
                            ShowSubItem(Menu.index, gItemIndexNum[Menu.index]);
                            Item.flag = 0;
                            SubItem.flag = 1;
                        }
                    }
                    else if(Menu.index == OUT_TYPE)
                    {
                        if(gItemParam[OUTTYPE]==OUT_PWM)
                        {
                            ShowSubItem(Menu.index, gItemIndexNum[Menu.index]);
                            Item.flag = 0;
                            SubItem.flag = 1;
                        }
                    }
                }
                break;

            case BIOS::KEY::F3:

                if(Item.flag)
                {
                    if(Menu.index == TIME_SET)gRunFlag = 1; // TimeSet 
                    Menu.flag = 1;
                    Item.flag = 0;
                    M_Flag = 1;
                }
                else if(SubItem.flag)
                {
                    AnalyzeFrame(gItemParam[INTYPE]);       // Э
                    memcpy(gLCD_Buf, gLCD_Backup, LCD_BUF_WIDTH);
                    ShowWaveToLCD();
                    Menu.flag = 1;
                    ShowPopItem(Menu.index);
                    SubItem.flag = 0;
                    Item.flag = 1;
                    Menu.flag = 0;
                }
                else
                {
                    Menu.flag = 1;
                    Item.flag = 0;
                    M_Flag = 1;
                }
                break;

            case BIOS::KEY::F4:
                if(Item.flag)
                {
                    if(Menu.index == TRIGGER)
                    {
                        // TriCond
                        if((gItemIndexNum[TRIGGER] == 1) && ((gItemParam[TRICOND] & 0x00FF) != 0) && ((gItemParam[TRICOND] & 0x00FF) != 5))
                        {
                            if((gItemParam[TRICOND] >> (8 + (gItemParam[TRICOND] & 0x00FF) - 1)) & 0x01)
                                // 
                                gItemParam[TRICOND] = gItemParam[TRICOND] & (~(0x01 << (8 + (gItemParam[TRICOND] & 0x00FF) - 1)));
                            else
                                // 
                                gItemParam[TRICOND] = gItemParam[TRICOND] | (0x01 << (8 + (gItemParam[TRICOND] & 0x00FF) - 1));
                        }
                        // TriMask
                        else if((gItemIndexNum[TRIGGER] == 2) && ((gItemParam[TRIMASK] & 0x00FF) != 0) && ((gItemParam[TRIMASK] & 0x00FF) != 5))
                        {
                            if((gItemParam[TRIMASK] >> (8 + (gItemParam[TRIMASK] & 0x00FF) - 1)) & 0x01)
                                // 
                                gItemParam[TRIMASK] = gItemParam[TRIMASK] & (~(0x01 << (8 + (gItemParam[TRIMASK] & 0x00FF) - 1)));
                            else
                                // 
                                gItemParam[TRIMASK] = gItemParam[TRIMASK] | (0x01 << (8 + (gItemParam[TRIMASK] & 0x00FF) - 1));
                        }
                    }
                    else if(Menu.index == OUT_TYPE)
                    {
                        PIO_SendData(gItemParam[OUTTYPE]);
                    }
                    else if(Menu.index == FILE_CTRL)
                    {
                        if(gItemIndexNum[FILE_CTRL] == EXIT)
                        {
                            ExitApplication();
                        }
                        else if(gItemIndexNum[FILE_CTRL] == BMP)
                        {
                            Clear_File_ICO(SAVE_ICO_X0, SAVE_ICO_Y0);
                            File_ST = Save_Bmp(gItemParam[SAVEBMP]);
                        }
                        else if(gItemIndexNum[FILE_CTRL] == CSV)
                        {
                            Clear_File_ICO(SAVE_ICO_X0, SAVE_ICO_Y0);
                            File_ST = Save_Csv(gItemParam[SAVECSV]);
                        }
                        DispFileInfo(File_ST);
                        gBatFlag = 1;
                        ShowBattery();
                    }
                    else if(Menu.index == SETTING)
                    {
                        if(gItemIndexNum[SETTING] == SAVEPARAM)
                        {
                            File_ST = SaveParameter();
                        }
                        else if(gItemIndexNum[SETTING] == RSTPARAM)
                        {
                            M_Flag = 1;
                            RestoreParameter();
                            File_ST = SaveParameter();
                            gBatFlag = 1;
                            ShowAllMenu();
                        }
                        if(gItemIndexNum[SETTING]<=RSTPARAM)
                        {
                            DispFileInfo(File_ST);
                            gBatFlag = 1;
                            ShowBattery();
                        }  
                    }
                }
                else if(Menu.flag)                // 
                {
                    if(gItemIndexNum[TIME_SET] < gItemMaxNum[TIME_SET])
                    {
                        gItemIndexNum[TIME_SET]++;
                    }
                    else
                    {
                        gItemIndexNum[TIME_SET] = 0;
                    }
                }
                break;
            case BIOS::KEY::Up: // Up
                if(Menu.flag)
                {
                    if(gItemIndexNum[TIME_SET] == TB_T1)
                    {
                        if((gItemParam[T1POSI] + gItemParamStep[T1POSI]) < gItemParam[T2POSI])
                        {
                            gItemParam[T1POSI] = gItemParam[T1POSI] + gItemParamStep[T1POSI];
                        }
                        else
                        {
                            gItemParam[T1POSI] =  gItemParam[T2POSI];
                        }
                    }
                    else
                    {
                        if(gItemParam[TIMEBASE + gItemIndexNum[TIME_SET]] < gItemParamMax[TIMEBASE + gItemIndexNum[TIME_SET]])
                        {
                            gItemParam[TIMEBASE + gItemIndexNum[TIME_SET]] =
                                gItemParam[TIMEBASE + gItemIndexNum[TIME_SET]] + gItemParamStep[TIMEBASE + gItemIndexNum[TIME_SET]];
                        }
                    }

                    if(gItemIndexNum[TIME_SET] == TB_TB)
                    {
                        ZC_Scale();
                    }
                    else if(gItemIndexNum[TIME_SET] == TB_ZC) 
                    {
                        gTimeBase = gItemParam[TIMEBASE];
                        memcpy(gLCD_Buf, gLCD_Backup, LCD_BUF_WIDTH);
                        ShowWaveToLCD();
                    }
                    else if(gItemIndexNum[TIME_SET] == TB_X) 
                    {
                        gXposiAdd = 1;
                        ZC_Scale();
                    }
                    else
                    {
                        memcpy(gLCD_Buf, gLCD_Backup, LCD_BUF_WIDTH);
                        ShowWaveToLCD();
                    }
                }
                else if(Item.flag)                  
                {
                    if(gItemIndexNum[Menu.index] > 0)
                    {
                        gItemIndexNum[Menu.index]--;
                    }
                }
                else if(SubItem.flag)         
                {
                    switch(Menu.index)
                    {
                    case IN_TYPE:
                        if(INPUT_SUB_INDEX_NUM > 0)
                        {
                            INPUT_SUB_INDEX_NUM--;
                        }
                        break;
                    case OUT_TYPE:
                        if(OUT_SUB_INDEX_NUM > 0)
                        {
                            OUT_SUB_INDEX_NUM--;
                        }
                        break;
                    }  
                }

                break;
            case BIOS::KEY::Down: // Down
                if(Menu.flag)
                {
                    if(gItemIndexNum[TIME_SET] == TB_T2)
                    {
                        if((gItemParam[T2POSI] - gItemParamStep[T2POSI]) >= gItemParam[T1POSI])
                        {
                            gItemParam[T2POSI] = gItemParam[T2POSI] - gItemParamStep[T2POSI];
                        }
                        else
                        {
                            gItemParam[T2POSI] = gItemParam[T1POSI];
                        }
                    }
                    else
                    {
                        if(gItemParam[TIMEBASE + gItemIndexNum[TIME_SET]] > gItemParamMin[TIMEBASE + gItemIndexNum[TIME_SET]])
                        {
                            gItemParam[TIMEBASE + gItemIndexNum[TIME_SET]] =
                                gItemParam[TIMEBASE + gItemIndexNum[TIME_SET]] - gItemParamStep[TIMEBASE + gItemIndexNum[TIME_SET]];
                        }
                    }

                    if(gItemIndexNum[TIME_SET] == TB_TB)
                    {
                        ZC_Scale();
                    }
                    else if(gItemIndexNum[TIME_SET] == TB_ZC) 
                    {
                        gTimeBase = gItemParam[TIMEBASE];
                        memcpy(gLCD_Buf, gLCD_Backup, LCD_BUF_WIDTH);
                        ShowWaveToLCD();
                    }
                    else if(gItemIndexNum[TIME_SET] == TB_X) 
                    {
                        gXposiDec = 1;
                        ZC_Scale();
                    }
                    else
                    {
                        memcpy(gLCD_Buf, gLCD_Backup, LCD_BUF_WIDTH);
                        ShowWaveToLCD();
                    }
                }
                else if(Item.flag)
                {
                    if(gItemIndexNum[Menu.index] < gItemMaxNum[Menu.index])
                    {
                        gItemIndexNum[Menu.index]++;
                    }
                }
                else if(SubItem.flag)                
                {
                    switch(Menu.index)
                    {
                    case IN_TYPE:
                        if(INPUT_SUB_INDEX_NUM < INPUT_SUB_MAX_NUM)
                        {
                            INPUT_SUB_INDEX_NUM++;
                        }
                        break;
                    case OUT_TYPE:
                        if(OUT_SUB_INDEX_NUM < OUT_SUB_MAX_NUM)
                        {
                            OUT_SUB_INDEX_NUM++;
                        }
                        break;
                    }
                }

                break;

            case BIOS::KEY::Left: // Left
                if(Menu.flag)
                {
                    if(Menu.index > 0) Menu.index--;
                }
                else if(Item.flag)
                {
                    if((Menu.index == TIME_SET) && (gItemIndexNum[TIME_SET] == TB_X))
                    {
                        gXposiDec = 1;
                        UpdateXposiTime();
                        ShowScaleRuleTime(RULE_X, RULE_Y, SHOW_WIDTH, gItemParam[TIMEBASE]);
                    }
                    else if((Menu.index == TIME_SET) && (gItemIndexNum[TIME_SET] == TB_T1))
                    {
                        if((gItemParam[I_INDEX] - gItemParamStep[I_INDEX]) >= gItemParamMin[I_INDEX])
                        {
                            gItemParam[I_INDEX] = gItemParam[I_INDEX] - gItemParamStep[I_INDEX];
                        }
                        else
                        {
                            gItemParam[I_INDEX] = gItemParamMin[I_INDEX];
                        }
                        while(READ_KEY == K4_HOLD)
                        {
                            if((gItemParam[I_INDEX] - gItemParamStep[I_INDEX]) >= gItemParamMin[I_INDEX])
                            {
                                gItemParam[I_INDEX] = gItemParam[I_INDEX] - gItemParamStep[I_INDEX];
                            }
                            else
                            {
                                gItemParam[I_INDEX] = gItemParamMin[I_INDEX];
                            }
                            ShowPopItem(Menu.index);
                        }
                    }
                    else if((Menu.index == TIME_SET) && (gItemIndexNum[TIME_SET] == TB_T2))
                    {
                        if((gItemParam[I_INDEX] - gItemParamStep[I_INDEX]) >= gItemParam[T1POSI])
                        {
                            gItemParam[I_INDEX] = gItemParam[I_INDEX] - gItemParamStep[I_INDEX];
                        }
                        else
                        {
                            gItemParam[I_INDEX] = gItemParam[T1POSI];
                        }
                        while(READ_KEY == K4_HOLD)
                        {
                            if((gItemParam[I_INDEX] - gItemParamStep[I_INDEX]) >= gItemParam[T1POSI])
                            {
                                gItemParam[I_INDEX] = gItemParam[I_INDEX] - gItemParamStep[I_INDEX];
                            }
                            else
                            {
                                gItemParam[I_INDEX] = gItemParam[T1POSI];
                            }
                            ShowPopItem(Menu.index);
                        }
                    }
                    else if((Menu.index == TRIGGER) && ((gItemIndexNum[TRIGGER] == 1) || (gItemIndexNum[TRIGGER] == 2)))
                    {
                        if((gItemParam[I_INDEX] & 0x00FF) > gItemParamMin[I_INDEX])
                        {
                            gItemParam[I_INDEX] = gItemParam[I_INDEX] - gItemParamStep[I_INDEX];
                        }
                    }
                    else
                    {
                        if((gItemParam[I_INDEX] - gItemParamStep[I_INDEX]) >= gItemParamMin[I_INDEX])
                        {
                            gItemParam[I_INDEX] = gItemParam[I_INDEX] - gItemParamStep[I_INDEX];
                        }
                        else
                        {
                            gItemParam[I_INDEX] = gItemParamMin[I_INDEX];
                        }
                    }

                    if(Menu.index == OUT_TYPE) PIO_Init(gItemParam[OUTTYPE]);
                    if(Menu.index == IN_TYPE) DefaultTir(gItemParam[INTYPE]);
                }
                else if(SubItem.flag)                
                {
                    if((Menu.index == IN_TYPE) && (gItemIndexNum[IN_TYPE] == 0))
                    {
                        if(INPUT_SUB_PARAM > INPUT_SUB_PARAM_MIN)
                        {
                            INPUT_SUB_PARAM--;
                        }
                    }
                    else if(Menu.index == OUT_TYPE)
                    {
                        if(OUT_SUB_PARAM > OUT_SUB_PARAM_MIN)
                        {
                            OUT_SUB_PARAM--;
                        }
                    }
                }

                break;
            case BIOS::KEY::Right: // Right
                if(Menu.flag)
                {
                    if(Menu.index < Menu.total) Menu.index++;
                }
                else if(Item.flag)
                {
                    if((Menu.index == TIME_SET) && (gItemIndexNum[TIME_SET] == TB_X))
                    {
                        gXposiAdd = 1;
                        UpdateXposiTime();
                        ShowScaleRuleTime(RULE_X, RULE_Y, SHOW_WIDTH, gItemParam[TIMEBASE]);
                    }
                    else if((Menu.index == TIME_SET) && (gItemIndexNum[TIME_SET] == TB_T1))
                    {
                        if((gItemParam[I_INDEX] + gItemParamStep[I_INDEX]) < gItemParam[T2POSI])
                        {
                            gItemParam[I_INDEX] = gItemParam[I_INDEX] + gItemParamStep[I_INDEX];
                        }
                        else
                        {
                            gItemParam[I_INDEX] =  gItemParam[T2POSI];
                        }
                        while(READ_KEY == K4_HOLD)
                        {
                            if((gItemParam[I_INDEX] + gItemParamStep[I_INDEX]) < gItemParam[T2POSI])
                            {
                                gItemParam[I_INDEX] = gItemParam[I_INDEX] + gItemParamStep[I_INDEX];
                            }
                            else
                            {
                                gItemParam[I_INDEX] =  gItemParam[T2POSI];
                            }
                            ShowPopItem(Menu.index);
                        }
                    }
                    else if((Menu.index == TIME_SET) && (gItemIndexNum[TIME_SET] == TB_T2))
                    {
                        if((gItemParam[I_INDEX] + gItemParamStep[I_INDEX]) < gItemParamMax[I_INDEX])
                        {
                            gItemParam[I_INDEX] = gItemParam[I_INDEX] + gItemParamStep[I_INDEX];
                        }
                        else
                        {
                            gItemParam[I_INDEX] =  gItemParamMax[I_INDEX];
                        }
                        while(READ_KEY == K4_HOLD)
                        {
                            if((gItemParam[I_INDEX] + gItemParamStep[I_INDEX]) < gItemParamMax[I_INDEX])
                            {
                                gItemParam[I_INDEX] = gItemParam[I_INDEX] + gItemParamStep[I_INDEX];
                            }
                            else
                            {
                                gItemParam[I_INDEX] =  gItemParamMax[I_INDEX];
                            }
                            ShowPopItem(Menu.index);
                        }
                    }
                    else if((Menu.index == TRIGGER) && ((gItemIndexNum[TRIGGER] == 1) || (gItemIndexNum[TRIGGER] == 2)))
                    {
                        if((gItemParam[I_INDEX] & 0x00FF) < gItemParamMax[I_INDEX])
                        {
                            gItemParam[I_INDEX] = gItemParam[I_INDEX] + gItemParamStep[I_INDEX];
                        }
                    }
                    else
                    {
                        if((gItemParam[I_INDEX] + gItemParamStep[I_INDEX]) < gItemParamMax[I_INDEX])
                        {
                            gItemParam[I_INDEX] = gItemParam[I_INDEX] + gItemParamStep[I_INDEX];
                        }
                        else
                        {
                            gItemParam[I_INDEX] =  gItemParamMax[I_INDEX];
                        }
                    }
                    if(Menu.index == OUT_TYPE) PIO_Init(gItemParam[OUTTYPE]);
                    if(Menu.index == IN_TYPE) DefaultTir(gItemParam[INTYPE]);
                }
                else if(SubItem.flag)               
                {
                    if((Menu.index == IN_TYPE) && (gItemIndexNum[IN_TYPE] == 0))
                    {
                        if(INPUT_SUB_PARAM < INPUT_SUB_PARAM_MAX)
                        {
                            INPUT_SUB_PARAM++;
                        }
                    }
                    else if(Menu.index == OUT_TYPE)
                    {
                        if(OUT_SUB_PARAM < OUT_SUB_PARAM_MAX)
                        {
                            OUT_SUB_PARAM++;
                        }
                    }
                }
                break;
            }
//          if(gKeyActv != K1_ACTp) Beep_mS(50);
            if(gKeyActv & 0xF0F0) BIOS::SYS::Beep(50);  // TODO: gabo

            ShowTopTitle();
            if(Item.flag && (gKeyActv != K1_ACTn)) ShowPopItem(Menu.index);
            if(SubItem.flag)                    // 
            {
                ShowSubItem(Menu.index, gItemIndexNum[Menu.index]);
            }
            HW_Ctrl(Menu.index, gItemIndexNum[Menu.index]);
            ShowWindowPrecent();
            if(Menu.index == TIME_SET) ShowTimeBaseStr();
            ShowTimeMarkValue();
            ShowMeasureValue();
            ResetPowerOffTime();
            gKeyActv = 0;
        }
        //===================Test==========================
        if(M_Flag)  //
        {
            M_Flag = 0;
            memcpy(gLCD_Buf, gLCD_Backup, LCD_BUF_WIDTH);
            ShowWaveToLCD();
            if(Menu.index == TIME_SET)
            {
                ZC_Scale();
            }
        }
        StandbyAndPowerOff();
        ShowBattery();
    }  
    Interrupt_Deinit();
    return 0;
}

/*******************************************************************************
 * FunctionName : Hardware_Init
 * Description  : Ӳ
 * Param        : void 
*******************************************************************************/
void Hardware_Init(void)
{
    BIOS::SYS::Beep(200);
    DiskConfig();                 // 
    __Bios(IN_PORT, INIT);        // DAC
//  __Bios(IN_PORT, 3000);        // 3V
    __Bios(IN_PORT, gItemParam[THRESHOLD]*100);

}

/*******************************************************************************
 * FunctionName : Show_Startup_Info
 * Description  : 
 * Param        : void 
*******************************************************************************/
void Show_Startup_Info(void)
{
    Disp_Str8x14(0,      90, BLK, BLK, PRN, "                                        ");
    Disp_Str8x14(0,      70, BLK, BLK, PRN, "                                        ");
    Disp_Str8x14(8,      90, WHT, BLK, PRN, "       Logic Analyzer APP");
    Disp_Str8x14(27 * 8, 90, WHT, BLK, PRN,                           APP_VERSION);
    Disp_Str8x14(8,      70, WHT, BLK, PRN, "        System Initializing...       ");
    BIOS::SYS::DelayMs(1000);
    if(__Bios(FPGADEV, DONE) == 0) while(1){}       // FPGA
    
}

/*******************************************************************************
 * FunctionName : FPGA_Startup_Wave
 * Description  : 
 * Param        : void 
*******************************************************************************/
void FPGA_Startup_Wave(void)
{
    u8  FpgaInfo = 0;

    FPGA_ReadWrite(FPGA_RST, 0, 0);                        // оƬ
    FPGA_ReadWrite(WINDOW_Y, 1, YHIGH);                    // 
    FPGA_ReadWrite(WINDOW_X, 2, XWDTH);                    // 
    memset(gLCD_Buf, 0, LCD_BUF_WIDTH);
    memset(gLCD_Backup, 0, LCD_BUF_WIDTH);
    FPGA_LCD_RW(WFRM_SAVE, XWDTH, gLCD_Buf);               // 
    BIOS::SYS::DelayMs(5);
    gT0Posi = gItemParam[T0POSI];
    gT1Posi = gItemParam[T1POSI];
    gT2Posi = gItemParam[T2POSI];
    gZCPosi = gItemParam[ZCPOSI];
    FPGA_ReadWrite(T0_POSN, 2, gItemParam[T0POSI]);        // 
    FPGA_ReadWrite(T1_POSN, 2, gItemParam[T1POSI]);        // 
    FPGA_ReadWrite(T2_POSN, 2, gItemParam[T2POSI]);        // 
    FPGA_ReadWrite(ZC_POSN, 2, gItemParam[ZCPOSI]);        // 
//  FPGA_ReadWrite(ZC_POSN, 2, 150);                       // 
    BIOS::SYS::DelayMs(2);
//  FPGA_ReadWrite(FLSH_ATTR, 2, 0xFFFF);                  // 
//  FPGA_LCD_RW(WFRM_SAVE, XWDTH, gLCD_Buf);               // 
    Set_Block(X0, Y0, XWDTH + X0 - 1, YHIGH + Y0 - 1);     // 
    __Bios(DISPDEV, FPGA_MOD);                             // 
    FPGA_ReadWrite(WFRM_DRAW, 0, 0);                       // ˢ
    do
    {
        FpgaInfo = FPGA_ReadWrite(FPGA_INFO, 0, 0) >> 8;   // 
    } while(FpgaInfo & BUSY);                              // 
    __Bios(DISPDEV, MCU_DRV);
}
/*******************************************************************************
 * FunctionName : Set_SysParam
 * Description  : 
 * Param        : void 
*******************************************************************************/
void Set_SysParam(void)
{
    Menu.flag = 1;                               // 
    Menu.index = 0;                              // 
    Menu.total = 6;                              // 

    Item.flag = 0;                               // 
    Item.index = 0;                              // 
    Item.total = 0;                              // 
    gTimeBase = gItemParam[TIMEBASE];

    gTriCond_X = (u8)gItemParam[TRICOND];
    gTriMask_X = (u8)gItemParam[TRIMASK];
    gStandByFlag = 1;
    ResetPowerOffTime();
    PIO_Init(gItemParam[OUTTYPE]);               // 
//  DefaultTir(gItemParam[INTYPE]);
    __Bios(DISPDEV, gItemParam[BKLIGHT] * 10);
    __Bios(BUZZDEV, gItemParam[VOLUME] * 10);
}

/******************************  END OF FILE  *********************************/
