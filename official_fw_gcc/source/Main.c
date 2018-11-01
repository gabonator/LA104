/**********************(C) COPYRIGHT 2017 e-Design Co.,Ltd.*********************
 * FileName    : Main.c
 * Description : 
 * Author      : SNAKE
 * History     : 
 *              Beta 1.04  保存BMP图片为4位改为16位图片
 *              APP V1.01  软关机测试
 *******************************************************************************/
#include "MyDefine.h"

void Hardware_Init(void);
void Show_Startup_Info(void);
void FPGA_Startup_Wave(void);
void Set_SysParam(void);

//===============================APP版本号======================================
u8   APP_VERSION[] = "APP V1.01x";                 // 不得超过12个字符


/*******************************************************************************
 * FunctionName : main
 * Description  : 主程序入口
*******************************************************************************/
void main(void)
{
    u8  M_Flag = 0;
    u8  File_ST = 0;

    Hardware_Init();                             // 硬件初始化
    Show_Startup_Info();                         // 开机信息
    ReadParameter();                             // 读取系统参数
    Clr_Scrn(BLK);                               // 清屏
    FPGA_Startup_Wave();
    Set_SysParam();
    ShowAllMenu();
    File_Num();                                  // 读取文件编号

    while(1)
    {
        ShutdownTest();                          // 软关机测试
        ShortcutBMP();                           // K1和K4快捷截图
        if(Menu.flag)KeyQuickAct();              // T1，T2，水平位移快捷操作

        if(gKeyActv)
        {
            switch(gKeyActv)
            {
            case K1_ACTp:
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
                        Beep_mS(50);
                    }
                }
                break;

            case K2_ACTp:
                if(Menu.flag)
                {
                    ShowPopItem(Menu.index);
                    Menu.flag = 0;
                    Item.flag = 1;
                }
                else if(Item.flag)
                {
                    if(Menu.index == IN_TYPE)
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

            case K3_ACTp:

                if(Item.flag)
                {
                    if(Menu.index == TIME_SET)gRunFlag = 1; // TimeSet 退出时刷新波形
                    Menu.flag = 1;
                    Item.flag = 0;
                    M_Flag = 1;
                }
                else if(SubItem.flag)
                {
                    AnalyzeFrame(gItemParam[INTYPE]);       // 协议子菜单退出时刷新解析
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

            case K4_ACTp:
                if(Item.flag)
                {
                    if(Menu.index == TRIGGER)
                    {
                        // TriCond子菜单
                        if((gItemIndexNum[TRIGGER] == 1) && ((gItemParam[TRICOND] & 0x00FF) != 0) && ((gItemParam[TRICOND] & 0x00FF) != 5))
                        {
                            if((gItemParam[TRICOND] >> (8 + (gItemParam[TRICOND] & 0x00FF) - 1)) & 0x01)
                                // 置0
                                gItemParam[TRICOND] = gItemParam[TRICOND] & (~(0x01 << (8 + (gItemParam[TRICOND] & 0x00FF) - 1)));
                            else
                                // 置1
                                gItemParam[TRICOND] = gItemParam[TRICOND] | (0x01 << (8 + (gItemParam[TRICOND] & 0x00FF) - 1));
                        }
                        // TriMask子菜单
                        else if((gItemIndexNum[TRIGGER] == 2) && ((gItemParam[TRIMASK] & 0x00FF) != 0) && ((gItemParam[TRIMASK] & 0x00FF) != 5))
                        {
                            if((gItemParam[TRIMASK] >> (8 + (gItemParam[TRIMASK] & 0x00FF) - 1)) & 0x01)
                                // 置0
                                gItemParam[TRIMASK] = gItemParam[TRIMASK] & (~(0x01 << (8 + (gItemParam[TRIMASK] & 0x00FF) - 1)));
                            else
                                // 置1
                                gItemParam[TRIMASK] = gItemParam[TRIMASK] | (0x01 << (8 + (gItemParam[TRIMASK] & 0x00FF) - 1));
                        }
                    }
                    else if(Menu.index == OUT_TYPE)
                    {
                        PIO_SendData(gItemParam[OUTTYPE]);
                    }
                    else if(Menu.index == FILE_CTRL)
                    {
                        if(gItemIndexNum[FILE_CTRL] == BMP)
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
                else if(Menu.flag)                // 主菜单时，时间选项切换
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
            case ENCD_2p: // Up
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
            case ENCD_2n: // Down
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

            case ENCD_1n: // Left
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
            case ENCD_1p: // Right
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
            if(gKeyActv & 0xF0F0) Beep_mS(50);  // 长按和按下不Beep

            ShowTopTitle();
            if(Item.flag && (gKeyActv != K1_ACTn)) ShowPopItem(Menu.index);
            if(SubItem.flag)                    // 显示子菜单选项
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
        if(M_Flag)  //子菜单状态下不刷新FPGA显示
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
}

/*******************************************************************************
 * FunctionName : Hardware_Init
 * Description  : 硬件初始化
 * Param        : void 
*******************************************************************************/
void Hardware_Init(void)
{
    __Bios(PWRCTRL, INIT);        // 电源设备初始化设置
    __Bios(KEYnDEV, INIT);        // 按键类输入设备初始化设置
    __Bios(NIVCPTR, 0x8000);      // 中断向量基址 = 0x0000
//  __Bios(SYSTICK, 1000);        // 设置嘀嗒中断时长 = 1000uS
    SysTick_Config(SystemCoreClock / 1000);
    __Bios(BUZZDEV, INIT);        // 蜂鸣器初始化设置 如果不初始化SI2302会发烫
    __Bios(BUZZDEV, 50);
    Beep_mS(200);
    __Bios(FLSHDEV, INIT);        // SPI磁盘初始化设置
    __Bios(USBDEV, INIT);         // USB初始化
    DiskConfig();                 // 文件系统初始化
    __Bios(IN_PORT, INIT);        // DAC端口初始化
//  __Bios(IN_PORT, 3000);        // 3V
    __Bios(IN_PORT, gItemParam[THRESHOLD]*100);

}

/*******************************************************************************
 * FunctionName : Show_Startup_Info
 * Description  : 显示开机提示信息页面
 * Param        : void 
*******************************************************************************/
void Show_Startup_Info(void)
{
    Disp_Str8x14(0,      90, BLK, BLK, PRN, "                                        ");
    Disp_Str8x14(0,      70, BLK, BLK, PRN, "                                        ");
    Disp_Str8x14(8,      90, WHT, BLK, PRN, "       Logic Analyzer APP");
    Disp_Str8x14(27 * 8, 90, WHT, BLK, PRN,                           APP_VERSION);
    Disp_Str8x14(8,      70, WHT, BLK, PRN, "        System Initializing...       ");
    Delay_mS(1000);
    if(__Bios(FPGADEV, DONE) == 0) while(1){}       // FPGA程序出错时不进入APP
    
}

/*******************************************************************************
 * FunctionName : FPGA_Startup_Wave
 * Description  : 开机FPGA波形
 * Param        : void 
*******************************************************************************/
void FPGA_Startup_Wave(void)
{
    u8  FpgaInfo = 0;

    FPGA_ReadWrite(FPGA_RST, 0, 0);                        // 芯片内部逻辑复位
    FPGA_ReadWrite(WINDOW_Y, 1, YHIGH);                    // 设置显示窗垂直高度
    FPGA_ReadWrite(WINDOW_X, 2, XWDTH);                    // 设置显示窗水平宽度
    memset(gLCD_Buf, 0, LCD_BUF_WIDTH);
    memset(gLCD_Backup, 0, LCD_BUF_WIDTH);
    FPGA_LCD_RW(WFRM_SAVE, XWDTH, gLCD_Buf);               // 设置波形曲线数据
    Delay_mS(5);
    gT0Posi = gItemParam[T0POSI];
    gT1Posi = gItemParam[T1POSI];
    gT2Posi = gItemParam[T2POSI];
    gZCPosi = gItemParam[ZCPOSI];
    FPGA_ReadWrite(T0_POSN, 2, gItemParam[T0POSI]);        // 设置 T0 标线位置
    FPGA_ReadWrite(T1_POSN, 2, gItemParam[T1POSI]);        // 设置 T1 标线位置
    FPGA_ReadWrite(T2_POSN, 2, gItemParam[T2POSI]);        // 设置 T2 标线位置
    FPGA_ReadWrite(ZC_POSN, 2, gItemParam[ZCPOSI]);        // 设置 ZC 标线位置
//  FPGA_ReadWrite(ZC_POSN, 2, 150);                       // 设置 ZC 标线位置
    Delay_mS(2);
//  FPGA_ReadWrite(FLSH_ATTR, 2, 0xFFFF);                  // 设置闪烁属性标志
//  FPGA_LCD_RW(WFRM_SAVE, XWDTH, gLCD_Buf);               // 设置波形曲线数据
    Set_Block(X0, Y0, XWDTH + X0 - 1, YHIGH + Y0 - 1);     // 设置 LCD 波形显示窗口
    __Bios(DISPDEV, FPGA_MOD);                             // 切换为 FPGA 控制显示
    FPGA_ReadWrite(WFRM_DRAW, 0, 0);                       // 刷新波形显示图像
    do
    {
        FpgaInfo = FPGA_ReadWrite(FPGA_INFO, 0, 0) >> 8;   // 读 FPGA 状态
    } while(FpgaInfo & BUSY);                              // 不在 DRAW 状态时退出
    __Bios(DISPDEV, MCU_DRV);
}
/*******************************************************************************
 * FunctionName : Set_SysParam
 * Description  : 初始化系统参数
 * Param        : void 
*******************************************************************************/
void Set_SysParam(void)
{
    Menu.flag = 1;                               // 菜单标志位
    Menu.index = 0;                              // 菜单当前第几个选项
    Menu.total = 6;                              // 菜单选项总数

    Item.flag = 0;                               // 子菜单标志位
    Item.index = 0;                              // 子菜单标志位
    Item.total = 0;                              // 子菜选项总数
    gTimeBase = gItemParam[TIMEBASE];

    gTriCond_X = (u8)gItemParam[TRICOND];
    gTriMask_X = (u8)gItemParam[TRIMASK];
    gKeyActv = 0;                                // 按键消抖
    gStandByFlag = 1;
    ResetPowerOffTime();
    PIO_Init(gItemParam[OUTTYPE]);               // 初始化可编程IO协议类型
//  DefaultTir(gItemParam[INTYPE]);
    __Bios(DISPDEV, gItemParam[BKLIGHT] * 10);
    __Bios(BUZZDEV, gItemParam[VOLUME] * 10);
}

/******************************  END OF FILE  *********************************/
