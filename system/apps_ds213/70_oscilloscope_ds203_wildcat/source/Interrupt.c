/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : Interrupt.c  
 Version   : DS203_APP Ver 2.3x                                  Author : bure
*******************************************************************************/
#include "Interrupt.h"
#include "Function.h"
#include "Menu.h"
#include "BIOS.h"
#include "Process.h"

vu8  Cursor_Cnt, Key_Wait_Cnt, Key_Repeat_Cnt, Key_Buffer, Cnt_20mS,Key_Mid_Speed=0,RepeatTimerR=0,RepeatTimerL=0;         //                , IntCount, ModInit=0;
vu16 Cnt_mS;

vu8  Twink, Blink,ClearStatus=0;
u8   Volume=20, Light, LoBeepLevel=0;
vu16 Delay_Cnt, Beep_mS, Key_Status_Last, Sec_Cnt, PD_Cnt, NotificationTimer=0,DelayLoopBreak=0,SpecFrameCount,
                PersHoldTimer, SecondsTick, ConfNameTimer, CursorDisplayTimer, AutoSetTimer;
vu8 Tim2Factor=1;
u16 ToneBurstCounter=0;
vu8  BurstLimit=0;
u8  SweepResetFlag;
vs16 NoiseIP[100];
vs16 FilterOut;
vu32 LimitTransfer;
vu8  FineAdjust=2;
u8   FineAdjustLimit=4;
vu16 FineAdjustFactor[17]={10000,5000,2000,1000,500,200,100,50,20,10,5,2,1,1,1,1,1};

vu32 Wait_Cnt; 

void TIM2_IRQHandler(void)
{

  u16 i;               
  u32 Limit=0;         
  u32 Reset;

  Reset=LimitTransfer;
  if((_Kind==5)&&(_Frqn>15))DAC->DHR12R1=(u32)RandomGen();  // take over seeding from int4 above 100khz limit

//==============================================SWEEP MODE===============================================          
if(_Kind<5){ 
 if((Sweep==1)&&((Cnt_mS%SweepMod)==0)){                                                 //SWEEP MODE 
//------------------------------------------RESET SWEEP TO START-----------------------------------------

   if(GenFreqShift<=-100){                                                               //     >>> NEED TO TEST UPDATE EVENT FLAG AT INIT
               
     if(_Frqn<=SweepIndex){               //LOW FREQS AND FAST SWEEP RATES (just to keep program going, not normally used...)
                
       if(SweepResetFlag==0){
          
         if(_Kind == PWM){
           TIM4->SR &= 0xFFFE;                                                           //update event bit reset  
         }else {                                       
           DMA2->IFCR |= 0x2000;                                                         //reset finish bit
         }
         SweepResetFlag=1;
         goto BypassFunction;
        
       }else{     
       
         if(_Kind == PWM){
           if((TIM4->SR&1)==0)goto BypassFunction;
         }else {                                       
           if((DMA2->ISR & 0x2000)==0)goto BypassFunction;
         }
         SweepResetFlag=0;
         GenFreqShift=100;                                                               //reset and use as counter to generate space
         ClearStatus=1;                                                                  //starts space between sweeps
           
         if(_Kind == PWM){
             for(i=0;i<720;i++)ATT_DATA[i]=2048;                                         //clear ATT_DATA to create space
             __Set(ANALOG_ARR, A_Tab[9]);                                                  //shuts off PWM, turns on Analog
         }else{                                       
           for(i=0;i<720;i++)ATT_DATA[i]=2048;                                           //clear ATT_DATA to create space
           OutputAdjust();
         }
               
       }                                                                                //if sweepresetflag
           
     }else{                             //HIGH FREQS AND LOW SWEEP RATES, (if GenFreqShift<=-100)
                                        //END OF SWEEP: SHUT OFF WAVE AND START SPACE                 
       GenFreqShift=100;                                                               //reset and use as counter to generate space
       ClearStatus=1;                                                                  //starts space between sweeps
       Limit=0;
         
       if(_Kind == PWM){

           if(_Frqn<15){ 
             TIM4->SR &= 0xFFFD;                                                           //capture compare event bit reset  
             while(Limit++<Reset){                                               
               if(TIM4->SR&2){                                                             //wait for capture compare event
                 TIM4->CCMR1=0x005C;                                                       //force output high
                 break;                                   
               }
             }
             TIM4->SR &= 0xFFFE;                                                           //update event bit reset  
             Limit=0; while(Limit++<Reset){if(TIM4->SR&1)break;}                         //wait for wave (forced high) to finish             
             TIM4->CCMR1=0x0078;                                                           //restore capture compare output
           }else if(_Frqn<17){
             while(Limit++<7){}                                                           //delay, attempt to catch wave when high
           }

           GPIOB->CRL = 0x34BBB438;       // puts PWM port(TIM4/PB6) in input, floating mode + configures other bits  
           TIM4->CR1 = 0x0080;            // SQR_OUT = Disable. Stops counter 0x80=auto reload preload enable
                                          // PA4 and PB6 hard wired together as common output
           GPIOA->CRL   = 0x111011BB;     // puts analog (DAC port PA4) in input, analog mode + configures other bits
           DAC->CR = 0x0003;              // DAC= Enable without output buffer (connects DAC to port PA4, also PB6 through external connection)
           TIM7->CR1 = 0x0085;            // DAC_CLK = Enable (update on overflow only)
           TIM4->EGR=0x0001;              // generate update event for full reset in case counter stopped before reset
       }else {                                                                           

         if((_Frqn==15)&&(_Kind<2)&&(SweepIndex==8))for(i=0;i<(M_Factor*48);i++)Waste++;         //100Khz delay for sine/triangle

         DMA2->IFCR |= 0x2000;                                                                   //reset finish bit
         while((DMA2->ISR & 0x2000)==0){if(Limit++>Reset)break;}                                 //wait for waveform to finish
         DMA2->IFCR|=0x00002000;                     //clear transfer complete int flag

         for(i=0;i<720;i++)ATT_DATA[i]=2048;                                                     //clear ATT_DATA to create space
         OutputAdjust();
       }
     }                                                                                           //if(_Frqn<=SweepIndex), else
//------------------------------------------SWEEP/SPACE COUNT DOWN-----------------------------------------
   }else{                                                                 //if(GenFreqShift<=-100)
     GenFreqShift-=SweepStep;                                             //sweep or use to create space
     if((ClearStatus)&&(GenFreqShift<40)){                                //end space           space=100-xx mS
       GenFreqShift=100;                                                  //reset after space
       if(_Kind == PWM){                     //START OF NEW SWEEP 
           TIM4->CNT=TIM4->ARR;                                           //load counter to auto reload value for quick reset
           TIM4->CCER=0x0001;                                             //turn PWM back on if turned off
           TIM4->PSC = D_Tab[_Frqn].PSC; 
           GPIOA->CRL  |= 0x40000;                                        // DAC_OUT = Disable (puts analog port(PA4)in input, floating mode)
           TIM7->CR1 = 0x0084;                                            // DAC clock disable
           DAC->CR = 0;                                                   // DAC = Disable
           EnablePWM=1;
       }else{
         DMA2_Channel4->CCR &= ~DMA_CCR1_EN;
         WaveGen();                                                       //reload ATT_DATA
         ResetDMA2_Ch4Params(720/ScaleIndex[_Frqn]);
       }
       ClearStatus=0;
     }

   } 
   if(ClearStatus==0) OutputAdjust();

//==============================================BURST MODE===============================================          
 }else if((Sweep>1)&&((Cnt_mS%SweepMod)==0)){                                          
//------------------------------------------RESET BURST TO START-----------------------------------------

   if((ClearStatus==0)&&(ToneBurstCounter>=BurstLimit)){                                  //burst limit = xx to 200

     if(_Frqn<=SweepIndex){             //FOR LOW FREQS AND HIGH SWEEP SPEEDS

       if(SweepResetFlag==0){
         if(_Kind == PWM){
           TIM4->SR &= 0xFFFE;                                                           //update event bit reset  
         }else {                                       
           DMA2->IFCR |= 0x2000;                                                         //reset finish bit
         }
         SweepResetFlag=1;
         goto BypassFunction;
       }else{ 
         if(_Kind == PWM){
           if((TIM4->SR&1)==0)goto BypassFunction;                                       //this section to provide program flow only, not likely to be
         }else {                                                                         //using burst freq lower than burst rate
           if((DMA2->ISR & 0x2000)==0)goto BypassFunction;
         }
         SweepResetFlag=0;
         ToneBurstCounter=0;
         ClearStatus=1;                                                                  //starts space between sweep
         if(_Kind == PWM){
           TIM4->CCER=0x0000;                                                            //shut off PWM
         }else{                                       
           for(i=0;i<720;i++)ATT_DATA[i]=2048;                                           //clear ATT_DATA to create space
         }
         OutputAdjust();                                                                 //reset for beginning of new burst
       }                                                                                 //if sweepresetflag 

     }else{                              //HIGH FREQS AND LOW SWEEP SPEEDS
                                                                                       //END BURST, START SPACE
       ToneBurstCounter=0;
       ClearStatus=1;                                                                  //starts space between sweeps
       Limit=0;

       if(((SweepIndex<2)&&(_Frqn>5))||(_Frqn>13))for(i=0;i<(FineAdjust*FineAdjustFactor[_Frqn-6]);i++)Waste++;

       if(_Kind == PWM){                                                               //SHUT OFF PWM
         TIM4->SR &= 0xFFFE;                                                           //update event bit reset  
         while((TIM4->SR&1)==0){if(Limit++>Reset)break;}                               //wait for timer to reset signaling end of waveform                             
         TIM4->CCER=0x0000;                                                            //shut off PWM
       }else {                                                                         //SHUT OFF ANALOG WAVES
         DMA2->IFCR |= 0x2000;                                                         //reset finish bit 
         while((DMA2->ISR & 0x2000)==0){if(Limit++>Reset)break;}                       //wait for waveform to finish
         for(i=0;i<720;i++)ATT_DATA[i]=2048;                                           //clear ATT_DATA to create space
       }
       OutputAdjust();                                                                 //reset for beginning of new burst

     }                                                                                 //if(_Frqn<=SweepIndex)
//------------------------------------------BURST/SPACE COUNTDOWN-----------------------------------------
   }else{                                                                              //((ClearStatus==0)&&(ToneBurstCounter>BurstLimit))

     ToneBurstCounter+=SweepStep;                                                      //sweep or use to create space
     if((ClearStatus)&&(ToneBurstCounter>((200-BurstLimit)+60))){                      //END SPACE, START NEW BURST           
       ToneBurstCounter=0;                                                             //reset after space
       if(SweepIndex<2)Cnt_mS=19;                                                              //ModInit=1;else ModInit=0;    //special case for sweepmod>1 
       if(_Kind == PWM){
         TIM4->CNT=0x0000;                                                             //reset counter
         TIM4->CCER=0x0001;				                               //TURN PWM BACK ON
       }else{                                                                          //TURN ANALOG WAVES BACK ON
         DMA2_Channel4->CCR &= ~DMA_CCR1_EN;
         WaveGen();                                                                    //reload ATT_DATA
         ResetDMA2_Ch4Params(720/ScaleIndex[_Frqn]);
       }
       ClearStatus=0;
     }  

   } 
   if(ClearStatus==0)OutputAdjust();

 }                                                                                    //if sweep>1
}
//===================================================END====================================================
BypassFunction:

 TIM2->SR=0;
}



void TIM3_IRQHandler(void)
{
  
 u8 KeyCode;

  SpecFrameCount++;
  __Set(KEY_IF_RST, 0);                      //Clear TIM3 interrupt flag
  if(Cnt_mS > 0)   Cnt_mS--;
   else {

    //Cnt_mS=20;                             //use with 1mS IRQ timing 
    if((SweepIndex<2)&&(Sweep>1)&&(_Kind<5))
      Cnt_mS=19; else Cnt_mS=20;             //resetting to 19 provides correct 20mS timing since it includes 0 (necessary for SweepIndex 0 and 1)        
                                             //reset to 20 since other parts of program already compensate for this

    if(Wait_Cnt >0)  Wait_Cnt--;	     //for Synchro	
    if(Delay_Cnt >20) Delay_Cnt -= 20;
    else              Delay_Cnt  = 0;
     if(Beep_mS >=20)  Beep_mS   -= 20;
       else              __Set(BEEP_VOLUME, 0); // Beep off
    if(Cnt_20mS < 50) Cnt_20mS++;            //here again, this goes from 0 to 50, providing 1.02 seconds timing but is not a big issue here
    else {                                   // Do it pre sec.
      Cnt_20mS = 0;
      __Set(BETTERY_DT, 1);                  //Battery Detect
      Sec_Cnt++;
      if(PD_Cnt > 0) PD_Cnt--;
    }
    Cursor_Cnt++;
    if (NotificationTimer==80)Cursor_Cnt=0;  //synchronize blinker with message 
    if(Cursor_Cnt >= 15) {                   //12*20mS=240mS  15=300ms
      Cursor_Cnt=0;
      Twink=!Twink;
      Blink = BLINK;
    }
        if(SecondsTick>0)SecondsTick--;			//int based timers
 	if(DelayLoopBreak>0)DelayLoopBreak--;
	if(NotificationTimer>0)NotificationTimer--;
        if(PersHoldTimer>0)PersHoldTimer--; 
        if(ConfNameTimer>0)ConfNameTimer--;
        if(CursorDisplayTimer>0)CursorDisplayTimer--;
        if(AutoSetTimer>0)AutoSetTimer--;

    if(RepeatTimerR)    RepeatTimerR--;
    if(RepeatTimerL)    RepeatTimerL--;
    if(Key_Mid_Speed)   Key_Mid_Speed--;	
    if(Key_Wait_Cnt)    Key_Wait_Cnt--;
    if(Key_Repeat_Cnt)  Key_Repeat_Cnt--;
    KeyCode=Read_Keys();
    if(KeyCode !=0) {
      Key_Buffer = KeyCode;
      if(SetLowVolume){
        __Set(BEEP_VOLUME,((9-LoBeepLevel)*5*(Title[VOLUME][CLASS].Value-1))/9);    // Fast key repeat volume
      }else{
        __Set(BEEP_VOLUME, 5*(Title[VOLUME][CLASS].Value-1));                       // Volume
      }
      Beep_mS = 60; 
    }
  }
}



void TIM4_IRQHandler(void){                                                         //noise filter
 u16 i;
 static s32 tmp;
 static u8 index;
 static u8 SeedIndex=0;

 if(_Kind==5){
  if(InitiateNoise){
    DAC->DHR12R2=(u32)RandomGen();
    tmp=0;
    index=0;
    InitiateNoise=0;
    FilterOut=0x0800;
    for(i=0;i<100;i++)NoiseIP[i]=0;
    tmp=0;
  }else{
    if(_Frqn<8){
      DAC->DHR12R1=(u32)FilterOut;
      NoiseIP[index]=((u32)DAC->DOR2&0x0FFF)-0x0800;              
      tmp+=NoiseIP[index];         
      if(index<99)tmp-=NoiseIP[index+1];
        else {tmp-=NoiseIP[0]; DAC->DHR12R2=(u32)RandomGen();}             //seed for DAC2 from seed generator mod 100
      if(_Frqn<3)FilterOut=(tmp/25)+0x0800;                                //normalize output 
          else if(_Frqn<6)FilterOut=(tmp/27)+0x0800;
            else FilterOut=(tmp/30)+0x0800;
      if(++index>99)index=0;
    }else if(_Frqn<14){
      DAC->DHR12R1=(u32)FilterOut;
      NoiseIP[index]=((u32)DAC->DOR2&0x0FFF)-0x0800;              
      tmp+=NoiseIP[index];         
      if(index<9)tmp-=NoiseIP[index+1];
        else tmp-=NoiseIP[0]; 
      if(++SeedIndex>17){DAC->DHR12R2=(u32)RandomGen();SeedIndex=0;}       //seed for DAC2 from seed generator mod 17          
      if(_Frqn<12)FilterOut=(tmp/10)+0x0800;
            else FilterOut=(tmp/8)+0x0800;
      if(++index>9)index=0;
    }else if(++SeedIndex>(17)){DAC->DHR12R1=(u32)RandomGen();SeedIndex=0;}
 
 }
  if(FilterOut<0)FilterOut=0;else if(FilterOut>0x0FFF)FilterOut=0x0FFF;    //provide proper clipping limits
 }
 TIM4->SR=0;
}
