
/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : Main.c  
 Version   : DS203_APP Ver 2.5x                                  Author : bure
*******************************************************************************/
#include "Interrupt.h"
#include "Function.h"
#include "platform.h"
#include "Calibrat.h"
#include "Process.h"
#include "Draw.h"
#include "BIOS.h"
#include "Menu.h"
#include "File.h"
#include <string.h>


/*******************************************************************************
Version changes
APP V2.30: since this version no longer to compatible PCB_V2.6 the following version of the motherboard
           Since this version is no longer compatible SYS_V1.31 the following version
           Sleep timing was revised to 600 seconds (Main.c)
           Modify, and add a new SYS library functions (BIOS.s)
           Modify the boot information display program (Main.c)
APP V2.31: an increase of boot identify FPGA load configuration completion discriminant (Main.c)
           Increase the Licence rights management functionality Demo program example (Ident.c, Main.c),,
           Modified analog channel correction function entry and exit operations (calibrat.c)
           Increase the the 144MHz alternately sampling mode function (Process.c)
APP V2.32 from the version from the IAR 4.42 and version 5.0 can be used in parallel
           Source did not change, the increase of the folder IAR_V5_Prpject
APP V2.33 modified scan <1uS, display the refresh BUG (Process.c,)
           Modified in the calibration state, the operation message BUG (Calibrat.c)
APP V2.34 changed by channel separate calibration (Calibrat.c & Main.c,)
           Modify the calibration mode of operation (option Calibrat.c)
APP V2.35 modified in the calibration process BUG (Calibrat.c),
           Modified, <5uS scan, the suspension can not BUG (Process.c)
           To optimize the display data handler (Process.c)
           Increase of the analog channel automatic zero balance function (Main.c, Process.c, Calibrat.c)
APP V2.36 part of the calibration operation into automatic mode (Calibrat.c, Process.c, Function.c)
           Modify the boot loader to the operating parameters (Main.c)
APP V2.37 to further improve and optimize the display data handler (Process.c)
           Modify the 32-bit signed and unsigned integer conversion process rounded BUG (Function.c,)
           Increase the pulse width duty cycle of the time and frequency measurement function (Process.c, Menu.c)
APP V2.40 increase write U disk to create the file function (Main.c, and Flies.c dosfs.c)
           Modify the save file number is displayed when BUG (Menu.c)
APP V2.41 increase the file format for the BUF's read / write sample buffer data files (Main.c, Flies.c, Menu.c)
           Increased the file format for the CSV export sample buffer data files (Main.c Flies.c, Menu.c)
APP V2.42 for space-saving file system to SYS_V1.40 module (ASM.s, Flies.c, dosfs.c)
    	       Changed use "SerialNo.WPT" file is stored parameter table (Flies.c)
           Note: the APP V2.42 or later must be used in conjunction with the SYS V1.40 or later
APP V2.43 modify the adjustment of the analog channels stalls BUG (Main.c),
APP V2.44 modified to save the parameters in the calibration operation BUG (Calibrat.c),
           Increase the power load parameters, the success of Tips (Main.c)
APP V2.45 modified to read and write BUF file recovery Display the corresponding menu BUG (Files.c)
           Delete the read test information when the BUF file feedback (Main.c)
APP V2.50 rewrite based on the new FAT12 file system, file read and write procedures (Files.c, ASM.s)
           Changes to TH, TL measurements the display BUG (Menu.c,)
           Optimized with dimensionless values ??show the correlation function (Menu.c Function.c, Calibrat.c)
           Modify the pulse width trigger the BUG (Process.c)
APP V2.51 modify Vmin and Vmax, Vpp measured BUG (Process.c)
*******************************************************************************/
                                                 
#define APP_VERSION       "GCCv1.7W5.6 APP(2.51+SmTech1.8+PMOS69 fixes)"
#define APP_REVISION      "Revision(W5.6) by Wildcat            "

#define LINE2  "!PRESS    >    !#!    !?!     !]!    < O >    < O >  !"
#define LINE3  "!                                < ADJ > !< MENU > "
#define LINE5  "!SHORT> !HOLD! AUTO METERS SELECT SET TRG  !BUFFER  "
#define LINE6  "       !reset TRIG sav/ld  ITEM  CHANNEL!(spec/env)"
#define LINE7  "                 !(config)      !(XYper)/ (Change) "
#define LINE4  "                               (Raw-Nor)/ !(Spc)!  "  
#define LINE8  "                               (Spc Func) (Func)  "
#define LINE9  "       !CH on!                                     "
#define LINE10  " LONG!> V/DIV !MAN  STAND! T-BASE MTR PGE !MENU/MTR "       
#define LINE11 "       !short TRIG   BY   !short    (w/mtrs! on)!   "
#define LINE12 "        !cut               !cut  !WAV CAL STDBY TIM!"      
#define LINE13 "                                  !(w/mtrs !off)   "
#define LINE14 "                         !METERS!                  "
#define LINE15 " HOLD!> CHART !CAL!  SAVE  CURSOR TRIGGER   !HOLD   "    
#define LINE16 "             (adj)!config RSTRCT HOLDOFF  MIN/MAX! "

typedef struct {char*ChartLine;}DisplayLine;
DisplayLine DisplayChart[15]= {{(char*)LINE2},{(char*)LINE3},{(char*)LINE5},{(char*)LINE6},{(char*)LINE7},{(char*)LINE4},{(char*)LINE8},
                              {(char*)LINE9},{(char*)LINE10},{(char*)LINE11},{(char*)LINE12},{(char*)LINE13},{(char*)LINE14}, 
                              {(char*)LINE15},{(char*)LINE16}};             

//uc8 PROJECT_STR[20] = "Demo PROG. Ver 1.00";
u8 OldCurrent;
u8 OldDet;
u8 FlagFrameMode;
u8 OldFrame;
u8 FlagMeter;
u8 TrgAuto;
u16 OldPosX;
u16 OldPosY;
s16 PreviousTrigSelect[4];
u8  LastSelected=0;
u8  XposFlag=0;
u8  C_Dmeter[8]={4,8,9,10,11,4,10,11};
u8  UpdateMeter=4;
u8  Timeout=1;
extern u8 _vectors[];
u8  ShortBuffXpos;
u32 Tmp;
u8  SelectiveUpdate=0;
u8  UpdateFlag=0;
u8  BTwinkState=0;
u8  SaveCurrent[2]={0,0};
u8  ChStatus[4]={0,0,0,0};
u8  ABTrigStatus=0;
u8  ChOnStatus[2]={1,1};
u8  RefreshDisplay=0;
u8  OSStatus=0;
u8  FastRepeatR=0;
u8  FastRepeatL=0;
const char NoneStr[13]="None        ";
const char DisableSTR[37]="SCOPE DISABLED- PRESS ANY KEY TO RUN";

u8 OldMeter;
u8 TrgAutoOld;
u16 OldTrack1X;
u16 OldTrack2X;
s16 _Vt1Old;
s16 _Vt2Old;
u16 XposRef;		//Xpos compensated for interpolated ranges to show equivalent screen position 
u8  UpdateCount=0;
u8 ShowFFT=0;
extern u8 UpdateWindow;
u8  ClearDir=0;

void UpdateBigMeters(void);
void EnableTitleMeters(u8 Service);
void UpdateMeterPage(void);
u8 ConfigFile(u8 service);
u16 GetHoldoffIndPos(void);
void EnableChannel(u8 track);
void UpdateTriggerMem(void);
void SelUpdate(u8 detail);
void TriggFlagUpdate(void);
void MessageHandler(u8 number);
void CurDetailUpdate(void);
void ChannelStatus(u8 service);
void MeterUpdate(u8 start,u8 limit);
void TrigMemory(u8 i);
void PWMdutyControl(u8 Dir);
void PWMscaleReset(u8 Dir);
void BurstAdjust(u8 Dir);
void InitChart(void);
void ProcessEditName(void);
void ResetEditList(void);
u8   LeftToggleSpecialFunctions(void);
void Config_init(void);
u8 CursorSpeedLogic(u8 enable);
void UpdateFrameMode(void);

#warning Gabo: disabled
//NVIC_InitTypeDef NVIC_InitStructure;

typedef struct {char*Message;}SelectMessage; 
u8 MessageNumber;

#define SAVESET       " "                              
#define WAVCAL_ON     "  WAVE CALIBRATION - ON   "   //1
#define WAVCAL_OFF    "  WAVE CALIBRATION - OFF  "   //2
#define STDBYTIM_ON   "   STANDBY TIMER - ON     "   //3
#define STDBYTIM_OFF  "   STANDBY TIMER - OFF    "   //4
#define CURMETERS_ON  "CURSOR SELECT METERS - ON "   //5
#define CURMETERS_OFF "CURSOR SELECT METERS - OFF"   //6
#define TRGHLD_ON     "  TRIGGER HOLDOFF  - ON   "   //7
#define TRGHLD_OFF    "  TRIGGER HOLDOFF  - OFF  "   //8	
#define SHOWEDGE_ON   "     MIN/MAX HOLD ON      "   //9  
#define SHOWEDGE_OFF  "     MIN/MAX HOLD OFF     "   //10   
#define WAIT_RESET    "   WAITING FOR RESET...   "   //11  
#define NOR_DISPLAY   " NORMAL WAVEFORM DISPLAY  "   //12
#define RAW_DATA      "     RAW DATA DISPLAY     "   //13
#define CUR_DIS_ON    "     CURSOR DISPLAY ON    "   //14
#define CUR_DIS_OFF   "     CURSOR DISPLAY OFF   "   //15

//#define SHOWEDGE_ON   "      EDGE FILTER OFF      "   //9  
//#define SHOWEDGE_OFF  "      EDGE FILTER  ON      "   //10   

SelectMessage Show[16]= {{(char*)SAVESET},{(char*)WAVCAL_ON},{(char*)WAVCAL_OFF},{(char*)STDBYTIM_ON},{(char*)STDBYTIM_OFF},{
                              (char*)CURMETERS_ON},{(char*)CURMETERS_OFF},{(char*)TRGHLD_ON},{(char*)TRGHLD_OFF},{
                              (char*)SHOWEDGE_ON},{(char*)SHOWEDGE_OFF},{(char*)WAIT_RESET},{(char*)NOR_DISPLAY},{(char*)RAW_DATA},{
                              (char*)CUR_DIS_ON},{(char*)CUR_DIS_OFF}};

uc16 TrigReset[22]=   {300, 150, 100, 75, 35, 15, 10, 10, 5, 3,   	//for auto trig mode, set time to reset after loosing data		
                        2,      2,   2,   2,  2,  2,  2, 2, 2, 2,    			
                        2,      2};

uc16 NoteTimer[3]=    {250, 125, 50};                			//*20ms

/*******************************************************************************
  main : Main routine.
*******************************************************************************/
int main(void)
{
  u16 Tmp;
  s32 i;
  u16  Second = 0;//,Offset, Result,Count_FPS = 0  
  u8 j=0;  //          ,h=0;
  s32 TmpVT;
  s16 Vt1Old=0;
  s16 Vt2Old=0;
  u16 loopcount=0;
  char DelFileName[13];
  const char AutoStr[4][6]={"!Man!","!1/4!","!1/2!","!3/4!"};

  __Display_Str(32, 50, WHT, PRN, APP_VERSION);
  __Display_Str(96, 130, WHT, PRN, APP_REVISION);
  Y_Attr = (Y_attr*)__Get(VERTICAL);     
  X_Attr = (X_attr*)__Get(HORIZONTAL);
  G_Attr = (G_attr*)__Get(GLOBAL);
  T_Attr = (T_attr*)__Get(TRIGGER);

  if(memcmp((u8*)__Get(13),"2.81",4)==0)HardwareVersion=1; else HardwareVersion=0;         //HDWVER

  InitiateCalData();
  LoadBaseBuffers();				
  Load_Attr();                                 // assignment Y_Attr
  FineAdjust=2;
  i = Load_Param(0); 
  if(i == 0)  // read the default boot parameters
    __Display_Str(88, 30, GRN, PRN,   "Loaded configuration file");
  else       
    __Display_Str(80, 30, YEL, PRN,  "Configuration file not found");  
  
  if(FineAdjust>4)FineAdjust=2;  
  Beep_mS = 500;
  Key_Buffer=0;
  Delayms(2000); 
  App_init(1);

  SetIRQ2Priority();

  #warning Gabo: moise generator disabled
/*
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);                  
  NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQChannel;              //TIM4 IRQ setup for noise generator
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;        
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
*/
//----------------- TIM2_IRQ Used for sweep and burst generator ----------------
  TIM2->PSC  =72-1; 
  TIM2->ARR  =1000-1;       // 1mS             
  TIM2->DIER =0x0001;       // enable TIM2_IRQ 
  TIM2->CR1 = 0x0085;       // enable timer
  TIM_2IRQControl();

  Config_init();
  OldMode=_Mode;
  DisableCursorTimer=1;
  Update=2;			//=2 initiates delayed trig loop to prevent locking up with no trigger and empty buffer
  Title[FILE][0].Value=LOAD;    //preset file menu to load config files
  Title[FILE][1].Value=0;       //config file number (0=boot file)
  Title[FILE][2].Value=CFG;


//-------------------------------------------------------------------------- ************** LOOP MAIN *****************
  while (1){
  
//-------------------------------------------------------------------------- Gestione Entra e Uscita da Modalit

    if(_Kind!=PWM)PWAdjustMode=0;
    if((ChartLogic())&&(ChartMode))OsChartFlag=1;else OsChartFlag=0;

    if (_Mode!=OldMode)
    {
      if (_Mode ==X_Y ){       // entra in XY_S 
       BackGround_Reset(1);
        if (OldMode<6){        //4
          OldTrack1X=Title[TRACK1][POSI].Value;
          OldTrack2X=Title[TRACK2][POSI].Value;
        }
	OffsetX=135;
        OffsetY=100;
        ChannelStatus(0);						//save status 
        EnableChannel(TRACK1);
        EnableChannel(TRACK2);
        OldMeter=FlagMeter;
        if (FlagMeter==0) {EnableMeter();UpdateMeterPage();}
        _Vt1Old=_Vt1;
        _Vt2Old=_Vt2;
        Vt1Old=0;
	Vt2Old=0;
        TrgAutoOld=TrgAuto;
        TrgAuto=2;
        OldFrame=FrameMode; 
        FlagFrameMode=0;
      }
     
      if ((_Mode<6) &&(OldMode>5)){     // esce da XY_S o XY_A
        ChannelStatus(1);						//restore status 
        Title[TRACK1][POSI].Value = OldTrack1X;
        Title[TRACK2][POSI].Value = OldTrack2X;
        BackGround_Reset(1);
        if (OldMeter==0)  DisableMeter();
        _Vt1=_Vt1Old;
        _Vt2=_Vt2Old;
        TrgAuto=TrgAutoOld;
        if(OldFrame)FlagFrameMode=1;
        Update_View_Area();    // in case file menu display was showing 
      }
    }
   
    if(Title[TRIGG][SOURCE].Value !=4) OldMode=_Mode;
    if (_Mode==X_Y)Title[TRACK1][POSI].Value=100;               
    if (_Mode==X_Y)Title[TRACK2][POSI].Value=100;

    if (_4_source>11)ShowFFT=1;
    else{
      ShowFFT=0;
      DownConvert=0;
      if(DownConvertMode){DownConvertMode=0; DownConvertRestore();}
    }

    OsBufferLogicFlag=OSBufferLogic();

    //-------------------------------------------------------------------------- ********* LIFE *************    
    if (((__Get(USB_POWER)>0)||(Timeout==0))&&(PD_Cnt>0)) PD_Cnt = 600;	  //don't time out into standby on external power or if timeout is disabled and not already in standby
    if(PD_Cnt == 0){               
      __Set(BACKLIGHT, 0);                    // turn off the backlight
      __Set(STANDBY, EN);                     // enter low power states
    } else {

        if((Update)&&((_Mode!=SCAN)||(ChartLogic()))){    
          if((!ChartLogic())||(FrameMode==0))
            _X_posi.Value=GetXpos(XposRef);                 //align interpolated time bases with the others            
	}

        if (SelectiveUpdate&1){		      // restore screen after removing chart
          if (_Mode==X_Y){
            ClearScreenArea(0,400,214,215);   //clear top line
          } 
          ClearScreenArea(0,11,12,214);       //clear left mark area
          DisableCursorTimer=2;
        }  

    Synchro();                              // simultaneous display of waveform data of each track

    if ((TrgAuto>0)&&(_Mode!=SCAN)&&(_Mode!=AUTO)&&(_Mode!=SGL)){  
      if(__Get(FIFO_START)== 0){			           //check to see if not triggered w/auto trig on
        loopcount++;							
        if (loopcount>TrigReset[_T_base]){			   //after proper wait time, re-initialize trig	
          if(Title[TRIGG][SOURCE].Value==4){			   //in A&B mode, reset only non triggered ch
            if((Sweep)&&(_Tr_kind==8)&&(_Kind<5)){
              if(TrigSourceEnable==0)_Vt2=_2_posi+10;		   
              if(TrigSourceEnable==1)_Vt1=_1_posi+10;		   
            }else{
              if(TrigSourceEnable==0)_Vt2=_2_posi;		   //alternate channel function call at end of synchro has shifted over to
              if(TrigSourceEnable==1)_Vt1=_1_posi;		   //other ch if one ch is not triggered so need to reset alternate ch
            }
            Update_Trig(0);
          }else{
            if((Sweep)&&(_Tr_kind==8)&&(_Kind<5)){
              _Vt1=_1_posi+10;                                     //keep trig level away from zero for time based gen sweep/burst synch
              _Vt2=_2_posi+10;
            }else{                               
              _Vt1=_1_posi;			                   // set trig at signal zero point in hope to get device to trigger
              _Vt2=_2_posi;
            }
            Update_Trig(1);
          }
          UpdateMarkLogic();
          loopcount=0;
        }
      }else{								//if triggered
        if(Title[TRIGG][SOURCE].Value==4){				//in A&B mode
          if(TrigSourceEnable==0)ABTrigStatus|=1;			//ch A is triggered
          if(TrigSourceEnable==1)ABTrigStatus|=2;			//ch B is triggered
          if(ABTrigStatus==3) {loopcount=0;ABTrigStatus=0;}		//if both ch are triggered, reset count and trigger status
        }else{
          loopcount=0;	
        } 
      }				//if get fifo start else...
    }else loopcount=0;		//if trgauto>...

      if(ToggleName){
        if(ConfNameTimer==0){
          UpdateMarkLogic();
          ToggleName=0;
        }
      }

      if((FlagMeter==0)&&(Update)){ 
         Title[T_VERNIE][2].Flag &= 0xFD; 				
        _D_V_Source.Flag &= 0xFD;            
        _Delta_V.Flag    &= 0xFD; 	      //shut update delta display flags with meter off	
        _Delta_T.Flag    &= 0xFD; 
      } 



      //Count_FPS++;			

//============================================REGULAR METER TIMING====================================================
      if(Second != Sec_Cnt) {
        Second = Sec_Cnt;
        Result_FPS = Count_FPS;
        Count_FPS = 0;

        if((__Get(V_BATTERY)>(PrevBatLevel+40))||(__Get(V_BATTERY)<(PrevBatLevel-40))){	//update with greater than 40Mv change (WAS +/-20Mv)
          BatLevelCompensation(); 
          PrevBatLevel=__Get(V_BATTERY);
          SetOffset(0,_A_Range,_1_posi);				//provides update for Ka3
          SetOffset(1,_B_Range,_2_posi);
        } 

        Update_Battery();

        if (FlagMeter==1){						// refresh the measured values ??per second

          if(EnableMeterCalc){
            DETflag=1;
            Display_Value(FRQ);
            DETflag=0;
          }else LastFreqReadout=0;

          EnablePaS=1;
          for(i=0; i<9; ++i)  Display_Value(i);

          if(((__Get(FIFO_START)!=0)||((_Mode!=NORH)&&(_Mode!=NORHLD)))||((DownConvertInitiate==1)&&(FFTt1Mode))){ //hold reset to show meter values when in normal mode not triggered
            if((_T_base > 10)||((_T_base>8)&&(FastMode==1)))ResetSum();else SumResetFlag=1;		//in slow mode wait until frame is completed before resetting
            WaitForTrigFlag=0; 
          }else WaitForTrigFlag=1;                             //allows reset to occur prior to meter loads on triggering after holding values

        }							

        //if ((FlagMeter==0)&&(EnableMeterCalc==0))ResetSum();      
        if(((FlagMeter==0)&&(EnableMeterCalc==0))&&
          ((DownConvertMode==0)||(DownConvertInitiate==1)))ResetSum();                //DownConvert initiates here
      }

      if((ChartLogic())&&(CursorDisplaySelect<4))CalculateTvernier(1);             //Xpos/Tvernier display moving update


//============================================================================================================================

        if (((Cnt_20mS%16)>7)&&((Cnt_20mS%16)<16)&&(UpdateFlag==1))UpdateFlag=0;

//===========================================LARGE METER TIMING===========================================================
        if (((Cnt_20mS%16)>=0)&&((Cnt_20mS%16)<8)&&(UpdateFlag==0)){    //update meters %16= 3 times/sec %24= 2 times/sec  %12= 4 times/sec
            UpdateFlag=1;						      //note that big meter highlight blink rate is tied to update speed	
          if(UpdateCount==2){
            EnablePaS=1;						//enables TH, TL and duty% to update 1/sec
            UpdateCount=0;
          }else{
            EnablePaS=0;
            UpdateCount++;
          }

            //if((_State.Value)&&(CursorDisplayTimer>20))CursorDisplayTimer=75;            //allow hold to freeze cursor value display
            //if((ChartLogic())&&(CursorDisplaySelect<4))CalculateTvernier(1);             //Xpos/Tvernier display moving update 3/sec

            GenTrigColor();                                                              //also monitor generator trig mode 3/sec

            if(_Kind == PWM)LimitTransfer=((D_Tab[_Frqn].PSC+1)*D_Tab[_Frqn].ARR)/2;     //bailout calculations for sweep wave ending functions
              else LimitTransfer=A_Tab[_Frqn]*(TIM7->PSC+1)*125;     

            UpdateBigMeters(); 	

            if((FlagMeter!=1)&&(EnableMeterCalc)){                                         //allow to get frequency for detector mode display with meters off
              DETflag=1;
              Display_Value(FRQ);
              DETflag=0;
            }

            if (((FlagMeter==2)||((FlagMeter!=1)&&(EnableMeterCalc)))||((DownConvertMode)&&((DownConvertInitiate!=1)||(FFTt1Mode)))){
              if(((__Get(FIFO_START)!=0)||((_Mode!=NORH)&&(_Mode!=NORHLD)))||((DownConvertMode)&&(FFTt1Mode))){
                if((_T_base > 10)||((_T_base>8)&&(FastMode==1))){
                  if((DownConvertMode)&&(DownConvertInitiate==0))Display_Value(0);
                  ResetSum();
                }else{
                  SumResetFlag=1;		//in slow mode wait until frame is completed before resetting
                  if((__Get(FIFO_START)==0)&&(DownConvertMode)                           //allows FFT1 mode to work without a triggered signal 
                  &&(DownConvertInitiate==0)&&(FFTt1Mode))Display_Value(0);
                 } 
                WaitForTrigFlag=0; 
              }else WaitForTrigFlag=1;
            }

            if ((FlagMeter==2)&&(UpdateMeter==4)){			    
              FlagMeter=1;  		       //display value on small meter
              Display_Value(0);
              FlagMeter=2;                   //return
            } 

   }
//============================================================================================================================
 
      if (FlagMeter>0){
        Display_Meter();
      }
      Display_Title();
							    //1-Twink, INV, OR PRN
    if((Current != FILE)&&(NotificationTimer>4))Print_Str(95,0,0x0405,PRN,Show[MessageNumber].Message);               //show message
    if((Current != FILE)&&(NotificationTimer>0)&&(NotificationTimer<5)) {Update_View_Area(); NotificationTimer=0;}    //end after delay

      if(Update){                                                            // handle button to refresh item
        if((DisableCursorTimer)&&(Update==2))DisableCursorTimer++; 
        UpdateCursorMeter();

	if ((SelectiveUpdate&1)&&(FlagMeter>0))EnableMeter();     
        for(i=0;i<4;i++){
 	  if (TrgAuto==i)  Print_Str(365,  216, ((SCRN<<8)+Title[TRIGG][SOURCE].Value), i,(char*)AutoStr[i]);
        }  
        if ((SelectiveUpdate & 0x02)==0)Update_Range();	             	         //these 4 are exclusions, bit set prevents update
        if ((SelectiveUpdate & 0x04)==0)Update_Base();
        if ((SelectiveUpdate & 0x08)==0)Update_Output();
        if ((SelectiveUpdate & 0x10)==0){if(Title[TRIGG][SOURCE].Value==4) Update_Trig(0); else Update_Trig(1);}
        UpdateMarkLogic();
        BackLight(0);

	if (SelectiveUpdate & 0x20){
          ClearHoldFlag|=1;				        //transfer clear hold calls as selectiveupdate gets reset 
          if(_T_base<11)SlowModeSkip=2;				//slowmodeskip allows x frames to finish to properly update buffer
        }
	if (SelectiveUpdate & 0x40){
          ClearHoldFlag|=2;
          if(_T_base<11)SlowModeSkip=2;
        }		

        Tmp=InvertA;
        TmpVT=InvertB;
        InvertA=0;InvertB=0;
        if((!UartLogic())&&(!i2cLogic())&&(!SpiLogic())&&(_Mode!=X_Y)){
          if((Title[0][SOURCE].Value==2)||(Title[0][SOURCE].Value==4))InvertA=1;
          if((Title[1][SOURCE].Value==2)||(Title[1][SOURCE].Value==4))InvertB=1;
        }

        if((Tmp!=InvertA)||(TmpVT!=InvertB)){if(Title[TRIGG][SOURCE].Value==4) Update_Trig(0); else Update_Trig(1);}

	if (Current<4)Title[Current][3].Flag|= UPDAT;
        if((Current == TRIGG) && (_Det==3)){
         if ((FrameMode==0)||((Options&1)==0)||(_Mode==AUTO)||(OsBufferLogicFlag)||(_Mode==X_Y)||(_Mode==SGL)||(_Mode==SCAN)||(_T_base>9)) _Det=0; 
         Title[Current][_Det].Flag |= UPDAT;
        }
											
        if((Current != FILE)&&(NotificationTimer<43))   Update_View_Area();	//after 3/4 second, allow notification overwrite if button pushed														
		if (FlagMeter>0) {
			_D_V_Source.Flag |= UPDAT;            // Updat delta V 
			_Delta_V.Flag    |= UPDAT; 
			_Delta_T.Flag    |= UPDAT;            // Updat delta T 
		}else{
              if ((Current == V_VERNIE)&&(_Det == 2)) _Det=0;
              if ((Current == T_VERNIE)&&(_Det == 2)) _Det=0;
            }
 
        if (FrameMode==2) SaveShortBuffXpos=XposRef;								//save trig cursor in config file  
        if(Update)Update--;                           // Update finish
	SelectiveUpdate=0;
        UpdateScale=1;
        OsBufferLogicFlag=OSBufferLogic();
        
        if ((Current==FILE)&&(_Det==DIR)){
          if(Title[9][2].Value==7){
            Ext[0]='B';Ext[1]='I';Ext[2]='N';Ext[3]=0;  
          }else{
            for(i=0;i<3;i++)Ext[i]=F_EXT[Title[9][2].Value][i+1];
          }

          if(ClearDir){
            PrintDir(0,0,3);                                          //clear file list when changing in SPEC or MAP modes
            ClearDir=0;                                  
          }
          ReadDir((char*)Ext);
          if(Label[Title[9][3].Value][0]==0){if(Title[9][3].Value>0)Title[9][3].Value--;}    //if presently selected last file non existant
          if(Label[0][0]==0)for(i=0;i<13;i++)Label[0][i]=NoneStr[i];                         //no files found
          if(DirRange>0){if(Label[14][0]==0)for(i=0;i<13;i++)Label[14][i]=NoneStr[i];}       //no more files found
          PrintDir(0,0,2);
        }
        if((BufferRestore)&&(Title[RUNNING][STATE].Value==0)){UpdateFileMenu();BufferRestore=0;}
      }
 
      if((FlagMeter==0)&&(ClearMeterAreaFlag)){Clear_Meter_Area();Update=1;} ClearMeterAreaFlag=0;

      if(_T_base > 10){
        if(UpdateBackground==3)UpdateBackground=0;
        if(UpdateBackground==1)UpdateBackground=3;                            
      }

    }//else, if power down

    if((_State.Value == HOLD)&&((__Get(FIFO_FULL)!= 0)||(__Get(FIFO_START)== 0))){
        _State.Value = 2; 
        _State.Flag |= UPDAT;
    }

    if((ChartLogic())&&(FrameMode==0)){  
      if(JumpCnt<=380)ScrollFlag=2;
    }

    //--------------------------------------------------------------------------  TRIGGER AUTOMATICO 
    if((TrgAuto>0)&&((_Mode!=SCAN)||(AutoSetFlag))){                                                                   

      if((Current == TRIGG) && (_Det==2)){
        if (((_Mode==NORH)||(_Mode==NORHLD)||(_Mode==NORC))&&(_T_base < 10)&&(Options&1)&&(!OsBufferLogicFlag)) _Det=3; else _Det=0;
        Title[Current][_Det].Flag |= UPDAT;  
      }

      if ((Title[TRIGG][SOURCE].Value == TRACK1)||((Title[TRIGG][SOURCE].Value==4)&&(TrigSourceEnable==0))){
        TmpVT=(((((aT_Min+Ka1[_A_Range])-_1_posi)*Ka2[_A_Range])/1024)+((aT_Max-aT_Min)*(TrgAuto*2)/8))+_1_posi;

        if ((TmpVT>Vt1Old +5) || (TmpVT<Vt1Old -5) || (AutoTrigIni) ||(AutoSetFlag) ){
          _Vt1=TmpVT;
          if(Title[TRIGG][SOURCE].Value==4) Update_Trig(0); else Update_Trig(1);
          UpdateMarkLogic();
          Vt1Old=_Vt1;
          AutoTrigIni=0;
        } 
      }
      if ((Title[TRIGG][SOURCE].Value == TRACK2)||((Title[TRIGG][SOURCE].Value==4)&&(TrigSourceEnable==1))){
        TmpVT=(((((bT_Min+Kb1[_B_Range])-_2_posi)*Kb2[_B_Range])/1024)+((bT_Max-bT_Min)*(TrgAuto*2)/8))+_2_posi;

        if ((TmpVT>Vt2Old +5) || (TmpVT<Vt2Old -5) || (AutoTrigIni) ||(AutoSetFlag) ){
          _Vt2=TmpVT;
          if(Title[TRIGG][SOURCE].Value==4) Update_Trig(0); else Update_Trig(1);
          UpdateMarkLogic();
          Vt2Old=_Vt2;
          AutoTrigIni=0;
        } 
      }
    }

    //--------------------------------------------------------------------------  FRAME MODE

    UpdateFrameMode();
    if(Update==1)SelectiveUpdate|=0x08;                 //anything setting update up to this point has nothing to do with gen output
    if(RefreshDisplay){                                         //update display when changing buffer size while not triggered un uart mode
      _State.Value=1;						//shift to hold mode so data is not transfered		
      Process();					        //update screen so x pos can be changed
      _State.Value=0;						//return to run mode
      RefreshDisplay=0;
    } 

   //--------------------------------------------------------------------------  GESTIONE TASTI MENU	
    if(Key_Buffer) { 
      if(PD_Cnt == 0) {
	 App_init(1);            // exit the power saving state
         Key_Buffer=0;		// don't execute any functions when just getting device out of standby with any key press
         Update_View_Area();    // in case file menu display was showing 
         DisableCursorTimer=3;

         if(ChartLogic())InitChart();

      }

      PD_Cnt = 600;                         // 600 seconds
   //--------------------------------------------------------------------------------------------------------------

	   for (i=0;i<4;i++){						//clear memorized trigger settings of ch if ch is turned off
 	     if (Title[i][SOURCE].Value == HIDE)TrigMemory(i);  
	   }

   //---------------------------------------------------------------------------------------------------------------------

      if(Key_Buffer == KEY1){

      DisableCursorTimer=1;
      Delay_Cnt = 1500;

      if(AutoSetFlag>1){                                                                        //bail out of autoset
        AutoSetFlag=0;
        AutoSetTimer=0;                
        reset_parameter();
        if (_Mode!=SCAN) {
          if(FlagFrameMode==0)XposRef=GetXposRef(OldPosi);	
          if(FlagFrameMode==1)XposRef=ShortBuffXpos;
          _X_posi.Value=GetXpos(XposRef);
        }
        CurDefTime=OldCurDefTime;
        if(MeterStatus){
          FlagMeter=MeterStatus;
          EnableMeter();
        }
        App_init(0);
        Update_Range();
        Update_Base();
        goto bypasslongpress;
      }

      if(AutoSetTimer)AutoSetFlag=1;
      AutoSetTimer=25;                                                                          //x20mS double click timer

    while (Delay_Cnt > 1000)
    {
       if((__Get(KEY_STATUS)& KEY1_STATUS)!=0){
        _State.Flag |= UPDAT;                          						// set the corresponding update flag
        if((_State.Value==0)&&(AutoSetFlag==0)){                                                //0=run 1=hold also has 2
           ClearMeters();                                                                       //clears meters if untriggered in NORH
          _State.Value=1;  
        }else{
          DigChLockout=0;
          _State.Value=0;

          if(ChartLogic())InitChart();else{
            SelectiveUpdate |=0x60;								   			
            if(_Mode!=NORH)ClearTrackBuff(1);else ClearLeadingEdge=1;                           //REMOVING NORH CONDITION WOULD CLEAR WAVE ALONG WITH METERS...
            if((   (_Mode == SGL)||(_Mode==NORH)||(Options&4)||((FrameMode>0)&&(_T_base<3))||((FrameMode==0)&&(_T_base<6)))   &&   (_Mode!=NORHLD)    ){    //in run mode if frame >1 1/2 sec
              if(!(ChartLogic())){__Set(FIFO_CLR, W_PTR);cleardatabuf(2);}                                   
              if(Options&4) {ClearHold(3);ClearMinMax(3);}
              if(OsBufferLogicFlag)InitiateOSBuffers();
              if(_T_base<11)JumpCnt=0;				
              EnablePaS=1;
              ResetSum();
            }
          }

          if((_T_base<3)&&(_Mode!=SCAN)&&(_Mode!=NORHLD)){               
            MessageNumber=11;		      			//wait for reset
            if(OSBufferLogic()){	
              NotificationTimer=(NoteTimer[_T_base]*(150-_X_posi.Value))/150;              
            }else{
              NotificationTimer=NoteTimer[_T_base];              
            }
          }
          if(_Mode==NORHLD){cleardatabuf(2);ClearFFTbuffer();}
          //InitXY=1;						//resets XY persistence if enabled. Using button 5 center press instead
          if(DownConvertMode)DownConvertShiftEnable=2;
        }							//allows to see waveform prior to re-enabling by pressing button again
        goto bypasslongpress;
      }				//if get key status
    }				//while

      Beeper(125); BackLight(1);
      while ((Delay_Cnt >0)&&(Delay_Cnt <=1000)){					// long press
         if(Delay_Cnt<875)BackLight(0);
	 if((__Get(KEY_STATUS)& KEY1_STATUS)!=0){

	 _Curr[_Det].Flag |= UPDAT;

         if(((ListLogic())||(_Mode==X_Y))&&(EditListActive(1))){
           PrintDir(0,0,3);                                                                             //clear file list when leaving
           if(_Mode==X_Y)InitXY=1;
         }

	 switch (Current){	
	   case TRACK1:
             if (_Det==RANGE){

               if(_1_source==0){
                 AlignTbaseSweep(4);
                 _1_source=ChOnStatus[0]; 
                 LastSelected=1;
               } else {
                 AlignTbaseSweep(5);
                 ChOnStatus[0]=_1_source; 
                 _1_source=0;
               }

             }else{
               if(Title[TRACK4][SOURCE].Value<10)EnableChannel(TRACK1);		//if FFT on, do not turn channel trace on
	     }	
	     break;
	   case TRACK2:
             if (_Det==RANGE){

               if(_2_source==0){
                 AlignTbaseSweep(4);
                 _2_source=ChOnStatus[1]; 
                 LastSelected=2;
               } else {
                 AlignTbaseSweep(5);
                 ChOnStatus[1]=_2_source; 
                 _2_source=0;
               }

             }else{
                if(_4_source<10)EnableChannel(TRACK2);
	     }	
             break;
           default:
             if ((Title[TRACK1][SOURCE].Value != HIDE)&&(Title[TRACK2][SOURCE].Value == HIDE)){			//Cha A on B off
               Current = TRACK1;
	       LastSelected=1;
	     }else if ((Title[TRACK1][SOURCE].Value == HIDE)&&(Title[TRACK2][SOURCE].Value != HIDE)){		//Cha B on A off
               Current = TRACK2;
	       LastSelected=2;
	     }else if(_4_source<10){										//if FFT off												//both on or off
               switch (LastSelected){
	         case 0:
	         case 1:
                   Current = TRACK1;
                   EnableChannel(TRACK1);
                   break;
	         case 2:
                   Current = TRACK2;
                   EnableChannel(TRACK2);
               } 					//switch
             }else{											//FFT on, both tracks on or both off
               if(_4_source<14)Current=TRACK2;else Current=TRACK1;					//go to ch v/div but do not turn on	
             } 
           } //switch
           _Det = RANGE;

	 Title[TRACK1][SOURCE].Flag |= UPDAT;	
	 Title[TRACK2][SOURCE].Flag |= UPDAT;
         CurDetailUpdate();
         goto bypasslongpress;
       }   				// if get key status

     }					//while delay count 0 to 1000

          Beeper(60); 
	  if(Delay_Cnt == 0){
             j=0; 
             for(i=204;i>49;i-=11){
                Print_Str(0,i,0x040C,PRN,DisplayChart[j++].ChartLine);
             }

	     Delayms(300);
	     Key_Buffer=0;

	    while (1) {
              if (Key_Buffer>0){
                SelectiveUpdate=1;
		BackGround_Reset(1);                
                Update_View_Area();
                break;
              } 
	    }
	  } 
       
     } 				//if key buffer
      //--------------------------------------------------------------------------------------------------------------
      if(Key_Buffer== KEY2){                                                    // ===--- TASTO 2 PREMUTO ---===   
        Delay_Cnt = 3000;                                                       // Carica il temporizzatore del tasto permuto
        while (Delay_Cnt > 2500){                                                  // Se il tempo non 
          if((__Get(KEY_STATUS)& KEY2_STATUS)!=0){                              // CAMBIO MODO TRIGGER
           DisableCursorTimer=1;
           if ((Title[TRIGG][SOURCE].Value != TRACK3)&&(Title[TRIGG][SOURCE].Value != TRACK4)){
        	_Curr[_Det].Flag |= UPDAT;
            if (TrgAuto==0){
		  TrgAuto=2;
		  Vt1Old=0;
		  Vt2Old=0;
		}else {
             TrgAuto++;					  // AUTO TRIGGER
             }          
            if((Sweep)&&(_Tr_kind==8)&&(TrgAuto==2)&&(_Kind<5))TrgAuto++;                 //auto trig= 1/2 does not work well with kind=GEN
            if (TrgAuto>3) {TrgAuto=1;}							  // short press
            Title[Current][_Det].Flag |= UPDAT;
            if ((Current == TRIGG) && (_Det==2)){ Title[Current][_Det].Flag |= UPDAT;_Det=0;}
           } 
            goto bypasslongpress;
          }//if get key
        }//while

        Beeper(125); BackLight(1);
 
	while ((Delay_Cnt >0)&&(Delay_Cnt <=2500)){					// long press
          if(Delay_Cnt<2375)BackLight(0);
          if((__Get(KEY_STATUS)& KEY2_STATUS)!=0){
           if ((Title[TRIGG][SOURCE].Value != TRACK3)&&(Title[TRIGG][SOURCE].Value != TRACK4)){
         	_Curr[_Det].Flag |= UPDAT;
		if (TrgAuto>0) TrgAuto=0;
		Current = TRIGG;
		_Det=2;
            Title[Current][_Det].Flag |= UPDAT;
           }
          goto bypasslongpress;
          }// if get key status
        } // while delay count

        Beeper(60); 
        if(Delay_Cnt == 0){  

          DisableCursorTimer=1;
          if(Current<2){
            CalibrateMode=1; 
            Calibrat(Current); 
            CalibrateMode=0; 
            goto BypassExclusions;
          }else{

            if(Current!=9){                                                         //not in file menu
              if(OffsetSelect)OffsetSelect=0;else OffsetSelect=1;
              goto bypassOSselect;
            } 
          }    //if current <2 else

        }      // if delay cnt
      }
      //--------------------------------------------------------------------------------------------------------------     
      if(Key_Buffer== KEY3){                                                    // ===--- TASTO 3 PREMUTO --====

     DisableCursorTimer=1;
     if((Current!=FILE)||(_Curr[2].Value!=CFG)){
       Delay_Cnt = 1500;                                                       // Carica il temporizzatore del tasto permuto
       while (Delay_Cnt > 1000){                                                  //WAS >0           Se il tempo non 
          if((__Get(KEY_STATUS)& KEY3_STATUS)!=0){ 
 

          if(DownConvertMode==0){
           if (FlagMeter==2) 			//if in big meters, shut off 
            {
              if (_Mode != X_Y){
                if((ListLogic())&&(EditListActive(1)))PrintDir(0,0,3);
                DisableMeter();
                if((_4_source==SPEC_A)||(_4_source==SPEC_B)||(_Mode==NORHLD))Clear_Meter_Area();
              }else{				//toggle the 2 meters in xy mode, instead of shutting meters off
                FlagMeter=1;
                EnableMeter();
              }
              goto bypasslongpress;
           } 
            else
            {
              if (FlagMeter==1){		//if in small meters go to big meters
                if(Current > METER_3)Current=METER_3;
		if(UpdateMeter==4){
                  if(Current==METER_1)Current=METER_2;
                }else{
                  if(Current==METER_0)Current=METER_1;
                  if((Current < METER_0)&&(Current>T_VERNIE))Current=T_VERNIE;  //if not in meter item select mode
                }  
                FlagMeter=2;
              }
              //TimedDeltaView=0;
              if( (FlagMeter==0)&&(ListLogic())&&(EditListActive(1)) )PrintDir(0,0,3);
              EnableMeter();			//if meters off, go to small meters    
              goto bypasslongpress;
            }
           }else goto bypasslongpress; 
              
          } //if get key status
       } //while delay count

       Beeper(125); BackLight(1);
 
        while ((Delay_Cnt >0)&&(Delay_Cnt <=1000)){					// long press
          if(Delay_Cnt<875)BackLight(0);

          if((__Get(KEY_STATUS)& KEY3_STATUS)!=0){

            if(Title[OUTPUT][0].Value!=6){                                                    //if generator is not turned off 
              Print_Str(15, 90, 0x0405, PRN, (char*)DisableSTR);
              Delayms(900);
              Key_Buffer=0;
              __Set(BACKLIGHT, 4);                                                            // set backlight dim
              while(1){if(Key_Buffer>0)break;}
              InitXY=1;
              Key_Buffer=0;
              BackLight(0);
              PD_Cnt = 600;
              goto BypassUpdate;
            }else{  
              Print_Str(119, 90, 0x0405, PRN,   "LONG PRESS-STANDBY");
              Delayms(900);
              PD_Cnt = 0;               
              __Set(BACKLIGHT, 0);                    // turn off the backlight
              __Set(STANDBY, EN);                     // enter low power states
               goto bypasslongpress;		
            }

           }  //IF GET KEY STATUS
          }  // WHILE DELAY COUNT...
    
        Beeper(60); 
        if(Delay_Cnt == 0){			    //extra long press
          for(i=0;i<3;i++)_Curr[i].Flag |= UPDAT;
          SaveCurrent[0]=Current;		      //save current menu item position
          SaveCurrent[1]=_Det; 
          Current=FILE;
          Title[FILE][0].Value=0;		      //save file
          Title[FILE][2].Value=4;		      //conf file
          _Curr[1].Value=ConfigFileNumber;            	            

          if(ListOverride){
            _Det=3;
            ProcessEditName();
          }else _Det=1;			
         
          for(i=0;i<3;i++)Title[FILE][i].Flag|=UPDAT;

        } 
      }else{					      //Current = file	

        if(_Curr[2].Value==CFG){  
          if(_Curr[0].Value == SAVE){
            if(_Det==3){
              Edited=1;
              ProcessFileName();
              for(i=0;i<8;i++)LastAccessedConfig[i]=SelectedFileName[i]; 
              ProcessEditName();                        //loads processed name into edit function
              ListOverride=1;
            }else ListOverride=0;

            i=ConfigFile(SAVE);
            Edited=0; 
            FileMessage(i);  
          }else{
            if ((Current==FILE)&&(_Det==DIR)){
              for(j=0;j<8;j++)LastAccessedConfig[j]=SelectedFileName[j]=Label[Title[9][3].Value][j];
            }
            j=Title[FILE][3].Value;                       //keep present hilighted list position
            i=ConfigFile(LOAD);
            if(ListOverride){                             //keep list after loading new config from it
              Title[FILE][3].Value=j;
              Detail[9]=3;             
              ProcessEditName();                        //loads processed name into edit function
            }
            for(j=0;j<3;j++)Title[FILE][j].Flag|=UPDAT;
	  } 
 
       }
      } 

     } //if key buffer

      //-------------------------------------------------------------------------------------------------------------        
                                                    // ===--- TASTO 4 PREMUTO --====
						//Key 4 short press has been swapped with center press left toggle
if(Key_Buffer== KEY4){

    Delay_Cnt = 1500;
    while (Delay_Cnt > 1000)
    {
       if((__Get(KEY_STATUS)& KEY4_STATUS)!=0){

	    _Curr[_Det].Flag |= UPDAT;

            if((Current==FILE)&&(_Det==DIR)){
              if(ListLogic())PrintDir(0,0,3);                                                  //clear file list when leaving
              Title[FILE][1].Flag |=UPDAT;
              if((_Mode==X_Y)&&(Title[9][0].Value==1))InitXY=1;
            }

            if((Current==OUTPUT)&&((_Kind==5)||(_Kind==7)||(_Kind==8))){
              if(_Det==0)_Det=1;else _Det=0; 
              goto BypassUpdate;
            }

            if((Current==OUTPUT)&&(_Kind==6)){_Det=0;goto BypassUpdate;}

            if ((Current == TRACK3)&&(_Det == 0))_Det = 2;

            if ((Current == TRACK4)&&(_Det == 0)){
              if(_4_source>11)_Det=1;else _Det=2;                                        //in fft modes, to engage MAG modes  
              if(_4_source>10)goto bypasslongpress;                                      //in fft modes, 11=spec
            }else if ((Current == TRACK4)&&(_Det == 2)&&(_4_source>9)){                  //in map mode
              _Det = 0;
              goto bypasslongpress;
            }

           if(Current==6){if(_Mode==X_Y){if(_Det==1)_Det=2;}else if((_Det==2)&&(_Mode!=NORHLD))_Det=3;}


     	   if ((((Options&1)==0)||(_Mode==AUTO)||(_Mode==X_Y)||(OsBufferLogicFlag)||(_Mode==SGL)||
             (_Mode==SCAN)||(_T_base>9))&&(Current == TRIGG)&&(_Det==2))_Det=3;	//skip over det3 (will go to 0)


                      if((FlagMeter==0)&&((Current==V_VERNIE)||(Current==T_VERNIE))&&(_Det==V2)){ //skip meter select if meters off
                        _Curr[_Det].Flag |= UPDAT;
                        _Det=0;                                                                    //go to V1 or T1 = 0
                      }else{
			if(Current < METER_0){                                                     // change the Detail
			  _Curr[_Det].Flag |= UPDAT;
			  if(_Det < 3)    _Det += 1;
			  else            _Det  = 0;
                          if(Current == OUTPUT){if(Title[OUTPUT][KIND].Value==PWM){if(_Det==OUTATT)_Det=0;}else if(_Det==DUTYPWM)_Det+=1;} 
			  if(_Curr[_Det].MARK & NOT)                        _Det  = 0; 
			  if(_Curr[_Det].MARK & NUM2)                       _Det  = 0; 
			  if((_Curr[_Det].MARK & NUM3)&&(Current != FILE)&& (Current != OUTPUT) )  _Det  = 0;  

                          if((Current==FILE)&&(_Det==3)){
                            if((_Curr[2].Value==7)&&(Title[FILE][0].Value==0))_Det=0;              //do not edit ROM backup file 
                          }

 			  _Curr[_Det].Flag |= BLINK;
			} else {                                                                   // change the measurement object

                          if(Meter[Current-METER_0].Item>1){                     //don't change source for VBT AND FPS (items 0 and 1) 		
			  Meter[Current-METER_0].Flag |= UPDAT;

			  if((Meter[Current-METER_0].Item>VPP)||(Meter[Current-METER_0].Item==FRQ)){
			    if(Meter[Current-METER_0].Track <=  TRACK4) 
                            Meter[Current-METER_0].Track += 1;
			    if(Meter[Current-METER_0].Track > TRACK4) 
                            Meter[Current-METER_0].Track  = TRACK1;
                          }else{
                            if(Meter[Current-METER_0].Track==TRACK1)
                              Meter[Current-METER_0].Track=TRACK2;
                                else Meter[Current-METER_0].Track=TRACK1;
                          }   
                        } 
		      }//if current, else (if in meter section)
                    }

            if(EditListActive(0)){                                                      //filename edit 
              CharValue=EditChar[CharIndex][0];
            } 


                goto bypasslongpress;
          }                     //if get key status
        }                      //while delay count >0

        Beeper(125); BackLight(1);
 
         while ((Delay_Cnt >0)&&(Delay_Cnt <=1000)){					// long press
           if(Delay_Cnt<875)BackLight(0);
           if((__Get(KEY_STATUS)& KEY4_STATUS)!=0){
             _Curr[_Det].Flag |= UPDAT;


             if((_4_source>11)&&((DownConvertMode)||(FFTt1Mode))){
               if((Current==3)&&(_Det==1)){
                 Current=10;
                 _Det=0;
                 goto bypasslongpress;
               }else if((Current==10)&&(_Det==0)&&(Detail[3]==1)){
                 Current=3;
                 _Det=1;
                 goto bypasslongpress;
               } 
             }

             if(((ListLogic())||(_Mode==X_Y))&&(EditListActive(1))){
               PrintDir(0,0,3);                                                        //clear file list when leaving
               if(_Mode==X_Y)InitXY=1;
             }
             if (Current==TRACK1)LastSelected=1;
             if (Current==TRACK2)LastSelected=2;
             Current = T_BASE;
             _Det = BASE;
             CurDetailUpdate();
             goto bypasslongpress;		
           }  //if key status
         }   //while delay cnt

        Beeper(60); 
        if(Delay_Cnt == 0){
          DisableCursorTimer=1;

          if (CurDefTime==0){
            CurDefTime=1;
            ClearMeters();
            if(Current != FILE){
                MessageHandler(5);			//cur select meters on
            }
      	    Print_Str(153,216,0x0405,PRN, "X");
          }else{
            CurDefTime=0;
            if(Current != FILE){
                MessageHandler(6);			//cur select meters off
            }
            Print_Str(153,216,0x0405,PRN, " ");
          }
        }   //if delay cnt
      }	  //if key buffer

//----------------------------------------------------------------------------------------------------------------
      if(Key_Buffer == K_INDEX_DEC){                                            // ===--- TASTO 5 --- PREMUTO --====

         if((((_Mode==SCAN)&&(!ChartLogic()))||((ChartLogic())&&(FrameMode>0)))
           &&(Current==T_BASE)&&(_Det==2))goto BypassUpdate;                      //bypass changing Xpos in chart mode short buffers

         if((_4_source>11)&&(Current==3)&&(_Det==1)){
           if(DownConvertMode){
             if(DownConvert==5)DownConvert=2;
             else if(DownConvert==50)DownConvert=20;
             else if(DownConvert==500)DownConvert=200;
             else if(DownConvert>1)DownConvert/=2;
             DownConvertShiftEnable=1;
           }else if(FFTt1Mode){
             if(_T1>0)_T1--;
             if(_T1>2)_T1-=CursorSpeedLogic(1);
           }
           goto BypassUpdate;
         }

         if((DownConvertMode)&&(Current==T_BASE)&&(_Det==BASE))goto BypassUpdate;

         if((OffsetSelect==1)&&(Current>1)){
           if(ADCoffset>0)ADCoffset--;
           Update_Range();
           goto bypasslongpress;
         }

         if((OffsetSelect==2)&&(Current>1)){
           if(PPM_Comp>-999)PPM_Comp--;
           goto bypasslongpress;
         }

         if(EditListActive(1)){                                                      //file directory read 
           if(Title[9][3].Value>0)Title[9][3].Value--;else
             {if(DirRange>0)DirRange--;}   
           goto bypasslongpress;
         }

         if(EditListActive(0)){                                                      //filename edit 
           if((CharValue>65)&&(CharValue<91))CharValue--;                             //letters                      
           else if((CharValue>47)&&(CharValue<57))CharValue++;
           else switch(CharValue){
                  case 65: CharValue=32;break;
                  case 32: CharValue=48;break;
                  case 63: CharValue=48;break;                                        //63= non available char flag   
                  case 57: CharValue=95;break;
                  case 95: CharValue=45;break;
                  case 45: CharValue=41;break;
                  case 41: CharValue=40;break;
                  case 40: CharValue=38;break;
                  case 38: CharValue=37;break;
                }

           EditChar[CharIndex][0]=CharValue;
           UpdateEditBox(0);
           goto bypasslongpress;
         }

         if((Current==T_BASE)&&(_Det==BASE)&&(ChartLogic())&&(AutoSaveSelect)){
           if(AutoSaveSelect==1){if(AutoSaveBuf>0)AutoSaveBuf--;}
           if(AutoSaveSelect==2)ChartMode=0;
           goto BypassUpdate;
         }

         if (_Mode == X_Y){
           if((Current == TRACK1)&&(_Det == POSI) && (OffsetX>45)) OffsetX=OffsetX-2;
           if((Current == TRACK2)&&(_Det == POSI) && (OffsetY>30)) OffsetY=OffsetY-2;
         }  
         SelUpdate(POSI);							                           //clear min/max hold

        if(((Current==TRACK1)||(Current==TRACK2))&&(_Det==1))AlignTbaseSweep(0);                           //changing coupling mode                      

        if((Current==5)&&(_Det==1)&&(_Kind==8)){                                //uart gen mode
          if(GenUartAdj==0){                                                    //adjust baud index, translate to baud rate
            if(GenBaudAdjSpeed){                                                //fast mode
              if(GenBaudIndex<800){                                             //set up break points for fast adj mode
                GenBaudIndex+=20;
              }else if(GenBaudIndex<2800){
                GenBaudIndex+=50;
              }else if(GenBaudIndex<12970){
                GenBaudIndex+=100;
              }else if(GenBaudIndex<14970){
                GenBaudIndex+=50;
              }else{
                if(GenBaudIndex<15851)GenBaudIndex+=20;else GenBaudIndex=15871; 
              }  
            }else{if(GenBaudIndex<15871)GenBaudIndex++;}                       //fine mode 
          }
          if(GenUartAdj==1)if(GenUartMode>1)GenUartMode--;
          if(GenUartAdj==2)GenUartStopBits=0;
          if(GenUartAdj==3)GenUartCont=0;
          goto bypasslongpress;
        }
        
        if((Current==BK_LIGHT)&&(HboldAdjust)){
          if(Hbold<2)Hbold++; 
          goto bypasslongpress;
        } 

        if((Current==BK_LIGHT)&&(FastDimAdjust)){
          if(FastDim>0)FastDim--; 
          goto bypasslongpress;
        } 

        if((Current==VOLUME)&&(AdjBeepLevel)){
          if(LoBeepLevel<9)LoBeepLevel++; 
          goto bypasslongpress;
        }

        if((T1Start)&&(Current==T_VERNIE)&&((_Det==T1)||(_Det==T2))&&(Title[TRACK1][COUPLE].Value>2)&&(Title[TRACK1][COUPLE].Value<5)){         //in serial decode, tie T1 to T2
          if((_T1>0)&&(_T2>0)){_T1--;_T2--;}         
          goto bypasslongpress;
        }

        if((SpiLogic())&&(Current==TRACK1)&&(_Det==1)&&(SpiAdj)){
          if(SpiAdj==1){
            if(SpiMode>0)SpiMode--;
          }          
          if(SpiAdj==2)SpiBitOrder=1;
          if(SpiAdj==3){
            if(SpiNumBits==2)SpiNumBits=0; else {
              if(SpiNumBits==0)SpiNumBits=-3; else if(SpiNumBits>(-145))SpiNumBits--;
            }
          }          
          goto BypassUpdate;
        }

        if((UartLogic())&&(Current==TRACK1)&&(_Det==1)&&(DataAdj==1)){
          if(DataSize>5){
            if(DataSize==15)DataSize=9;else DataSize--;
          }          
          goto BypassUpdate;
        }

        if((Current==OUTPUT)&&(_Det==0)&&(_Kind==0)){                                           //clear buffer before changing to Arbt
          for(i=0;i<720;i++)ATT_DATA[i]=2048;
        }

        if((Current==OUTPUT)&&(_Det==1)&&(GenAdjustMode==1)&&((_Kind<5)||(_Kind==7))){          //changing sweep or burst rate

         if(_Kind==7){ 
          if(GenFreqShift<100)GenFreqShift++;
          OutputAdjust();
         }else{
          if((Sweep==1)||(Sweep==2)){
            if(SweepIndex>0)SweepIndex--;
            TIM_2IRQControl();
            AlignTbaseSweep(2);           
            if(Title[TRIGG][SOURCE].Value==4) Update_Trig(0); else Update_Trig(1);
          }else if(Sweep==3){

            if((SweepIndex<2)&&(_Frqn>5)){
              if(FineAdjust>0)FineAdjust--;else BurstAdjust(0);
            }else{if(_Frqn<14)BurstAdjust(0);else{if(FineAdjust>0)FineAdjust--;else BurstAdjust(0);}}

            TIM_2IRQControl();           
            if(Title[TRIGG][SOURCE].Value==4) Update_Trig(0); else Update_Trig(1);
          }else{
            if(GenFreqShift<100)GenFreqShift++;
            OutputAdjust();
          }
         }

         goto BypassUpdate;
        }

        //if ( ((Current == T_VERNIE)||(Current == V_VERNIE)) && (FlagMeter==0)) TimedDeltaView=150;

        if(Current < METER_0){

          for (i=0;i<4;i++){
	      if ((Current==i)&&(_Det == SOURCE)){		         			//ch selected and submenu on source
		  if (Title[i][SOURCE].Value == 1){						//about to be shut down
		    if (Title[TRIGG][SOURCE].Value ==i){				        //if trig selected
		      UpdateTriggerMem();
                      TriggFlagUpdate();
 	              Update_Trig(1);						
		    }							  //if not selected, about to be shut down, purge # from buffer
                    TrigMemory(i);  
                  }  //Title[i][source]  
              }      //current=
          }          //for

          if((Current == TRIGG)&&(Detail[Current]==2)){     						        // trigger level adjustment

            j=1;if(SetLowVolume){j=2;if(FlagFrameMode==0)j++;}
            for(i=0;i<j;i++){          
              if(Title[TRIGG][SOURCE].Value==4){  
                if(V_Trigg[0].Value > 0)V_Trigg[0].Value--; 		         
                if(V_Trigg[1].Value > 0)V_Trigg[1].Value--; 		         
              }else{
                if(V_Trigg[_Trigg[SOURCE].Value].Value > 0) 
                V_Trigg[_Trigg[SOURCE].Value].Value--;
              }  
            }

          }	else  if ((Current == TRIGG)&&(_Det == 3)){								  //trigger holdoff
            if ((FrameMode==2)&&(_T_base < 10)){
              if (Title[7][3].Value>30) Title[7][3].Value-=30; else Title[7][3].Value=0;
              Title[7][3].XPOS=GetHoldoffIndPos();
              SelectiveUpdate=0x1E;										   //do not update anything
            }  
          } else if((Current == BK_LIGHT)||(Current == VOLUME)){//  adjust backlight or volume
            if(_Curr[1].Value > 0)   _Curr[1].Value--;
          } else if((Current == T_BASE)&&(_Det == XPOSI)){   							   // X_POSI adjustment
            ClearMeters();                                                                                         //clears meters if untriggered in NORH

              _X_View.Flag |= UPDAT;                              // refresh X_View
              if (XposRef>150){
                if(SetLowVolume){
                  if(Key_Mid_Speed)_Curr[_Det].Value-=6;else _Curr[_Det].Value-=30;
                }else _Curr[_Det].Value--;

              }else if(_Curr[_Det].Value>0){
                if(SetLowVolume){
                  if(XposRef>30){
                    XposRef-=4;
                    goto bypasslongpress; 
                  } else _Curr[_Det].Value--;
                }else _Curr[_Det].Value--;

              }
              XposRef=GetXposRef(_Curr[_Det].Value);								

          } else if((Current==OUTPUT)&&(_Det==2)){               // PWM duty% adjust
             if(PWAdjustMode){ 
               PWMdutyControl(1);  
               goto BypassUpdate;
             }else {if(_Curr[_Det].Value>300)_Curr[_Det].Value-=300;else _Curr[_Det].Value=0;}
          } else {                                              //  for other current item
		if ((Current==TRACK1)&&(_Det==SOURCE)){

                  if(_1_source==0){                                                              //ch A about to be turned on, else off
                    LastSelected=1;  
                    SelectiveUpdate|=0x20;       
                    AlignTbaseSweep(4);
                  }else if(_1_source==1)AlignTbaseSweep(5);   
                }
		if ((Current==TRACK2)&&(_Det==SOURCE)){

                  if(_2_source==0){                                                              //ch B about to be turned on, else off
                    LastSelected=2;  
                    SelectiveUpdate|=0x40;       
                    AlignTbaseSweep(4);
                  }else if(_2_source==1)AlignTbaseSweep(5);   
                }

                if((_Mode==NORHLD)&&(Current==6)&&(_Det==3)){
                  if(PerstFrameNumber>0)PerstFrameNumber-=((PerstFrameNumber/65)+1);
                }else{
                  if ((Current == T_BASE)&&(_Det == BASE)){
                    if(_Curr[_Det].Value==0){
                      if((_Mode==SCAN)&&(SubIndex<13)){
                        SubIndex++;
                        if(SubIndex==1)InitChart();
                      }

                   }else if(_Curr[_Det].Value>0)_Curr[_Det].Value--;
                 }else{

            if(_Curr[_Det].Value > 0){
              _Curr[_Det].Value--;
              if((Current==6)&&(_Det==3))_Curr[_Det].Value-=(_Curr[_Det].Value/65); 	//establish 3 breakpoints for speed change
              SelUpdate(RANGE); 
              if ((Current<2)&&(_Det==RANGE))Update_Range(); 
            } 
            else if(_Curr[_Det].MARK & CIRC) _Curr[_Det].Value =_Curr[_Det].Limit;
       
            if(((Current==0)||(Current==1))&&(_Det==0)&&(_Mode==X_Y)){
              if(Title[Current][0].Value>0)Title[Current][0].Value--;
            }

           } 
          }

            if((Current == TRIGG)&&(Detail[Current]==0)){                       //trig source has just been changed
              if(Title[TRIGG][SOURCE].Value==4){				//changed to AltAB 
                if(_Mode==X_Y){
                  _Curr[_Det].Value--;
                }else{                
                  OldMode=Title[T_BASE][MODE].Value;							//save prev mode	
                  ChannelStatus(0);						//save status 
                } 
              } 
              if((Title[TRIGG][SOURCE].Value==3)&&(_Mode!=X_Y)){				//changed from AltAB 
                Title[T_BASE][MODE].Value=OldMode;							//reset to prev mode	
                Title[T_BASE][MODE].Flag |= UPDAT;
                ChannelStatus(1);						//save status 
                Title[TRACK1][RANGE].Flag |= UPDAT; Title[TRACK2][RANGE].Flag |= UPDAT;
              } 
              Title[T_BASE][BASE].Flag |= UPDAT; 				
            }

            if ((Current == T_BASE)&&(_Det == BASE)){
              if(Sweep){if(Title[TRIGG][SOURCE].Value==4) Update_Trig(0); else Update_Trig(1);}
              if(!OSBufferLogic()){if((__Get(FIFO_START)!=0)||((_Mode!=NORH)&&(_Mode!=NORHLD)))__Set(TRIGG_MODE, UNCONDITION);} //temporarily setting this allows x_attr to properly update if in untriggered condition
            }
	    if((FrameMode<2)&&(Current == T_BASE)&&(_Det == BASE)){                 
              Tmp=GetXposRef(_X_posi.Value);
              if (Tmp>150) XposRef=Tmp;					//only align trig vernier when on screen when changing time bases
	    }  
          }

          if((Current==OUTPUT)&&(_Det==3)&&(Sweep)){
            if(FrameEndFlag)WaveGen();                     
            goto BypassUpdate;
          }

          if((Current==OUTPUT)&&(_Det==1))PWMscaleReset(1);

          if((Current == T_BASE)&&(_Det == MODE)){     // T_BASE MODE selection			//at this point mode has been changed
            if (_Mode==SCAN) {if (FrameMode>0)ShortBuffXpos=XposRef; else OldPosi=GetXpos(XposRef);}	//previous mode was auto
            if (_Mode==X_Y) {if (FrameMode>0)XposRef=ShortBuffXpos; else XposRef=GetXposRef(OldPosi);}	//previous mode was scan

           Title[RUNNING][STATE].Value = RUN;         // STATE = RUNNING 
            Title[RUNNING][STATE].Flag |= UPDAT;       // refresh RUNNING STATE
          }
          if((Current == OUTPUT)&&(_Kind<4)){
             if (Title[OUTPUT][FRQN].Value > 16) 
            Title[OUTPUT][FRQN].Value = 16;            // upper limit of the analog signal frequency is 20KHz
           }

          if(Current == FILE){                                          //changing file extensions
            if(_Curr[0].Value == 0){                                                 //SAVE (edit filename) 
              if((_Curr[2].Value == 5)||(_Curr[2].Value == 6)){_Curr[2].Value = 4;ResetEditList();}
            }
          }

          if((Current == FILE)&&(_Curr[2].Value == CFG)) if(_Curr[1].Value > 9)_Curr[1].Value=9;           
          if((Current==FILE)&&(_Det==2))ResetEditList();             //reset list and edit defaults when changing file types
          for(i=0;i<4;i++)_Curr[i].Flag |= UPDAT;
        } else {

          Meter[Current-METER_0].Flag |= UPDAT;

             if(Meter[Current-METER_0].Item >VBT)	 	   
               Meter[Current-METER_0].Item -= 1;          
             else                     
               Meter[Current-METER_0].Item  = TL;
 
             if((Meter[Current-METER_0].Track>1)&&(Meter[Current-METER_0].Track<4)){
               if(Meter[Current-METER_0].Item ==VPP)Meter[Current-METER_0].Item =FRQ;
               if(Meter[Current-METER_0].Item ==RMS)Meter[Current-METER_0].Item =FPS;
             }
 
             if(Meter[Current-METER_0].Item == FPS){
               SaveCurrent[0]=Current;		      
               SaveCurrent[1]=Meter[Current-METER_0].Track; 
               Meter[Current-METER_0].Track = 4;
	     } 	

             if(Meter[Current-METER_0].Item == TL){ 
               if(Current==SaveCurrent[0])
		 Meter[Current-METER_0].Track =SaveCurrent[1];
               else
                 Meter[Current-METER_0].Track = UpdateMeter;
             }

        }
        if(_Curr[_Det].Value>2)_Curr[_Det].Value-=(CursorSpeedLogic(0));
      }
      //---------------------------------------------------------------------------------------------------------------------       
      if(Key_Buffer == K_INDEX_S)
	{                                              // ===--- TASTO 5 centrale PREMUTO --====
						//center press left toggle has been swapped with key 4 short press
            DisableCursorTimer=1;
 	    if ((Current != FILE)) {

		Delay_Cnt = 1500;
      	while (Delay_Cnt > 1000){
                                                
            if((__Get(KEY_STATUS)& K_INDEX_S_STATUS)!=0){

                  if(OffsetSelect){
                    OffsetSelect^=0x03;                                                 //toggle between =1 and =2
                    goto bypassOSselect;
                  } 

                  if((Current==TRIGG)&&(_Det==2))DisableCursorTimer=0;
                  //SPECIAL FUNCTIONS FOR LEFT TOGGLE CENTER PRESS
                  if(LeftToggleSpecialFunctions())goto bypasslongpress;

                  j=0;
		  for (i=0;i<4;i++){
                    if(Title[i][SOURCE].Value != HIDE)j++;
                  }
                  if(j>0){
                    if ((((_4_source==11)||(_4_source>13))&&(_1_source!=HIDE))||
                         (((_4_source==10)||(_4_source==12)||(_4_source==13))&&(_2_source!=HIDE)))j--;
                  }

                  if (j>2){                   		//if more than 2 active channels, use as shortcut to trigg source select
	            _Curr[_Det].Flag |= UPDAT;
            	    for (i=0;i<4;i++){											
                      if (Current==i){									//================================================================
		        if (Title[i][SOURCE].Value != HIDE){
		          if (Title[TRIGG][SOURCE].Value !=i){						//if not already selected
	   		    for (j=0;j<3;j++) PreviousTrigSelect[j]=PreviousTrigSelect[(j+1)];		//lifo stack
		            PreviousTrigSelect[3]=Title[TRIGG][SOURCE].Value;				//save previous
                            Title[TRIGG][SOURCE].Value = i;						//set trig to selected track
                            if((i==3)&&((_4_source==11)||(_4_source>13)))Title[TRIGG][SOURCE].Value=0;	//set to FFT channels
                            if((i==3)&&((_4_source==10)||(_4_source==12)||(_4_source==13)))Title[TRIGG][SOURCE].Value=1;
		            Update_Trig(1);						
                            goto BypassShortcut;        //if new source selected, do not use as shortcut
	                  }
		        }  //if turned on
	              }    //if current
                    }      //check all channels

                    Current=TRIGG;			//shortcut to trigg source select
                    _Det=SOURCE;
BypassShortcut:
                    TriggFlagUpdate();
		    goto bypasslongpress;
                  }
                   Title[T_BASE][BASE].Flag |= UPDAT; 				
		   if((_Mode!=X_Y)&&(Title[TRIGG][SOURCE].Value==1)&&(Title[TRACK1][SOURCE].Value!=0)&&(Title[TRACK2][SOURCE].Value!=0)&&(Title[TRACK3][SOURCE].Value==0)&&(Title[TRACK4][SOURCE].Value==0)){
                     OldMode=Title[T_BASE][MODE].Value;			//if 2 analog chs are the only ones on and trigger source = chB
                     Title[TRIGG][SOURCE].Value=4;			//cycle to include A&B mode
		     Update_Trig(1);
                   }else{						//cycle between any other 2 channels
          		for (i=0;i<4;i++){											
			    if (Title[i][SOURCE].Value != HIDE){
				 if (Title[TRIGG][SOURCE].Value !=i){						//if not already selected
                                   if(Title[TRIGG][SOURCE].Value==4){						//do not save A&B position
                                     Title[T_BASE][MODE].Value=OldMode;                     	             
                                     Title[T_BASE][MODE].Flag |= UPDAT;
                                     Update_Base(); 
                                   }else{
	   		             for (j=0;j<3;j++) PreviousTrigSelect[j]=PreviousTrigSelect[(j+1)];		//lifo stack
		                     PreviousTrigSelect[3]=Title[TRIGG][SOURCE].Value;				//save previous
                                   }  
                                   Title[TRIGG][SOURCE].Value = i;						//set trig to selected track
                                   if((i==3)&&((_4_source==11)||(_4_source>13)))Title[TRIGG][SOURCE].Value=0;	//set to FFT channels
                                   if((i==3)&&((_4_source==10)||(_4_source==12)||(_4_source==13)))Title[TRIGG][SOURCE].Value=1;
                                   TriggFlagUpdate();
				   Update_Trig(1);
                                   goto bypasslongpress;
				 }
		            }  
                        }
                   }

         goto bypasslongpress;
          }					// if keystatus
	}                                       // while delay count

        Beeper(125); BackLight(1);
 
        while ((Delay_Cnt >0)&&(Delay_Cnt <=1000)){					// long press, change meter pages
          if(Delay_Cnt<875)BackLight(0);
          if((__Get(KEY_STATUS)& K_INDEX_S_STATUS)!=0){

 	      if (FlagMeter>0){
                SaveCurrent[0]=0;			//erase item source memory
                SaveCurrent[1]=0;
                if (UpdateMeter<4){
                 if((FlagMeter==2)&&(UpdateMeter==3)&&(Current==METER_1))Current=METER_2;
                 UpdateMeter=UpdateMeter+1;
                 if  ((_Mode==X_Y)&&(UpdateMeter>1)) UpdateMeter=4;
                }
                else{			//UpdateMeter==4, default pages for both meter types
                 if (FlagMeter==2){
                   ClearScreenArea(314,399,125,180);
                   if((Current < METER_0)&&(Current>T_VERNIE))Current=T_VERNIE;
                   if((Current==METER_0)||(Current==METER_2))Current=METER_1;
                 } 
                 UpdateMeter=0;		
                } 
                UpdateMeterPage();

              }else{			// flagmeter=0					//toggle wave calibration

            if (CalFlag==0){
                CalFlag=1;
                if(Current != FILE){
                MessageHandler(1);			//wave calibration on
                }   
               	  Print_Str(137,216,0x0405,PRN, "C");
            }else{
              CalFlag=0;
              if(Current != FILE){
                MessageHandler(2);			//wave calibration off
              } 
                	  Print_Str(137,216,0x0405,PRN, "U");
            }

          }
          goto bypasslongpress;

        }                        //if keypress
      }					// delay cnt
        
         Beeper(60); 
         if(Delay_Cnt == 0){		//extra long press
           if ((Options&1)==0) {
             Options|=1;
             if (((_Mode==NORH)||(_Mode==NORHLD)||(_Mode==NORC))&&(_T_base < 10)&&(!OsBufferLogicFlag)){Current = TRIGG; _Det = 3;}
	     Title[Current][_Det].Flag |= UPDAT;
                MessageHandler(7);			//trigger holdoff on
           }else{
             Options&=0xFE;
             if ((Current==TRIGG)&&(_Det==3)) _Det=0; 
             Title[Current][_Det].Flag |= UPDAT;
                MessageHandler(8);			//trigger holdoff off
           }
         }
      }          		      // if current

        if(Current == FILE){
          Print_Str(230, 0, (SCRN<<8)+ TEXT1, PRN, " >   ");
        if(_Curr[0].Value == SAVE){

          if(_Det==DIR){
            ProcessFileName();                         //SelectedFileName = (processed) EditChar
          }  

          switch (_Curr[2].Value){
          case BMP:
            i = Save_Bmp(_Curr[1].Value);
            if(_Det!=3)_Curr[1].Value++;
            break;
          case DAT:
            i=Save_Dat(_Curr[1].Value);
            if(_Det!=3)_Curr[1].Value++;
            Delayms(900);
            break;
          case BUF:
            i=Save_Buf(_Curr[1].Value);
            if(_Det!=3)_Curr[1].Value++;
            break;
          case CSV:
            i=Save_Csv(_Curr[1].Value);
            if(_Det!=3)_Curr[1].Value++;
            break;
          case 4:        				//configuration file
            if(_Det==3){
              Edited=1;
              for(i=0;i<8;i++)LastAccessedConfig[i]=SelectedFileName[i]; 
              ListOverride=1;
              ProcessEditName();                        //loads processed name into edit function
            }else ListOverride=0;
            i=ConfigFile(SAVE);
            Edited=0;
            break;
          case 5:                                       //cannot save arb or uart files
          case 6:
            i=200;
            break;
          case 7:
            i=Save_Img();                               //ROM backup image file
          }

        } else { 					//current = load

reload:
          if ((Current==FILE)&&(_Det==DIR)){
            for(i=0;i<8;i++)SelectedFileName[i]=Label[Title[9][3].Value][i];
          }

          i=0;
          if(_Curr[2].Value==DAT) i = Load_Dat(_Curr[1].Value);

          if(_Curr[2].Value==BUF){
            if(DownConvertMode){DownConvertMode=0; DownConvertRestore();}
            i=Load_Buf(_Curr[1].Value); 
            if(_Det!=DIR)_Curr[1].Value++;                                                   
              else{if(Title[9][3].Value<14)Title[9][3].Value++;else DirRange++;}               
            TempKp1=_Kp1;
            if(ChartLogic())ClearLeadingEdge=0;
          }


          if(_Curr[2].Value==BMP){
            if(DownConvertMode){DownConvertMode=0; DownConvertRestore();}
            i = Load_Bmp(_Curr[1].Value);
            if((Current==FILE)&&(_Det==DIR)){
                if(i==0)for(j=0;j<8;j++)EditChar[j][0]=SelectedFileName[j];
                if(Title[9][3].Value<14)Title[9][3].Value++;else DirRange++;                

              ReadDir((char*)Ext);
              if((Label[Title[9][3].Value][0]==0)||(Label[Title[9][3].Value]==NoneStr)){
                if(Title[9][3].Value>0)Title[9][3].Value--;
              }

            }else _Curr[1].Value++; 

            Key_Buffer=0;


            if(i==OK){
              FileMessage(i);
   	      while (1) {

                if (Key_Buffer>0){
                  App_init(0);
	          Delayms(150);
                  Config_init();

		  Update_View_Area();
                  if(Key_Buffer==K_INDEX_S)goto reload;
                  if(Key_Buffer == K_ITEM_S){

                    if((Current==FILE)&&(_Det==DIR)){                                         //use block to allow prev BMP load in list 

                      if(Title[9][3].Value>0){
                        Title[9][3].Value--;
                        for(j=0;j<8;j++){if(SelectedFileName[j]!=Label[Title[9][3].Value][j])break;}
                        if(j>6){
                          if(Title[9][3].Value>0)Title[9][3].Value--;
                            else if(DirRange>0)DirRange--;
                        } 
                      }else if(DirRange>0){
                        DirRange--;
                      }
	
                    }else{                  
                      if(_Curr[1].Value>1)_Curr[1].Value-=2;
                        else _Curr[1].Value=0;
                    }

                    goto reload;
                  }
                  if(FlagMeter)Clear_Meter_Area(); 
                  goto bypasslongpress;                         //break;
                } 
              }
            }else if(FlagMeter){EnableMeter();Display_Title();}

          }

          if(_Curr[2].Value==CFG){
            j=Title[FILE][3].Value;                     //keep present hilighted list position 
            i=ConfigFile(LOAD);
            Current=FILE;				//stay in file menu if loading file with button 5
            if(ListOverride){                           //keep list after loading new config
              Title[FILE][3].Value=j;
              for(j=0;j<8;j++)LastAccessedConfig[j]=SelectedFileName[j];
              _Det=3;
              ProcessEditName();                        //loads processed name into edit function
            }
            for(j=0;j<3;j++)Title[FILE][j].Flag|=UPDAT;
	    if(i == 0)goto bypasslongpress;
          }

          if(_Curr[2].Value==5){                        //load arbt file
            i = Load_Arbt(_Curr[1].Value);
            Update_Output();
          }

          if(_Curr[2].Value==6){                        //load uart file
            i = Load_Uart(_Curr[1].Value);
            if(_Kind==8)StartUart=1;  
          }

          if(_Curr[2].Value==CSV){                      //load CSV file not available
            i = 200;
          }

          if(_Curr[2].Value==7){                        //load bin image not available
            i = 200;
          }

          if((_Det==DIR)&&(_Curr[2].Value<3)){                                  //use last loaded filename as edit default
            if(i==0)for(j=0;j<8;j++)EditChar[j][0]=SelectedFileName[j];
          } 

        }
        FileMessage(i);
        }  
    }

      //----------------------------------------------------------------------------------------------------------------    
   if(Key_Buffer == K_INDEX_INC){                                            // ===--- TASTO 5 +++ PREMUTO --====
        
     if((((_Mode==SCAN)&&(!ChartLogic()))||((ChartLogic())&&(FrameMode>0)))                //no xpos in SCAN mode except large buf chart mode
       &&(Current==T_BASE)&&(_Det==2))goto BypassUpdate;

     if((_4_source>11)&&(Current==3)&&(_Det==1)){
       if(DownConvertMode){
         if(DownConvert==2)DownConvert=5;
         else if(DownConvert==20)DownConvert=50;
         else if(DownConvert==200)DownConvert=500;
         else if(DownConvert<1010)DownConvert*=2;
         DownConvertShiftEnable=1;
         goto BypassUpdate;
       }else if(FFTt1Mode){
         if(_T1<255)_T1++;
         if(_T1<253)_T1+=CursorSpeedLogic(1);
       }
     }

     if((DownConvertMode)&&(Current==T_BASE)&&(_Det==BASE))goto BypassUpdate;

     if((OffsetSelect==1)&&(Current>1)){
       if(ADCoffset<54)ADCoffset++;
       Update_Range();
       goto bypasslongpress;
     }

     if((OffsetSelect==2)&&(Current>1)){
       if(PPM_Comp<999)PPM_Comp++;
       goto bypasslongpress;
     }

     if(EditListActive(1)){                                                             //file directory read 
       if(Title[9][3].Value<14)Title[9][3].Value++;else DirRange++;   
       goto bypasslongpress;
     }

     if(EditListActive(0)){                                                             //filename edit 

       if((CharValue>64)&&(CharValue<90))CharValue++;
       else if((CharValue>48)&&(CharValue<58))CharValue--;
       else  switch(CharValue){
               case 32: CharValue=65;break;
               case 63: CharValue=65;break;                                             //63= non available chars   
               case 37: CharValue=38;break;
               case 38: CharValue=40;break;
               case 40: CharValue=41;break;
               case 41: CharValue=45;break;
               case 45: CharValue=95;break;
               case 95: CharValue=57;break;
               case 48: CharValue=32;break;
             }

       EditChar[CharIndex][0]=CharValue;
       UpdateEditBox(0);
       goto bypasslongpress;
     }

     if((Current==T_BASE)&&(_Det==BASE)&&(ChartLogic())&&(AutoSaveSelect)){
       if(AutoSaveSelect==1){if(AutoSaveBuf<2)AutoSaveBuf++;}
       if(AutoSaveSelect==2){
         if(ChartMode==0){
           ChartMode=1;
           InitiateOSBuffers();
         }
       }
       goto BypassUpdate;
     }

     if (_Mode == X_Y){
           if((Current == TRACK1)&&(_Det == POSI) && (OffsetX<215)) OffsetX=OffsetX+2;
           if((Current == TRACK2)&&(_Det == POSI) && (OffsetY<170)) OffsetY=OffsetY+2;
         }  
         SelUpdate(POSI);  
   
        if(((Current==TRACK1)||(Current==TRACK2))&&(_Det==1))AlignTbaseSweep(1);                               //changing coupling mode                      

        if((Current==5)&&(_Det==1)&&(_Kind==8)){                                //uart gen mode
          if(GenUartAdj==0){                                                    //adjust baud index, translate to baud rate
            if(GenBaudAdjSpeed){                                                //fast mode
              if(GenBaudIndex<800){                                             //set up break points for fast adj mode
                if(GenBaudIndex>35)GenBaudIndex-=20;else GenBaudIndex=16;
              }else if(GenBaudIndex<2800){
                GenBaudIndex-=50;
              }else if(GenBaudIndex<12970){
                GenBaudIndex-=100;
              }else if(GenBaudIndex<14970){
                GenBaudIndex-=50;
              }else{
                GenBaudIndex-=20;
              }  
            }else{if(GenBaudIndex>16)GenBaudIndex--;}
          }
          if(GenUartAdj==1)if(GenUartMode<6)GenUartMode++;
          if(GenUartAdj==2)GenUartStopBits=1;
          if(GenUartAdj==3)GenUartCont=1;
          goto bypasslongpress;
        }

        if((T1Start)&&(Current==T_VERNIE)&&((_Det==T1)||(_Det==T2))&&(Title[TRACK1][COUPLE].Value>2)&&(Title[TRACK1][COUPLE].Value<5)){         //in serial decode, tie T1 to T2
          if((_T1<Title[T_VERNIE][T1].Limit)&&(_T2<Title[T_VERNIE][T2].Limit)){_T1++;_T2++;}
          goto bypasslongpress;
        }

        if((Current==BK_LIGHT)&&(HboldAdjust)){
          if(Hbold>0)Hbold--; 
          goto bypasslongpress;
        } 

        if((Current==BK_LIGHT)&&(FastDimAdjust)){
          if(FastDim<3)FastDim++; 
          goto bypasslongpress;
        } 

        if((Current==VOLUME)&&(AdjBeepLevel)){
          if(LoBeepLevel>0)LoBeepLevel--; 
          goto bypasslongpress;
        } 

        if((SpiLogic())&&(Current==TRACK1)&&(_Det==1)&&(SpiAdj)){
          if(SpiAdj==1){
            if(SpiMode<3)SpiMode++;
          }          
          if(SpiAdj==2)SpiBitOrder=0;
          if(SpiAdj==3){
            if(SpiNumBits==(-3))SpiNumBits=0; else {
              if(SpiNumBits==0)SpiNumBits=2; else if(SpiNumBits<192)SpiNumBits++;                        
            }      
          }          
          goto BypassUpdate;
        }

        if((UartLogic())&&(Current==TRACK1)&&(_Det==1)&&(DataAdj==1)){
          if(DataSize<19){
            if(DataSize==9)DataSize=15;else DataSize++;
          }          
          goto BypassUpdate;
        }

        if((Current==OUTPUT)&&(_Det==1)&&(GenAdjustMode==1)&&((_Kind<5)||(_Kind==7))){                        //changing sweep or burst rate

         if(_Kind==7){ 
          if(GenFreqShift>(-100))GenFreqShift--;
          OutputAdjust();
         }else{
          if((Sweep==1)||(Sweep==2)){
            if(SweepIndex<9)SweepIndex++;
            TIM_2IRQControl();           
            AlignTbaseSweep(2);           
            if(Title[TRIGG][SOURCE].Value==4) Update_Trig(0); else Update_Trig(1);
          }else if(Sweep==3){

            if((SweepIndex<2)&&(_Frqn>5)){
              if(FineAdjust<FineAdjustLimit)FineAdjust++;else BurstAdjust(1);
            }else {if(_Frqn<14)BurstAdjust(1);else {if(FineAdjust<FineAdjustLimit)FineAdjust++;else BurstAdjust(1);}}

            TIM_2IRQControl();           
            if(Title[TRIGG][SOURCE].Value==4) Update_Trig(0); else Update_Trig(1);
          }else{
          if(GenFreqShift>(-100))GenFreqShift--;
          OutputAdjust();
          }
         }

          goto BypassUpdate;
        }

        if ((TrgAuto>0)&&(Current == TRIGG) && _Det==2) _Det=0;
        if(Current < METER_0){

         for (i=0;i<4;i++){			     			      //tracks 1 through 4
           if ((Current==i)&&(_Det == SOURCE)){
             if (Title[TRIGG][SOURCE].Value ==i){			      //if trig selected

               switch (Current){					      //ch about to be shut down	
                 case TRACK1:
                 case TRACK2:
 	             //if (Title[i][SOURCE].Value == 2)UpdateTriggerMem();
 	             if (Title[i][SOURCE].Value == 4)UpdateTriggerMem();
		     break;
                 case TRACK3:
 	             if (Title[i][SOURCE].Value == 1)UpdateTriggerMem();
		     break;
                 case TRACK4:
 	            if (Title[i][SOURCE].Value == 9)UpdateTriggerMem();
               }
               TriggFlagUpdate();
	       Update_Trig(1);						

             } //if title trig source...              
             TrigMemory(i);  
           }  //if current=i
         }    //for	

          if((Current == TRIGG)&&(Detail[Current]==2)){         // trigger level adjustment

            j=1;if(SetLowVolume){j=2;if(FlagFrameMode==0)j++;}
            for(i=0;i<j;i++){          
              if(Title[TRIGG][SOURCE].Value==4){
                if(V_Trigg[0].Value < 200)V_Trigg[0].Value++; 	
                if(V_Trigg[1].Value < 200)V_Trigg[1].Value++; 	
              }else{ 
                if(V_Trigg[_Trigg[SOURCE].Value].Value < 200) 
                  V_Trigg[_Trigg[SOURCE].Value].Value++;
              }
            } 

          }	else  if ((Current == TRIGG)&&(_Det == 3)){								  //trigger holdoff
            if ((FrameMode==2)&&(_T_base < 10)){
              if (Title[7][3].Value<3360) Title[7][3].Value+=30;
              Title[7][3].XPOS=GetHoldoffIndPos();
              SelectiveUpdate=0x1E;										//do not update anything
            }
          } else if ((Current == BK_LIGHT)||(Current == VOLUME)){// adjust backlight or volume
            if(_Curr[1].Value < _Curr[1].Limit)   _Curr[1].Value++;
    	    } else if ((Current == T_BASE)&&(_Det == XPOSI) ){                                           	 // *******>>>>>> ALT X_POSI adjustment
                ClearMeters();                                                                                   //clears meters if untriggered in NORH

                _X_View.Flag |= UPDAT;
                if (XposRef<151) {
	          if(SetLowVolume){
                    if (XposRef<30)_Curr[_Det].Value++;
                    else{
                      XposRef+=4;
                      goto bypasslongpress;
                    }
                  }else if (_Curr[_Det].Value < (_Curr[_Det].Limit))_Curr[_Det].Value++;

                }else{
                  if(SetLowVolume){ 
                    if(_Curr[_Det].Value <(_Curr[_Det].Limit-30)){
                      if(Key_Mid_Speed)_Curr[_Det].Value += 6;
                      else _Curr[_Det].Value += 30;                                            
                    }
                    else if (_Curr[_Det].Value <(_Curr[_Det].Limit-6)) _Curr[_Det].Value +=6;
                    else if (_Curr[_Det].Value < (_Curr[_Det].Limit)) _Curr[_Det].Value ++;
                  }else if (_Curr[_Det].Value < (_Curr[_Det].Limit))_Curr[_Det].Value ++;
                }                

                XposRef=GetXposRef(_Curr[_Det].Value);						

            } else if ((Current == T_VERNIE) && ((_Det==T1) || (_Det==T2))){
                    if (FlagMeter>0){
                      if (_Curr[_Det].Value <(_Curr[_Det].Limit-95)) _Curr[_Det].Value ++;
                      if(_Curr[_Det].Value<(_Curr[_Det].Limit-97))_Curr[_Det].Value+=(CursorSpeedLogic(0));
                    }
                    else
                    {
                      if (_Curr[_Det].Value <_Curr[_Det].Limit-(MIN_X-2)) _Curr[_Det].Value ++; 
                      if(_Curr[_Det].Value<(_Curr[_Det].Limit-MIN_X))_Curr[_Det].Value+=(CursorSpeedLogic(0));
                    }
           } else if((Current==OUTPUT)&&(_Det==2)){               // PWM duty% adjust
             if(PWAdjustMode){ 
               PWMdutyControl(0);  
               goto BypassUpdate;
             }else{if(_Curr[_Det].Value<29399) _Curr[_Det].Value+=300; else _Curr[_Det].Value=29999;}
           } else {                                              // the current item for otheR

                if(_Curr[_Det].Value<(_Curr[_Det].Limit-2))_Curr[_Det].Value+=(CursorSpeedLogic(0));

		if ((Current==TRACK1)&&(_Det==SOURCE)){

                  if(_1_source==0){                                                                //ChA is about to be turned on
                    AlignTbaseSweep(4);
                    LastSelected=1;  
                    SelectiveUpdate|=0x20;       
                  }else if (_1_source==4)AlignTbaseSweep(5);                  

                }
		if ((Current==TRACK2)&&(_Det==SOURCE)){
                  if(_2_source==0){                                                                //ChA is about to be turned on
                    AlignTbaseSweep(4);
                    LastSelected=2;  
                    SelectiveUpdate|=0x40;       
                  }else if (_2_source==4)AlignTbaseSweep(5);                  
                }

          if((_Mode==NORHLD)&&(Current==6)&&(_Det==3)){
            if(PerstFrameNumber<252)PerstFrameNumber+=((PerstFrameNumber/65)+1);

          }else{

           if ((Current == T_BASE)&&(_Det == BASE)){

             if(_Curr[_Det].Value==0){
               if((_Mode==SCAN)&&(SubIndex>0))SubIndex--;else _Curr[_Det].Value++; 
             }else if(_Curr[_Det].Value< _Curr[_Det].Limit)_Curr[_Det].Value++;


           }else{

              if(_Curr[_Det].Value < _Curr[_Det].Limit){
                _Curr[_Det].Value++;
                if((Current==6)&&(_Det==3))_Curr[_Det].Value+=(_Curr[_Det].Value/65); 
                SelUpdate(RANGE);
                if ((Current<2)&&(_Det==RANGE))Update_Range(); 
              }else if(_Curr[_Det].MARK & CIRC)_Curr[_Det].Value  = 0;

              if(((Current==0)||(Current==1))&&(_Det==0)&&(_Mode==X_Y)){
                if(Title[Current][0].Value>0)Title[Current][0].Value++;
                if(Title[Current][0].Value>4)Title[Current][0].Value=0;
              }
           }
          }

          if((Current==OUTPUT)&&(_Det==3)&&(Sweep)){                            
            if(FrameEndFlag)WaveGen();
            goto BypassUpdate;
          }

          if((Current==OUTPUT)&&(_Det==1))PWMscaleReset(0);

            if((Current == TRIGG)&&(Detail[Current]==0)){                       //trig source has just been changed
              if(Title[TRIGG][SOURCE].Value==4){				//changed to AltAB 
                if(_Mode==X_Y){							
                  _Curr[_Det].Value=0;						//skips over alt ab	
                }else{
                  OldMode=Title[T_BASE][MODE].Value;							//save prev mode	
                  ChannelStatus(0);						//save status
                }
              } 

              if((Title[TRIGG][SOURCE].Value==0)&&(_Mode!=X_Y)){				//changed from AltAB 
                Title[T_BASE][MODE].Value=OldMode;							//reset to prev mode	
                Title[T_BASE][MODE].Flag |= UPDAT;
                ChannelStatus(1);						//restore status
                Title[TRACK1][RANGE].Flag |= UPDAT; Title[TRACK2][RANGE].Flag |= UPDAT;
              } 
              Title[T_BASE][BASE].Flag |= UPDAT; 				
            }

            if ((Current == T_BASE)&&(_Det == BASE)){
              if(Sweep){if(Title[TRIGG][SOURCE].Value==4) Update_Trig(0); else Update_Trig(1);}
              if(!OSBufferLogic()){if((__Get(FIFO_START)!=0)||((_Mode!=NORH)&&(_Mode!=NORHLD)))__Set(TRIGG_MODE, UNCONDITION);} //temporarily setting this allows x_attr to properly update if in untriggered condition
            }
	    if((FrameMode<2)&&(Current == T_BASE)&&(_Det == BASE)){                 
              if (XposRef>150)XposRef=GetXposRef(_X_posi.Value);
	    }  
          }
          if((Current == T_BASE)&&(_Det == MODE)){     // T_BASE MODE selection				//at this point, mode has been changed
            if (_Mode==SCAN) {if (OldFrame>0)ShortBuffXpos=XposRef; else OldPosi=GetXpos(XposRef);}		//previous mode was xy
            if (_Mode==AUTO) {if (FrameMode>0)XposRef=ShortBuffXpos; else XposRef=GetXposRef(OldPosi);}		//previous mode was scan
            Title[RUNNING][STATE].Value = RUN;         // STATE = RUNNING 
            Title[RUNNING][STATE].Flag |= UPDAT;       // refresh RUNNING STATE
          }
          if((Current == OUTPUT)&&(_Kind<4)){
            if(Title[OUTPUT][FRQN].Value > 17) 
	        Title[OUTPUT][FRQN].Value = 17;          // upper limit of the analog signal frequency is 200KHz
          }

          if(Current == FILE){
            if(_Curr[0].Value == 0){                                               //save/edit file  
              if((_Curr[2].Value == 5)||(_Curr[2].Value == 6)){_Curr[2].Value = 7;ResetEditList();}
            }
          }

          if((Current == FILE)&&(_Curr[2].Value == CFG)) if(_Curr[1].Value > 9)_Curr[1].Value=9;           
          if((Current==FILE)&&(_Det==2))ResetEditList();                 //reset file list hilight to top when changing file types 
          for(i=0;i<4;i++)_Curr[i].Flag |= UPDAT;
        } else {
				       // in meter section	
          Meter[Current-METER_0].Flag |= UPDAT;

            if(Meter[Current-METER_0].Item < TL)	    
              Meter[Current-METER_0].Item += 1;          
            else{                     
              Meter[Current-METER_0].Item  = VBT;
              SaveCurrent[0]=Current;		      
              SaveCurrent[1]=Meter[Current-METER_0].Track; 
	      Meter[Current-METER_0].Track = 4;
            }

            if(Meter[Current-METER_0].Item == VDC){ 
               if(Current==SaveCurrent[0])
		 Meter[Current-METER_0].Track =SaveCurrent[1];
               else
                 Meter[Current-METER_0].Track = UpdateMeter;
            }

            if((Meter[Current-METER_0].Track>1)&&(Meter[Current-METER_0].Track<4)){
              if(Meter[Current-METER_0].Item ==VDC)Meter[Current-METER_0].Item =FRQ; 
              if(Meter[Current-METER_0].Item ==MAX)Meter[Current-METER_0].Item =CIR; 
            }
        }
      }
      //---------------------------------------------------------------------------------------------------------------- 
      if(Key_Buffer == K_ITEM_DEC){                                             // ===--- TASTO 6 --- PREMUTO --====
        if(Current < METER_0){
          _Curr[_Det].Flag|=UPDAT;

          if(EditListActive(0)){                                                              //filename edit 
            if(CharIndex>0){                                                                  //allow right toggle to escape edit box
              CharIndex--;
              CharValue=EditChar[CharIndex][0];
              UpdateEditBox(3);
              goto bypasslongpress;
            }
          } 

          if((DownConvertMode)&&(DownConvertInitiate==0)&&(_4_source>11)&&(Current==3)&&(_Det==1)){
            DownConvertShiftEnable=0; 
            FFTdownConvert(2);
            goto bypasslongpress;
          } 

          if((Current==FILE)&&(_Det==DIR)&&(_Mode==X_Y)&&(Title[9][0].Value==1))InitXY=1;

          if(Current==T_VERNIE){Current=V_VERNIE; goto NoDec;}   //skip over file menu's regular place

          if(Current==FILE){
            if((_Det==3)&&(Title[FILE][3].Value>0)){
              Title[FILE][3].Value=0;
            }else if((_Det==3)&&(DirRange>0)){ 
              if(DirRange>13)DirRange-=14;else DirRange=0;
            }else{
              if((_Det==3)&&(ListLogic()))PrintDir(0,0,3);                                          //clear file list when leaving
              if((FlagMeter==1)||((FlagMeter==2)&&(UpdateMeter==4))){Current=VOLUME;goto NoDec;}
              Current=T_VERNIE;
              goto NoDec;
            }
            goto bypasslongpress;
          }

          if (FlagMeter>0){
            //if(Current == TRACK1)  {Current = VOLUME;}	//loop around
            if(Current == TRACK1)  {Current = TRACK1;}	//don't loop around
            else                  {Current --;}
          }
          else
          {
            //if(Current == TRACK1)  {Current = T_VERNIE;}  //loop around
            if(Current == TRACK1)  {Current = TRACK1;}	//don't loop around
            else                   {Current --;}
            if ((Current == V_VERNIE)&&(_Det == 2)) _Det=0; //if meters off							
            if ((Current == T_VERNIE)&&(_Det == 2)) _Det=0; //if meters off							
          }
	    if (Current<4)Title[Current][3].Flag|= UPDAT;
          if(Current == RUNNING) Current --;               // Jump over Item 4
NoDec:
          CurDetailUpdate();
          if (Current==FILE){
            if((_Curr[2].Value == CFG)&&(_Curr[1].Value > 9))_Curr[1].Value=9;           
            for(i=0;i<4;i++)_Curr[i].Flag |= UPDAT;  
	  }

        } else if (FlagMeter==1){			//IN METER SECTION
          Meter[Current-METER_0].Flag |= UPDAT; 
          if(Current == METER_0) Current = METER_8; else Current --;
          Meter[Current-METER_0].Flag |= BLINK;
        } else if (FlagMeter==2){
          if (UpdateMeter!=4){if(Current==METER_1)Current=METER_3; else Current--;}
          else{
            if(Current==METER_0){
              Meter[Current-METER_0].Flag &= (!UPDAT & !BLINK);             
              Current=METER_3;
            }else if(Current==METER_2){
              Meter[Current-METER_0].Flag |= UPDAT;
              Meter[Current-METER_0].Flag |= BLINK;            
              Current=METER_0;
            }else Current--;
          }
        }else Current=T_VERNIE;

      }
	  //---------------------------------------------------------------------------------------------------------------------
      if(Key_Buffer == K_ITEM_S){    // ===--- TASTO 6 centrale PREMUTO --====

     DisableCursorTimer=1;
     Delay_Cnt = 1500;
     while (Delay_Cnt > 1000){
       if((__Get(KEY_STATUS)& K_ITEM_S_STATUS)!=0){

         if((Current==3)&&(_Det==1)&&(_4_source>11)&&(DownConvertMode==0)){
           if(FFTt1Mode)FFTt1Mode=0;else FFTt1Mode=1;
           goto bypasslongpress;
         }

         if(EditListActive(0)){                                                                  //filename edit 
           EditChar[CharIndex][0]=32;
           CharValue=32;
           goto bypasslongpress;
         }

         if((Current==FILE)&&(_Curr[2].Value==BUF)&&(_Curr[0].Value==LOAD)&&(_State.Value>0)){
           if (_Det==3){
             if(Title[9][3].Value>0){
               Title[9][3].Value--;
               for(i=0;i<8;i++){if(SelectedFileName[i]!=Label[Title[9][3].Value][i])break;}
               if(i>6){
                 if(Title[9][3].Value>0)Title[9][3].Value--;
                   else if(DirRange>0)DirRange--;
               } 
             }else if(DirRange>0){
               DirRange--;
             }
             ReadDir((char*)Ext);
             for(i=0;i<8;i++)SelectedFileName[i]=Label[Title[9][3].Value][i];
             i=Load_Buf(_Curr[1].Value); 
             FileMessage(i);
             if(Title[9][3].Value<14)Title[9][3].Value++;else DirRange++;   
           }else{          
             if(_Curr[1].Value>1)_Curr[1].Value-=2;else _Curr[1].Value=0;
             i=Load_Buf(_Curr[1].Value++); 
             Title[9][1].Flag|=UPDAT;
           }
           if(ChartLogic())ClearLeadingEdge=0;
           goto bypasslongpress;
         }

         if(Current==BK_LIGHT){
           FastDimAdjust=0;
           if(HboldAdjust==0)HboldAdjust=1;else HboldAdjust=0;
           goto bypasslongpress;
         }

         if((Current==5)&&(_Det==1)&&(_Kind==8)){                                //uart gen mode
           if(GenUartAdj<3)GenUartAdj++;else GenUartAdj=0;
           goto bypasslongpress;
         }                

      if((Current==OUTPUT)&&(_Det==1)&&(GenAdjustMode==1)&&(_Kind<5)){
          if(Sweep==0){                                                 //shift from continuous to sweep mode, sweep rate adjust 
            PrevShift=GenFreqShift; 
            Sweep=1;
            if(_Kind==4)Update_Output();
          }
            else if(Sweep==1){                                          //shift from sweep mode to burst mode, burst rate adjust 
              GenFreqShift=PrevShift; 
              Sweep=2;
            } 
              else if(Sweep==2)Sweep=3;                                 //in burst mode, shift from burst rate to burst length adjust
                else if(Sweep==3)Sweep=4;                                 //in burst mode, shift from burst length to freq adjust
                  else if(Sweep==4){PrevShift=GenFreqShift; Sweep=0;}     //shift from burst mode, freq adjust to continuous mode 
          TIM_2IRQControl();
          for(i=0;i<4;++i)Title[OUTPUT][i].Flag |= UPDAT;
          if(Title[TRIGG][SOURCE].Value==4) Update_Trig(0); else Update_Trig(1);
          if((Sweep==2)&&(_Kind==4))Update_Output();           
          goto bypasslongpress;
      }

      if((SpiLogic())&&((Current==TRACK1)||(Current==TRACK2))&&(_Det==1)&&(SpiAdj>0)){
        if(SpiAdj<3)SpiAdj++;else SpiAdj=1;
        goto bypasslongpress;
      }

        if((_4_source==11)&&(Current==TRACK4))              {
          if(SpecMode<2)SpecMode++;else SpecMode=0; 
	  Title[TRACK4][1].Value=SpecMode;
        }else{
          if(Current != FILE){
           if(_Mode==X_Y)goto bypasslongpress;		//don't change buffer size this way in xy mode

            if (FlagFrameMode==0){                      //entering change from full buffer mode to single window
              FlagFrameMode=1;
              SelectiveUpdate |=0x60;
              if (_Mode!=SCAN) {
                    if(XposRef>150){ 
                      OldPosi=GetXpos(XposRef);
                    }else{
                      OldPosi=XposRef;
                    }
                    XposRef=ShortBuffXpos;
                    Update=2;
              }
            }else if(OSBuffer==0){                      
              OSBuffer=1; 
              OSAvg=1;
              Update_Base();
              if(OSBufferLogic())InitiateOSBuffers();
            }else if(OSAvg==1){
              OSAvg=0;
            }else{                                     //entering change from short buffers to full buffer mode 
              if(_State.Value>0)DigChLockout=1;        //when paused, don't display dig chs until reset, contains OS data at this point
              OSBuffer=0;
              OSAvg=0;  
              FlagFrameMode=0;
                for(i=0;i<397;i++){                             
                  TrackBuff[(i*4)+2] = _3_posi;		     	
                  TrackBuff[(i*4)+3] = _4_posi;             
                }
              SelectiveUpdate |=0x60;

              if(ChartLogic()){                                        // chart mode reset
                ScrollFlag=2;
                _X_posi.Value=0;
                XposRef=GetXposRef(_X_posi.Value);
              }

              Update_Base();
              if(Title[TRIGG][SOURCE].Value==4) Update_Trig(0); else Update_Trig(1);
              if (_Mode!=SCAN) {
              
                ShortBuffXpos=XposRef;
                if(OldPosi>150){
                  XposRef=GetXposRef(OldPosi);
                }else XposRef=OldPosi;

              } 
              if ((Current == TRIGG) && (_Det==3)) _Det=0; Title[Current][_Det].Flag |= UPDAT;

            }
            AlignTbaseSweep(3);
	    Update_View_Area();
//====================================Changing buffer size while stopped in uart mode==========================================
            if((Title[TRACK1][1].Value>2)&&(_Status == RUN)&&((_Mode==NORH)||(_Mode==NORHLD))&&(__Get(FIFO_START)==0)){        
              RefreshDisplay=1;
            }
//=============================================================================================================================

           }//if current !file

          }//else, 4source=11
          goto bypasslongpress;
	 }				//if get key status
	}				//while

        Beeper(125); BackLight(1);
 
    while ((Delay_Cnt >0)&&(Delay_Cnt <=1000)){					// long press
     if(Delay_Cnt<875)BackLight(0);
     if((__Get(KEY_STATUS)& K_ITEM_S_STATUS)!=0){

       if(EditListActive(0)){                                                      //filename edit 
         for(i=0;i<8;i++)EditChar[i][0]=32;
         CharValue=32;
         goto bypasslongpress;
       }

       if((Current==FILE)&&(_Det==3)&&(_Curr[0].Value==LOAD)){                    //file delete

         for(i=0;i<13;i++)DelFileName[i]=Label[Title[9][3].Value][i];
         for(i=0;i<8;i++){
           switch(DelFileName[i]){                                              //process unavailable chars to display dot
             case 39:                   //'
             case 64:                   //@
               DelFileName[i]=63;       //dot
               break;
             default:
               if(((DelFileName[i]>32)&&(DelFileName[i]<37))||(DelFileName[i]>124))DelFileName[i]=63;
           } 
         }
         Print_Str(92,0,0x0405,PRN,"WARNING DELETE ");         
         Print_Str(92+(8*15),0,0x0405,PRN,DelFileName);         

         for(i=0;i<8;i++)DelFileName[i]=Label[Title[9][3].Value][i];
         for(i=0;i<3;i++)DelFileName[i+8]=Ext[i];
         DelFileName[11]=0;

         Key_Buffer=0;
         while (1) {
           if(Key_Buffer>0){
             if(Key_Buffer == K_ITEM_S){
               i=DeleteFile(SecBuff,DelFileName); 
               if(i==0){
                 if(DirRange>0)DirRange--;                                      
                 //if(ListLogic())PrintDir(0,0,3);                            //clear file list when changing in SPEC or MAP modes
                 if(ListLogic())ClearDir=1;                                   //clear file list when changing in SPEC or MAP modes
               }
               FileMessage(i);
               break;
             }else{
               FileMessage(200);
               break;
             } 
           } 
         }
         Delayms(200);
         Update_View_Area();
         for(i=0;i<3;i++)Title[FILE][i].Flag|=UPDAT;
         goto bypasslongpress;
       }


       if((Current==OUTPUT)&&(_Det==1)&&(GenAdjustMode==1)){
         if(Sweep==3){                                                      //in burst mode, shift from burst length to burst rate adjust
           Sweep=2;
           goto bypasslongpress;
         }else if(Sweep==4){
           Sweep=3;
           goto bypasslongpress;
         }
       }

       _Curr[_Det].Flag |= UPDAT;
        if (FlagMeter==0){
			if (((Current <= METER_8)&& (Current >= METER_0)) || (Current==BK_LIGHT) || (Current==VOLUME) ){
				Current=TRACK1 ;
			}

            if(Current != FILE){
                if (Timeout==0){
                  Timeout=1;
                  MessageHandler(3);			//standby timer on
                }else{
                  Timeout=0;
                  MessageHandler(4);			//standby timer off
                }  
             }

        } else {									//with meters on

			if(Current < METER_0) {						//shift over to meters
                          if(((ListLogic())||(_Mode==X_Y))&&(EditListActive(1))){
                            PrintDir(0,0,3);                                            //clear file list when leaving
                            if(_Mode==X_Y)InitXY=1;
                          }
                          OldCurrent=Current;
			  if(UpdateMeter==4)Current = METER_0;else Current=METER_1;
			} else {							//over to main menus, track1
                              if(FlagMeter==1){
                                MeterUpdate(0,9);
                              }else if(FlagMeter==2){
                                MeterUpdate(0,4);
                              }
			  Current = OldCurrent;
                          if(Current==FILE)for(i=0;i<3;i++)Title[FILE][i].Flag|=UPDAT;
			}
         }
           goto bypasslongpress;

	  }//get key
       }//delay count

       Beeper(60); 
       if(Delay_Cnt == 0){		//extra long press

              if ((Options&4)==0){
                  Options|=4;           //set bit 2
                   MeterUpdate(1,9);
                   if(Current != FILE){
                    MessageHandler(9);
                   }
                  Print_Str(145,216,0x0405,PRN,"H");
                  SelectiveUpdate |=0x60;
               }else{
                  Options&=0xFB;		      //clear bit 2
                  MeterUpdate(1,9);
                  if(Current != FILE){
                    MessageHandler(10); 
                  }
                  Print_Str(145,216,0x0405,PRN," ");  
              } 
       }
     }

      //---------------------------------------------------------------------------------------------------------------  
      if(Key_Buffer == K_ITEM_INC){                                             // ===--- TASTO 6 +++ PREMUTO --====
        if(Current < METER_0){
          _Curr[_Det].Flag |= UPDAT;

          if(EditListActive(0)){                                               //filename edit 
            if(CharIndex<7)CharIndex++;
            CharValue=EditChar[CharIndex][0];
            UpdateEditBox(1);
            goto bypasslongpress;
          } 
  
          if((DownConvertMode)&&(DownConvertInitiate==0)&&(_4_source>11)&&(Current==3)&&(_Det==1)){
            DownConvertShiftEnable=0;
            FFTdownConvert(1);
            goto bypasslongpress;
          } 

          if(Current==FILE){
            if(_Det==3){                                                       //scroll list down one page (limit to 14 to see last entry   

              i=ReadDir((char*)Ext);if(i>0)i--;
              if(Title[FILE][3].Value<i)
                Title[FILE][3].Value=i;
              else if(i==14){           
                DirRange+=14;                                                  //check how many new entries available in next page
                DirRange-=(15-ReadDir((char*)Ext));                            //jump ahead by that amount 
                ReadDir((char*)Ext);                                           //update now before writing to screen
              } 
              goto bypasslongpress;

            }else goto NoInc;
          } 


          if ((FlagMeter==0) && (Current == T_VERNIE))  {
                        Current=FILE;
			goto NoInc;
	  }

          if(Current == VOLUME)					
                        Current=FILE;
          else if((FlagMeter==2)&&(UpdateMeter!=4)){
            if (Current<T_VERNIE)Current++;else {Current=FILE;goto NoInc;}
            if(Current==FILE)Current++;
          }else {Current ++;if(Current==FILE)Current++;}

	  if (Current<4)Title[Current][3].Flag|= UPDAT;
          if ((FlagMeter==0)&&(Current==V_VERNIE)&&(_Det == 2)) _Det=0;
          if ((FlagMeter==0)&&(Current==T_VERNIE)&&(_Det == 2)) _Det=0;

NoInc: 
          if(Current == RUNNING) Current ++;              // Jump over RUNNING
          CurDetailUpdate();


          if(Current==FILE){
            if((_Det==DIR)&&(Title[9][0].Value==0)){                           //filename edit 
              CharValue=EditChar[CharIndex][0];
            } 
            if((_Curr[2].Value == CFG)&&(_Curr[1].Value > 9))_Curr[1].Value=9;           
            for(i=0;i<4;i++)_Curr[i].Flag |= UPDAT;
          }
        }else if (FlagMeter==1){					//IN METER SECTION
            Meter[Current-METER_0].Flag |= UPDAT;  
            if(Current == METER_8) Current=METER_0; else Current ++;
            Meter[Current-METER_0].Flag |= BLINK;
        }else if (FlagMeter==2){
          if(UpdateMeter!=4){if(Current==METER_3)Current=METER_1;else Current++;}  //skip over meter 0 (bat volt), not displayed w/3 big meters
          else{ 
            if(Current == METER_0){
		Meter[Current-METER_0].Flag &= (!UPDAT & !BLINK);
                Current=METER_2;
            }else if(Current==METER_3){
               Current=METER_0;
               Meter[Current-METER_0].Flag |= UPDAT;
               Meter[Current-METER_0].Flag |= BLINK;
            }else Current++;
          }          
        }

      }
      //----------------------------------------------------------------------------------------------------------------     


bypasslongpress:
     if((Key_Buffer!=K_INDEX_DEC)&&(Key_Buffer!=K_INDEX_INC)){OffsetSelect=0;}  //reset on all buttons except adjusts
bypassOSselect:

     if(_4_source!=SPEC_A)SpecRow=25;
     MeterDefaults();	
     if((_1_source==0)&&(_2_source==0)&&(_3_source==0)&&(_4_source>9)){				//if FFT alone, set to proper ch
	if(_4_source>13)Title[TRIGG][SOURCE].Value=0;
        if((_4_source==13)||(_4_source==12))Title[TRIGG][SOURCE].Value=1;
        TriggFlagUpdate();
        Update_Trig(1);						
     }else if (Title[TRIGG][SOURCE].Value==4){					//if trig source is a&b
        Title[T_BASE][MODE].Value=NORH;						//set normal trig mode
        Title[T_BASE][MODE].Flag |= UPDAT;
        if(Title[TRACK1][SOURCE].Value==0)Title[TRACK1][SOURCE].Value=1;	//turn both analog channels on if selecting a&b trig mode
        if(Title[TRACK2][SOURCE].Value==0)Title[TRACK2][SOURCE].Value=1;
        Title[TRACK3][SOURCE].Value=HIDE;					//shut down dig chs
        Title[TRACK4][SOURCE].Value=HIDE;
        for(i=0;i<4;i++)Title[i][SOURCE].Flag |= UPDAT;
        Title[TRACK1][RANGE].Flag |= UPDAT; Title[TRACK2][RANGE].Flag |= UPDAT;
        TriggFlagUpdate();
      }							 

      if((Current==OUTPUT)&&(_Det==3)){
        WaveGen();
        SelectiveUpdate|=0x08;
      }          

      if (Update!=2){
						  //full updates
        if((Current == T_BASE)&&(_Det == MODE)&&((Key_Buffer==K_INDEX_DEC)||(Key_Buffer==K_INDEX_INC)))goto BypassExclusions;
						  //note inclusions are 2 negates, eg: if current is not tbase, set bit to not update base
                                                  //update base inclusions
        if (!((Current == T_BASE)&&(_Det == BASE)&&((Key_Buffer==K_INDEX_DEC)||(Key_Buffer==K_INDEX_INC))))SelectiveUpdate|=0x04; 								
                                                  //update trigger inclusions
        if (!((((Current == TRIGG)&&((Title[TRIGG][SOURCE].Value<4)||(_Det<2)))||(Current<4)||((Current==T_VERNIE)
          &&(Title[TRIGG][SOURCE].Value<4)))&&((Key_Buffer==K_INDEX_DEC)||(Key_Buffer==K_INDEX_INC)||(Key_Buffer==K_INDEX_S))))
            SelectiveUpdate|=0x10;		
                                                  //update range inclusions
        if (!((Current<4)&&(_Det<3)&&((Key_Buffer==K_INDEX_DEC)||(Key_Buffer==K_INDEX_INC))))SelectiveUpdate|=0x02;

                                                  //shift Y positioning
BypassExclusions:

	if((Current<4)&&(_Det==3)&&((Key_Buffer==K_INDEX_DEC)||(Key_Buffer==K_INDEX_INC))){
          SetOffset(0,_A_Range,_1_posi);
          SetOffset(1,_B_Range,_2_posi);
          if((_Mode!=X_Y)&&(_4_source!=SPEC_A)&&(_4_source!=SPEC_B)&&(ListOverride)){ 
             ConfNameTimer=110;
             ToggleName=1;
             Update_Mark();
          }
        }  

        if((Current == T_BASE)&&(_Det == 2)&&((Key_Buffer==K_INDEX_DEC)||(Key_Buffer==K_INDEX_INC))){
          if((_Status == RUN)&&((((_Mode==NORH)||(_Mode==NORHLD))&&(__Get(FIFO_START)==0))||(_T_base <8))){  //trying to shift X position with device not triggered in run mode or at very slow speeds
            _X_posi.Value=GetXpos(XposRef);     			//align interpolated time bases with the others
            _State.Value=1;						//shift to hold mode so data is not transfered		
            Process();							//update screen so x pos can be changed
            _State.Value=0;						//return to run mode
          } 

          if((ChartLogic())&&(FrameMode==0)){  
            if((JumpCnt>(379+_X_posi.Value)))ScrollFlag=0;else ScrollFlag=2;
          }
        }
                                                           //NORMH mode: if changing timebase reset meters,
        if  ((Current==T_BASE)&&((_Mode==NORH)||(_Mode==NORHLD))&&(_Det == BASE)&&((Key_Buffer==K_INDEX_DEC)||(Key_Buffer==K_INDEX_INC))){
            EnablePaS=1;		//enable resetsum to reset TH, TL, duty%
            ResetSum();
        }

        if(_Mode==NORHLD){
          ClearTrackBuff(1);                       		
          UpdateBackground=1;
          if((FrameMode==0)&&(_T_base<11)){    			
            __Set(FIFO_CLR, W_PTR);
            JumpCnt=0;
          }
        }

 	if((_State.Value==0)&&(TempPar[0]==0xAA55)){
          reset_parameter();
          XposRef=GetXposRef(_X_posi.Value);
          Update_Base();
          if(!ChartLogic()){
            //ChartFlag=0;
            Update_Trig(2);
          }
          Update_Range(); 
        }
        Update = 1;
      }//if update !=2
                                                  //always include output exclusions
      if (!((Current == OUTPUT)&&((Key_Buffer==K_INDEX_DEC)||(Key_Buffer==K_INDEX_INC))))
        {if(!((_Kind==8)&&(StartUart)))SelectiveUpdate|=0x08;}
          else if(_Det==1)AlignTbaseSweep(2);       //if changing frequencies, if in sweep/detector mode
BypassUpdate:

      if((Current==OUTPUT)&&(_Det==0)&&((Key_Buffer==K_INDEX_DEC)||(Key_Buffer==K_INDEX_INC)))Update_Output();

      if((OSBufferLogic()!=OSStatus)){
        if(OSBufferLogic()){
          if((Current!=FILE)&&(_Curr[2].Value!=BUF))InitiateOSBuffers();
          PrevKind=0xFF;
        }
        Update_Base();
        if(Title[TRIGG][SOURCE].Value==4) Update_Trig(0); else Update_Trig(1);
      } 

      OSStatus=OSBufferLogic();
      if(_1_source>0)ChOnStatus[0]=_1_source;
      if(_2_source>0)ChOnStatus[1]=_2_source;
      Key_Buffer=0;
      OsBufferLogicFlag=OSBufferLogic();

    }
  }
 
}

void MeterDefaults(void){

              if ((UpdateMeter==4)&&(FlagMeter>0)){

                Meter[0].Flag |= UPDAT;
                Meter[0].Item  =  VBT;
                Meter[0].Track = 4;  
                if(FlagMeter==1){
                  MeterUpdate(1,9);
                }else if(FlagMeter==2){
                  MeterUpdate(1,4);
                }

                if ((_Mode==X_Y) || ( (Title[TRACK3][SOURCE].Value == HIDE) && (Title[TRACK4][SOURCE].Value == HIDE)))
                {
                  if(FlagMeter==1){              
                    Meter[1].Item  =  RMS; Meter[1].Track = 0;
                    Meter[2].Item  =  VDC; Meter[2].Track = 0;
                    Meter[3].Item  =  VPP; Meter[3].Track = 0;
                    Meter[4].Item  =  FRQ; Meter[4].Track = 0;
                    Meter[5].Item  =  RMS; Meter[5].Track = 1;
                    Meter[6].Item  =  VDC; Meter[6].Track = 1;
                    Meter[7].Item  =  VPP; Meter[7].Track = 1;
                    Meter[8].Item  =  FRQ; Meter[8].Track = 1;
                  }else{
                    Meter[2].Item  =  VDC; Meter[2].Track = 0;
                    Meter[3].Item  =  VDC; Meter[3].Track = 1;
                  }
                }  
                else  
                {
                  if(FlagMeter==1){              
                    Meter[1].Item  =  FRQ; Meter[1].Track = 0;
                    Meter[2].Item  =  VDC; Meter[2].Track = 0;
                    Meter[3].Item  =  FRQ; Meter[3].Track = 1;
                    Meter[4].Item  =  VDC; Meter[4].Track = 1;
                    Meter[5].Item  =  FRQ; Meter[5].Track = 2;
                    Meter[6].Item  =  CIR; Meter[6].Track = 2;
                    Meter[7].Item  =  FRQ; Meter[7].Track = 3;
                    Meter[8].Item  =  CIR; Meter[8].Track = 3;
                  }else{ 
                    Meter[2].Item  =  VDC; Meter[2].Track = 0;
                    Meter[3].Item  =  VDC; Meter[3].Track = 1;
                  }
                }
               }
}


void DisableMeter(void)
{
              if (((Current <= METER_8)&& (Current >= METER_0)) || (Current==BK_LIGHT) || (Current==VOLUME) ){_Curr[_Det].Flag |= UPDAT;Current=TRACK1 ;}
              FlagMeter=0;// modo no meter
              MAX_X=(386+MIN_X);
              if ((Current == V_VERNIE)&&(_Det == 2)) _Det=0;
              if ((Current == T_VERNIE)&&(_Det == 2)) _Det=0;
              }
 
void EnableMeter(void)
{
  u8 i;
            if(UartLogic()==0){
              if (Title[T_VERNIE][T1].Value>Title[T_VERNIE][T1].Limit-95) Title[T_VERNIE][T1].Value=Title[T_VERNIE][T1].Limit-95;
              if (Title[T_VERNIE][T2].Value>Title[T_VERNIE][T2].Limit-95) Title[T_VERNIE][T2].Value=Title[T_VERNIE][T2].Limit-95;
            }
 
            Clear_Meter_Area();
            if (FlagMeter==0)FlagMeter=1;
            MAX_X=(300 + MIN_X);
            Update=1; 
            MeterDefaults();

        if (FlagMeter==1){
            MeterUpdate(1,9);
            EnableTitleMeters(1); 
        }

        if (FlagMeter==1){
            Display_Meter();
            for(i=0; i<9; ++i)Display_Value(i);
        }else{
          UpdateBigMeters();
          if(UpdateMeter==4)EnableTitleMeters(1);else EnableTitleMeters(0);
        } 
}

void EnableTitleMeters(u8 Service){

            Title[V_VERNIE][2].Flag |= UPDAT; 
            Title[T_VERNIE][2].Flag |= UPDAT;
            Title[V_VERNIE][3].Flag |= UPDAT; 
            Title[T_VERNIE][3].Flag |= UPDAT;
          if(Service){
            Title[BK_LIGHT][0].Flag |= UPDAT; 
            Title[VOLUME][0].Flag |= UPDAT; 
            Title[BK_LIGHT][1].Flag |= UPDAT; 
            Title[VOLUME][1].Flag |= UPDAT; 
            Meter[0].Flag |= UPDAT;
            Meter[0].Item  =  VBT;
            Meter[0].Track = 4;          }
}

void UpdateBigMeters(void){
u8 i;
     if (FlagMeter==2){
        if(UpdateMeter==4){
          for (i=2;i<4;i++){
            if(i==(Current-METER_0)){
              BTwinkState=!BTwinkState;
              BTwink=BTwinkState;
            }else BTwink=1;
            Display_Value(i);
         }
        }else{
          for (i=1;i<4;i++){
            if(i==(Current-METER_0)){
              BTwinkState=!BTwinkState;
              BTwink=BTwinkState;
            }else BTwink=1;
            Display_Value(i);
          }
        }
      }
}

void UpdateMeterPage(void){
u8 CounterUpdate;

                if(FlagMeter==2){
                  if(UpdateMeter==4){
                     ClearScreenArea(314,399,125,180);
                     EnableTitleMeters(1);
                  }else EnableTitleMeters(0); 
                }				

                switch (UpdateMeter){
                case 0:
                case 1:
                  if(FlagMeter==1){
                    Meter[0].Flag |= UPDAT;
                    Meter[0].Item  =  VBT;
                    Meter[0].Track = 4;
                  }

                  for(CounterUpdate=1;CounterUpdate<9;++CounterUpdate){
                    Meter[CounterUpdate].Flag |= UPDAT;
                    Meter[CounterUpdate].Item  =  CounterUpdate+1; 		
                    Meter[CounterUpdate].Track = UpdateMeter;
                  }
                  break;
                case 2:				

                  for(CounterUpdate=1;CounterUpdate<9;CounterUpdate++){
                    Meter[CounterUpdate].Flag |= UPDAT;
                    Meter[CounterUpdate].Item  = C_Dmeter[CounterUpdate-1]; 		
		    if (CounterUpdate<6)Meter[CounterUpdate].Track = 2; else Meter[CounterUpdate].Track = 3;
                  }  

                  if(FlagMeter==1){
                    Meter[0].Flag |= UPDAT;
                    Meter[0].Item  =  VBT;
                    Meter[0].Track = 4;
                  }else{
                    EnableTitleMeters(0); 
                  } 
		  break;
                case 3:

                  for(CounterUpdate=1;CounterUpdate<9;CounterUpdate++){
                    Meter[CounterUpdate].Flag |= UPDAT;
                    Meter[CounterUpdate].Item  = C_Dmeter[CounterUpdate-1]; 		
		    if (CounterUpdate<6)Meter[CounterUpdate].Track = 3; else Meter[CounterUpdate].Track = 2;
                  }  
                  if(FlagMeter==1){
                    Meter[0].Flag |= UPDAT;
                    Meter[0].Item  =  VBT;
                    Meter[0].Track = 4;
                  }else{
                    EnableTitleMeters(0); 
                  }  
                 break;
                case 4:
		  MeterDefaults();
                }				// switch
}

void Config_init(void){
  DownConvertMode=0;
  if (FlagMeter>0)MAX_X=300+MIN_X;
    else {
      MAX_X=386+MIN_X;
      if(Current>=METER_0)Current=T_VERNIE;
    }
  OldTrack1X=Title[TRACK1][POSI].Value;
  OldTrack2X=Title[TRACK2][POSI].Value;
  OldMeter=FlagMeter;
  _Vt1Old=_Vt1;
  _Vt2Old=_Vt2;
  ClearTrackBuff(1);
  SelectiveUpdate |=0x60;
  Options|=2;           //set bit 1, shuts off edge filter (shows sharp edges)
  TrgAutoOld=TrgAuto;
  if (FlagFrameMode>0){
    XposRef=SaveShortBuffXpos;
    if(!ChartLogic())OldPosi=_X_posi.Value;
  }else{
    XposRef=GetXposRef(_X_posi.Value);								
    ShortBuffXpos=SaveShortBuffXpos;
  }

  InvertA=0;InvertB=0;
  if((!UartLogic())&&(!i2cLogic())&&(!SpiLogic())&&(_Mode!=X_Y)){
    if((Title[0][SOURCE].Value==2)||(Title[0][SOURCE].Value==4))InvertA=1;
    if((Title[1][SOURCE].Value==2)||(Title[1][SOURCE].Value==4))InvertB=1;
  }

  Title[7][3].XPOS=GetHoldoffIndPos();
  __Set(TRIGG_MODE, UNCONDITION);	//allows arrays to update if in untriggered condition

  if(OSBufferLogic()){
    InitiateOSBuffers();
  }

  PrevKind=0xFF;
  VDiff[0]=(HiBatLevel[0]-LoBatLevel[0]);			
  VDiff[1]=(HiBatLevel[1]-LoBatLevel[1]);			
  BatLevelCompensation();
  AutoFFT=Title[TRACK4][2].Value;
  if(AutoFFT>1)FFTGain=AutoFFT-2;else{if(AutoFFT==0)FFTGain=8;}
  SpecMode=Title[TRACK4][1].Value;
  //PerstFrameNumber=0;			//allow saved frame#
  Raw=0;                                //overwrite saved Raw so always off by default 
  ShowFFT=1;
  OsBufferLogicFlag=OSBufferLogic();
  Process();				
  Update_View_Area();                   // in case file menu display was showing
  PrevSweepIndex=-1;
  PWAdjustMode=0;                       // prevents conflict when initializing
  StartUart=0;
  UartFileSize=0;
  Update_Output();       
  if((DataSize<5)||(DataSize>19))DataSize=8;
  Ch1TLevel=_Vt1;                       //use UpdateTLevels here instead? Compensates for Ka1 and Ka2
  Ch2TLevel=_Vt2;
  ChannelStatus(0);						//save status 
  if(ChartLogic())InitChart();

}

u16 GetHoldoffIndPos(void){
  return (((((Title[7][3].Value)*100)/3480)*207)/100)+91;
}

u8 ConfigFile(u8 service){
u8 i;
          if(service==0){                   //save config  

            PWAdjustMode=0;                  
            if (FrameMode==2){
              SaveShortBuffXpos=XposRef;
            //_X_posi.Value=GetXpos(OldPosi);  
              _X_posi.Value=OldPosi;  
            } else SaveShortBuffXpos=ShortBuffXpos;

            Current=SaveCurrent[0];			//return to menu item selected before saving config
            _Det=SaveCurrent[1]; 
            i=Save_Param(Title[FILE][1].Value);
            _Curr[_Det].Flag |= UPDAT;

  	    Title[FILE][0].Value=LOAD;    		//preset file menu to load config files
            if(i==0)ConfigFileNumber=Title[FILE][1].Value;  			//file number     
            Title[FILE][2].Value=CFG;

          }else{			    //load config

            ConfigFileNumber=_Curr[1].Value;
            i = Load_Param(_Curr[1].Value);
            TIM_2IRQControl();
            PrevSweepIndex=-1;
            PWAdjustMode=0;                       // prevents conflict when initializing
            ResetFlag=2;
            StartUart=0;
            UartFileSize=0;
            InitiateUartGen=0;
            Update_Output();       

	    if(i == 0){  
              __Set(TRIGG_MODE, UNCONDITION);         //temporarily setting this allows x_attr to properly update if in untriggered condition 
	      App_init(1);
              Config_init();
  	      Title[FILE][0].Value=LOAD;    		//preset file menu to load config files
              Title[FILE][1].Value=ConfigFileNumber;  			//file number     
              Title[FILE][2].Value=CFG;
              Update_View_Area();			//in case new loaded config has current=file
              Update=2;					//to update delayed trigger loop under no trigger condition & bypass selective update
            }
          }
          return i;

}

void EnableChannel(u8 track){


  if(Title[track][SOURCE].Value == HIDE)  {
    AlignTbaseSweep(4);
    Title[track][SOURCE].Value = 1;
    LastSelected=track+1;
    Title[track][SOURCE].Flag|= UPDAT;
  }
  if(OSBufferLogic())Update_Trig(2);

}

void UpdateTriggerMem(void){
	u8 j;
                      Title[TRIGG][SOURCE].Value = PreviousTrigSelect[3];
                      for (j=3;j>0;j--) PreviousTrigSelect[j]=PreviousTrigSelect[(j-1)];
 		      PreviousTrigSelect[0]=0;  

}

void SelUpdate(u8 detail){
  if((Current<2)&&(_Det==detail))SelectiveUpdate|=(0x20+(Current*0x20));   
}

void TriggFlagUpdate(void){
u8 i;
  for(i=0;i<3;i++)Title[TRIGG][i].Flag=UPDAT;
}

void MessageHandler(u8 number){
  MessageNumber=number;		      
  NotificationTimer=80;                 //x 20Ms
}

void CurDetailUpdate(void){
          _Curr[_Det].Flag |= BLINK;
          _Curr[_Det].Flag |= UPDAT;
}

void ChannelStatus(u8 service){
u8 i;
  for(i=0;i<4;i++){
    if(service){				  //restore channel status	
      Title[i][SOURCE].Value=ChStatus[i];
      Title[i][SOURCE].Flag |= UPDAT;
    }else ChStatus[i]=Title[i][SOURCE].Value;     //save channel status
  }
}


void MeterUpdate(u8 start, u8 limit){
u8 i;
          for(i=start;i<limit;++i)Meter[i].Flag |= UPDAT;
}

void TrigMemory(u8 i){             
u8 j,k;
	     if (PreviousTrigSelect[0]==i)PreviousTrigSelect[0]=0;	//clear to default (ch1) first entry if last ch memorized shut down
             for (j=1;j<4;j++){										
	       if (PreviousTrigSelect[j]==i) for (k=j;k>0;k--) PreviousTrigSelect[k]=PreviousTrigSelect[k-1];
	     }  
}


void BackLight(s8 shift){
u8 tmp=Title[BK_LIGHT][CLASS].Value+1;
   if(tmp>3)shift*=2;
   if(tmp>8)shift=-shift;  
   __Set(BACKLIGHT, 10*(tmp+shift));
}

void PWMdutyControl(u8 Dir){
   if(Dir==1){if(TIM4->CCR1<TIM4->ARR)TIM4->CCR1++;else TIM4->CCR1=TIM4->ARR;}                   //decrease pulse length
   if(Dir==0){if(TIM4->CCR1>1)TIM4->CCR1--;else TIM4->CCR1=1;}                                   //increase pulse length
   UpdatePWMDuty(1);
}

void PWMscaleReset(u8 Dir){
s32 Tmp;
  
  if((Sweep!=1)&&(PWAdjustMode==1)){
    if((_Frqn>10)||((_Frqn>9)&&(Dir==1))){
      if (TIM4->CCR1<(TIM4->ARR/2)){                                                                 //preserve negative pulses (duty%>50)
        if(TIM4->CCR1>=(D_Tab[_Frqn].ARR/2)-1)TIM4->CCR1=(D_Tab[_Frqn].ARR/2)-1;
      }else{                                                                                         //positive pulses (duty%<50)
        Tmp=TIM4->ARR-TIM4->CCR1;
        Tmp=D_Tab[_Frqn].ARR-Tmp;   
        if(Tmp<(D_Tab[_Frqn].ARR/2))Tmp=(D_Tab[_Frqn].ARR)/2;  
        TIM4->CCR1=Tmp;   
      }
    }else{

       if (TIM4->CCR1<(TIM4->ARR/2)){                                                                           //negative pulses 
         if(Dir==0){                                                                                            //shifting freq range up   
           Tmp=  ( (TIM4->CCR1 * (D_Tab[_Frqn-1].PSC+1) )      /     (D_Tab[_Frqn].PSC+1) ) ; 
           if(Tmp<(D_Tab[_Frqn].ARR/2)-1)TIM4->CCR1=Tmp;else TIM4->CCR1=(D_Tab[_Frqn].ARR/2)-1;
          }
         if(Dir==1){                                                                                            //shifting freq range down   
           Tmp=  ( (TIM4->CCR1 * (D_Tab[_Frqn+1].PSC+1) )      /      (D_Tab[_Frqn].PSC+1) ) ; 
           if(Tmp>0)TIM4->CCR1=Tmp;else TIM4->CCR1=1;
         }
       }else{                                                                                                   //positive pulses                                                                                                 
         if(Dir==0){                                                                                               
           Tmp= ( (  ( (TIM4->ARR-TIM4->CCR1) * (D_Tab[_Frqn-1].PSC+1) * 10) / (D_Tab[_Frqn].PSC+1))+5 )/10   ; 
           if(  Tmp<(D_Tab[_Frqn].ARR/2) )TIM4->CCR1=      (D_Tab[_Frqn].ARR-(Tmp+1));     else TIM4->CCR1=(D_Tab[_Frqn].ARR/2); 
          }
         if(Dir==1){                                                                                               
           Tmp= (   (TIM4->ARR-TIM4->CCR1) * (D_Tab[_Frqn+1].PSC+1)  / (D_Tab[_Frqn].PSC+1))   ; 
           TIM4->CCR1= (D_Tab[_Frqn].ARR-Tmp) ; 
         }
       }

    }  
    UpdatePWMDuty(0);
  }
}

void SetIRQ2Priority(void){
#warning Gabo: sweep disabled
/*
u8 pre=0,sub=0;
                 
  if(Sweep==1){                                                                //SWEEP MODE 
    if(((_Kind==0)||(_Kind==2))&&(SweepIndex>7)&&(_Frqn==13)){pre=0;sub=1;}    //20Khz sine, saw
      else if(_Kind==1){pre=1;sub=1;}                                          //triangle waves
  }
  if(Sweep>1){                                                                 //BURST MODE
    if((_Kind==3)&&(_Frqn==15)){pre=1;sub=1;}                                  //100Khz square waves
  }


  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQChannel;                          //TIM2 IRQ priority setup for sweep/burst generator
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pre;        
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub;             
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
*/
}

void BurstAdjust(u8 Dir){

   if(Dir==1){
     if(SweepStep==1){
       if(BurstLimit<200)BurstLimit++;
     }else{
       BurstLimit&=0xFE;                                                                
       if(BurstLimit<200)BurstLimit+=2;
     }
   }else{
      if(SweepStep==1){
       if(BurstLimit>0)BurstLimit--;
     }else{
        BurstLimit&=0xFE;                                                                
        if(BurstLimit>0)BurstLimit-=2;
     }
   }
}

void InitChart(void){
            if(ChartLogic()&&ChartMode)OsChartFlag=1;else OsChartFlag=0;

            __Set(FIFO_CLR, W_PTR);                             //clear any pending FPGA flags for good measure

            JumpCnt=0;                                          //reset buffer scan
            __Set(T_BASE_PSC, 100-1);                           //slow down write pointer so read ptr can catch up
            __Set(T_BASE_ARR, 24000-1); 
            Process();                                          //move read pointer up to write ptr position

            __Set(TRIGG_MODE,3);  		                //set up chart mode 
            __Set(V_THRESHOLD,255);                                
            cleardatabuf(3);                                    //clear buffers
            ClearTrackBuff(2);
            if(OsChartFlag)InitiateOSBuffers();
            ScrollFlag=2;       
            _X_posi.Value=0;                                    //reset to start
            XposRef=GetXposRef(_X_posi.Value);
            JumpCnt=0;          			
              
            __Set(T_BASE_PSC, TbaseSub[SubIndex-1].Psc);        //restore timebase
            __Set(T_BASE_ARR, TbaseSub[SubIndex-1].Arr);

}

void ProcessEditName(void){
u8 i;

  for(i=0;i<8;i++){

      if(((LastAccessedConfig[i]>64)&&(LastAccessedConfig[i]<91))||
        ((LastAccessedConfig[i]>47)&&(LastAccessedConfig[i]<58)))EditChar[i][0]=LastAccessedConfig[i];
      else switch(LastAccessedConfig[i]){
             case 32:
             case 95:
             case 45:
             case 41:
             case 40:
             case 38:
             case 37:
               EditChar[i][0]=LastAccessedConfig[i];
               break;
             default: EditChar[i][0]=63;                        //63(dot)= flag to use original char in LastAccessedConfig
           }
  }
}

void ResetEditList(void){
u8 i;
            Title[FILE][DIR].Value=0;
            DirRange=0;  
            CharIndex=0;
            for(i=0;i<15;i++)Label[i][0]=0;
            if((ListOverride)&&(_Curr[2].Value==CFG)){
              for(i=0;i<8;i++)SelectedFileName[i]=0;
              ProcessEditName();
            }else{
              for(i=0;i<8;i++){EditChar[i][0]=32;SelectedFileName[i]=0;}            
            }

            CharValue=32;
}

u8 LeftToggleSpecialFunctions(void){
u8 i;

                  if((Current==3)&&(_4_source>11)&&(_Det==1)){
                    if(ChartLogic())return 1; 
                    if(DownConvertMode==0){
                      OSBuffer=0;                                    
                      OSAvg=0;  
                      Update_Base();
                      DownConvertMode=1;
                      DownConvertShiftEnable=0;
                      DCbase=_T_base;
                      DCmode=Title[6][0].Value;
                      DCframe=FlagFrameMode;                                            
                      if(FastMode){
                        FlagFrameMode=0;                       
                        UpdateFrameMode();
                        Update_View_Area();
                      }  
                      DCmeter=FlagMeter;
                      DCt1=_T1;
                      DisableMeter();
                      for(i=0;i<3;i++){Title[i][0].Value=0;Title[i][0].Flag |= UPDAT;}
                      Title[6][0].Value=2;                                              //set mode to NORM
                      Title[6][0].Flag |= UPDAT;	
                      if(DownConvert==0)DownConvert=1;
                      DownConvertInitiate=1;
                    }else{DownConvertMode=0; DownConvertRestore();}
                    return 1;
                  }

                  if((ChartLogic())&&(Current==T_BASE)&&(_Det==1)){                             
                    if(AutoSaveSelect<2)AutoSaveSelect++;else AutoSaveSelect=0;
                    return 1;
                  }              

                  if(Current==BK_LIGHT){
                    HboldAdjust=0;
                    if(FastDimAdjust==0)FastDimAdjust=1;else FastDimAdjust=0;
                    return 1;
                  }

                  if(Current==VOLUME){
                    if(AdjBeepLevel==0)AdjBeepLevel=1;else AdjBeepLevel=0;
                    return 1;
                  }

                  if((Current==5)&&(_Det==1)&&(_Kind==8)){                                //uart gen mode
                    if(GenBaudAdjSpeed==0)GenBaudAdjSpeed=1;else GenBaudAdjSpeed=0;
                    return 1;
                  }                

                  if((SpiLogic())&&(Current==TRACK1)&&(_Det==1)){
                    if(SpiAdj==0)SpiAdj=1;else SpiAdj=0;
                    return 1;
                  }              
 
                  if((SpiLogic())&&(Current==TRACK2)&&(_Det==1)){
                    if(SpiChartFlag==0)SpiChartFlag=1;else SpiChartFlag=0;
                    return 1;
                  }              


                  if((UartLogic())&&(Current==T_VERNIE)){                             
                    if(T1Start==0)T1Start=1;else T1Start=0;
                    return 1;
                  }              

                  if((UartLogic())&&(Current==TRACK1)&&(_Det==1)){
                    if(DataAdj==0)DataAdj=1;else DataAdj=0;
                    return 1;
                  }              
 
                  if((Current==OUTPUT)&&(_Det==1)&&((_Kind<5)||(_Kind==7))){
                    if(GenAdjustMode==1)GenAdjustMode=0;else GenAdjustMode=1;
                    return 1;
                  }

                  if((Current==OUTPUT)&&(_Det==2)){
                    if(PWAdjustMode==1)PWAdjustMode=0;else PWAdjustMode=1;
                    return 1;
                  }

                  if((Current==T_VERNIE)&&(_Det==2)){
                    if(DisableCursorDisplay&1){
                      DisableCursorDisplay&=0xFE;
                      MessageHandler(14);
                    }else{
                      DisableCursorDisplay|=1;
                      MessageHandler(15);
                    }
                    return 1;
                  }

                  if((Current==V_VERNIE)&&(_Det==2)){
                    if(DisableCursorDisplay&2){
                      DisableCursorDisplay&=0xFD;
                      MessageHandler(14);
                    }else{
                      DisableCursorDisplay|=2;
                      MessageHandler(15);
                    }
                    return 1;
                  }

                  if(_Mode==X_Y){
                    if(XYper)XYper=0;else XYper=1;
                    return 1;
                  }
                  if((_Mode==NORHLD)&&(Current==6)&&(_Det==0)){
                    if(Raw)Raw=0;else Raw=1;
                    MessageHandler(12+Raw); 
                    return 1;
                  }
                  return 0; 
}

u8 CursorSpeedLogic(u8 enable){                                         
u8 i=0;                                                             
    if((SetLowVolume)&&((enable)||((((Current==T_VERNIE)||(Current==V_VERNIE))    //SetLowVolume is fast repeat key flag 
    &&(_Det<2))||((Current<4)&&(_Det==3))))){
      i=1;
      if(FlagFrameMode==0)i++;
    }
    return i;
}

void UpdateFrameMode(void){
u16 Tmp;

    if (FlagFrameMode==1){										//short buffer mode

          if (_Mode==SCAN){
              if(_T_base>9) _X_posi.Value=150; else _X_posi.Value=0;                             
              FrameMode=1;
          }else{
            if(OsBufferLogicFlag){                              //note this is only active in non interpolated ranges, so KP is not a factor
              if(_X_posi.Value>150)_X_posi.Value=150;           //limit xpos shift to left of screen in oversampling mode
              if(_T_base<16) Tmp=(1208-(_X_posi.Value*8))+(StartOffset*8);
                else Tmp=(908-(_X_posi.Value*6))+(StartOffset*6);
              __Set_Param(8,(Tmp&0xFF));                        //stabilize window position and shift trigger point to match xpos rather
              __Set_Param(9,((Tmp>>8)&0xFF));                   //than shifting window to keep buffer size at 512 (X8= 4096)  
            }else{              
	      if ((_Mode!=SGL)||(OsBufferLogicFlag)){
  		Tmp= 150-(((1024 - X_Attr[Title[T_BASE][BASE].Value].KP)*150 + 512)/1024); 
		if (_X_posi.Value>Tmp) {
                  _X_posi.Value=Tmp;  //allow xpos to shift to left of screen in short buffer mode, add this to buffer size
	          XposRef=GetXposRef(Tmp);
	          Update=1; 
	        }
	        ShortBuffXpos=XposRef;	
              }
            }
            FrameMode=2; 	
          }

    }
    else   													//if in long buffer mode
    {														// shift trigger "splice" to left of screen

     if((SubIndex==0)||(_Mode!=SCAN)||(_T_base>0)){

       if ((_Mode==SCAN)&&(_T_base>9)){				
          if (XposFlag==0){	           //if flag set and in fast scan modes do nothing after going once in block and setting xpos to 150
            _X_posi.Value=150;									
            Update=1; 										
            XposFlag=1;
          }                                                         
       }else if (XposFlag==1){             //with flag set and all modes but scan with faster time bases            
          if (_Mode==SCAN)_X_posi.Value=0; //don't reset xpos if not in scan mode
          XposFlag=0;					
          Update = 1; 
       }else if (_Mode==SCAN){             //if flag not set, after going into above and setting flag
          if (_X_posi.Value>0){            //will drop in here as long as in scan mode with slower time base
            _X_posi.Value=0;               //make sure xpos is at zero and update if it's not after setting it to 0
            Update = 1; 
          }
       }

     }
     FrameMode=0;
    }


}

/*********************************  END OF FILE  ******************************/
