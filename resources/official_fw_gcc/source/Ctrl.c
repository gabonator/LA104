/**********************(C) COPYRIGHT 2017 e-Design Co.,Ltd.*********************
 * FileName    : Ctrl.c
 * Description : 
 * Author      : 
 * History     :
*******************************************************************************/
#include "MyDefine.h"

u16 ZC_Cnt = 0;
u16 L_EmptyCnt = 0;
u16 R_EmptyCnt = 0;
u16 ZC_Point = 0;
u16 X_Position = 0;
u32 TransPointSum = 0;
u32 DiffTime = 0;

u8  gRunFlag = 0;
u8  gXposiAdd = 0;
u8  gXposiDec = 0;
u16 gX_Position = 0;
u16 gStartCnt = 0;
u16 gZC_Posi = 4096;
u64 gNowX_Time = 0;
u64 gLastX_Time = 0;
u64 gTransTimeSum = 0;


//-------------+-------+-------+-------+-------+-------+-------+-------+-------+
uc16 OUT_PSC[] = {
    128 - 1,  128 - 1,   64 - 1,   64 - 1,   16 - 1,   16 - 1,    4 - 1,    4 - 1,
    1 - 1,      1 - 1,    1 - 1,    1 - 1,    1 - 1,    1 - 1,    1 - 1,    1 - 1 };
//-------------+-------+-------+-------+-------+-------+-------+-------+-------+
uc16 OUT_ARR[] = {
    56250 - 1, 28125 - 1, 22500 - 1, 11250 - 1, 22500 - 1, 9000 - 1, 18000 - 1, 9000 - 1,
    14400 - 1, 7200 - 1,  3600 - 1,  1440 - 1,  720 - 1,   360 - 1,  144 - 1,   72 - 1 };

/*******************************************************************************
 * FunctionName : TriConfig
 * Description  : 触发设置 
 *                gItemParam[TRICOND], gItemParam[TRIMASK]
 *                高8bit为送FPGA对应的数据
 *                低8bit为菜单选项数据
 * Param        : void 
*******************************************************************************/
void TriConfig(void)
{
    u16 PulseWidth = 0;

    FPGA_ReadWrite(TRG_COND, 1, gItemParam[TRICOND] >> 8);
    Delay_mS(1);
    FPGA_ReadWrite(MEM_MASK, 1, gItemParam[TRIMASK] >> 8);
    Delay_mS(1);
    PulseWidth = (u16)(gItemParam[TRIWIDTH] * gTB_Scale[gItemParam[TIMEBASE]]);
    FPGA_ReadWrite(MEM_TWTH, 2, PulseWidth);
    Delay_mS(1);
    FPGA_ReadWrite(TRG_KIND, 1, gItemParam[TRITYPE]);
    Delay_mS(1);
}

/*******************************************************************************
 * FunctionName : DefaultTir
 * Description  : 协议默认触发模式
 * Param        : u8 Type 协议类型
*******************************************************************************/
void DefaultTir(u8 Type)
{
    switch (Type)
    {
    case IN_USER:
        gItemParam[TRICOND] &= 0xFF;
        gItemParam[TRIMASK] &= 0xFF;
        gItemParam[TRICOND] |= (0x00 << 8);
        gItemParam[TRIMASK] |= (0x0E << 8);   //只使能第1通道
        break;
    case IN_SPI:
        gItemParam[TRICOND] &= 0xFF;
        gItemParam[TRIMASK] &= 0xFF;
        gItemParam[TRICOND] |= (0x00 << 8);
        gItemParam[TRIMASK] |= (0x07 << 8);   //只使能第4通道
        break;
    case IN_I2C:
        gItemParam[TRICOND] &= 0xFF;
        gItemParam[TRIMASK] &= 0xFF;
        gItemParam[TRICOND] |= (0x00 << 8);
        gItemParam[TRIMASK] |= (0x0E << 8);   //只使能第1通道
        break;
    case IN_UART:
        gItemParam[TRICOND] &= 0xFF;
        gItemParam[TRIMASK] &= 0xFF;
        gItemParam[TRICOND] |= (0x00 << 8);
        gItemParam[TRIMASK] |= (0x0E << 8);   //只使能第1通道
        break;
    }
}

/*******************************************************************************
 * FunctionName : Process
 * Description  : 数据处理
 * Param        : void 
*******************************************************************************/
void Process(void)
{
    u16 i = 0, j = 0;
    u8  ICO_Cnt = 0;
    u8  Buf[4] = { 0 };
    u8  SampInfo = 0;
    u8  SamplFlag = 0;
    u32 SamplData = 0;

    gTimeBase = gItemParam[TIMEBASE];                   // 保存采样时的时基
    memset(gSamplBuf, 0, RECORD_DEPTH * CNTx4K);        // 清除采样缓冲区数据
    TriConfig();                                        // 采样设置

    FPGA_ReadWrite(SMPL_CLR, 1, 0);                     // 采样清零
    Delay_mS(2);
    gFullmS = 1000;                                     // 等待采样时间
//  防止按键保存之前的状态，后期需要调整
//  Delay_mS(500);
    do                                                  // 等待满足触发条件
    {
        SampInfo = (u8)(FPGA_ReadWrite(FPGA_INFO, 0, 0) >> 8);
        SamplFlag = 1;
        ICO_Cnt = Show_Run_ICO(4, TITLE_Y + 2, CYN_, GRAY, PRN, ICO_Cnt);
        if (gKeyActv & K1_ACTp)                          // 按键强制退出采样
        {
            gBeepFlag = 1;
            SamplFlag = 0;
            break;
        }
        if (gKeyActv & K4_ACTp)                          // 按键强制退出采样
        {
            Beep_mS(50);
            gKeyActv = 0;
            PIO_SendData(gItemParam[OUTTYPE]);
        }
    }while ((SampInfo & FPGA_TRI) == 0);                 // Tri
//  while ((SampInfo & FPGA_FULL) == 0);                // Full
//  while ((SampInfo & FPGA_PRES) == 0);                // Ptrsamp

    Delay_mS(500);                                      // 等待0.5s在读取数据

//  SamplFlag = 1;                                      // 强制采样完成
    if (SamplFlag == 1)
    {
        gRunFlag = 1;

        for (i = 0; i < RECORD_DEPTH; /*i++*/)
        {
            SamplData = FPGA_RW_4Byte(MEM_READ, 3, 0);  // 读取一包数据

            Buf[0] = (u8)(SamplData);                   // 采样状态信息
            Buf[1] = (u8)(SamplData >> 8);              // 时间低8位
            Buf[2] = (u8)(SamplData >> 16);             // 时间中8位
            Buf[3] = (u8)(SamplData >> 24);             // 4bit通道数据和4bit时间高4bit

//          memcpy(&gSamplBuf[CNTx4K * i], Buf, CNTx4K);// 整个采集存到缓冲区
//          i++;

            if ((Buf[0] & (FPGA_EMPTY | FPGA_FULL)) == (FPGA_EMPTY | FPGA_FULL)) //Full和Empty则跳出
            {
//              测试查看数据使用，实际Full和Empty时数据无效
//              memcpy(&gSamplBuf[CNTx4K * i], Buf, 1);
                memset(&gSamplBuf[CNTx4K * i], Buf[0], 1);
                break;
            } else if ((Buf[0] & FPGA_FULL) == FPGA_FULL)
            {
                memcpy(&gSamplBuf[CNTx4K * i], Buf, CNTx4K);
                i++;
                gFullmS = 500;
            } else if ((Buf[0] & FPGA_EMPTY) == FPGA_EMPTY)
            {
//              测试查看数据使用，实际Empty时数据无效
//              memcpy(&gSamplBuf[CNTx4K * i], Buf, 1);
                memset(&gSamplBuf[CNTx4K * i], Buf[0], 1);
                if (gKeyActv & K1_ACTp)                      // 按键暂停时强制退出采样
                {
                    gBeepFlag = 1;
                    break;
                }
                if (gFullmS == 0)                            // 超过采样时间强制退出采样
                {
                    gBeepFlag = 1;
                    break;
                }
            } else if ((Buf[0] & FPGA_BUSY) != FPGA_BUSY)
            {
                memcpy(&gSamplBuf[CNTx4K * i], Buf, CNTx4K);
                i++;
                gFullmS = 500;
            }

        }
        for (j = i; j < RECORD_DEPTH; j++)                   // 无效数据补齐为最后有效数据
        {
            memset(&gSamplBuf[CNTx4K * j + 0], gSamplBuf[CNTx4K * (i)+0], 1);
            memset(&gSamplBuf[CNTx4K * j + 1], 1, 1);
            memset(&gSamplBuf[CNTx4K * j + 2], 0, 1);
            memset(&gSamplBuf[CNTx4K * j + 3], (gSamplBuf[CNTx4K * (i - 1) + 3] & 0xF0), 1);
        }
        AnalyzeFrame(gItemParam[INTYPE]);                   // 协议数据框架分析
    }

    gTransTimeSum = CalculateTimeSum(RECORD_DEPTH);          // 计算采样各个跳变的时间总和
    ProcessSmallWins();
    ZC_Scale();                                              // 图像显示
}

/*******************************************************************************
 * FunctionName : ZC_Scale
 * Description  : 
*******************************************************************************/
u8 ZC_Scale(void)
{
    u8  CH_Status = 0;                                       // 通道数据

    u16 i = 0;
    u16 TimeToPoint = 0;
//  u16 PointSum = 0;
    u32 TransTime = 0;
    u64 TransTimeSum = 0;
    u64 StartTime = 0;

//  s32 L_Point = 0;
    s32 R_Point = 0;
//  s32 L_Cnt = 0;
    s32 R_Cnt = 0;

    memset(gLCD_Buf, 0, LCD_BUF_WIDTH);                      // 显示屏幕数据清零
    if (gRunFlag || (gXposiAdd | gXposiDec))
    {
        gRunFlag = 0;
        StartTime = UpdateXposiTime();                       // 读取Xposi偏移时间
//      计算大于偏移时间的跳变序号
        for (i = TRI_START_NUM + 1; i < RECORD_DEPTH; i++)    // +1为时间在状态的下一包数据
        {
            TransTime = TransformTime(i);
            TransTimeSum += TransTime;
            if (TransTimeSum >= StartTime)
            {
                DiffTime = TransTimeSum - StartTime;         // 偏移后第一个不完整波形的显示时间
                gStartCnt = i - 1;                           // 状态和时间数据相差一个包
                break;
            } else gStartCnt = i - 1;
        }
        ZC_Cnt = gStartCnt;
        ZC_Point = 0;
    }
    if (gLastX_Time == 0)
    {
//      图形显示分ZC轴左右两边显示
//      L_Point = 0;
        R_Point = 2;
//      L_Cnt = ZC_Cnt;
        R_Cnt = ZC_Cnt;

        for (i = 0; i < R_Point; i++)
        {
            CH_Status = TransStatusData(6 + i);              // 第6，7帧为最后预采样状态
            memset(&gLCD_Buf[i], CH_Status, 1);
        }
    } else
    {
//      图形显示分ZC轴左右两边显示
//      L_Point = 0;
        R_Point = 0;
//      L_Cnt = ZC_Cnt;
        R_Cnt = ZC_Cnt;
    }

    gItemParam[T0POSI] =  L_EmptyCnt;                       // 触发线位置更新

//  显示ZC右边数据
    for (i = R_Point; i <= SHOW_WIDTH; i++)
    {
        if ((R_Cnt < RECORD_DEPTH) && ((TransStatusInfo(R_Cnt) & FPGA_EMPTY) != FPGA_EMPTY))
        {
            CH_Status = TransStatusData(R_Cnt);
            if (R_Cnt == ZC_Cnt)
            {
                if ((gLastX_Time == 0) && (DiffTime > 2))
                {
                    TransTime = DiffTime - 2;              // 减去2的单位的预采样状态
                } else TransTime = DiffTime;
            } else
            {
                TransTime = TransformTime(R_Cnt + 1);
            }
            TimeToPoint = (TransTime * TIME_TO_POINT) / gTB_Scale[gItemParam[TIMEBASE]];
            if (R_Cnt == ZC_Cnt)
            {
                if (TimeToPoint < 1) TimeToPoint = 1;
            } else
            {
                while (TimeToPoint < 1)                      // 不足一个点时，累计下个时间到足一个点
                {
                    TransTime += TransformTime(++R_Cnt + 1);
                    TimeToPoint = (TransTime * TIME_TO_POINT) / gTB_Scale[gItemParam[TIMEBASE]];
                    if ((TransStatusInfo(R_Cnt) & FPGA_EMPTY) == FPGA_EMPTY) break;
                }
            }
            if ((R_Point + TimeToPoint) >= SHOW_WIDTH)       // 超过显示长度处理
            {
                TimeToPoint = SHOW_WIDTH - R_Point;
                CH_Status = AddAnalyzeStatus(CH_Status, R_Cnt);
                memset(&gLCD_Buf[R_Point], CH_Status, TimeToPoint);
            } else
            {
                CH_Status = AddAnalyzeStatus(CH_Status, R_Cnt);
                memset(&gLCD_Buf[R_Point], CH_Status, TimeToPoint);
            }
            if ((R_Point + TimeToPoint) <= SHOW_WIDTH) R_Point += TimeToPoint;
            else break;
            R_Cnt++;
            R_EmptyCnt = 0;
        } else
        {
            if (R_Point <= SHOW_WIDTH)
            {
                memset(&gLCD_Buf[R_Point], CH_Status, 1);
                R_Point++;
                R_EmptyCnt++;
            }
        }
    }

    memcpy(gLCD_Backup, gLCD_Buf, LCD_BUF_WIDTH);                          // 备份显示数据
    ShowWaveToLCD();                                                       // LCD显示图形
    ShowScaleRuleTime(RULE_X, RULE_Y, SHOW_WIDTH, gItemParam[TIMEBASE]);   // 刷新时间标尺
    ShowTimeBaseStr();

    return 1;
}

/*******************************************************************************
 * FunctionName : CalculateTimeSum
 * Description  : 计算n个转换的时间总和
 * Param        : u16 Cnt 
 * Return       : u64   
*******************************************************************************/
u64 CalculateTimeSum(u16 Cnt)
{
    u8  Info = 0;
    u16 i = 0;
    u64 TransTimeSum = 0;

    if (Cnt > RECORD_DEPTH) Cnt = RECORD_DEPTH;

    for (i = TRI_START_NUM; i < Cnt; i++)
    {
        Info = TransStatusInfo(i);
        if ((Info & FPGA_EMPTY) == FPGA_EMPTY) break;
        TransTimeSum += TransformTime(i);
    }

    return TransTimeSum;
}

/*******************************************************************************
 * FunctionName : ShowWaveToLCD
 * Description  : 刷新保存中的画面
 * Param        : void 
*******************************************************************************/
void ShowWaveToLCD(void)
{
    u8 Info = 0;

    FPGA_LCD_RW(WFRM_SAVE, XWDTH, gLCD_Buf);
    Delay_mS(5);
    if (gLastX_Time == 0) gItemParam[T0POSI] = TRI_POSI;
    else                gItemParam[T0POSI] = 300;          // 触发线消隐
    FPGA_ReadWrite(T0_POSN, 2, gItemParam[T0POSI]);
    FPGA_ReadWrite(T1_POSN, 2, gItemParam[T1POSI]);
    FPGA_ReadWrite(T2_POSN, 2, gItemParam[T2POSI]);
    FPGA_ReadWrite(ZC_POSN, 2, gItemParam[ZCPOSI]);
    gT0Posi = gItemParam[T0POSI];
    gT1Posi = gItemParam[T1POSI];
    gT2Posi = gItemParam[T2POSI];
    gZCPosi = gItemParam[ZCPOSI];
    Set_Block(X0, Y0, XWDTH + X0 - 1, YHIGH + Y0 - 1);     // 设置 LCD 波形显示窗口
    __Bios(DISPDEV, FPGA_MOD);                             // 切换为 FPGA 控制显示
    FPGA_ReadWrite(WFRM_DRAW, 0, 0);                       // 刷新波形显示图像
    do
    {
        Info = FPGA_ReadWrite(FPGA_INFO, 0, 0) >> 8;       // 读 FPGA 状态
    } while (Info & BUSY);                                  // 不在 DRAW 状态时退出
    __Bios(DISPDEV, MCU_DRV);

    Show_AnalyzeData(gItemParam[INTYPE]);
}

/*******************************************************************************
 * FunctionName : TransStatusData
 * Description  : 4bit状态位 1通道在低位
 * Param        : u16 Num 
 * Return       : u8   
*******************************************************************************/
u8 TransStatusData(u16 Num)
{
    return (gSamplBuf[CNTx4K * Num + 3] >> 4);
}

/*******************************************************************************
 * FunctionName : TransStatusInfo
 * Description  : 读取采样状态
 * Param        : u16 Num 
 * Return       : u8   
*******************************************************************************/
u8 TransStatusInfo(u16 Num)
{
    return gSamplBuf[CNTx4K * Num];
}
/*******************************************************************************
 * FunctionName : TransformTime
 * Description  : 一个转变的持续时间
 * Param        : u16 Num 
 * Return       : u32   
*******************************************************************************/
u32 TransformTime(u16 Num)
{
    u32 TransTime = 0;

    TransTime  =  gSamplBuf[CNTx4K * Num + 1];
    TransTime |= (gSamplBuf[CNTx4K * Num + 2]) << 8;
    TransTime |= (gSamplBuf[CNTx4K * Num + 3] & 0x0F) << 16; // 高4bit为4通道变化状态

    return TransTime;
}

/*******************************************************************************
 * FunctionName : AddAnalyzeStatus
 * Description  : 增加协议分析显示状态 
 *                数据从采样状态信息的bit6和bit7读取 bit6为协议分析1 
 * Param        : u8 Status 
 * Param        : u8 Num 
 * Return       : u8   
*******************************************************************************/
u8 AddAnalyzeStatus(u8 Status, u16 Num)
{
    Status = ((gSamplBuf[CNTx4K * Num] & 0x40) >> 2) | ((gSamplBuf[CNTx4K * Num] & 0x80) >> 2) | Status;
    return  Status;
}

/*******************************************************************************
 * FunctionName : UpdateXposiTime
 * Description  : 更新X轴偏移的开始时间
 * Return       : u64 偏移时间
*******************************************************************************/
u64 UpdateXposiTime(void)
{
    if (gXposiAdd == 1)                                   // Xposi增加
    {
        gXposiAdd = 0;
        gNowX_Time = gTB_Scale[gItemParam[TIMEBASE]];
//      单个记录的最大时间为10483200nS，就是10.4832mS，单位为10ns，所以少X10
        if ((gLastX_Time + gNowX_Time) > gTransTimeSum)
        {
            gLastX_Time = gTransTimeSum;                 // 读取的数据单位为10ns
        } else
        {
            gLastX_Time += gNowX_Time;
        }
    } else if (gXposiDec == 1)                              // Xposi减少
    {
        gXposiDec = 0;
        gNowX_Time = gTB_Scale[gItemParam[TIMEBASE]];
        if (gNowX_Time >= gLastX_Time)
        {
            gLastX_Time = 0;
        } else
        {
            gLastX_Time -= gNowX_Time;
        }
    }

    return gLastX_Time;
}
/*******************************************************************************
 * FunctionName : HW_Ctrl
 * Description  : 选项操作对应硬件控制
 * Param        : u8 MenuNum 
 * Param        : u8 ItemNum 
*******************************************************************************/
void HW_Ctrl(u8 MenuNum, u8 ItemNum)
{
    u16 TriWidth = 0;
    u8  FreqNum = 0;

    switch (MenuNum)
    {
    case IN_TYPE:
        break;
    case TIME_SET:
        break;
    case TRIGGER:
        switch (ItemNum + TRITYPE)
        {
        case TRITYPE:
            FPGA_ReadWrite(TRG_KIND, 1, gItemParam[TRITYPE]);
            break;
        case TRICOND:
            FPGA_ReadWrite(TRG_COND, 1, gItemParam[TRICOND] >> 8);
            break;
        case TRIMASK:
            FPGA_ReadWrite(MEM_MASK, 1, gItemParam[TRIMASK] >> 8);
            break;
        case TRIWIDTH:
            TriWidth = (u16)(gItemParam[TRIWIDTH] * gTB_Scale[gItemParam[TIMEBASE]]);
            FPGA_ReadWrite(MEM_TWTH, 2, TriWidth);
            break;
        case THRESHOLD:
            __Bios(IN_PORT, gItemParam[THRESHOLD] * 100);
            break;
        default:
            break;
        }
        break;
    case MEASURE:
        ShowMeasureSourceStr();
        break;
    case OUT_TYPE:
        if ((ItemNum == 0) && (gItemParam[OUTTYPE] == 0))
        {
            switch (gOutSubIndexNum[ItemNum][OUT_PWM])
            {
            case PWM1_EN:
                PIO_PWM_Ctrl(PWM1, gOutSubParam[OUT_PWM][PWM1_EN]);
                break;
            case PWM1_FREQ:
                FreqNum = gOutSubParam[OUT_PWM][PWM1_FREQ];
                PIO_PWM_SetFreq(PWM1, OUT_PSC[FreqNum], OUT_ARR[FreqNum]);
                PIO_PWM_SetDuty(PWM1, PWM_NUM1, (OUT_ARR[FreqNum] + 1) * gOutSubParam[OUT_PWM][PWM1_DUT1] / 10);
                PIO_PWM_SetDuty(PWM1, PWM_NUM2, (OUT_ARR[FreqNum] + 1) * gOutSubParam[OUT_PWM][PWM1_DUT2] / 10);
                break;
            case PWM1_DUT1:
                FreqNum = gOutSubParam[OUT_PWM][PWM1_FREQ];
                PIO_PWM_SetDuty(PWM1, PWM_NUM1, (OUT_ARR[FreqNum] + 1) * gOutSubParam[OUT_PWM][PWM1_DUT1] / 10);
                break;
            case PWM1_DUT2:
                FreqNum = gOutSubParam[OUT_PWM][PWM1_FREQ];
                PIO_PWM_SetDuty(PWM1, PWM_NUM2, (OUT_ARR[FreqNum] + 1) * gOutSubParam[OUT_PWM][PWM1_DUT2] / 10);
                break;
            case PWM2_EN:
                PIO_PWM_Ctrl(PWM2, gOutSubParam[OUT_PWM][PWM2_EN]);
                break;
            case PWM2_FREQ:
                FreqNum = gOutSubParam[OUT_PWM][PWM2_FREQ];
                PIO_PWM_SetFreq(PWM2, OUT_PSC[FreqNum], OUT_ARR[FreqNum]);
                PIO_PWM_SetDuty(PWM2, PWM_NUM1, (OUT_ARR[FreqNum] + 1) * gOutSubParam[OUT_PWM][PWM2_DUT1] / 10);
                PIO_PWM_SetDuty(PWM2, PWM_NUM2, (OUT_ARR[FreqNum] + 1) * gOutSubParam[OUT_PWM][PWM2_DUT2] / 10);
                break;
            case PWM2_DUT1:
                FreqNum = gOutSubParam[OUT_PWM][PWM2_FREQ];
                PIO_PWM_SetDuty(PWM2, PWM_NUM1, (OUT_ARR[FreqNum] + 1) * gOutSubParam[OUT_PWM][PWM2_DUT1] / 10);
                break;
            case PWM2_DUT2:
                FreqNum = gOutSubParam[OUT_PWM][PWM2_FREQ];
                PIO_PWM_SetDuty(PWM2, PWM_NUM2, (OUT_ARR[FreqNum] + 1) * gOutSubParam[OUT_PWM][PWM2_DUT2] / 10);
                break;
            default:
                break;
            }
        }
        break;
    case FILE_CTRL:

        break;
    case SETTING:
        switch (ItemNum + SAVEPAM)
        {
        case VOLUME:
            __Bios(BUZZDEV, gItemParam[VOLUME] * 10);
            break;
        case BKLIGHT:
            __Bios(DISPDEV, gItemParam[BKLIGHT] * 10 - 1);
            break;

        default:
            break;
        }
        break;
    default:
        break;
    }
}

/*******************************************************************************
 * FunctionName : StandbyAndPowerOff
 * Description  : 待机和自动关机控制
 * Param        : void 
*******************************************************************************/
void StandbyAndPowerOff(void)
{
    if (((gItemParam[STANDBY] != 0) && (gStandbyCnt == 0))) // 待机
    {
        __Bios(DISPDEV, 1);
        gStandByFlag = 1;
    }
    if ((gItemParam[POWEROFF] != 0) && (gAutoPwrCnt == 0))  // 关机
    {
        if (Get_USB_Vol() == 0)                             // USB充电时不自动关机
        {
            Beep_mS(200);
            Delay_mS(500);
            Beep_mS(200);
            Delay_mS(500);
            __Bios(PWRCTRL, DSBL);
        } else
        {
            gAutoPwrCnt = gItemParam[POWEROFF] * PWROFF_TIME_UNIT;
        }
    }
    if (GetBatteryVol() < 3200)                             // 电池电压小于3.2V关机
    {
        __Bios(PWRCTRL, DSBL);
    }
}

/*******************************************************************************
 * FunctionName : ResetPowerOffTime
 * Description  : 重置待机和关机计时
 * Param        : void 
*******************************************************************************/
void ResetPowerOffTime(void)
{
    if (gStandByFlag)
    {
        gStandByFlag = 0;
        __Bios(DISPDEV, gItemParam[BKLIGHT] * 10);
    }
    gStandbyCnt = gItemParam[STANDBY] * PWROFF_TIME_UNIT;
    gAutoPwrCnt = gItemParam[POWEROFF] * PWROFF_TIME_UNIT;
}

/*******************************************************************************
 * FunctionName : GetBatteryVol
 * Description  : 
 * Return       : u16   
*******************************************************************************/
u16 GetBatteryVol(void)
{
    u16 BatVol = 0;
    BatVol = VbtySum / 64;
    return BatVol;
}

/*******************************************************************************
 * FunctionName : Get_USB_Vol
 * Description  : 
 * Return       : u16   
*******************************************************************************/
u16 Get_USB_Vol(void)
{
    u16 USB_Vol = 0;
    USB_Vol = __Bios(PWRCTRL, VIN_ST);
    return USB_Vol;
}

/*******************************************************************************
 * FunctionName : ShowMeasureValue
 * Description  : 计算和显示测试值
*******************************************************************************/
void ShowMeasureValue(void)
{
    u8  Str[10] = { 0 };
    u16 UpNum[4] = { 0 };
    u16 DownNum[4] = { 0 };
    u32 P_PWSum[4] = { 0 };
    u32 N_PWSum[4] = { 0 };

    u8  Length = 0;
    u8  CH_Cnt = 0;
    u8  CurStatus = 0;
    u8  LastStatus = 0;

    u16 i = 0;
    u16 FirstTime = 0;
    u16 LastTime = 0;
    u16 Mark1Cnt = 0;
    u32 TransTime = 0;
    u64 TransTimeSum = 0;
    u64 StartTime = 0;
    u64 TimeMark1 = 0;
    u64 TimeMark = 0;

//  计算T1的时间
    TimeMark1 = gLastX_Time + gItemParam[T1POSI] * gTB_Scale[gItemParam[TIMEBASE]] / TIME_TO_POINT;
    StartTime = TimeMark1;
//  计算T1开始的跳变序号
    for (i = TRI_START_NUM + 1; i < RECORD_DEPTH; i++)
    {
        TransTime = TransformTime(i);
        TransTimeSum += TransTime;
        if (TransTimeSum >= StartTime)
        {
            FirstTime = TransTimeSum - StartTime;
            Mark1Cnt = i - 1;
            break;
        } else Mark1Cnt = i - 1;
    }
//  T1和T2的参数差
    if (gItemParam[T2POSI] >= gItemParam[T1POSI])
    {
        TimeMark = gItemParam[T2POSI] - gItemParam[T1POSI];
    } else
    {
        TimeMark = gItemParam[T1POSI] - gItemParam[T2POSI];
    }
//  T1和T2的时间差
    TimeMark = gLastX_Time + TimeMark * gTB_Scale[gItemParam[TIMEBASE]] / TIME_TO_POINT;
    StartTime = TimeMark;
    TransTimeSum = 0;
//  计算T2开始的跳变序号
    for (i = Mark1Cnt; i < RECORD_DEPTH; i++)                     // T1-T2时间段内处理
    {
        CurStatus = TransStatusData(i);                          // 当前状态
        if (i == Mark1Cnt) TransTime = FirstTime;
        else TransTime = TransformTime(i + 1);
        TransTimeSum += TransTime;
        if (i > Mark1Cnt)                                         // 第二数据开始处理跳变状态
        {
            for (CH_Cnt = 0; CH_Cnt < 4; CH_Cnt++)                // 边沿跳变计数
            {
                if ((((LastStatus >> CH_Cnt) & 0x01) == 0) && ((CurStatus >> CH_Cnt) & 0x01))
                {
                    UpNum[CH_Cnt]++;                             // 上升沿计数
                } else if (((LastStatus >> CH_Cnt) & 0x01) && ((CurStatus >> CH_Cnt) & 0x01) == 0)
                {
                    DownNum[CH_Cnt]++;                           // 下降沿计数
                }
            }
        }
        LastStatus = CurStatus;                                  // 记录上一个状态
        if (TransTimeSum >= StartTime)
        {
            LastTime = TimeMark - (TransTimeSum - TransTime);    // 最后一个时间
            for (CH_Cnt = 0; CH_Cnt < 4; CH_Cnt++)                // 补齐最后一帧
            {
                if ((CurStatus >> CH_Cnt) & 0x01)
                {
                    P_PWSum[CH_Cnt] += LastTime;
                } else
                {
                    N_PWSum[CH_Cnt] += LastTime;
                }
            }
            break;
        }
        for (CH_Cnt = 0; CH_Cnt < 4; CH_Cnt++)                    // 脉宽记录
        {
            if ((CurStatus >> CH_Cnt) & 0x01)
            {
                P_PWSum[CH_Cnt] += TransTime;                    // 正脉宽时间累加
            } else if (((CurStatus >> CH_Cnt) & 0x01) == 0)
            {
                N_PWSum[CH_Cnt] += TransTime;                    // 负脉宽时间累加
            }
        }
    }
//　显示测量数据
    for (i = 0; i < 2; i++)
    {
        Set_Color(BLK, WHT);
        u32ToStr_Unit(Str, P_PWSum[gItemParam[SOURCE1 + i]] * 10, (u8 *)TimeUnitStr, 3);
        Length = strlen((char *)Str);
        strncat((char *)Str, (char *)"      ", (6 - Length));
        Show_Str6x8(40 + 6 * 9 * 0,  MEASURE_Y + 12 * (1 - i),  PRN,  THIN, Str);
        u32ToStr_Unit(Str, N_PWSum[gItemParam[SOURCE1 + i]] * 10, (u8 *)TimeUnitStr, 3);
        Length = strlen((char *)Str);
        strncat((char *)Str, (char *)"      ", (6 - Length));
        Show_Str6x8(40 + 6 * 9 * 1,  MEASURE_Y + 12 * (1 - i),  PRN,  THIN, Str);
        u8ToDec3Str(Str, UpNum[gItemParam[SOURCE1 + i]]);
        Show_Str6x8(40 + 6 * 9 * 2,  MEASURE_Y + 12 * (1 - i),  PRN,  THIN, Str);
        u8ToDec3Str(Str, DownNum[gItemParam[SOURCE1 + i]]);
        Show_Str6x8(40 + 6 * 9 * 3,  MEASURE_Y + 12 * (1 - i),  PRN,  THIN, Str);
    }
}


/*******************************************************************************
 * FunctionName : ShortcutBMP
 * Description  : K1和K4快速截图
 * Param        : void 
*******************************************************************************/
void ShortcutBMP(void)
{
    u8 FileInfo = 0;

    if ((~__Bios(KEYnDEV, BITMAP) & KEYSBIT) == (K1_HOLD | K4_HOLD))
    {
        Beep_mS(50);
        Clear_File_ICO(SAVE_ICO_X0, SAVE_ICO_Y0); // 截屏
        FileInfo = Save_Bmp(gItemParam[SAVEBMP]);
        DispFileInfo(FileInfo);
        gBatFlag = 1;
        ShowBattery();
        ResetPowerOffTime();
        gKeyActv = 0;
    }
}

/*******************************************************************************
 * FunctionName : ShutdownTest
 * Description  : 未解锁状态下软关机测试
 * Param        : void 
*******************************************************************************/
void ShutdownTest(void)
{
    if (__Bios(SYSINFO, LICENCE) == 0)
    {
        if ((~__Bios(KEYnDEV, BITMAP) & KEYSBIT) == (K2_HOLD | K3_HOLD))
            __Bios(PWRCTRL, DSBL);
    }

}
/*******************************************************************************
 * FunctionName : PIO_SendData
 * Description  : 可编程IO口送数据
 * Param        : u8 Type 协议类型：EXT_SPI/EXT_UART/EXT_I2C
*******************************************************************************/
void PIO_SendData(u8 Type)
{
#define STRLEN  8

    u8 Buf[20] = { 0 };
    u8 i = 0;

    switch (Type)
    {
    case OUT_SPI:
        memcpy(Buf, "LA104SPI", STRLEN);
        ProgmIO_SPIWr(STRLEN, Buf);
        break;
    case OUT_I2C:
        ProgmIO_I2CWr(0x01, 0x00, 0x04);
        __Bios(DELAYuS, 50);
        ProgmIO_I2CWr(0x10, 0x22, 0x33);
        break;
    case OUT_UART:
        memcpy(Buf, "LA104UART", 9);
        for (i = 0; i < 9; i++)
        {
            ProgmIO_UARTTx(Buf[i]);
            __Bios(DELAYuS, 100);
//          Delay_mS(1);
        }
        break;

    default:
        break;
    }

}

/*******************************************************************************
 * FunctionName : PIO_PWM_SetFreq
 * Description  : PIO_PWM频率设置
 * Param        : u8 Channel 
 * Param        : u16 Psc 
 * Param        : u16 Arr 
*******************************************************************************/
void PIO_PWM_SetFreq(u8 Channel, u16 Psc, u16 Arr)
{
    if (Channel == PWM1)
    {
        __Bios(EXT_PWM, PWM1_PSC + Psc);
        __Bios(EXT_PWM, PWM1_ARR + Arr);
    } else if (Channel == PWM2)
    {
        __Bios(EXT_PWM, PWM2_PSC + Psc);
        __Bios(EXT_PWM, PWM2_ARR + Arr);
    }
}

/*******************************************************************************
 * FunctionName : PIO_PWM_SetDuty
 * Description  : PIO_PWM占空比设置
 * Param        : u8  Channel 通道
 * Param        : u8  Num     通道的频道编号
 * Param        : u16 Duty    占空比值
*******************************************************************************/
void PIO_PWM_SetDuty(u8 Channel, u8 Num, u16 Duty)
{
    if (Channel == PWM1)
    {
        if (Num == PWM_NUM1)      __Bios(EXT_PWM, PWM1_DUTY1 + Duty);
        else if (Num == PWM_NUM2) __Bios(EXT_PWM, PWM1_DUTY2 + Duty);
    } else if (Channel == PWM2)
    {
        if (Num == PWM_NUM1)      __Bios(EXT_PWM, PWM2_DUTY1 + Duty);
        else if (Num == PWM_NUM2) __Bios(EXT_PWM, PWM2_DUTY2 + Duty);
    }
}

/*******************************************************************************
 * FunctionName : PIO_PWM_Ctrl
 * Description  : PIO_PWM使能控制 
 * Param        : u8 Channel  PWM1/PWM2
 * Param        : u8 Status   ENBL/DSBL
*******************************************************************************/
void PIO_PWM_Ctrl(u8 Channel, u8 Status)
{
    if (Channel == PWM1)
    {
        if (Status == ENBL) __Bios(EXT_PWM, PWM1_CTRL + ENBL);
        else              __Bios(EXT_PWM, PWM1_CTRL + DSBL);
    } else if (Channel == PWM2)
    {
        if (Status == ENBL) __Bios(EXT_PWM, PWM2_CTRL + ENBL);
        else              __Bios(EXT_PWM, PWM2_CTRL + DSBL);
    }
}

/*******************************************************************************
 * FunctionName : PIO_AllDisable
 * Description  : 可编程接口全部关闭输出
 * Param        : void 
*******************************************************************************/
void PIO_AllDisable(void)
{
    __Bios(EXT_INP, INIT);
}

/*******************************************************************************
 * FunctionName : PIO_Init
 * Description  : 可编程IO口类型初始化
 * Param        : u8 Type 
*******************************************************************************/
void PIO_Init(u8 Type)
{
    u8 FreqNum = 0;

    switch (Type)
    {
    case OUT_PWM:
        __Bios(EXT_PWM, INIT);

        FreqNum = gOutSubParam[OUT_PWM][PWM1_FREQ];
        PIO_PWM_Ctrl(PWM1, gOutSubParam[OUT_PWM][PWM1_EN]);
        PIO_PWM_SetFreq(PWM1, OUT_PSC[FreqNum], OUT_ARR[FreqNum]);
        PIO_PWM_SetDuty(PWM1, PWM_NUM1, (OUT_ARR[FreqNum]) * gOutSubParam[OUT_PWM][PWM1_DUT1] / 10);
        PIO_PWM_SetDuty(PWM1, PWM_NUM2, (OUT_ARR[FreqNum]) * gOutSubParam[OUT_PWM][PWM1_DUT2] / 10);

        FreqNum = gOutSubParam[OUT_PWM][PWM2_FREQ];
        PIO_PWM_Ctrl(PWM2, gOutSubParam[OUT_PWM][PWM2_EN]);
        PIO_PWM_SetFreq(PWM2, OUT_PSC[FreqNum], OUT_ARR[FreqNum]);
        PIO_PWM_SetDuty(PWM2, PWM_NUM1, (OUT_ARR[FreqNum] + 1) * gOutSubParam[OUT_PWM][PWM2_DUT1] / 10);
        PIO_PWM_SetDuty(PWM2, PWM_NUM2, (OUT_ARR[FreqNum] + 1) * gOutSubParam[OUT_PWM][PWM2_DUT2] / 10);

        break;
    case OUT_SPI:
        __Bios(EXT_SPI, INIT);
        break;
    case OUT_I2C:
        __Bios(EXT_I2C, INIT);
        break;
    case OUT_UART:
        __Bios(EXT_UART, INIT);
        break;
    default:
        break;
    }
}

/*******************************************************************************
 * FunctionName : KeyQuickAct
 * Description  : T1，T2，X波轮快捷操作
 * Param        : void 
*******************************************************************************/
void KeyQuickAct(void)
{
    if (Menu.flag)
    {
        if ((READ_KEY == K4_HOLD) && (gKeyActv & ENCD_2p))
        {
            while (READ_KEY == K4_HOLD)
            {
                if (gItemIndexNum[TIME_SET] == TB_T1)
                {
                    if ((gItemParam[T1POSI] + gItemParamStep[T1POSI]) < gItemParam[T2POSI])
                    {
                        gItemParam[T1POSI] = gItemParam[T1POSI] + gItemParamStep[T1POSI];
                    } else
                    {
                        gItemParam[T1POSI] =  gItemParam[T2POSI];
                    }
                } else if (gItemIndexNum[TIME_SET] == TB_T2)
                {
                    if (gItemParam[TIMEBASE + gItemIndexNum[TIME_SET]] < gItemParamMax[TIMEBASE + gItemIndexNum[TIME_SET]])
                    {
                        gItemParam[TIMEBASE + gItemIndexNum[TIME_SET]] =
                            gItemParam[TIMEBASE + gItemIndexNum[TIME_SET]] + gItemParamStep[TIMEBASE + gItemIndexNum[TIME_SET]];
                    }
                } else if (gItemIndexNum[TIME_SET] == TB_X)
                {
                    gXposiAdd = 1;
                    ZC_Scale();
                    ShowWindowPrecent();
                }

                memcpy(gLCD_Buf, gLCD_Backup, LCD_BUF_WIDTH);
                ShowWaveToLCD();
            }
            Delay_mS(100);
            ShowTimeMarkValue();
            ShowMeasureValue();
            ResetPowerOffTime();
            gKeyActv = 0;
        } else if ((READ_KEY == K4_HOLD) && (gKeyActv & ENCD_2n))
        {
            while (READ_KEY == K4_HOLD)
            {
                if (gItemIndexNum[TIME_SET] == TB_T2)
                {
                    if ((gItemParam[T2POSI] - gItemParamStep[T2POSI]) >= gItemParam[T1POSI])
                    {
                        gItemParam[T2POSI] = gItemParam[T2POSI] - gItemParamStep[T2POSI];
                    } else
                    {
                        gItemParam[T2POSI] = gItemParam[T1POSI];
                    }
                } else if (gItemIndexNum[TIME_SET] == TB_T1)
                {
                    if (gItemParam[TIMEBASE + gItemIndexNum[TIME_SET]] > gItemParamMin[TIMEBASE + gItemIndexNum[TIME_SET]])
                    {
                        gItemParam[TIMEBASE + gItemIndexNum[TIME_SET]] =
                            gItemParam[TIMEBASE + gItemIndexNum[TIME_SET]] - gItemParamStep[TIMEBASE + gItemIndexNum[TIME_SET]];
                    }
                } else if (gItemIndexNum[TIME_SET] == TB_X)
                {
                    gXposiDec = 1;
                    ZC_Scale();
                    ShowWindowPrecent();
                }

                memcpy(gLCD_Buf, gLCD_Backup, LCD_BUF_WIDTH);
                ShowWaveToLCD();
            }
            Delay_mS(100);
            ShowTimeMarkValue();
            ShowMeasureValue();
            ResetPowerOffTime();
            gKeyActv = 0;
        }
    } else if (Item.flag)
    {}
}

