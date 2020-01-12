/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : Function.c  
 Version   : DS203 APP Ver 2.5x                          Author : bure & Kewei
*******************************************************************************/
#include "Interrupt.h"
#include "Function.h"
#include "BIOS.h"
#include "Menu.h"
#include "Process.h"
#include "File.h"

// Since we only use 3/4 of N_WAVE, we define only
// this many samples, in order to conserve data space.

u8 SetLowVolume=0;
s16 Sinewave[N_WAVE-N_WAVE/4];

sc16 SineQuad[129] = {
	0,	402,	804,	1206,	1608,	2009,	2410,	2811,
	3212,	3612,	4011,	4410,	4808,	5205,	5602,	5998,
	6393,	6786,	7179,	7571,	7962,	8351,	8739,	9126,
	9512,	9896,	10278,	10659,	11039,	11417,	11793,	12167,
	12539,	12910,	13279,	13645,	14010,	14372,	14732,	15090,
	15446,	15800,	16151,	16499,	16846,	17189,	17530,	17869,
	18204,	18537,	18868,	19195,	19519,	19841,	20159,	20475,
	20787,	21096,	21403,	21705,	22005,	22301,	22594,	22884,
	23170,	23452,	23731,	24007,	24279,	24547,	24811,	25072,
	25329,	25582,	25832,	26077,	26319,	26556,	26790,	27019,
	27245,	27466,	27683,	27896,	28105,	28310,	28510,	28706,
	28898,	29085,	29268,	29447,	29621,	29791,	29956,	30117,
	30273,	30424,	30571,	30714,	30852,	30985,	31113,	31237,
	31356,	31470,	31580,	31685,	31785,	31880,	31971,	32057,
	32137,	32213,	32285,	32351,	32412,	32469,	32521,	32567,
	32609,	32646,	32678,	32705,	32728,	32745,	32757,	32765,
        32767};

void LoadNwave(void){
u16 i;
u8 j=0;
  for(i=0;i<384;i++){
    if(i<128)Sinewave[i]=SineQuad[j++];
      else if(i<256)Sinewave[i]=SineQuad[j--];
        else Sinewave[i]=-SineQuad[j++];
  }
}

/*******************************************************************************
 
*******************************************************************************/
u32 Power(u8 x, u8 y) 
{ 
  u32 m = x;
  
  if(y == 0) return 1;
  while (--y) m *= x;
  return m;
}
/*******************************************************************************
 delayms:  milliseconds (mS) time delay procedure		Input: milliseconds to wait (72MHz frequency case)
*******************************************************************************/
void Delayms(u16 mS) 
{ 
  Delay_Cnt = mS;                //note that this times in steps of 20mS, with 1st 20mS being undefined: EG 1-20= ?-20, 21-40=?+20
  while (Delay_Cnt > 0){}
}

/*******************************************************************************
 Sign_int2Str: 32-digit switch to e-bit effective number of strings + dimensionless string
******************************************************************************/

void Int2Str(char *p, s32 n,const char *pUnit, u8 e, u8 Mode, s8 ShiftDigits)
{
  s32 i, j, m, c,mm;							//e= number of digits
  char  *k;								//Mode= SIGN=0 UNSIGN=1 STD=2
  u8  v=0;
  u8 numdigits=e;
  char id=pUnit[0];
  if((Mode==0)&&((n>=100000000)||(n<=-100000000)))ShiftDigits=3;   //special case for volts>100 to use "V" and remove period
  s32 nn=n;
  if(n == 0x80000000)
  {               // This value is defined as an invalid value
    *p++ = ' ';
	*p++ = '.';
    while(--e)  *p++ = ' ';
    if ((id=='u')&&(numdigits==3))*p++=' '; 
    *p = 0;  return;
  }
  if(Mode == SIGN)
  {
    if(n>0)
    {
      *p++ = '+';
      v='+';
    }
    if(n<0)
    {
      *p++ = '-';
      n=-n;
      v='-';
    }
    if(n==0)
    {
     *p++ = '0';
     *p = '.';
    }
  }
  m = n;			    //n= s32 number input 	
  i = 0;
  c = 5;

  while(m >= 10)                    // detection of n significant digits
  {				    //m holds input	
     m /= 10;			    //divide by 10 > In integer math, this just gets rid of the last digit
	 if(++i > e)    	    //count digits, add the one just eliminated and compare with e 
		c *= 10;            //if larger than e, multiply c (initiated at 5) by 10
  }                                 	//number of significant digits than e calculate the rounded median

  if(i >= e)                        	// plus the rounded value
	n += c;			    //add 5xxx to the next digit after e number of digits, provides half digit bias for rounding off
  m = n;			    //reload input with added bias into m
  mm= n;
  i = 0;

  while(m >= 10)                    	// re-detection of n significant digits
  {
	m /= 10;
	i++;
  }

  i++;   			    //to make up for i equaling number of digits minus 1
                                    //n now holds input, biased with 5 after least significant digit according to e number of digits

  
  i-=ShiftDigits;		    //shift number of digits back down to correct number if using up-scaling to increase resolution
 

  if(e > 3)			    //i holds the number of digits in input number 
  {
    m = (i-1)/3;		      
    i %= 3;
    if(i == 0)
		i = 3;
  }
  else				    //e=3 or less, normal integer division/modulo
  {
    m = i/3;
    i %= 3;
  }
  
  p += e;			    //p=output string pointer, inc by called for number of digits
  k = p+1;                          //pointer position for suffix
  				 						
  while(n >= Power(10, e))	    //strip away non significant digits, leaving the called for e number of digits in n
	n /= 10;  						      	 // interception of the highest e effective number of bits(digits...)

				    
  for(j=0; j<m; j++)		    // move pointer m "slots" to nul in front of desired string
	while(*pUnit++);

  if(ShiftDigits>0)m=1;		    //allows up-scaling to place decimal point in proper place, m now no longer relevant otherwise.

  if ((_T_Range<3)&&(id=='u')&&(numdigits==3)) while(*pUnit++);	      // special case for very large deltaT intervals, move to proper suffix

  do *k++ = *pUnit;						      // copy desired string + nul
  while(*pUnit++);	 						 // dimensional character string
 
  while(e--)                                   //in modes other than SIGN, p has only been increased by e...
  {
    *p-- = '0'+(n%10);			       // copy the digits of the value, n has had non significant digits stripped off
    n /= 10;				       // strip away last digit
    if((Mode != STD)&&(m > 0)&&(i == e))	      //&&(Mode != SIGN)
     {					       //if Mode is SIGN or UNSIGN, m>0 input (n) has at least as many digits as e(called for),
      //*p-- = '.';                              //i=e(digit position in number), insert point and move pointer down 1 position 
        if(ChartLogic()){
         if((Mode==0)&&((mm>=100000000)||(mm<=-100000000)))*p-- = ' ';else *p-- = '.';    // special case with rounding S>100, remove period 
       }else{
         if((Mode==0)&&((nn>=100000000)||(nn<=-100000000)))*p-- = ' ';else *p-- = '.';    // special case v>100, remove period 
       } 
     }                                         
  }                                               

  if((Mode!=STD)&&(Mode!=SIGN)&&(m==0))        //mode is UNSIGN
  {                                            //this is putting a 0. in front of a shifted range, 
    p++; //*p++;                               //putting a 0. effectively shifts the range, so it is also effectively changing the suffix,
    *p-- = '.';                                //throws reading way off. Need to not put point, either spaces of 0's
    *p= '0';                                   //OR keep range suffix the same...> fixed by shifting digits
  }
 
  if((Mode==STD)&&(m==0))        //m>0 if, with e(called for #of digits)>3 and original n(input number) had greater than 3 digits
    *p-- = ' ';                  //OR, if e=3 or less, original n had greater than 2 digits 	
		                 //insert space, but p now points on char before position passed (?)		     	

  if((Mode == SIGN)&&(p[1] == '.')&&(p[0] != '0')){
   if ((id=='u')&&(numdigits==3)){				// identify time vernier, no need for +, allow suffix to show properly
     p[0]='0';
   }else{
     p[7]=0;
     p[6]=p[5];
     p[5]=p[4];
     p[4]=p[3];
     p[3]=p[2];
     p[2]='.';
     p[1]='0';
     if(v=='-')
      p[0]='-';
     else
      p[0]='+';
   }
  }

  if ((id=='u')&&(numdigits==3)){			// identify time vernier
    if (p[0]=='+')p[0]=' ';			// strip off "+" sign on rest of readings to match values that start with 0
    if (nn<100){p--; p[0]='0'; p[1]='.';}	// special case for very small values
  }
}


/*******************************************************************************
 Two ASCII character Change to 1 Byte HEX data 
*******************************************************************************/
/*
u8 Str2Byte(char x,char y) // double-ASCII characters to 1 byte binary number		//these 2 functions not used...
{
  uc8 Hexcode[17]="0123456789ABCDEF";
  u8 i, Temp=0;
  
  if(x>='a' && x<='z')  x-=32;     // lowercase change to uppercase
  if(y>='a' && y<='z')  y-=32;     // lowercase change to uppercase
  for(i=0;i<16;i++){
    if(Hexcode[i]==x)  Temp+=i*16; // characters into the high four hexadecimal values
  }
  for(i=0;i<16;i++){
    if(Hexcode[i]==y)  Temp+=i;    // characters into four hexadecimal values
  }
  return Temp;
}

// *******************************************************************************
// u16ToDec5: Change 2 Byte to 5 decimal number string   
// *******************************************************************************

void u16ToDec5(char *p, u16 n)
{
    *p++ = '0'+n/10000;
    n %= 10000;
    *p++ = '0'+n/1000;
    n %= 1000;
    *p++ = '0'+n/100;
    n %= 100;
    *p++ = '0'+n/10;
    n %= 10;
    *p++ = '0'+n;
    *p = 0;
}
*/
/*******************************************************************************
 u8ToDec3: Change Byte to 3 decimal number string   
*******************************************************************************/
void u8ToDec3(char *p, u16 n,u8 service)                //will also work as u16 to decimal 3   
{
u8 i;
    if(service){*p++ = '0'+n/1000; n %= 1000;}		//allow function to work as a u16 to decimal 4
    for(i=100;i>0;i/=10){
      *p++ = '0'+n/i;
      n %=i;
    }
    *p = 0;
}

void u32ToDec7(char*p,u32 n){                            //need to consolidate these functions to a uToDec(x decimals) 
u32 i;
    for(i=1000000;i>0;i/=10){
      *p++ = '0'+n/i;
      n %=i;
    }
    *p = 0;
}


/*******************************************************************************
 s8ToPercen: Change sign char to +(-)x.xx string   
*******************************************************************************/
void s8ToPercen(char *p, s8 n)
{
    if(n >= 0)  *p++ = '+';
    else {
      *p++ = '-';
      n = -n;
    }
    *p++ = '0'+n/100;
    n %= 100;
    *p++ = '.';
    *p++ = '0'+n/10;
    n %= 10;
    *p++ = '0'+n;
    *p = 0;
}
/*******************************************************************************
 u8ToDec2: Change Byte to 2 decimal number string   
*******************************************************************************/
void u8ToDec2(char *p, u8 n)
{
//    *p++ = '0'+n/100;			
    n %= 100;
    *p++ = '0'+n/10;
    n %= 10;
    *p++ = '0'+n;
    *p = 0;
}
/*******************************************************************************
 Char2Hex: Change Byte to 2 hex number string  
*******************************************************************************/

void Char2Hex(char *p, u8 n)			
{
    if(n/16>9) *p++ = 'A'+(n/16-10);
    else       *p++ = '0'+n/16;
    n %= 16;
    if(n>9)    *p++ = 'A'+(n-10);
    else       *p++ = '0'+n;
    *p = 0;
}
/*******************************************************************************
 Shor2Hex: Change 2Bytes to 4 hex number string  
*******************************************************************************/
/*
void Shor2Hex(char *p, u16 n)
{
    if(n/0x1000 >9) *p++ = 'A'+(n/0x1000-10);
    else            *p++ = '0'+ n/0x1000;
    n %= 0x1000;
    if(n/0x100 >9)  *p++ = 'A'+(n/0x100-10);
    else            *p++ = '0'+ n/0x100;
    n %= 0x100;
    if(n/0x10 >9)   *p++ = 'A'+(n/0x10-10);
    else            *p++ = '0'+ n/0x10;
    n %= 0x10;
    if(n >9)        *p++ = 'A'+(n-10);
    else            *p++ = '0'+n;
    *p = 0;
}*/

/*******************************************************************************
 Word2Hex: Change 4 Bytes to 8 hex number string  
*******************************************************************************/

void Word2Hex(char *p, u32 n){
u32 Ref=0x10000000;
    do{
      if(n/Ref>9)*p++='A'+(n/Ref-10);
        else *p++='0'+n/Ref;
      n %=Ref;
      Ref/=0x10;
    }while(Ref>0);
    *p = 0;
}

/*******************************************************************************
 Int_sqrt: unsigned int square root
*******************************************************************************/
u32 Int_sqrt(u32 n)


/*{ u32 k;					
  if ( n == 0 ) return 0;
  k = 2*Int_sqrt(n/4)+1;
  if ( k*k > n ) return k-1;
  else return k;
}*/

{						
    u8 Round=0;
    unsigned int c = 0x8000;
    unsigned int g = 0x8000;

    if (n<10000000)Round=1;             //round off only smaller values, otherwise up-scaling limits dynamic range

    if(Round)n*=100;			//scale up to allow bias to center zero for integer math, provides proper rounding off
    for(;;) {				
        if(g*g > n)                 
            g ^= c;                 
        c >>= 1;				
        if(c == 0){
            if(Round)g+=5;		//add bias
            if(Round)g/=10; 		//scale back down, was scaled up to the square root of 100, =10			
            return g;
        } 
        g |= c;				
    }
}

/*******************************************************************************
Read_Keys:    Key status process                                Return: KeyCode
*******************************************************************************/
u8 Read_Keys(void)
{
  u16 Key_Status, Key_Status_Now;
  u8  KeyCode=0;

  Key_Status_Now = ~__Get(KEY_STATUS);//~KeyScan();
  Key_Status = Key_Status_Now &(~Key_Status_Last);
  if(Key_Status){                               // New key push on
    SetLowVolume=0;
    Key_Wait_Cnt=30;                        // Close auto repeat 25=50mS
    Key_Mid_Speed=100;
    if(Key_Status & K_ITEM_D_STATUS)   KeyCode = K_ITEM_DEC;    // K9    right toggle -
    if(Key_Status & K_ITEM_S_STATUS)   KeyCode = K_ITEM_S;      // K10
    if(Key_Status & KEY3_STATUS)       KeyCode = KEY3;          // K3 
    if(Key_Status & KEY4_STATUS)       KeyCode = KEY4;          // K4 
    if(Key_Status & K_INDEX_D_STATUS)  KeyCode = K_INDEX_DEC;   // K5   left toggle -
    if(Key_Status & K_INDEX_I_STATUS)  KeyCode = K_INDEX_INC;   // K6   left toggle +
    if(Key_Status & K_INDEX_S_STATUS)  KeyCode = K_INDEX_S;     // K7  
    if(Key_Status & KEY2_STATUS)       KeyCode = KEY2;          // K2
    if(Key_Status & KEY1_STATUS)       KeyCode = KEY1;         // K1
    if(Key_Status & K_ITEM_I_STATUS)   KeyCode = K_ITEM_INC;    // K8   right toggle +
  } else {
    if(Key_Status_Now & Key_Status_Last){       // Key push hold on
      if((Key_Wait_Cnt || Key_Repeat_Cnt)==0){
        if(Key_Status_Now & K_INDEX_D_STATUS)  KeyCode = K_INDEX_DEC;   // K5 
        if(Key_Status_Now & K_INDEX_I_STATUS)  KeyCode = K_INDEX_INC;   // K6


        if((Key_Status_Now & K_INDEX_D_STATUS)||(Key_Status_Now & K_INDEX_I_STATUS)){   
	  switch (Current){                                                                     //fast toggle repeat rate for:
	    case T_VERNIE:                                                                      //time cursors
	    case V_VERNIE:                                                                      //volt cursors
              Key_Repeat_Cnt = 2;
              break;
	    case TRIGG:	
              if ((_Det==2)||(_Det==3))Key_Repeat_Cnt = 2; else Key_Repeat_Cnt = 5;  		//trigger level or trigger delay value
              break;
	    case OUTPUT:
              if ((_Det==2)||(_Det==3))Key_Repeat_Cnt = 2; else Key_Repeat_Cnt = 5;  		//PWM duty cycle or out level
              if((_Det==1)&&(_Kind==8)&&(GenUartAdj==0))Key_Repeat_Cnt = 2;                               
              if((_Det==1)&&(GenAdjustMode==1)&&((((Sweep==0)||(Sweep>2))&&(_Kind<5))||(_Kind==7))){
                if(((_Kind<5)&&(Sweep==3)&&((_Frqn>13)||((SweepIndex<2)&&(_Frqn>5))))&&((FineAdjust==0)||(FineAdjust>=FineAdjustLimit))){
                  Key_Repeat_Cnt = 5;
                }else Key_Repeat_Cnt = 2;
              }
              break;
	    case T_BASE:
              if ((_Det==2)||(_Det==3)) Key_Repeat_Cnt = 2; else Key_Repeat_Cnt = 5;		//xpos in large buffer mode
              break;
	    case TRACK1:
	    case TRACK2:
              if((SpiLogic())&&(_Det==1)&&(SpiAdj==3)){Key_Repeat_Cnt = 2;break;}
	    case TRACK3:
	    case TRACK4:
              if (Detail[Current]==3) Key_Repeat_Cnt = 2; else Key_Repeat_Cnt = 5;		//ypos in track menus
              if((_4_source>11)&&(Current==3)&&(_Det==1)){
                if(DownConvertMode){
                  if((FFTt1Mode==0)&&(Key_Status_Now & K_INDEX_I_STATUS))Key_Repeat_Cnt=15;
                }else{
                  if(FFTt1Mode)Key_Repeat_Cnt = 2;
                }
              }
              break;
	    default:
            Key_Repeat_Cnt = 5;
          }
        }

        if(Key_Status_Now & K_ITEM_D_STATUS)   {KeyCode = K_ITEM_DEC;  Key_Repeat_Cnt = 5;}    // K9 don't use fast repeat for menu shift
        if(Key_Status_Now & K_ITEM_I_STATUS)   {KeyCode = K_ITEM_INC;  Key_Repeat_Cnt = 5;}    // K8 
        if(Key_Repeat_Cnt==2){
          SetLowVolume=1;
        }else SetLowVolume=0;
      }                                             //if((Key_Wait_Cnt || Key_Repeat_Cnt)==0)
    }else SetLowVolume=0;                           //if(Key_Status_Now & Key_Status_Last)       // Key push hold on
  }                                                 //if key_status, else
  Key_Status_Last = Key_Status_Now;

  return KeyCode;
}

void s16ToDec3(char *p, s16 n){      //Change signed 16 bit int to +/-xxx string
u8 i;
    if(n >= 0)  *p++ = '+';
    else {
      *p++ = '-';
      n = -n;
    }
    for(i=100;i>0;i/=10){
      *p++ = '0'+n/i;
      n %=i;
    }
    *p = 0;
}


u16 AsciiToU16(char *p,u8 n){
  u8 i;
  u16 Value=0;
    for(i=1;i<=n;i++)Value+=((*p++-48)*Power(10,(n-i)));
    return Value;
}



void s8ToDec2(char *p, s8 n){      //Change signed 8 bit int(99 to -99) to +/-xx string
    if(n > 0)  *p++ = '+';
    else if(n < 0){
      *p++ = '-';
      n = -n;
    }else *p++= ' ';

    if((n/10)>0)*p++ = '0'+n/10;

    n %= 10;
    *p++ = '0'+n;
    *p = 0;
}


void S32ToFloat3(char *p,s32 n){                //converts uV to volts with 3 digits floating point precision
s8 i,Ndigits=-1,Neg=0;                          //need 6 char array inc term nul
s32 nn;

    n/=1000;                                    //convert uV to mV: smallest value called for here is 2000uV
    if(n==0){*p++='0';*p=0;return;}               
    if(n<0){n=-n;Neg=1;}
    if(n>999999)n=999999;                       //limit to 999V to prevent overflow
    nn=n;

    while(nn>0){                                //# of digits to strip off to get to most significant digit
      nn/=10;
      Ndigits++;
    }
    if(Neg)       *p++='-';

    if(Ndigits<3) *p++='.';                     //for values less than unity
    if(Ndigits<2) *p++='0';                 
    if(Ndigits==0)*p++='0';

    for(i=Ndigits;i>=0;i--){          
      *p++=48+((n/Power(10,i))%10);             //select and write digits
      if(i==(Ndigits-2))break;                  //stop after 3 digits 
      if(i==3)*p++='.';                         //write "floating point" to proper location
    }  
    *p=0;
}



#warning Gabo: fft disabled
#if 0
/************************************************************************

	FFT Audio Analysis

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

************************************************************************/

// Fixed-point in-place Fast Fourier Transform
// All data are fixed-point short integers, in which -32768
// to +32768 represent -1.0 to +1.0 respectively. Integer
// arithmetic is used for speed, instead of the more natural
// floating-point.
//
// For the forward FFT (time -> freq), fixed scaling is
// performed to prevent arithmetic overflow, and to map a 0dB
// sine/cosine wave (i.e. amplitude = 32767) to two -6dB freq
// coefficients.
//
// Written by:  Tom Roberts  11/8/89
// Made portable:  Malcolm Slaney 12/15/94 malcolm@interval.com
// Enhanced:  Dimitrios P. Bouras  14 Jun 2006 dbouras@ieee.org
// Ported to PIC18F:  Simon Inns 20110104
// Ported to stm32:  Pedro Sim

/*
fix_fft() - perform forward fast Fourier transform.
fr[n],fi[n] are real and imaginary arrays, both INPUT AND
RESULT (in-place FFT), with 0 <= n < 2**m
*/

void fix_fft(s32 fr[], s32 fi[], short size)		// m = number of bits of the FFT size (ex: 1024 -> 10 bits)
{                                                       // using 32 bit bins allows 18db greater dynamic range 

	long int mr = 0, nn, i, j, l, k, istep, n, tr, ti, qr, qi ;		
	short wr, wi, m;			
        u8 shift=0;

        if(AutoFFT==0)shift=1;
	
	m = LOG2_FFTSize;
	
	n = size;

	nn = n - 1;

	/* max FFT size = N_WAVE */

	/* decimation in time - re-order data */
	for (m=1; m<=nn; ++m)
	{
		l = n;
		do
		{
			l >>= 1;
		} while (mr+l > nn);
		
		mr = (mr & (l-1)) + l;
		if (mr <= m) continue;		
		
		tr = fr[m];			
		fr[m] = fr[mr];
		fr[mr] = tr;
		ti = fi[m];			
		fi[m] = fi[mr];
		fi[mr] = ti;
	}

	l = 1;
	k = LOG2_N_WAVE-1;
	
	while (l < n)
	{
		/*
		fixed scaling, for proper normalization --
		there will be log2(n) passes, so this results
		in an overall factor of 1/n, distributed to
		maximize arithmetic accuracy.

		It may not be obvious, but the shift will be
		performed on each data point exactly once,
		during this pass.
		*/
		
		// Variables for multiplication code
		long int c;

		istep = l << 1;
		for (m=0; m<l; ++m)
		{
			j = m << k;
			/* 0 <= j < N_WAVE/2 */
			wr =  Sinewave[j+N_WAVE/4];
			wi = -Sinewave[j];

			wr >>= 1;
			wi >>= 1;
			
			for (i=m; i<n; i+=istep)
			{
				j = i + l;
				
				// Multiplications unrolled to prevent overhead
				// for procedural calls (the stm32 has an ALU with
				// H/W divide and single cycle multiply):

                                tr=(wr*(fr[j]>>shift))>>(14-shift);	//shift extends dynamic range for log display	
                                if(tr&1)tr++; tr>>=1;
                                c=(wi*(fi[j]>>shift))>>(14-shift);
                                if(c&1)c++;					
                                tr-=c>>1;

                                ti=(wr*(fi[j]>>shift))>>(14-shift);		
                                if(ti&1)ti++; ti>>=1;
                                c=(wi*(fr[j]>>shift))>>(14-shift);
                                if(c&1)c++;					
                                ti+=c>>1;

				qr = fr[i]>>1;
				qi = fi[i]>>1;

				fr[j] = qr - tr;
				fi[j] = qi - ti;
				fr[i] = qr + tr;
				fi[i] = qi + ti;

			}
		}
		
		--k;
		l = istep;
	}
}
#endif

void ProcessFileName(void){                                     //processes EditChar results into SelectedFileName for saving
s8 i,j,h,leading;
              leading=1;

              for(i=0;i<8;i++)SelectedFileName[i]=EditChar[i][0];

              for(i=0;i<8;i++){                                                      //remove leading spaces                                  
                if(SelectedFileName[i]==32){
                  if(leading){
                    for(h=0;h<7;h++){                                                //make sure all are removed
                      for(j=i;j<7;j++)SelectedFileName[j]=SelectedFileName[j+1];
                      SelectedFileName[7]=32;
                      if(SelectedFileName[i]!=32){leading=0;break;}
                    } 
                  }else SelectedFileName[i]=95;                                      //replace remaining spaces with underscores
                }else{
                  leading=0;                                                         //retrieve original non supported char
                  if(_Curr[2].Value==CFG){if(SelectedFileName[i]==63)SelectedFileName[i]=LastAccessedConfig[i];} 
                }

              }
              for(i=7;i>=0;i--){ 
                if(SelectedFileName[i]==95){
                  SelectedFileName[i]=32;                                           //replace trailing underscores with spaces leaving
                }else break;                                                        //only in-name underscores
              }
              if(SelectedFileName[0]==32){                                          //if filename entered is all spaces, no valid char
                for(i=0;i<8;i++){                                                   //will be shifted to first position. Replace all
                  SelectedFileName[i]=95;                                           //with underscores to produce valid name. 
                }
              }
}


/********************************* END OF FILE ********************************/
