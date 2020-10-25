/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  DS213_APP main.c                                               Author : bure
********************************************************************************
                               修改备注
  ----------------------------------------------------------------------------
  18.07.25 APP V1.01: 初始化增加了“开启 V- 及 ADC”操作
                      初始化中恢复了 AutoCalib 操作
  18.07.27 APP V1.02: 修正10V档位电子开关逻辑
                      修正开机按键无响声
  18.07.28 APP V1.03: 修复电池电压读数
                      开机无配置文件时自动校准后保存文件
                      配置文件不保留BAK（文件只能保持一个有效，簇号累加可能有问题）
                      完善待机时候LED呼吸效果
                      修复当“XP”移动到末端，断开输入信号，波形无法清除
                      single模式锁定，采样状态位FINISH==FULL？
                      增加音量和背光参数保存
                      完善CH_D的运行功能
                      完善输出口的模拟波形输出
  18.08.08 APP V1.04: 完善FAT12文件格式和文件保存类型
  18.08.10 APP V1.05: 增加CAL的时候按Shift时编码器累加（但没有最大最小值限制）
                      修复T0游标越界和触发位置
                      调整SCAN，取消NONE同步模式，SCAN等同步方式完善后修正？？？
                      增加DUT，TH，TL
                      <Vt触发模式不稳定，其他均可以
  18.08.14 APP V1.06: 保存文件从编号0开始
                      完善csv文件保存格式
                      完善saveDat和loadDat以及CH3的调出记录波形
                      文件保存后自动累加编号
  18.08.15 APP V1.07: 保存文件完成提示
                      删除TWTH
  18.08.15 APP V1.08: 增加开机自动校准
                      修正Buf保存格式
                      调整采样等待时间
  18.08.17 APP V1.09: 修正T0触发位置，（时基10us以下由于差值算法，T0与触发位置有偏移，未解决？？？）
 
  18.08.27 APP V1.10: 修正Load DAT问题
                      增加触发电压数值显示
                      增加待机时间菜单选择
  18.08.29 APP V1.11: 修正不待机时呼吸灯
                      增加电池电压显示
 
  18.09.03 APP V1.12: 修正待机时间保持参数功能
                      增加K4按键声音反馈
                      增加未解锁的时候，长按K4+K3关机（可能硬件不支持）
 
  18.09.27 APP V1.13: 修复低电量反复重启，电池小于3v自动关机
                      0.5us，0.2us，0.1us 时基修改为500ns，200ns，100ns
                      保存参数设置显示保存状态
                      将屏幕通道显示的1234改成ABCD
           APP V1.14: 修复低电量反复重启，电池小于3.2v自动关机

  18.11.09 APP V1.15: 改为若无开机参数配置文件时才调用开机校准，并保存开机参数
                      消除了保存参数时屏幕显示残留，并延长了信息显示停留时间
                      模拟输出波形采样点数，20KHz挡位时50点，其他挡位时100点
 
  18.12.22 APP V1.20: 配合 DFU3.73 与 FPGA STMU_213_016.bin 程序重新改写
                      修改了 APP V1.xx 存在的各种问题

  19.02.25 APP V1.21: 修改了 NORM 模式下显示波形不能保持的问题
                      修改了按下 K4 键后其他操作后自动退出 STOP 状态的问题

  19.03.08 APP V1.22: 修改了部分偏移误差较大的机器不能自动校正的问题
                      修改了各种同步模式下的波形显示方式
                      

*******************************************************************************/
#include "Sys.h"
#include "DS213Bios.h"
#include <library.h>

//gabo
__attribute__((__section__(".entry"))) int main(void);
extern void SysInt(void);
void USB_Init() {}
void AppDiskInit() {}
void InitFatParam() {}
/*extern "C"*/ int __GetEncoder()
{
    BIOS::KEY::EKey key = BIOS::KEY::GetKey();
    switch (key)
    {
        case BIOS::KEY::Up: return ENCD_1p; break;
        case BIOS::KEY::Down: return ENCD_1n; break;
        case BIOS::KEY::Right: return ENCD_2p; break;
        case BIOS::KEY::Left: return ENCD_2n; break;
        case BIOS::KEY::F1: return K1_ACTn; break;
        case BIOS::KEY::F2: return K2_ACTn; break;
        case BIOS::KEY::F3: return K3_ACTn; break;
        case BIOS::KEY::F4: return K4_ACTn; break;

        default:
            return 0;
    }
}

HwDrvDef  Hw;
u8  APP_VERSION[12] = { "Ver 1.22-gabo" };
u8  PwrDownSt = 0;

/*******************************************************************************

*******************************************************************************/
int main(void)
{
  pFunc1 __BiosInit = (pFunc1)0x08000101; // Point to DFU BiosInit
  __BiosInit((u32)&Hw);                   //
  
  static BIOS::OS::TInterruptHandler pCurrentSysTick = BIOS::OS::GetInterruptVector(BIOS::OS::ISysTick);
    BIOS::OS::SetInterruptVector(BIOS::OS::ISysTick, [](){
        pCurrentSysTick();
        SysInt();
    });
  BIOS::LCD::Clear(RGB565(000000));

/****** 仅 APP 单独 DEBUG 调试运行时采用，通过 DFU 调用时该部分不编译运行 ****
  
  Hw.pDevInit(PWRCTRL);
  Hw.pDevInit(KEYnDEV);
  Hw.pDevInit(DISPDEV);
  Hw.pDevInit(BUZZDEV);
  Hw.pClrScrn(BLK);
  u16 x0 = Hw.LogoX0, y0 = Hw.LogoY0;
  Hw.pDispLogo(x0, y0);
  *Hw.pPwm_Bkl = 50;
  *Hw.pBuz_Vol = 50;
//  Beep_mS(500);
  Hw.pDevInit(FPGADEV);
  Hw.pDevInit(FPGACFG);
  */
/******************************************************************************/
  
  DispStr(12*8, 90, WHT, CHAR, "Oscilloscope APP");
  DispStr(29*8, 90, WHT, CHAR,  APP_VERSION );
  
  Hw.pDevInit(USBDISK);
  Hw.pDevInit(USB_DEV);
  USB_Init();
  AppDiskInit();
  InitFatParam();

  *Hw.pOut_nPD = 1;                                // 开启 V- 及 ADC
  Hw.pDevInit(SGNL_IN);                            // 信号输入通道初始化
  Hw.pDevInit(SGNLOUT);                            // 信号输出通道初始化
  Hw.pDevInit(SO_ANLG);                            // 信号输出为模拟信号
  Hw.pDevInit(SO_DGTL);                            // 信号输出为数字信号
  FPGA_CtrlRW(A_C_CH, CHIP_RST);                   // FPGA #0 模块复位
  FPGA_CtrlRW(B_D_CH, CHIP_RST);                   // FPGA #1 模块复位
  SmplStart();                                     // FPGA 开始新一轮采样
  TrackClr();                                      // 清除显示波形轨迹
  
  uc8 STR1[] = "Parameter record not found";
  uc8 STR2[] = "Reload parameter from disk";
  uc8* STR   = (LoadParam()) ? STR1 : STR2;        // 读取预设开机参数
  DispStr(12*8, 30, GRN, CHAR, (u8*)STR);          // 显示读取结果信息

//  if(STR == STR1){                                 // 无预设开机参数配置文件
//    RowPosi(12*8, 50);
//    AddStr(TXT2C, CHAR, (u8*)"Correction");
//    Correction();                                  // 各个档零点校正
//  } else Delay_mS(2000);
  
  *Hw.pPwm_Bkl = Pop[SBKL].Val;                    // 设置背光亮度
  *Hw.pBuz_Vol = Pop[SVOL].Val;                    // 设置蜂鸣器音量
  
  //****************************** 系统主循环程序 ******************************
  KeyAct = 0;
  while (1){
    UpdtMainMenu();                                           // 更新主界面菜单
    DispSync();                                               // 显示同步处理 
    if(PopUpdt){
      UpdtPopMenu();                                          // 更新弹出窗菜单
      PopUpdt = 0;
    } 
    //============================== 按键处理循环 ==============================
    u16 Act = __GetEncoder();
    //gabo
    if (Act & K4_ACTn)
      break;

    KeyAct = 0;
    if(Act){                                                  // 有按键或编码器动作
      u32 Track = Menu[Item].Src;
      u32 Range = Menu[(Track == CH_A)? RNA : RNB].Val;
      u16 *pKn  = &Menu[Item].Val;
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ K1 Push ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & K1_ACTn){ 
        if(~nKeySt & K4){                                     // 按住 K4 时按 K1
          u32 r = Snapshot();                                 // 保存当前屏幕截图
          RowPosi(Menu[TM2].X0, Menu[TM2].Y0);
          if(r == OK) AddStr(TXT2C, CHAR, " Saveing OK  ");
          else        AddStr(WARNG, CHAR, " Saveing Err ");
          Delay_mS(2000);
          Menu[TM2].Flg |= UPDT;                              // 设置该项更新标志
        } else {                                              // 单独按下 K1
          Menu[RUN].Val = (Menu[RUN].Val)? HOLD : SMPL;       // 切换 暂停/运行 状态
          Menu[RUN].Flg |= UPDT;                              // 设置该项更新标志
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ K2 Push ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & K2_ACTn){
        if(~nKeySt & K4){                                     // 按住 K4 时按 K2
          u32 r = SaveParam();                                // 保存当前设置参数
          RowPosi(Menu[TM2].X0, Menu[TM2].Y0);
          if(r == OK) AddStr(TXT2C, CHAR, " File Saved  ");
          else        AddStr(WARNG, CHAR, " Save Error  ");
          Delay_mS(2000);
          Menu[TM2].Flg |= UPDT;                              // 恢复TM2显示项
        } else {                                              // 单独按下 K2
          PopHide = (PopHide) ? 0 : 1;                        // 打开/关闭 弹出窗
          if(!PopHide) UpdtPopMenu();                         // 更新弹出窗菜单
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ K3 Push ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & K3_ACTn){
        if(~nKeySt & K4){                                     // 按住 K4 时按 K3
          if(Hw.LicenceOk == 0) *Hw.pOut_PEn = 0;             // 解锁前测试关机
          else {
            RowPosi(12*8, 120);
            AddStr(TXT2C, CHAR, (u8*)"Correction");
            Correction();                                     // 各档零点校正
            RowPosi(23*8, 120);
            AddStr(TXT2C, CHAR, "Completed    ");
            Delay_mS(2000);
          }
        } else {                                              // 单独按下 K3
          if(PopHide){                                        // 主菜单下操作
            if(Item == V_T || Item == TRG){
              Menu[Item].Flg |= UPDT, Menu[Item].Src += 1;
              Menu[Item].Src &= 3;                            // 选择 CH A~D
              Menu[V_T].Src = Menu[Item].Src;
              Menu[V_T].Flg |= UPDT;                          // 三项数据同步
              Menu[TRG].Src = Menu[Item].Src; 
              Menu[TRG].Flg |= UPDT;
              Menu[T_0].Src = Menu[Item].Src;
              Menu[T_0].Flg |= UPDT;
            } else if(Item == YNP || Item == TM1 || Item == TM2){
              Menu[Item].Src += 1;
              Menu[Item].Src &= 3;                            // 选择 CH A~D
              Menu[Item].Flg |= UPDT;
            } else if(Item == VM1 || Item == VM2 || Item == CAL){
              Menu[Item].Src += 1;
              Menu[Item].Src &= 1;                            // 选择 CH A/B
              Menu[Item].Flg |= UPDT;
            } else if(Item >= V_1 && Item <= T_2) Menu[Item].Flg ^= INVR;
          } else PopProcess();                                // 弹出窗内操作
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Indx Encoder ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & ENCD_1n || Act & ENCD_1p){
        u32 Inc   = (~nKeySt & K4) ? 10 : 1;                  // 按住 K4 时增量 *10
        if(PopHide){                                          // 主菜单下的操作
          Menu[Item].Flg |= UPDT;  
          if     (Item == V_T) pKn = &Vt[Track];
          else if(Item == YNP) pKn = &Yn[Track];
          else if(Item == CAL) pKn = &Diff[Track][Range];
          if((Item == VM1 || Item == VM2)&& *pKn == RMS){     // 离开有效值测量时
            Menu[T_1].Flg &= ~INVR, Menu[T_2].Flg &= ~INVR;   // 消隐 T1,T2 游标
          }
          if(Item != V_T || Track < CH_C){
            if(Act & ENCD_1n) *pKn =IndxInc(*pKn, Inc, Item); // Indx+
            else              *pKn =IndxDec(*pKn, Inc, Item); // Indx-
          }
          if(Item == OUT){                                    // OUT 补丁
            Menu[FRQ].Flg |= UPDT;
            if(Menu[FRQ].Val > 10 && *pKn != DGTL) Menu[FRQ].Val = 10; 
          }
          if(Item == FRQ){                                    // FRQ 补丁
            if(*pKn > 10 && Menu[OUT].Val != DGTL) *pKn = 10; 
          }
          if(Item == SYN && *pKn == SLOW){                    // SYN 补丁
            if(Menu[TIM].Val > 5 ) Menu[TIM].Val = 5; 
            Menu[TIM].Flg |= UPDT;
          }
          if(Item == TIM && Menu[SYN].Val == SLOW){           // TIM 补丁
            if(*pKn > 5) *pKn = 5; 
          }
        } else {                                              // 弹出窗下的操作
          PopUpdt = 1;
          if(Act & ENCD_1n) PopValInc(Line, Inc);             // Line+
          else              PopValDec(Line, Inc);             // Line-
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Item+ Encoder ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & ENCD_2p){
        Menu[Item].Flg |= FLSH;                               // 前项置闪烁标志
        if(PopHide){                                          // 主菜单下的操作
          Item = (Item >= VM1) ? 0 : Item+1;                  // 当前项+1
        } else {                                              // 弹出窗下的操作
          Line = (Line == SPDT) ? 0 : Line+1;                 // 当前行+1
          PopUpdt = 1;
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Item- Encoder ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & ENCD_2n){
        Menu[Item].Flg |= FLSH;                               // 前项置闪烁标志
        if(PopHide){                                          // 主菜单下的操作
          Item = (Item > 0) ? Item-1 : VM1;                   // 当前项-1
        } else {                                              // 弹出窗下的操作
          Line = (Line > 0) ? Line-1 : SPDT;                  // 当前行-1
          PopUpdt = 1;
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      Beep_mS(20);
      if(Pop[SPDT].Val != 0) PD_Cnt = Pop[SPDT].Val*60;       // 重置休眠计时器
    }
    //==========================================================================
    if(*Hw.pSt_Vin == 1){
      PD_Cnt = Pop[SPDT].Val*60;                              // 重置休眠计时器
      
    }
    if(PD_Cnt == 0 && PwrDownEn && *Hw.pSt_Vin == 0){         // 进入省电状态
      Hw.pPowerDown(ENABLE);                                  // 低功耗供电模式
      *Hw.pPwm_Bkl = 0;                                       // 关闭背光
      *Hw.pOut_Clr = 1;                                       // FPGA 省电状态
      PwrDownSt = 1;
    } else if(PwrDownSt == 1){                                // 恢复运行状态
      Hw.pPowerDown(DISABLE);                                 // 正常供电模式
      *Hw.pPwm_Bkl = Pop[SBKL].Val;                           // 恢复背光
      *Hw.pOut_Clr = 0;                                       // FPGA 运行状态
      *Hw.pPwm_LED = LED_PWM_MAX;                             // 关闭呼吸灯
      PwrDownSt = 0;
      PD_Cnt = Pop[SPDT].Val*60;                              // 重置休眠计时器
    }
  }
  //****************************************************************************
    BIOS::OS::SetInterruptVector(BIOS::OS::ISysTick, pCurrentSysTick);
    return 0;
}

/******************************** END OF FILE *********************************/
