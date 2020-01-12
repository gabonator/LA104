/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : Calibrat.c  
 Version   : DS203_APP Ver 2.3x                                  Author : bure
*******************************************************************************/
#include "Interrupt.h"
#include "Function.h"
#include "Calibrat.h"
#include "Process.h"
#include "File.h"
#include "BIOS.h"
#include "Menu.h"

const char VS_STR[8][12]  ={"250-300mV", "!0.5-0.6V!","!1.0-1.2V!","!2.5-3.0V!",
                     "!5.0-6.0V!"," !10-12V! "," !25-30V! "," !50-60V! "};
const char V_UNIT1[8] ={'m','V','m','V','V',' ','k','V'};
const char CenterStr[5][12]= {"-<o ( )  >+","-< o( )  >+","-<  (o)  >+","-<  ( )o >+","-<  ( ) o>+"};
const char CenterStep[32]= "CENTER CAL SOURCE ON ADC STEP  ";

const char ClockEntry[5][39]= {"       COMPENSATE FOR CPU CLOCK       ","CONNECT FREQ COUNTER TO WAVE OUT(1MHZ)",
                               "ENTER ERROR IN PPM:  +IF HIGH -IF LOW ","L TOGGLE TO CHANGE-PRESS 2 TO CONTINUE",
                               "               (      )               "};

                               //shift > 3 char
s16 PPM_Comp=0;
u8 CalibrateMode=0;

void ClearTop(void);

/*******************************************************************************
  Calibrat : Calibrat routine
*******************************************************************************/
void Calibrat(u8 Channel)
{

  s8  Ma1[10], Mb1[10], Ma3[10], Mb3[10];
  u16 Ma2[10], Mb2[10], i, j;
  s16 TmpA, TmpB;
  u8  Range, k = 0, m, Step, Tmp, V_min, V_max;               
  s8  Offset;
  char n[10];
  u16 BatLevel[2]={0,0};
  char PPM_Str[5];
  
  Key_Buffer = 0; 
  __Set(STANDBY, DN);                                   // exit the power saving state
  //__Set(BACKLIGHT, 10*(Title[BK_LIGHT][CLASS].Value+1));
  BackLight(0);
  __Clear_Screen(BLACK);                                // clear the screen

  Delayms(300); 	

  Title[OUTPUT][KIND].Value=PWM;    	//set to PWM
  Title[OUTPUT][FRQN].Value=18;     	//set to 1mhz
  Title[OUTPUT][DUTYPWM].Value=15000;   //set to 50%
  GenAdjustMode=0;
  Sweep=0;
  Update_Output();

  j=0; 
  for(i=200;i>144;i-=11){
    if(i==156)i=145;   
    Print_Str(40,i,0x0405,PRN,(char*) ClockEntry[j++]);
  }

  while(Key_Buffer){};			//wait for key release
  while(1){			
    s16ToDec3(PPM_Str, PPM_Comp);
    Print_Str(176,145,0x0405,PRN,PPM_Str);
    if(Key_Buffer==K_INDEX_DEC) {if(PPM_Comp>-999)PPM_Comp--;}  		// K5   left toggle -
    if(Key_Buffer==K_INDEX_INC) {if(PPM_Comp<999)PPM_Comp++;}  		   	// K5   left toggle -
    if(Key_Buffer==KEY2)break; 
    Key_Buffer = 0; 
  }

  Title[OUTPUT][KIND].Value=6;    	                //set to OFF
  Update_Output();

  Key_Buffer = 0; 
  __Clear_Screen(BLACK);                                // clear the screen
  
  __Set(ADC_MODE, SEPARATE);                            // Set Separate mode
  __Set(ADC_CTRL, EN);       
  __Set(TRIGG_MODE, UNCONDITION);                       // set any trigger
  _Status = RUN;
  __Set(BEEP_VOLUME, 5*(Title[VOLUME][CLASS].Value-1)); // Reload volume
  Beep_mS = 500;                                        // buzzer ring 500mS
  Range = 0;
  Step  = 0;
  m     = 0;
  
  __Set(T_BASE_PSC, X_Attr[_100uS].PSC);                // T_BASE = 100uS
  __Set(T_BASE_ARR, X_Attr[_100uS].ARR);

  __Set(CH_A_COUPLE, DC);
  __Set(CH_B_COUPLE, DC);
  
  for(j=0; j<220; j+=20){                               // draw table
    for(i=0; i<399; i++){
      __Point_SCR(i, j);
      __LCD_SetPixl(WHT);
    }
  }

  for(i=0; i<399; i++){
    __Point_SCR(i, 239);
    __LCD_SetPixl(WHT);
  }
  __Point_SCR(  0,  0);
  for(j= 0; j<239; j++)  __LCD_SetPixl(WHT); 

  for(i=44;i<333;i+=44){
    __Point_SCR(i,20);
    for(j=20; j<201; j++)  __LCD_SetPixl(WHT); 
    if(i==132)i+=24;
  }

  __Point_SCR(398, 0);
  for(j= 0; j<239; j++)  __LCD_SetPixl(WHT); 

  Print_Str(  6, 185, 0x0005, PRN, "CH_A");              // display the form title bar
  Print_Str( 49, 185, 0x0005, PRN, "ZERO");
  Print_Str( 93, 185, 0x0005, PRN, "DIFF");
  Print_Str(137, 185, 0x0005, PRN, "VOLTAGE");
  Print_Str(206, 185, 0x0105, PRN, "CH_B");
  Print_Str(249, 185, 0x0105, PRN, "ZERO");
  Print_Str(293, 185, 0x0105, PRN, "DIFF");
  Print_Str(338, 185, 0x0105, PRN, "VOLTAGE");
    
  for(i=0; i<=G_Attr[0].Yp_Max; i++){
    Print_Str(  6, 166-(i*20), 0x0005, PRN, Y_Attr[i].STR); // display range
    Print_Str(206, 166-(i*20), 0x0105, PRN, Y_Attr[i].STR);
    Ma1[i] = Ka1[i];  Ma2[i] = Ka2[i];  Ma3[i] = Ka3[i];    // backup parameters before calibration 
    Mb1[i] = Kb1[i];  Mb2[i] = Kb2[i];  Mb3[i] = Kb3[i];
  }

  while (1){

/*
    if (__Get(USB_POWER)>0) PD_Cnt = 600;		 
    if(PD_Cnt == 0){               
      __Set(BACKLIGHT, 0);                               // turn off the backlight
      __Set(STANDBY, EN);                                // enter low power state
      return;
    }
*/

    PD_Cnt = 600;                                        // do we need to enable standby timer in calibration mode?

    __Set(CH_A_RANGE, Range);  __Set(CH_B_RANGE, Range);
    Delayms(20); 
    __Set(FIFO_CLR, W_PTR);
    Delayms(20); 
    a_Avg = 2048;  b_Avg = 2048;
    V_max=0; V_min=0;               

    for(i=0; i <4096; i++){
      TransferFIFO(i,0); 
      Tmp=(DataBuf[i] & 0xFF )-ADCoffset;    			   
      if(Channel == TRACK1){
        if(i==0)V_max=V_min=Tmp;
        if(Tmp>V_max)V_max=Tmp;
        if(Tmp<V_min)V_min=Tmp;
      }
      a_Avg+=Tmp;
      Tmp= ((DataBuf[i]>>8) & 0xFF )-ADCoffset;
      if(Channel == TRACK2){
        if(i==0)V_max=V_min=Tmp;
        if(Tmp>V_max)V_max=Tmp;
        if(Tmp<V_min)V_min=Tmp;
      }
      b_Avg+=Tmp;	
    }

    if(Step==7){    
      TmpA  = ((Ka1[Range]*10)+(((Ka2[Range]*10*((a_Avg/4096)-25))+ 5120)/1024));       //scaled X10, return with ShiftDigits=1 for int2str
      TmpB  = ((Kb1[Range]*10)+(((Kb2[Range]*10*((b_Avg/4096)-25))+ 5120)/1024));       //compensate re: 0V point
      if(Channel == TRACK1){
        Offset=((V_max-(a_Avg/4096))+(V_min-(a_Avg/4096)));				//estimate position of signal within step by
      }											//comparing the level of noise in the steps 
      if(Channel == TRACK2){								//above the signal to the noise in the the steps 
        Offset=((V_max-(b_Avg/4096))+(V_min-(b_Avg/4096)));                             //below the signal
      }
      if(Offset<-2)Offset=-2; if(Offset>2)Offset=2; Offset+=2;				//limit to +/- 2 steps
      Print_Str( 3*8, 208, 0x0305, PRN,(char*)CenterStep);				//31 chars		
      Print_Str( 36*8, 208, 0x0405, PRN,(char*)CenterStr[Offset]);			//11 chars    			
    }else{
      TmpA  = Ka1[Range] +(Ka2[Range]*(a_Avg/4096)+ 512)/1024;				//for offset calibration w/no signal
      TmpB  = Kb1[Range] +(Kb2[Range]*(b_Avg/4096)+ 512)/1024;
    }

    if(Blink){ 
      Blink = 0;
      switch (Step){  
      case 0:
        Range = 0;
        SetOffset(0,Range,40);
        SetOffset(1,Range,40);
    
        Print_Str( 2*8, 222, 0x0305, PRN,   "CAL W/LOW BATT (1/4 CHARGE)  THEN W/AC CHARGER");
        Print_Str(   8, 208, 0x0305, PRN,   "        PLEASE CONNECT");
        Print_Str(29*8, 208, 0x0305, PRN,   "INPUT TO ");
        Print_Str(38*8, 208, 0x0405, PRN,   "GND      ");
        Print_Str(   8,   6, 0x0305, PRN,   "   PRESS   KEY TO CONFIRM THE INPUT VOLTAGE   ");
        Print_Str(10*8,   6, 0x0405, Twink, " ");
        if(Channel == TRACK1){
          Print_Str( 23*8, 208, 0x0005, PRN, " CH_A ");
          for(i=0; i<=G_Attr[0].Yp_Max; i++){
            Ka1[i] = 0; Ka2[i] = 1024; Ka3[i] = 0;         // set calibration parameters to initial values
          }				     	
        }
        if(Channel == TRACK2){
          Print_Str( 23*8, 208, 0x0105, PRN, " CH_B ");
          for(i=0; i<=G_Attr[0].Yp_Max; i++){
            Kb1[i] = 0; Kb2[i] = 1024; Kb3[i] = 0;         // set calibration parameters of initial values
          }
        }
        break;
      case 1:
        Print_Str(   8,   6, 0x0305, PRN,   "   AUTOMATIC CALIBRATION IN PROGRESS...       ");
        if(Channel == TRACK1){
          s8ToPercen(n, TmpA - 40);
          Print_Str( 45, 166-(Range*20), 0x0005, INV, n);
          Ka1[Range] -= TmpA - 40; 
        } 
        if(Channel == TRACK2){
          s8ToPercen(n, TmpB - 40);
          Print_Str(245, 166-(Range*20), 0x0105, INV, n);
          Kb1[Range] -= TmpB - 40; 
        }
        Range++;
        if(Range > G_Attr[0].Yp_Max){ 
          Range = 0;  Step++;
        } 
        SetOffset(0,Range,40);
        SetOffset(1,Range,40);
        k = 0;
        break;
      case 2:
        k++;
        if(k >= 8) k = 0;
        if(Channel == TRACK1){
          s8ToPercen(n, TmpA - 40);
          Print_Str( 45, 166-(Range*20), 0x0005, PRN, n);
          if(TmpA - 40 != 0)  Ka1[Range] -= TmpA - 40;
          else                k = 0;           
        } 
        if(Channel == TRACK2){
          s8ToPercen(n, TmpB - 40);
          Print_Str(245, 166-(Range*20), 0x0105, PRN, n);
          if(TmpB - 40 != 0)  Kb1[Range] -= TmpB - 40;
          else                k = 0; 
        }
        if(k == 0)  Range++;
        if(Range > G_Attr[0].Yp_Max){ 
          Range = 0;  Step++;
        } 
        SetOffset(0,Range,40);
        SetOffset(1,Range,40);
        break;
      case 3:
        k++;
        SetOffset(0,Range,160);
        SetOffset(1,Range,160);
        if((Channel == TRACK1)&&(TmpA > 140))  Step++;
        if((Channel == TRACK2)&&(TmpB > 140))  Step++;
        if(k > 20)  Step++;
        break;
      case 4:
        k = 0;
        if(Channel == TRACK1){
          s8ToPercen(n, TmpA - 160);
          Print_Str( 89, 166-(Range*20), 0x0005, INV, n);
          Ka3[Range] -= (1024*(TmpA-160)+80)/160;
        } 
        if(Channel == TRACK2){
          s8ToPercen(n, TmpB - 160);
          Print_Str(289, 166-(Range*20), 0x0105, INV, n);
          Kb3[Range] -= (1024*(TmpB-160)+80)/160;
        }
        Range++;
        if(Range > G_Attr[0].Yp_Max){ 
          Range = 0;  Step++;
        } 
        SetOffset(0,Range,160);
        SetOffset(1,Range,160);
        break;
      case 5:
        k++;
        if(k >= 8) k = 0;
        if(Channel == TRACK1){
          s8ToPercen(n, TmpA - 160);
          Print_Str( 89, 166-(Range*20), 0x0005, PRN, n);
          if(TmpA - 160 != 0) Ka3[Range] -= (1024*(TmpA-160)+80)/160;
          else                k = 0;           
        } 
        if(Channel == TRACK2){
          s8ToPercen(n, TmpB - 160);
          Print_Str(289, 166-(Range*20), 0x0105, PRN, n);
          if(TmpB - 160 != 0) Kb3[Range] -= (1024*(TmpB-160)+80)/160;
          else                k = 0; 
        }
        if(k == 0)  Range++;
        if(Range > G_Attr[0].Yp_Max){ 
          Range = 0;  Step++;
        } 
        SetOffset(0,Range,160);
        SetOffset(1,Range,160);
        break;
      case 6:
        k++;
        if(k > 20)  Step++;
        Range = 0;
        if(m < 2){
          SetOffset(0,Range,40);
          SetOffset(1,Range,40);
          if((Channel == TRACK1)&&(TmpA < 50)){
            Step = 1;
            m++;
          }
          if((Channel == TRACK2)&&(TmpB < 50)){
            Step = 1;
            m++;
          }
        } else {
          SetOffset(0,Range,25);
          SetOffset(1,Range,25);
          if((Channel == TRACK1)&&(TmpA < 55))  Step++;
          if((Channel == TRACK2)&&(TmpB < 55))  Step++;
        }   

        if(Step==7){
          if(Channel == TRACK1){                                            
            BatLevel[0]=__Get(V_BATTERY);			            //capture corresponding bat level right after setting offsets	  
            for(i=0; i<=G_Attr[0].Yp_Max; i++) Ka2[i] = Ma2[i];             //load previously saved values as starting point
          }								    //so as to not have to start all over 	
          if(Channel == TRACK2){
            BatLevel[1]=__Get(V_BATTERY);					    
            for(i=0; i<=G_Attr[0].Yp_Max; i++) Kb2[i] = Mb2[i];
          }
          ClearTop();
        }
        break;
      case 7:
        Print_Str( 4*8, 222, 0x0305, PRN,   " INPUT ");				
        Print_Str(11*8, 222, 0x0405, Twink, (char*)VS_STR[Range]);
        Print_Str(20*8, 222, 0x0305, PRN,   " STANDARD VOLTAGE TO ");
        Print_Str(   8,   6, 0x0305, PRN,   "MODIFY VOLTAGE:   ...     ");      
        Print_Str(18*8,   6, 0x0405, Twink, "-");
        Print_Str(22*8,   6, 0x0405, Twink, "+");
        Print_Str(27*8,   6, 0x0305, PRN,   "SELECT RANGE:   ---   ");
        Print_Str(42*8,   6, 0x0405, Twink, "<");
        Print_Str(46*8,   6, 0x0405, Twink, ">");
        if(Channel == TRACK1){
          if(TmpA > 10){
            Int2Str(n, TmpA * Y_Attr[Range].SCALE, V_UNIT, 4, UNSIGN,1);	//show 4 digits, with decimal shifted left 1, with
          } else {								//values read at X10 scale. Allows better precision
            Int2Str(n, 0, V_UNIT, 3, SIGN,0);					//while adjusting gain calibration, -25 already done
          }
          Print_Str( 134, 166-(Range*20), 0x0005, Twink, n);
          Print_Str(41*8, 222,            0x0005, PRN,   "CH_A  ");
        } 
        if(Channel == TRACK2){
          if(TmpB > 10){
            Int2Str(n, TmpB * Y_Attr[Range].SCALE, V_UNIT, 4, UNSIGN,1);
          } else {
            Int2Str(n, 0, V_UNIT, 3, SIGN,0);
          }
          Print_Str( 334, 166-(Range*20), 0x0105, Twink, n);
          Print_Str(41*8, 222,            0x0105, PRN,   "CH_B  ");
        }
        break;
      case 8:                             //"    PRESS   ---   TO SELECT THE NEXT OPERATION"
        m = 0;
        Print_Str(   8,   6, 0x0305, PRN,   "    PRESS   ---  ");
        Print_Str(12*8,   6, 0x0405, Twink, "<");
        Print_Str(16*8,   6, 0x0405, Twink, ">");
        Print_Str(17*8,   6, 0x0305, PRN,   " TO SELECT THE NEXT OPERATION   ");
        Print_Str(   8, 216, 0x0305, PRN,   "  PRESS   TO ");
      //Print_Str(14*8, 216, 0x0405, PRN,   "CONFIRM THE RE-CALIBRATION ");
        Print_Str(14*8, 216, 0x0405, PRN,   "RESTART THE RE-CALIBRATION ");
        Print_Str( 9*8, 216, 0x0405, Twink, " ");       
        if(Channel == TRACK1)  Print_Str(41*8, 216, 0x0005, PRN, "CH_A "); 
        if(Channel == TRACK2)  Print_Str(41*8, 216, 0x0105, PRN, "CH_B ");
        break;                            //"  PRESS   TO CONFIRM THE RE-CALIBRATION CH_A  "
      case 9:                             //             "SELECT THE CALIBRATION CH_A      "
        Print_Str( 9*8, 216, 0x0405, Twink, " ");       
        Print_Str(14*8, 216, 0x0405, PRN,   "SELECT THE CALIBRATION ");
        if(Channel == TRACK1)  Print_Str(37*8, 216, 0x0105, PRN, "CH_B     "); 
        if(Channel == TRACK2)  Print_Str(37*8, 216, 0x0005, PRN, "CH_A     ");
        break;
      case 10:                            //             "Exit WITHOUT SAVING RESULTS      "
        Print_Str( 9*8, 216, 0x0405, Twink, " ");       
        Print_Str(14*8, 216, 0x0405, PRN, "Exit WITHOUT SAVING RESULTS      ");
        break;										
      case 11:                            //             "Exit AND SAVE CALIBRATION RESULTS"
        Print_Str(8, 222, 0x0405, PRN,   "PRESS BUTTON 2  TO SAVE IF CALIBRATING WITH BATT");
        Print_Str(8, 208, 0x0405, PRN,   " PRESS BUTTON 3  IF CALIBRATING WITH AC ADAPTER ");
        break;
      case 12:                            //             "Exit AND RESTORE SYSTEM DEFAULTS "
        Print_Str( 9*8, 216, 0x0405, Twink, " ");       
        Print_Str(14*8, 216, 0x0405, PRN,   "Exit AND RESTORE SYSTEM DEFAULTS ");
        break;
      }
    }
    if(Key_Buffer){ 
      PD_Cnt = 600;                               // reset the waiting timer to 600 seconds
      if((Range <= G_Attr[0].Yp_Max)&&(Step == 7)){
        if(Channel == TRACK1){
          Print_Str(134, 166-(Range*20), 0x0005, PRN, n);
        }
        if(Channel == TRACK2){
          Print_Str(334, 166-(Range*20), 0x0105, PRN, n);
        }
      } 
      switch (Key_Buffer){  
      case KEY2:
        if(Step == 0)  Step++;
        if((Step == 8)||(Step == 9)){
          ClearTop();
          if(Step == 9) Channel = 1 - Channel;
          for(i=0; i<=G_Attr[0].Yp_Max; i++){
            if(Channel == TRACK1){
              Print_Str( 45, 166-(i*20), 0x0005, PRN, "     ");   
              Print_Str( 89, 166-(i*20), 0x0005, PRN, "     ");   
              Print_Str(134, 166-(i*20), 0x0005, PRN, "       ");  
            }
            if(Channel == TRACK2){
              Print_Str(245, 166-(i*20), 0x0105, PRN, "     ");
              Print_Str(289, 166-(i*20), 0x0105, PRN, "     ");
              Print_Str(334, 166-(i*20), 0x0105, PRN, "       ");
            }
          }
          Step = 0;; 
        } 
        if(Step >= 10){
          if(Step == 10){
            for(i=0; i<=G_Attr[0].Yp_Max; i++){
              Ka1[i] = Ma1[i];  Ka2[i] = Ma2[i];  Ka3[i] = Ma3[i];
              Kb1[i] = Mb1[i];  Kb2[i] = Mb2[i];  Kb3[i] = Mb3[i];
            }
            Save_Param(0);                         // do not save the calibration parameters
            ClearTop();
            Print_Str( 8, 216, 0x0405, PRN, "          CALIBRATION DATA NOT SAVED            ");
          } 
          if(Step == 11){

	    if(BatLevel[0]>0){
              LoBatLevel[0]=BatLevel[0];    		           //Low battery correction reference levels
              for(i=0;i<10;i++){              
                LKa1[i]=Ka1[i];
                LKa2[i]=Ka2[i]; 
                LKa3[i]=Ka3[i];
              }
            }

  	    if(BatLevel[1]>0){
              LoBatLevel[1]=BatLevel[1];    		           
              for(i=0;i<10;i++){              
                LKb1[i]=Kb1[i];
                LKb2[i]=Kb2[i]; 
                LKb3[i]=Kb3[i];
              }
            }

            Save_Param(0);                         // save parameters after correction       "
            ClearTop();
            Print_Str( 8, 216, 0x0405, PRN, "SAVING THE CALIBRATION DATA FROM BAT LOW CORRECT");
          }  
          if(Step == 12){ 

              InitiateCalData();
              LoBatLevel[0]=0;
              LoBatLevel[1]=0;
              HiBatLevel[0]=0;
              HiBatLevel[1]=0;
              PPM_Comp=0;

            Save_Param(0);  // clear the calibration parameters, save the default values
            ClearTop();
            Print_Str( 8, 216, 0x0405, PRN, "       RESTORED DEFAULT CALIBRATION DATA        ");
          }
          Delayms(900);                                      
          App_init(1);
          Config_init();
          return;
        }
        break;
      case KEY3:
        if(Step == 11){
	  if(BatLevel[0]>0){
              HiBatLevel[0]=BatLevel[0];    		           //High battery correction reference levels
              for(i=0;i<10;i++){              
                HKa1[i]=Ka1[i];
                HKa2[i]=Ka2[i]; 
                HKa3[i]=Ka3[i];
              }
          }
  	  if(BatLevel[1]>0){
              HiBatLevel[1]=BatLevel[1];    		           
              for(i=0;i<10;i++){              
                HKb1[i]=Kb1[i];
                HKb2[i]=Kb2[i]; 
                HKb3[i]=Kb3[i];
              }
          }
          Save_Param(0);                         // save parameters after correction       "
          ClearTop();
          Print_Str( 8, 216, 0x0405, PRN, " SAVING THE CALIBRATION DATA FROM HIGH CORRECT  ");
          Delayms(900);                                      
          App_init(1);
          Config_init();
          return;
        }

        break;
      case K_ITEM_DEC:
        if((Step == 7)&&(Range > 0)) Range--;  
        if( Step >= 9)  {ClearTop(); Step--;}
        if( Step == 8)               Step = 12;
        break;
      case K_ITEM_INC:
        if(Step >= 8)  {ClearTop();Step++;}
        if(Step > 12)  Step = 8;
        if(Step == 7)  Range++;  
        if(Range  > G_Attr[0].Yp_Max){  
          Range = 0;        
          ClearTop();
          Step++;
        }
        break;
      case K_INDEX_DEC:
        if(Step == 7){  
          if((Channel == TRACK1)&&(TmpA > 35))  Ka2[Range] --;   //all were -=2/+=2
          if((Channel == TRACK2)&&(TmpB > 35))  Kb2[Range] --;
        }
        break;
      case K_INDEX_INC:
        if(Step == 7){  
          if((Channel == TRACK1)&&(TmpA > 35))  Ka2[Range] ++;
          if((Channel == TRACK2)&&(TmpB > 35))  Kb2[Range] ++;
        }
        break;
      }
      Key_Buffer = 0;
    }
  }
}

void ClearTop(void){
u8  i;
u16 j;
          for(i=201;i<239;i++){
            for (j=1;j<398;j++){	      
              __Point_SCR(j,i);	              	
              __LCD_SetPixl(BLACK);
            }
          }
}


/*********************************  END OF FILE  ******************************/
