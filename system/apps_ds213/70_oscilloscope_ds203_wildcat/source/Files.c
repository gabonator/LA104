/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : File.c  
 Version   : DS203_APP Ver 2.5x                                  Author : bure
*******************************************************************************/
#include <string.h>
#include "Function.h"
#include "Process.h"
#include "BIOS.h"
#include "File.h"
#include "Menu.h"
#include "Calibrat.h"
#include "Interrupt.h"

u16 SectorSize = 0;
u8 flash_mode = 0;
//u8  SecBuff[LARGEST_SECTOR_SIZE];
u8  SecBuff[0x1800];                                      //make enough room to load the entire 2MB device FAT, stack reduced to make room
uc8 DiskDevInfo_8M[]={"8MB Internal"};
void Print_Clk(u8 Phase);
u8 OverWriteWarn(void);
void ProcessConfigName(void);
void FileMessage(u8 i);
u8 WriteFileSec(u8* SecBuff, u16* pCluster, u32* pDirAddr);
u8 FileEntryAdd(u8* pBuffer, u32* pDirAddr, char* Filename);
u8 WriteSector(u16 n, u16* pCluster,u32* pDirAddr);
s32 CSVdata(u8 Ch,u8 Inv,u16 i,u8 data);
u8 CSVdataModeLogic(void);

u8  SaveShortBuffXpos;
u8  FileBuff[1600];
u16 TempPar[74];
u8  Versions;
u16 ArbtSampleNumber;
u32 UartFileSize=0;
char Label[15][13];
char Ext[4]={'C','F','G',0};
u16  DirRange=0;
char SelectedFileName[9];
char ConfigFileName[9];
char LastAccessedConfig[9]={32,32,32,32,32,32,32,32,0};
u8  Edited=0;
u8  BufferRestore=0;
u32 RootStart=0x00004000;
u32 RootStop =0x00007F00;
uc32 FatStart=0x00001000;
u16 CSVposition;

const char Offset[5]= {" Os="};

const char SPeriod[7]=    {"   PER="}; 
const char Sampling[4][14]={ {"   ChA: VOLTS"},{"   ChB: VOLTS"},{"   ChC: 0= "},{"   ChD: 0= "} };
const char OSsampling[4][14]={ {" ChA:HI VOLTS"},{" ChA:LO VOLTS"},{" ChB:HI VOLTS"},{" ChB:LO VOLTS"} };

const char SamplingPeriod[22][8]={ {"33.333mS"},{"16.667mS"},{"6.6667mS"},{"3.3333mS"},{"1.6667mS"},{"666.67uS"},{"333.33uS"},
                                   {"166.67uS"},{"66.667uS"},{"33.333uS"},{"16.667uS"},{"6.6667uS"},{"3.3333uS"},{"1.6667uS"},
                                   {"666.67nS"},{"333.33nS"},{"166.67nS"},{"69.444nS"},{"41.667nS"},{"27.778nS"},{"13.889nS"},
                                   {"13.889nS"} };

const char SubSamplingPeriod[13][8]= { {"3.3333mS"},{"6.6667mS"},{"16.667mS"},{"33.333mS"},{"66.667mS"},{"166.7mS"},{"333.33mS"},
                                       {"500.00mS"},{"1.0000 S"},{"2.0000 S"},{"4.0000 S"},{"10.000 S"},{"20.000 S"} };

const char DigSampling[4][12]={ {" ChA X "},{" ChB X "},{" ChC +20=1"},{" ChD +20=1"} };
const char AmplitudeSteps[8][6]=     {  {"2.00mV"},{"4.00mV"},{"8.00mV"},{"20.0mV"},{"40.0mV"},{"80.0mV"},{"200 mV"},{"400 mV"} };
const char AmplitudeX10Steps[8][6]=  {  {"20.0mV"},{"40.0mV"},{"80.0mV"},{"200 mV"},{"400 mV"},{"800 mV"},{"2.00 V"},{"4.00 V"} };

const char ErrorCode[5][6]={" OK  "," FULL"," NONE"," ERR"," CNCL"};

const char Clk[4][2]={"|","/","-","$"};

uc16 BMP_Color[16] = { WHT,  CYAN, CYAN_,  YEL,  YEL_, PURPL, PURPL_, GRN,   		//for 16 color BMP's
                      GRN_, GRAY, ORANGE, BLUE, RED,  BLACK, 0x87F0,  0x7BEF};    

						      //16 color BMP file header	
uc8  BmpHead16[54] = { 0X42, 0X4D, 0XF8, 0XBB, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X76, 0X0, 0X00, 0X00, 0X28, 0X00,	
                      0X00, 0X00, 0X90, 0X01, 0X00, 0X00, 0XF0, 0X00, 0X00, 0X00, 0X01, 0X0, 0X04, 0X00, 0X00, 0X00,
                      0X00, 0X00, 0X82, 0XBB, 0X00, 0X00, 0X12, 0X0B, 0X00, 0X00, 0X12, 0XB, 0X00, 0X00, 0X10, 0X00,
                      0X00, 0X00, 0X00, 0X00, 0X00, 0X00};
                                                                                             //byte 28= 0x04 bits/pixel
                                                       //64K color BMP file header 
uc8  BmpHead64[66] =  { 0X42, 0X4D, 0X42, 0XEE, 0X02, 0X00, 0X00, 0X00, 0X00, 0X00, 0X42, 0X0, 0X00, 0X00, 0X28, 0X00,	
                      0X00, 0X00, 0X90, 0X01, 0X00, 0X00, 0XF0, 0X00, 0X00, 0X00, 0X01, 0X0, 0X10, 0X00, 0X03, 0X00,
                      0X00, 0X00, 0X00, 0XEE, 0X02, 0X00, 0X12, 0X0B, 0X00, 0X00, 0X12, 0XB, 0X00, 0X00, 0X00, 0X00,
                      0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X1F, 0X00, 0X00, 0X00, 0XE0, 0X07,0X00, 0X00, 0X00, 0XF8,
                      0X00, 0X00};
                                                                                             //byte 28= 0x10 bits/pixel 

/*******************************************************************************
Initialize the file system		return value: 0x00 = successful
Primarily sets variables for flash_mode and SectorSize
*******************************************************************************/
u8 InitFileSystem() {
  u32 ptr;
  u8 Ver[8];
  
  if (SectorSize!=0) return 0;
	
  ptr=__Get(DFUVER);
  memcpy(Ver,(u8*)ptr,5);
  ptr=(Ver[1]-'0')*100 +(Ver[3]-'0')*10 +(Ver[4]-'0'); 
  if(ptr<=311){
    flash_mode=FLASH_2M;
	SectorSize = FLASH_2M_SECTOR_SIZE;
  }
  else{
    ptr=__Get(DEVICEINFO);
    if(memcmp((u8*)ptr,DiskDevInfo_8M,3)==0){
		flash_mode=FLASH_8M;
		SectorSize = FLASH_8M_SECTOR_SIZE;
	}
    else {
		flash_mode=FLASH_2M;
		SectorSize = FLASH_2M_SECTOR_SIZE;
	}
  }  
  if (SectorSize>LARGEST_SECTOR_SIZE) { //This should never happen
	SectorSize = 0;
	return 255;
  }

    if(flash_mode==FLASH_2M){                                   //root dir starting/ending addresses
      RootStart=0x00004000;
      RootStop =0x00007F00;
    }else{
      RootStart=0x00003000;
      RootStop =0x00006F00;
    }

  return 0;
 }


/*******************************************************************************
 Print_Clk:  progress indicator
*******************************************************************************/
void Print_Clk(u8 Phase)
{
  if((AutoSaveBuf==0)||(ChartLogic()==0))Print_Str(250,0,0x0405,INV,(char*)Clk[Phase]); 
}

/*******************************************************************************
Open the specified file extension		input: The file extension		return value: 0x00 = successful
*******************************************************************************/
u8 Make_Filename(u8 FileNum, char* FileName)
{
  char Num[4];
  
  u8ToDec3(Num, FileNum,0);
  FileName[4]  = Num[0];
  FileName[5]  = Num[1];
  FileName[6]  = Num[2];
  return InitFileSystem();
} 
/*******************************************************************************
Obtained corresponds to the current color palette number
*******************************************************************************/
u8 Color_Num(u16 Color)			//for 16 color BMP's
{
  if(Color == WHT)                   return 0;
//else if(Color== CYAN  ) return 1;
  else if((Color== CYAN)||(Color==0xEF60)||(Color==0xF7A0)) return 1;
//else if(Color== YEL   ) return 3;
  else if((Color== YEL) ||(Color==0x077D)||(Color==0x07BE)) return 3;
  else if(Color== PURPL ) return 5;
  else if(Color== GRN   ) return 7;
//else if(Color== CYAN_ ) return 2;
  else if((Color==CYAN_)||(Color==0xDEE0))return 2;       //include alt fast dims 
//else if(Color== YEL_  ) return 4;
  else if((Color== YEL_)||(Color==0x06FB))return 4;
  else if(Color== PURPL_) return 6;
  else if(Color== GRN_  ) return 8;
  else if(Color== GRAY  ) return 9;
  else if(Color== ORANGE) return 10;
  else if(Color== BLUE  ) return 11;
  else if(Color== RED   ) return 12;
  //else                              return 13;
  else if(Color== BLACK ) return 13;                //add black as viable color so background saves properly
  else if(Color== 0x87F0) return 14;                //bright green used in serial decoding hex values and arrows added
  else if(Color== 0x7BEF) return 15;                //dark gray for i2c arrows
  else                    return 9;                 //anything else would be grid background mixed with something so show grid, not black holes
}

u8 Save_Img(void){

  u32 i;
  u16 j=0,k=0;
  char ChkSum=0,RChkSum=0;
  u16 pCluster[3];
  u32 pDirAddr[1]; 
  if (InitFileSystem()!=0) return 10;                                                    //does not use make_filename so may not be initiated

  if(__OpenFileWr(SecBuff,"ROM_IMG BIN", pCluster, pDirAddr)!=OK) return DISK_ERR;
  for(i=0;i<0x80000;i++){                                                            //SecBuff here will be either 512 or 4096
    SecBuff[j]=*(char*)(0x8000000+i);                               
    ChkSum^=SecBuff[j];
    if(++j>=SectorSize){
      if (WriteSector(k++,(u16*)pCluster,(u32*)pDirAddr))return WR_ERR;
      j=0;
    }
  }
  if(__CloseFile(SecBuff,0x80000,pCluster,pDirAddr)!= OK) return WR_ERR;

  i=__OpenFileRd(SecBuff,"ROM_IMG BIN",pCluster,pDirAddr);                             //check file for checksum
  if(i != OK) return i;
  j=0;
  for(i=0;i<0x80000;i++){                                                            
    if((j>=SectorSize)||(j==0)){
      j=0;
      if(__ReadFileSec(SecBuff,pCluster)!= OK) return RD_ERR;
      Print_Clk((k++>>1)& 3);                                                            // progress indication                         
    }
    RChkSum^=SecBuff[j++];
  }
  if(RChkSum!=ChkSum)return WR_ERR;
  return 0;

}

/*******************************************************************************
Load_Dat: Load the saved screen image of the original data		input: The file number		return value: 0x00 = successful
*******************************************************************************/
u8 Load_Dat(u8 FileNum)
{
  char Filename[12] = "FILE    DAT"; 
  u16 i;
  u16 pCluster[3];
  u32 pDirAddr[1]; 

    if ((Current==9)&&(_Det==3)){
      for(i=0;i<8;i++)Filename[i]=SelectedFileName[i];
    }else{
      if (Make_Filename(FileNum, Filename)!=0) return 1;
    }

        i = __OpenFileRd(SecBuff, Filename, pCluster, pDirAddr);
        if(i != OK) return i;

	  if(flash_mode==FLASH_2M){
		for(i=0; i<4; i++){
		  if(__ReadFileSec(SecBuff, pCluster)!= OK) return RD_ERR;
		  memcpy(&FileBuff[i*400], SecBuff,400);
		}
	  }
	  else{
		if(__ReadFileSec(SecBuff, pCluster)!= OK) return RD_ERR;
		for(i=0; i<4; i++)
		  memcpy(&FileBuff[i*400], &SecBuff[i*512],400);
	  }

  return 0;
}
/*******************************************************************************
Save_Dat: save current screen to display the original image data		input: the file number		return value: 0x00 = success
*******************************************************************************/
u8 Save_Dat(u8 FileNum)
{
  char Filename[13] = "FILE    DAT"; 
  u16 i, j;
  u16 pCluster[3];
  u32 pDirAddr[1]; 
  
    if ((Current==9)&&(_Det==3)){
      for(i=0;i<8;i++)Filename[i]=SelectedFileName[i];
    }else{
      if (Make_Filename(FileNum, Filename)!=0) return 1;
    }

  if(__OpenFileWr(SecBuff, Filename, pCluster, pDirAddr)!=OK) return DISK_ERR;

    memset(SecBuff, 0, LARGEST_SECTOR_SIZE);//512

	  if(flash_mode==FLASH_8M){
		for(j=0; j<4; j++){
		  for(i=0; i<397; i++) SecBuff[i+j*512] = TrackBuff[i*4 + j];
		  SecBuff[397+j*512] = Title[j][POSI].Value;
		}  
                if (WriteSector(j,(u16*)pCluster,(u32*)pDirAddr))return WR_ERR;
	  }
	  else
	  {
		for(j=0; j<4; j++){
		  for(i=0; i<397; i++) SecBuff[i]= TrackBuff[i*4 + j];
		  SecBuff[397] = Title[j][POSI].Value;
                  if (WriteSector(j,(u16*)pCluster,(u32*)pDirAddr))return WR_ERR;
		}
	  }

  if(__CloseFile(SecBuff, 0x0800, pCluster, pDirAddr)!= OK) return WR_ERR;
  return OK;
}
/*******************************************************************************
Save_Bmp: save the current screen image as BMP		input: file number		return value: 0x00 = successful
*******************************************************************************/
u8 Save_Bmp(u8 FileNum)
{
  u32 FileSize;
  char Filename[12] = "IMAG    BMP"; 
  u16 k, i,x=0, y=0, j, ColorH, ColorL; //, tmp;        // j,  ColorH, ColorL;
  register u16 tmp;
  u16 pCluster[3];
  u32 pDirAddr[1]; 

    if ((Current==9)&&(_Det==3)){
      for(i=0;i<8;i++)Filename[i]=SelectedFileName[i];
    }else{
      if (Make_Filename(FileNum, Filename)!=0) return 1;
    }

  if(__OpenFileRd(SecBuff, Filename, pCluster, pDirAddr)==0){            // original file exists
    if(OverWriteWarn())return 200;
    if(DeleteFile(SecBuff,Filename))return WR_ERR; 
  }

  if(__OpenFileWr(SecBuff, Filename, pCluster, pDirAddr)!=OK) return DISK_ERR;

if((_4_source==10)||(_4_source==11)){					//only save in 64K color if displaying spectrograph or map
  memcpy(SecBuff, BmpHead64, 66);					//64K COLOR BMP
  i = 0x0042; // the image data stored address
  k = 0;
  for(y=0; y<240; y++){
    for(x=0; x<400 ; x++){
      __Point_SCR(x, y);
      tmp =__LCD_GetPixl();
      SecBuff[i++]=tmp&0xFF; SecBuff[i++]=tmp>>8;
      if(i>=SectorSize){ //512
        if (WriteSector(k++,(u16*)pCluster,(u32*)pDirAddr))return WR_ERR;
        i=0; 
      }
    }
  }
  FileSize=0x2F000;
}else{
  memcpy(SecBuff, BmpHead16, 54);					//16 COLOR BMP
  i = 0x0036; // palette to store the starting address
  for(j=0; j<16; ++j){
    SecBuff[j*4 +i+0]=(BMP_Color[j]& 0xF800)>>8; // Blue
    SecBuff[j*4 +i+1]=(BMP_Color[j]& 0x07E0)>>3; // Green
    SecBuff[j*4 +i+2]=(BMP_Color[j]& 0x001F)<<3; // Red
    SecBuff[j*4 +i+3]= 0;                        // Alpha
  }
  i = 0x0076; // the image data stored address
  k = 0;
  for(y=0; y<240; y++){
    for(x=0; x<400 ; x+=2){
      __Point_SCR(x, y);
      ColorH =__LCD_GetPixl();
      __Point_SCR(x+1, y);
      ColorL =__LCD_GetPixl();
      SecBuff[i] =(Color_Num(ColorH)<<4)+ Color_Num(ColorL);
      i++;
      if(i>=SectorSize){
        if (WriteSector(k++,(u16*)pCluster,(u32*)pDirAddr))return WR_ERR;
        i=0; 
      }
    }
  }
  FileSize=0xBC00;
}

  if(i!=0){
    i=WriteFileSec(SecBuff, pCluster, pDirAddr);
    if(i)return i;
  }
  if(__CloseFile(SecBuff, FileSize, pCluster, pDirAddr)!= OK) return WR_ERR;	//2nd var just seems to be size stamp, alterbios 
  return 0;                                                                     //does not seem to care 
}

u8 Load_Bmp(u8 FileNum){
  u8  i,y,start,FinishFactor;
  u16 j,h,x;
  char Filename[13] = "IMAG    BMP"; 
  
  u16 pCluster[3];
  u32 pDirAddr[1]; 
  
  if ((Current==9)&&(_Det==3)){
    for(i=0;i<8;i++)Filename[i]=SelectedFileName[i];
  }else{
    if (Make_Filename(FileNum, Filename)!=0) return 1;
  }

  i = __OpenFileRd(SecBuff, Filename, pCluster, pDirAddr);
  if(i != OK) return i;
  if(__ReadFileSec(SecBuff, pCluster)!= OK) return RD_ERR;

  if(flash_mode==FLASH_8M)FinishFactor=8; else FinishFactor=1;

  if(SecBuff[28]==16){						//if file is 64k color type
    x=0;
    y=0;
    for(h=0; h<(376/FinishFactor); h++){
      if(h==0)start=66;else start=0;
      for(j=start;j<(512*FinishFactor);j+=2){
        __Point_SCR(x,y);
        __LCD_SetPixl(((u16)(SecBuff[j+1]<<8))|SecBuff[j]);
        if(x<399)x++;else {x=0;y++;}
        if((x>=399)&&(y>=239))goto EndOfFile;
      }
      if(__ReadFileSec(SecBuff, pCluster)!= OK) return RD_ERR;
    }
  }else if(SecBuff[28]==4){					//16 color type
    x=0;
    y=0;
    for(h=0; h<(96/FinishFactor); h++){
      if(h==0)start=0x0076;else start=0;
      for(j=start;j<(512*FinishFactor);j++){
        __Point_SCR(x,y);
        __LCD_SetPixl(BMP_Color[SecBuff[j]>>4]);      
        __Point_SCR(x+1,y);
        __LCD_SetPixl(BMP_Color[SecBuff[j]&0x0F]);      
        if(x<398)x+=2;else {x=0;y++;}
        if((x>=398)&&(y>=239))goto EndOfFile;
      }
      if(__ReadFileSec(SecBuff, pCluster)!= OK) return RD_ERR;
    }  
  }  

EndOfFile:
  return 0;
}


/*******************************************************************************
Save_Buf: save the data collection buffer in BUF format		input: file number		return value: 0x00 = successful
*******************************************************************************/
u8 Save_Buf(u8 FileNum)
{
  u8   i;
  u16  j;
  char Filename[12] = "DATA    BUF"; 
  u16* p ;
  
  u16 pCluster[3];
  u32 pDirAddr[1];

    if ((Current==9)&&(_Det==3)){
      for(i=0;i<8;i++)Filename[i]=SelectedFileName[i];
    }else{
      if (Make_Filename(FileNum, Filename)!=0) return 1;
    }

  if(__OpenFileRd(SecBuff, Filename, pCluster, pDirAddr)==0){                       // original file exists
    if((AutoSaveBuf==0)||(ChartLogic()==0)){
      if(OverWriteWarn())return 200;
    }
    if(DeleteFile(SecBuff,Filename))return WR_ERR; 
  }
  if(__OpenFileWr(SecBuff, Filename, pCluster, pDirAddr)!=OK) return DISK_ERR;

  if(OSBufferLogic()){                                                                //store OS data in unused part of DataBuf 
    for(i=0;i<2;i++){
      for(j=512;j<1024;j++){
        DataBuf[j+(i*512)]&=0x00FFFFFF;                                                          
        DataBuf[j+(i*512)]|=(u32)OsFFTData[i][j-512]<<24;    
      }
    }
    for(i=0;i<2;i++){
      for(j=1536;j<2048;j++){
        DataBuf[j+(i*512)]&=0x00FFFFFF;                                                          
        DataBuf[j+(i*512)]|=(u32)RMSdata[i][j-1536]<<24;    
      }
    }
  }

  for(i=0; i<(BUFFER_SIZE/SectorSize); i++){                                         // < 16384/   512 or 4096   =    32 or 4
    memcpy(SecBuff, &(DataBuf[i*SectorSize/4]), SectorSize);
    if (WriteSector(i,(u16*)pCluster,(u32*)pDirAddr))return WR_ERR;
  }
  memset(SecBuff, 0, 512);
  p =(u16*)SecBuff;
  for(i=0; i<4; i++){                     // save each corresponding value in the display menu
    *p++ = Title[i][0].Value;
    *p++ = Title[i][1].Value;
    *p++ = Title[i][2].Value;
    *p++ = Title[i][3].Value;
  }
  *p++ = Title[6][0].Value;
  *p++ = Title[6][1].Value;

  *p++ = 0x00FF & Ka1[_A_Range];
  *p++ = Ka2[_A_Range];
  *p++ = 0x00FF & Kb1[_B_Range];
  *p++ = Kb2[_B_Range];
  *p++ = OSBuffer;
  *p++ = FlagFrameMode;
  *p++ = OSAvg;
  *p++ = SubIndex;

  *p++ = Title[7][0].Value;
  *p++ = Title[7][1].Value;
  *p++ = ChartMode;

  j=WriteFileSec(SecBuff, pCluster, pDirAddr);
  if(j)return j;
  if(__CloseFile(SecBuff, 0x4200, pCluster, pDirAddr)!= OK) return WR_ERR;
  return 0;
}


u8 WriteFileSec(u8* SecBuff, u16* pCluster, u32* pDirAddr){
  if( ((flash_mode==FLASH_8M)&&(*pCluster>2040))||                                 //abort file write if at end of drive space
    ((flash_mode==FLASH_2M)&&(*pCluster>4031)) ){                                  //Alterbios does not seem to return much of anything,                         
    __CloseFile(SecBuff, 0, pCluster, pDirAddr);                                   //doesn't return anything in SecBuff and pDirAddr
    return OVER;                                                                   //so may not return pCluster either, ends OK though, at
  }else{                                                                           //least doesn't loop around to destroy the FAT
    if(__ProgFileSec(SecBuff, pCluster)!= OK) return WR_ERR;         
  }
  return 0;
}


u8 Load_Arbt(u8 FileNum){
  u8 *p;
  u16 i,Sample=0;
  u8 Tmp,j=0,Start=0;
  char ParseStr[6];
  u16 pCluster[3];
  u32 pDirAddr[1]; 
  char Filename[12] = "DATA    ARB"; 

  if ((Current==9)&&(_Det==3)){
    for(i=0;i<8;i++)Filename[i]=SelectedFileName[i];
  }else{
    if (Make_Filename(FileNum, Filename)!=0) return 1;
  }

  i = __OpenFileRd(SecBuff, Filename, pCluster, pDirAddr);
  if(i != OK) return i;
  if(__ReadFileSec(SecBuff, pCluster)!= OK) return RD_ERR;
  p=(u8*)SecBuff;

  while(*p==48){p++;Start++;}                                   //strip off any leading 0's
  while(*p!=44){
    if((*p>47)&&(*p<58))ParseStr[j++]=*p++;else p++;            //retrieve "header" containing number of samples to be used 
    Start++;
    if((j>3)||(Start>200))return 1;                             //return ERR if samples >3 digits or comma not found after 200 chars
  }                                                             //break out and continue once comma delimiter is found
  p++;                                                          //move past delimiter 
  ArbtSampleNumber=AsciiToU16(ParseStr,j);                           
  j=0;
  if((ArbtSampleNumber<2)||(ArbtSampleNumber>720))              //return ERR if number of samples out of range   
    {ArbtSampleNumber=0;return 1;}

  while(Sample<720){

    for(i=Start;i<SectorSize;i++){
      Tmp=*p++;

      if(Tmp==44){                                              //once delimiter found, convert and load value
        ATT_DATA[Sample]= AsciiToU16(ParseStr,j); 
        if(ATT_DATA[Sample]<0)ATT_DATA[Sample]=0; 
        if(ATT_DATA[Sample]>4095)ATT_DATA[Sample]=4095;         //clip if out of range 
        j=0; Sample++;                                          //reset string position counter and advance sample position
      }else{                                                    //start loading string here
        if((Tmp>47)&&(Tmp<58))ParseStr[j++]=Tmp;                //eliminate white space or other non relevant chars
        if(j>4)j=4;                                             //limit to 5 valid characters 
      }

      if(Sample>(ArbtSampleNumber-1))goto EndOfFile;            //if sample goes to ArbtSampleNmber, is now past base 0 sample number index

    }//for(i=Start

    Start=0;                                                    
    if(__ReadFileSec(SecBuff, pCluster)!= OK) return RD_ERR;
	    p=(u8*)SecBuff;

  }

EndOfFile:
  return 0;
}

u8 ReadDir(char *ext){                          //Load 15 filenames matching extension in Label, starting with match # DirRange
  u16 n,EntryCounter=0;                         //returns the number of files found and loaded in Label[15], up to 15 
  u8 i;
  u32 pDirAddr[1]; 
  u8 pBuffer[266];  
  u8 LabelIndex=0;

    for(n=0;n<15;n++){
      for(i=0;i<13;i++)Label[n][i]=0;
    }

    for(*pDirAddr=RootStart; *pDirAddr<=RootStop;){             //scan for filenames in root directory
      if(__ReadDiskData(pBuffer, *pDirAddr, 256)!= OK)          //return and display error
        {FileMessage(0xFF);return 0;}            
      for(n=0; n<256;n+=32){
        if(pBuffer[n]==0)return LabelIndex;                     //no more entries, partially filled list page
        if((pBuffer[n+8]==ext[0])&&(pBuffer[n+9]==ext[1])       //look for file extension match
        &&(pBuffer[n+10]==ext[2])&&(pBuffer[n]!=0xE5)){         //and exclude deleted files
          if(EntryCounter>=DirRange){                           //start offset for next page
            for(i=0;i<8;i++)Label[LabelIndex][i]=pBuffer[n+i];  //copy first 8 chars of filename 
            Label[LabelIndex][8]=0x2E;                          //"."           
            for(i=0;i<3;i++)Label[LabelIndex][9+i]=ext[i];      //extension
            LabelIndex++;                           
          }
          EntryCounter++;
        }
        if(LabelIndex>=15)return LabelIndex;                    //page is full 
        *pDirAddr += 32;                                        //move down one entry
      }
    }
    return LabelIndex;
}

u8 DeleteFile(u8* pBuffer, char* FileName){
  u32 pDirAddr[1];    
  u16 pCluster[1];
  u16 i=0,n,End,DirPageSize,Offset;
  u8  WritePageNumber;

    if(flash_mode==FLASH_2M){
      End=256;
      DirPageSize=0x1800;
      WritePageNumber=24;
    }else{
      End=4096;
      DirPageSize=4096;
      WritePageNumber=1;
    }
                                                                                  
    for(*pDirAddr=RootStart; *pDirAddr<=RootStop;){                         //scan for filename in root directory
      if(__ReadDiskData(pBuffer, *pDirAddr, End)!= OK)return RD_ERR;            
      for(n=0; n<End; n+=32){
        for(i=0; i<11; i++){
          if(pBuffer[n + i]!= 0){
            if(pBuffer[n+i]!=FileName[i]) break;
            if(i == 10){
              *pCluster=*(u16*)(pBuffer+n+0x1A);                           //starting cluster in FAT table for selected file 
              goto FileNameFound;                                          //n represents the offset
            }
          }
        }
      }
      *pDirAddr+=End;
    }
    return RD_ERR; 
    
FileNameFound:
    pBuffer[n]=0xE5;                                                            //write deleted file flag at address offset 0 in pBuffer
    if(__ProgDiskPage(pBuffer,*pDirAddr)!=0)return WR_ERR;                      //write section back to disk (4096 bytes for 8M, 256 for 2M)

    if(__ReadDiskData(pBuffer,FatStart,DirPageSize)!= OK)return RD_ERR;         //2MB is 0x1800 long 
    if(*pCluster<2)return RD_ERR;                

    do{                                           //Follow the white rabbit, but don't get lost, or your files will end up cross-linked!
      Offset=*pCluster+(*pCluster/2);             //scale address to accommodate extra nibbles   
      if(flash_mode==FLASH_8M){
        if(Offset>=3072)return RD_ERR;            
        if(i++>2048)return RD_ERR;                //all good things MUST come to an end  
      }else{                          
        if(Offset>=6144)return RD_ERR;           
        if(i++>4096)return RD_ERR;               
      }                                                  
                                                           
      if(*pCluster&1){                                                                             
        *pCluster=pBuffer[Offset+1];                     //Most Significant byte, save value pointing to next link before zeroing
        *pCluster=(*pCluster<<4)+(pBuffer[Offset]>>4);   //Shift to proper position and add little endian nibble
        pBuffer[Offset]&=0x0F;                           //zero, to release link
        pBuffer[Offset+1]=0;                                                               
      }else{
        *pCluster=pBuffer[Offset+1]&0x0F;                //Construct 12 bit entry within proper 2 bytes of double entry "triplet", 
        *pCluster=(*pCluster<<8)+pBuffer[Offset];        //depending on whether the entry number is odd or even.(Convolutions 
        pBuffer[Offset]=0;                               //courtesy of Microsoft et al. who just HAD to pack a 12 bit entry into 
        pBuffer[Offset+1]&=0xF0;                         //1.5 bytes instead of 2, thus saving a small fraction of 1% of drive 
      }                                                  //space on a typical floppy; bytes were precious in the age of floppies...)      
    }while(*pCluster<0xFF0);                          

    for(i=0;i<WritePageNumber;i++){
      if(__ProgDiskPage(pBuffer+(i*256),FatStart+(i*256))!=0)return WR_ERR;     //write section back to disk 
    }

    if(flash_mode==FLASH_8M){                                                   //synch FAT2 with FAT1
      if(__ReadDiskData(pBuffer,0x1000,0x1000)!= OK) return RD_ERR;        
      if(__ProgDiskPage(pBuffer,0x2000)!= OK) return WR_ERR;              
    }                                                                             
    else{
      for(i=0; i<24; i++){                                                      
        if(__ReadDiskData(pBuffer,0x1000+(i*256),256)!=OK)return RD_ERR;        
        if(__ProgDiskPage(pBuffer,0x2800+(i*256))!= OK)return WR_ERR;           
      }                                                                         
    }                                                                           
    if(*pCluster<0xFF0)return RD_ERR;                                           
    return 0;
}

void FileMessage(u8 i){  
//const char ErrorCode[5][6]={" OK  "," FULL"," NONE"," ERR"," CNCL"};

        if((i==3)||(i==4))i-=2; else{
          if(i==200)i=4;else if(i>0)i=3;
        }
        Print_Str(230, 0,0x050A, PRN,(char*)ErrorCode[i]);
        if((_Curr[2].Value==BUF)&&(_Curr[0].Value == LOAD))Delayms(300);else Delayms(900);
        _Curr[1].Flag |= UPDAT; 
        _Curr[2].Flag |= UPDAT;
}


u8 Load_Uart(u8 FileNum){
  u16 n=0;
  u8 i;
  u16 pCluster[3];
  u32 pDirAddr[1]; 
  u8 pBuffer[266];  
  char Filename[12] = "DATA    UAR"; 
  UartFileSize=0;

  if ((Current==9)&&(_Det==3)){
    for(i=0;i<8;i++)Filename[i]=SelectedFileName[i];
  }else{
    if (Make_Filename(FileNum, Filename)!=0) return 1;
  }
  i = __OpenFileRd(SecBuff, Filename, pCluster, pDirAddr);
  if(i != OK) return i;

  if(FileEntryAdd(pBuffer, pDirAddr, Filename)!=OK)return RD_ERR;      //retrieve filename dir entry address  

  if(__ReadDiskData(pBuffer, *pDirAddr+28, 4)!= OK)return RD_ERR;      //read file size value at offset 28 of FAT12 directory entry                
  for(n=0;n<4;n++)UartFileSize+=(u32)pBuffer[n]<<(n*8);                //assemble u32 value from 4 bytes
  if(UartFileSize>4096)UartFileSize=4096;                              //limit to size of SecBuff
  //if(UartFileSize>0x1800)UartFileSize=0x1800;                        //limit to size of SecBuff
  n=0;
  while(n<UartFileSize){
    if(__ReadFileSec(&SecBuff[n], pCluster)!= OK) return RD_ERR;
    n+=SectorSize;
  }

  return 0;
}


u8 FileEntryAdd(u8* pBuffer, u32* pDirAddr, char* Filename){            //writes directory filename entry address in pDirAddr
u16 n;                                                             
u8 i;
    for(*pDirAddr=RootStart; *pDirAddr<=RootStop;){                     //scan for filename in root directory
      if(__ReadDiskData(pBuffer, *pDirAddr, 256)!= OK)return RD_ERR;            
      for(n=0; n<256; n+=32){
        for(i=0; i<11; i++){
          if(pBuffer[n + i]!= 0){
            if(pBuffer[n + i]!= Filename[i]) break;
            if(i == 10){
              return OK;                                                //with complete filename match, hold Directory address                         
            }
          }
        }
        *pDirAddr += 32;
      }
    }
    return RD_ERR;
}

/*******************************************************************************
Load_Buf: load saved data into acquisition buffer		input: file number		return value: 0x00 = successful
*******************************************************************************/
u8 Load_Buf(u8 FileNum)
{
  u8  i,j;
  u16 h;
  char Filename[13] = "DATA    BUF"; 
  u16 *p;
  
  u16 pCluster[3];
  u32 pDirAddr[1]; 

  save_parameter();

  if ((Current==9)&&(_Det==3)){
    for(i=0;i<8;i++)Filename[i]=SelectedFileName[i];
  }else{
    if (Make_Filename(FileNum, Filename)!=0) return 1;
  }
  i = __OpenFileRd(SecBuff, Filename, pCluster, pDirAddr);

  if(i != OK) return i;
  for(i=0; i<((BUFFER_SIZE/SectorSize)); i++){
    if(__ReadFileSec(SecBuff, pCluster)!= OK) return RD_ERR;
    memcpy(&(DataBuf[i*SectorSize/4]),SecBuff,SectorSize);
  }
  if(__ReadFileSec(SecBuff, pCluster)!= OK) return RD_ERR;
  p =(u16*)SecBuff;
  for(i=0; i<4; i++){
    for(j=0;j<4;j++){
      Title[i][j].Value = *p++;             // restore
      Title[i][j].Flag  = UPDAT;
    }
  }
  Title[6][0].Value = *p++;
  Title[6][1].Value = *p++;
  Title[6][0].Flag  = UPDAT;
  Title[6][1].Flag  = UPDAT;
      
  Ka1[_A_Range] = (*p++ );
  Ka2[_A_Range] = (*p++ );
  Kb1[_B_Range] = (*p++);
  Kb2[_B_Range] = (*p++);
  OSBuffer      =  *p++;
  FlagFrameMode =  *p++;
  OSAvg         =  *p++;
  SubIndex      =  *p++;

  Title[7][0].Value = *p++;
  Title[7][1].Value = *p++;
  Title[7][0].Flag  = UPDAT;
  Title[7][1].Flag  = UPDAT;
  Title[7][2].Flag  = UPDAT;

  ChartMode     =  *p++;
  if(ChartMode>1)ChartMode=0;

  if(Title[3][0].Value < 11){                      //set proper menu sub items for ch 4             
    if(Detail[3]==2)Detail[3]++;
  }else{
    if(Detail[3]==3)Detail[3]--;
  }

  if(OSBufferLogic()){                                                                //restore OS data 
    for(i=0;i<2;i++){
      for(h=512;h<1024;h++){
        OsFFTData[i][h-512]=DataBuf[h+(i*512)]>>24;
      }
    }
    for(i=0;i<2;i++){
      for(h=1536;h<2048;h++){
        RMSdata[i][h-1536]=DataBuf[h+(i*512)]>>24;
      }
    }
  }

  Title[RUNNING][STATE].Value = 2;                 // set to "HOLD" status
  Title[RUNNING][STATE].Flag |= UPDAT;             // set the update flag
  XposRef=GetXposRef(_X_posi.Value);
  UpdateFileMenu();
  BufferRestore=1;
  return 0;
}

void save_parameter(void){
u16 *p;
u8 i,j;

  if(TempPar[0]!=0xAA55){                      //do not save temp parameters again if already saved, indicates original parms not restored
    p = TempPar;                               //and BUF parameters still active from previous BUF load(s)
    *p++ = 0xAA55;
    for(i=0; i<4; i++){                     // save each corresponding value in the display menu
      for(j=0;j<4;j++)*p++ = Title[i][j].Value;
    }
    *p++ = Title[6][0].Value;
    *p++ = Title[6][1].Value;
    for(i=0; i<8; i++){
      *p++ = 0x00FF & Ka1[i];
      *p++ = Ka2[i];
      *p++ = 0x00FF & Kb1[i];
      *p++ = Kb2[i];
    }
    *p++ = OSBuffer;
    *p++ = FlagFrameMode;
    *p++ = OSAvg;
    *p++ = Title[6][2].Value;                                      //xpos
    *p++ = SubIndex;

    *p++ = Title[7][0].Value;
    *p++ = Title[7][1].Value;
    *p++ = Detail[3];                                              //sub menu position of ch 4
    *p++ = ChartMode;
  }

}

void reset_parameter(void)
{
  u16* p;
  u8   i,j;
  
  p=TempPar;
  p++;
  if(TempPar[0]!=0xAA55) return;
  for(i=0; i<4; i++){
    for(j=0;j<4;j++){
      Title[i][j].Value = *p++;             // restore
      Title[i][j].Flag  = UPDAT;
    }
  }
  Title[6][0].Value = *p++;
  Title[6][1].Value = *p++;
  Title[6][0].Flag  = UPDAT;
  Title[6][1].Flag  = UPDAT;
  
  for(i=0; i<8; i++){
    Ka1[i] = (s8)(*p++ );//& 0xff);
    Ka2[i] = (*p++ );
    Kb1[i] = (s8)(*p++);// & 0xff);
    Kb2[i] = (*p++);
  }
  OSBuffer      =  *p++;
  FlagFrameMode =  *p++;
  OSAvg         =  *p++;
  Title[6][2].Value = *p++;
  SubIndex      =  *p++; 

  Title[7][0].Value = *p++;
  Title[7][1].Value = *p++;
  Title[7][0].Flag  = UPDAT;
  Title[7][1].Flag  = UPDAT;
  Title[7][2].Flag  = UPDAT;

  Detail[3] = *p++;                                              //sub menu position of ch 4
  ChartMode = *p++;

  p = TempPar;
  *p++ = 0;
}

u8 WriteSector(u16 n, u16* pCluster,u32* pDirAddr){
          Print_Clk((n >>1)& 3);                                             // progress indicator
          return WriteFileSec(SecBuff, pCluster, pDirAddr);
}

u8 CSVdataModeLogic(void){                                                              
    return((((OSBuffer==0)||(_T_base>16))&&(DigChLockout==0)&&((!ChartLogic())||(ChartMode==0)))
          &&((_3_source)||(_4_source==1)||(_4_source==4)||(_4_source==5)) );
}

s32 CSVdata(u8 Ch,u8 Inv,u16 i,u8 data){
u8 range=Title[Ch][RANGE].Value;
s32 temp;
s8 *p1[2];u16 *p2[2];
p1[0]=Ka1;p1[1]=Kb1;p2[0]=Ka2;p2[1]=Kb2;

    p1[Ch]+=range;temp=*p1[Ch]+((data-ADCoffset)-Title[Ch][POSI].Value);                  
    p2[Ch]+=range;if(CalFlag>0)temp=(*p2[Ch]*temp)/1024;
    temp*= Y_Attr[range].SCALE;
    if(Title[Ch][SOURCE].Value>2)temp*=10;
    if(Inv)temp=-temp;
    return temp;
}

/*******************************************************************************
Save_Csv: save the acquisition buffer in CSV format		input: file number		return value: 0x00 = successful
*******************************************************************************/
u8 Save_Csv(u8 FileNum)
{
  char Num[4][6];
  char Val[4];
  char Filename[12] = "DATA    CSV"; 
  u32 i, k = 0,length = SectorSize;
  s32 temp;
  u8  count, j,h=1,m=1;
  u16 n = 0;
  
  u16 pCluster[3];
  u32 pDirAddr[1]; 

    if ((Current==9)&&(_Det==3)){
      for(i=0;i<8;i++)Filename[i]=SelectedFileName[i];
    }else{
      if (Make_Filename(FileNum, Filename)!=0) return 1;
    }

  length = SectorSize;

  if(__OpenFileRd(SecBuff, Filename, pCluster, pDirAddr)==0){                       // original file exists
    if((AutoSaveBuf==0)||(ChartLogic()==0)){                                        // overwrite file unconditionally in chart auto save mode  
      if(OverWriteWarn())return 200;
    }
    if(DeleteFile(SecBuff,Filename))return WR_ERR; 
  }
  if(__OpenFileWr(SecBuff, Filename, pCluster, pDirAddr)!=OK) return DISK_ERR;

//============================================HEADERS===========================================

  memcpy(&SecBuff[k],SPeriod,7);k+=7; 
  if(ChartLogic()){memcpy(&SecBuff[k],SubSamplingPeriod[SubIndex-1],8);k+=8;}
    else {memcpy(&SecBuff[k],SamplingPeriod[_T_base],8);k+=8;}
  SecBuff[k++]=0x2C;             

  for(i=0;i<4;i++){

    if(CSVdataModeLogic()){
      memcpy(&SecBuff[k],Offset,4);k+=4;
      u8ToDec3(Val,Title[i][POSI].Value,0);
      memcpy(&SecBuff[k],Val,3);k+=3;
    } 
    if(i<2){ 
      if(CSVdataModeLogic()){
        memcpy(&SecBuff[k],DigSampling[i],7);k+=7;
        if(Title[i][0].Value>2){memcpy(&SecBuff[k],AmplitudeX10Steps[Title[i][RANGE].Value],6);k+=6;}
        else {memcpy(&SecBuff[k],AmplitudeSteps[Title[i][RANGE].Value],6);k+=6;}
      }else if(((OSBuffer)&&(_T_base<17))||((ChartLogic())&&(ChartMode))){
        memcpy(&SecBuff[k],OSsampling[i],13);k+=13;
      }else{
        memcpy(&SecBuff[k],Sampling[i],13);k+=13;
      }
    }else{
      if(CSVdataModeLogic()){
        memcpy(&SecBuff[k],DigSampling[i],10);k+=10;
      }else if(((OSBuffer)&&(_T_base<17))||((ChartLogic())&&(ChartMode))){
        memcpy(&SecBuff[k],OSsampling[i],13);k+=13;
      }else{  
        SecBuff[k++]=0x2C;             
        break;
      }
    }
    SecBuff[k++]=0x2C;             
  }
  SecBuff[k++] = 0x0d;            
  SecBuff[k++] = 0x0a;

  if((OSBufferLogic())&&(_T_Range==16))m=2;
  if(_T_Range>16){
    if(CSVdataModeLogic())h=4;else h=2;
    m=2;
  }
  if(_T_Range>18)m=1;

  for(i=(discard+h); i<bag_max_buf; i++){
//==========================================TIME FIELD===========================================
    CSVposition=(i-(discard+m));
    CalculateTvernier(2);

    if((ChartLogic())&&(SubIndex>4)){                                             //HOURS
      for(j=0;j<4;j++){                         
        if(MinBypass==2){
          if(CursorDisplayStrH[j]>0){
            SecBuff[k++] = CursorDisplayStrH[j];
          }else SecBuff[k++] = 32;
        }else SecBuff[k++] = 32;        
        if(k >= length){
          if (WriteSector(n++,(u16*)pCluster,(u32*)pDirAddr))return WR_ERR;
          k=0;
        }
      }
      for(j=0;j<4;j++){                                                           //MINUTES                          
        if(MinBypass>0){
          if(CursorDisplayStrM[j]>0){
            SecBuff[k++] = CursorDisplayStrM[j];
          }else SecBuff[k++] = 32;
        }else SecBuff[k++] = 32;        
        if(k >= length){
          if (WriteSector(n++,(u16*)pCluster,(u32*)pDirAddr))return WR_ERR;
          k=0;
        }
      }
    }

    for(j=0;j<(8+Neg);j++){                                                             //SECONDS
      if(CursorDisplayStr[j]>0){
        SecBuff[k++] = CursorDisplayStr[j];
      }else SecBuff[k++] = 32;
      if(k >= length){
        if (WriteSector(n++,(u16*)pCluster,(u32*)pDirAddr))return WR_ERR;
        k=0;
      }
    }
    SecBuff[k++] = 0x2c;
    if(k >= length){
      if (WriteSector(n++,(u16*)pCluster,(u32*)pDirAddr))return WR_ERR;
      k=0;
    }

//============================================DATA===========================================

    if(CSVdataModeLogic()){                                                           //direct buffer value copy mode w/digital ch's 
      temp=Ka1[_A_Range]+((Ka2[_A_Range]*((DataBuf[i] & 0xff)-ADCoffset))+512)/1024;
      u8ToDec3(Num[0],temp,0);
      temp = Kb1[_B_Range]+((Kb2[_B_Range]*(((DataBuf[i] & 0xff00)>>8)-ADCoffset))+512)/1024;
      u8ToDec3(Num[1],temp,0);
      temp=Title[TRACK3][POSI].Value;
      if((DataBuf[i]&0x010000)!=0)temp+=20;
      u8ToDec3(Num[2],temp,0);
      temp=Title[TRACK4][POSI].Value;
      if((DataBuf[i]&0x020000)!=0)temp+=20;
      u8ToDec3(Num[3],temp,0);
    }else{                                                                            //scaled voltage display mode without digital ch's
      S32ToFloat3(Num[0],CSVdata(0,InvertA,i,(DataBuf[i]&0xff)));                     //CH A is always in same field
      if(((OSBuffer)&&(_T_base<17))||((ChartLogic())&&(ChartMode))){
        S32ToFloat3(Num[1],CSVdata(0,InvertA,i,((DataBuf[i]&0xff0000)>>16)));         //OS data for chA stored in chC  > chB
        S32ToFloat3(Num[2],CSVdata(1,InvertB,i,((DataBuf[i]&0xff00)>>8)));            //CH B > chC
        S32ToFloat3(Num[3],CSVdata(1,InvertB,i,((DataBuf[i]&0xff000000)>>24)));       //OS data for chB stored in chD > chD 
      }else{
        S32ToFloat3(Num[1],CSVdata(1,InvertB,i,((DataBuf[i]&0xff00)>>8)));            //CH B
      } 
    }

    for(j=0; j<4; j++){
      if((j>1)&&(!((CSVdataModeLogic())||(((OSBuffer)&&(_T_base<17))||((ChartLogic())&&(ChartMode)))))){    //do not export digital chs if not enabled
        SecBuff[k++] = 0x2c;                                                                //or if they do not contain OS data 
        if(k >= length){
          if (WriteSector(n++,(u16*)pCluster,(u32*)pDirAddr))return WR_ERR;
          k=0;
        }
        break;
      }
      for(count=0; count<6; count++){
        if(Num[j][count] == 0) break;
        SecBuff[k++] = Num[j][count];
        if(k >= length){
          if (WriteSector(n++,(u16*)pCluster,(u32*)pDirAddr))return WR_ERR;
          k=0;
        }
      } 
      SecBuff[k++] = 0x2c;
        if(k >= length){
          if (WriteSector(n++,(u16*)pCluster,(u32*)pDirAddr))return WR_ERR;
          k=0;
        }
    }

    if(i<(bag_max_buf-1))SecBuff[k++] = 0x0d;
        if(k >= length){
          if (WriteSector(n++,(u16*)pCluster,(u32*)pDirAddr))return WR_ERR;
          k=0;
        }
    if(i<(bag_max_buf-1))SecBuff[k++] = 0x0a;
        if(k >= length){
          if (WriteSector(n++,(u16*)pCluster,(u32*)pDirAddr))return WR_ERR;
          k=0;
        }
  }

//==========================================CLOSE FILE===========================================

  if(k != 0){
    memset(&SecBuff[k],0,(length-k));
    if (WriteSector(n,(u16*)pCluster,(u32*)pDirAddr))return WR_ERR;
  }

  if(__CloseFile(SecBuff, (n*length)+k, pCluster, pDirAddr)!= OK) return WR_ERR;

  if(flash_mode==FLASH_2M){                                                             //insert correct file length
    if(FileEntryAdd(SecBuff, pDirAddr, Filename)!=0)return RD_ERR;                      
    if(__ReadDiskData(SecBuff, *pDirAddr & 0xFFFFFF00, 256)!= OK)return RD_ERR;         
      *(u32*)(SecBuff+(*pDirAddr & 0xFF)+0x1C)=(n*length)+k;
    if(__ProgDiskPage(SecBuff, *pDirAddr & 0xFFFFFF00)!=0)return WR_ERR;
  }

  return OK;
}

/*******************************************************************************
Load_Parameter: 加载之前的工作参数                           Return: 0= Success
*******************************************************************************/
u8 Load_Param(u8 FileNum)
{ 
  u8  Sum = 0,j,BootLoad=0; 
  char Filename[12]="CONF    CFG";
  u16 i;
  u16* p =(u16*)SecBuff;
  
  u16 pCluster[3];
  u32 pDirAddr[1]; 
  Versions = 0x06;

  if(((Current==9)&&(_Det==3))||(AutoSetFlag)){
    for(i=0;i<8;i++)ConfigFileName[i]=Filename[i]=SelectedFileName[i];

    ProcessConfigName();    

    ListOverride=1;
  }else{
    if(FileNum==0){
      Word2Hex(Filename, __GetDev_SN());
      if (InitFileSystem()!=0) return 1;
      Filename[8] = 'W'; Filename[9] = 'P'; Filename[10] = 'T';
      BootLoad=1;
    }else Make_Filename(FileNum, Filename);
    ListOverride=0;
  }

  i = __OpenFileRd(SecBuff, Filename, pCluster, pDirAddr);
  if(i != OK) return i;

  if(__ReadFileSec(SecBuff, pCluster)!= OK) return RD_ERR;

  //if((Versions !=(*p & 0xFF))&&((Versions+0x10)!=(*p & 0xFF))) return VER_ERR;          // works with either V6 or V 0x16
  if(Versions !=(*p & 0xFF)) return VER_ERR;          
  Versions=(*p & 0xFF);

  for(i=0; i<512; ++i) Sum += SecBuff[i];
  if(Sum != 0) return SUM_ERR;                        // checksum error
  Current =(*p++ >>8);                                // restore Current Title
  for(i=0; i<7; i++){ 
    Detail[i*2]  = *p;
    Detail[i*2+1]= (*p++ >>8);                        // restore Detail
  }
  for(i=0; i<13; i++){
    for(j=0;j<4;j++)Title[i][j].Value= *p++;
  }
  for(i=0; i<9; i++){
    Meter[i].Item     = *p;
    Meter[i].Track    =(*p++ >>8);       // load measurements and the measurement object
	  }

 if(BootLoad){
  for(i=0; i<10; i++){
    Ka1[i] = *p;                         // restore the original channel A low error correction coefficient
    Kb1[i] =(*p++ >>8);                  // restore the original B-channel low error correction coefficient
    Ka2[i] = *p++;                       // restore the original channel A gain error correction factor
    Kb2[i] = *p++;                       // restore the B-channel gain error correction coefficient
    Ka3[i] = *p;                         // restore the original channel A high error correction coefficient
    Kb3[i] =(*p++ >>8);                  // restore the original B-channel high error correction factor
  }
 }else{					 // do not load correction factors if not loading boot config
    p+=40;				 // allows updating calibration without interference from different configs
 }

 
  V_Trigg[A].Value = *p++;
  V_Trigg[B].Value = *p++;               // restore the original A and B channel trigger threshold
  FlagFrameMode= *p;
  CurDefTime=(*p++ >>8);                 // CurDefTime w/flagframemode  
  FlagMeter= *p;
  UpdateMeter=(*p++ >>8);		     // add additional values in a way compatible with original version   	
  TrgAuto=*p;
  CalFlag=(*p++ >>8);			     // add CalFlag	
  OffsetX=*p;
  SaveShortBuffXpos=(*p++ >>8);	     // add Short Buffer Xpos	
  OffsetY=*p;
  Options=(*p++ >>8);			     // add Options

 if(BootLoad){
    LoBatLevel[0]=*p++;
    LoBatLevel[1]=*p++;
    HiBatLevel[0]=*p++;
    HiBatLevel[1]=*p++;
    for(i=0; i<10; i++){				
      LKa1[i] = *p;                         
      LKb1[i] =(*p++ >>8);               
      LKa2[i] = *p++;                      
      LKb2[i] = *p++;                   
      LKa3[i] = *p;                       
      LKb3[i] =(*p++ >>8);
    }                
    for(i=0; i<10; i++){
      HKa1[i] = *p;                       
      HKb1[i] =(*p++ >>8);               
      HKa2[i] = *p++;                      
      HKb2[i] = *p++;                       
      HKa3[i] = *p;                         
      HKb3[i] =(*p++ >>8);                  
    }
    PPM_Comp = *p++;
  }else p+=85;

    PerstFrameNumber = *p++;
    Raw = *p++;
    PrevShift = *p++; GenFreqShift=PrevShift;
    GenAdjustMode = *p++;

    if(BootLoad){                                            //"system" variable, only load on boot
      Sum=*p;  
      if((*p++>>8)==0xAB)ADCoffset=Sum; else ADCoffset=54;     //0xAB "key" assures default of 54 if ADCoffset value not originally saved         
    }else p++;

    SweepIndex =     *p++;
    Sweep =          *p++;
    BurstLimit =     *p++;
    Det =            *p++;
    FineAdjust=      *p++; 
    DataSize=        *p++;
    SpiMode=         *p++;     
    SpiBitOrder=     *p++;  
    SpiNumBits=      (s16)*p++;   
    SpiAdj=          *p++;
    SubIndex=        *p++;
    GenBaudIndex=    *p++;
    GenUartMode=     *p++;
    GenUartStopBits= *p++;
    OSBuffer=        *p++;         
    OSAvg=           *p++;
    if(BootLoad)FastDim=*p++;else p++;                        //"system" variable, only load on boot  
/*
    FreeRunEnable=   *p++;
    FPGAosFlag=      *p++;
    if(BootLoad)OS_Range=*p++;else p++;                     
*/  p++;p++;p++;

    if(BootLoad)Hbold=*p++;else p++;                         
    ChartMode=       *p++;
    if(BootLoad)LoBeepLevel=*p++;else p++;                    
    if(BootLoad)DisableCursorDisplay=*p++;else p++;

    if((GenUartMode<1)||(GenUartMode>6))GenUartMode=3;          //default to 8N
    if((GenBaudIndex<16)||(GenBaudIndex>15871))GenBaudIndex=7500;
    if(SpiNumBits<-145)SpiNumBits=8;                            //these would flag read errors from possible corrupt config files
    if(SpiNumBits>192)SpiNumBits=8;                             //in such a case return to defaults  
    if(SpiAdj>3)SpiAdj=0;
    if(SpiBitOrder>1)SpiBitOrder=0;
    if(SpiMode>3)SpiMode=0;
    if(SubIndex>13)SubIndex=0;
    if(FastDim>3)FastDim=0;
    if(Hbold>2)Hbold=0;
    if(LoBeepLevel>9)LoBeepLevel=0;
    if(DisableCursorDisplay>3)DisableCursorDisplay=0;
  return OK;
}
/*******************************************************************************
 Save_Parameter: save the current operating parameters		Return: 0= Success
*******************************************************************************/
u8 Save_Param(u8 FileNum)             // save the operating parameters table
{
  u8  Sum = 0;	 
  char Filename[12]="CONF    CFG";
  u16 i, Tmp[2];
  u16* p =(u16*)SecBuff;
  u8 transfer,j;  
  //u8 MissingFileFlag=1;

  u16 pCluster[3];
  u32 pDirAddr[1]; 
  if (InitFileSystem()!=0) return 1;
  Versions=0x06;

  if (Edited){
    for(i=0;i<8;i++)ConfigFileName[i]=Filename[i]=SelectedFileName[i];
    ProcessConfigName();
  }else{
    if(FileNum==0){
      Word2Hex(Filename, __GetDev_SN());
      Filename[8] = 'W'; Filename[9] = 'P'; Filename[10] = 'T';
    }else Make_Filename(FileNum, Filename);
  }

  switch (__OpenFileRd(SecBuff, Filename, pCluster, pDirAddr)){
  case OK:                                                     // original WPT file exists
    //MissingFileFlag=0;
    Tmp[0] = *pCluster;


  if((FileNum==0)&&(Edited==0)){     
    Filename[8] = 'B'; Filename[9] = 'A'; Filename[10] = 'K';  // turn into a BAK file
    if(__OpenFileWr(SecBuff, Filename, pCluster, pDirAddr)!= OK) return DISK_ERR;
    if(__ReadFileSec(SecBuff, Tmp     )!= OK) return RD_ERR;
    i=WriteFileSec(SecBuff, pCluster, pDirAddr);
    if(i)return i;
    if(__CloseFile(SecBuff, 512, pCluster, pDirAddr)!= OK) return WR_ERR;/**/
  }else{
    if(__OpenFileWr(SecBuff, Filename, pCluster, pDirAddr)!= OK) return DISK_ERR;
    memset(SecBuff, 0, SectorSize);
    if(Edited)goto SkipOpenFileWr;
  } 


  case NEW:                                                    // original WPT file does not exist


  if(Edited==0){
    if(FileNum==0){     
      Filename[8] = 'W'; Filename[9] = 'P'; Filename[10] = 'T';  // create WPT files
      if(__OpenFileWr(SecBuff, Filename, pCluster, pDirAddr)!= OK) return DISK_ERR;
      memset(SecBuff, 0, 512);
    }//else if((MissingFileFlag==1)&&(flash_mode==FLASH_2M))return WR_ERR;  // do not write new additional config files unless they already exist to prevent corruption (only on 2MB drives)			     	
  }else{
    if(__OpenFileWr(SecBuff, Filename, pCluster, pDirAddr)!= OK) return DISK_ERR;
    memset(SecBuff, 0, SectorSize);
  }
SkipOpenFileWr:

    *p++ =(Current <<8)+ Versions;           // save the parameter table version number and the current Title

    for(i=0; i<7; i++){ 
      *p++ =(Detail[i*2+1]<<8)+ Detail[i*2]; // Save the Detail
    }
    for(i=0; i<13; i++){                     // Save display each value of the corresponding item in the menu
      for(j=0;j<4;j++)*p++ = Title[i][j].Value;
    }
    for(i=0; i<9; i++){
      *p++ =(Meter[i].Track<<8)+ Meter[i].Item;    // Save the measurement items and measurement object
    }
    for(i=0; i<10; i++){
      transfer=Ka1[i];				   //load into unsigned var first, prevents error with added signed CH-A value spilling over into shifted CH-B when negative	
      *p++ = (Kb1[i]<<8)+ transfer;             
      *p++ =  Ka2[i];                              // save the current channel A gain error correction coefficient
      *p++ =  Kb2[i];                              // save the current B-channel gain error correction factor
      transfer=Ka3[i];
      *p++ = (Kb3[i]<<8)+ transfer;
    }
    *p++ = V_Trigg[A].Value;
    *p++ = V_Trigg[B].Value;                  // save the current A and B channels trigger threshold
    *p++ = (CurDefTime<<8)+FlagFrameMode;     // include Cursor defined selection 
    *p++ = (UpdateMeter<<8)+FlagMeter;        // include meter "page" along with flag		
    *p++ = (CalFlag<<8)+TrgAuto;		    // include wave amplitude calibration flag	
    *p++ = (SaveShortBuffXpos<<8)+OffsetX;    // include short buffer xpos
    *p++ = (Options<<8)+OffsetY; 		    // include Options
    *p++ = LoBatLevel[0];
    *p++ = LoBatLevel[1];
    *p++ = HiBatLevel[0];
    *p++ = HiBatLevel[1];
    for(i=0; i<10; i++){			//save low batt level data
      transfer=LKa1[i];				  
      *p++ = (LKb1[i]<<8)+ transfer;             
      *p++ =  LKa2[i];                             
      *p++ =  LKb2[i];                            
      transfer=LKa3[i];
      *p++ = (LKb3[i]<<8)+ transfer;
    }
    for(i=0; i<10; i++){			//save high batt level data
      transfer=HKa1[i];				   
      *p++ = (HKb1[i]<<8)+ transfer;             
      *p++ =  HKa2[i];                             
      *p++ =  HKb2[i];                              
      transfer=HKa3[i];
      *p++ = (HKb3[i]<<8)+ transfer;
    }
    *p++ = PPM_Comp;
    *p++ = PerstFrameNumber;			
    *p++ = Raw;
    *p++ = GenFreqShift;
    *p++ = GenAdjustMode;
    *p++ = (0xAB<<8)+ADCoffset;		      //use 0xAB as id key to insure reload picks up only saved value
    *p++ = SweepIndex;
    *p++ = Sweep;
    *p++ = BurstLimit;
    *p++ = Det;
    *p++ = FineAdjust;
    *p++ = DataSize;
    *p++ = SpiMode; 
    *p++ = SpiBitOrder;
    *p++ = SpiNumBits;
    *p++ = SpiAdj;
    *p++ = SubIndex;
    *p++ = GenBaudIndex;
    *p++ = GenUartMode;
    *p++ = GenUartStopBits;
    *p++ = OSBuffer; 
    *p++ = OSAvg;
    *p++ = FastDim;  
/*
    *p++ = FreeRunEnable;
    *p++ = FPGAosFlag;
    *p++ = OS_Range;
*/
    p++;p++;p++;

    *p++ = Hbold;
    *p++ = ChartMode;
    *p++ = LoBeepLevel;
    *p++ = DisableCursorDisplay;

    for(i=0; i<511; i++)  Sum += SecBuff[i];          // calculate the parameter table checksum
    SecBuff[511] = (~Sum)+ 1;
    i=WriteFileSec(SecBuff, pCluster, pDirAddr);
    if(i)return i;
    if(__CloseFile(SecBuff, 512, pCluster, pDirAddr)!= OK) return WR_ERR;
    return OK;
    default:  return WR_ERR;
  }//end switch (note, no breaks on cases) 
}

u8 OverWriteWarn(void){
    while(Read_Keys()){} 
    Print_Str((8*11)+3, 0, 0x0405, PRN,  "Warning:File already exists");
    Key_Buffer=0;
    while (1){if(Key_Buffer>0)break;} 
    Update_View_Area();
    _Curr[0].Flag |= UPDAT; 
    _Curr[1].Flag |= UPDAT; 
    _Curr[2].Flag |= UPDAT;
    Display_Title();			
    Print_Str(230, 0, (SCRN<<8)+ TEXT1, PRN, " >   ");
    if(Key_Buffer!=K_INDEX_S){Key_Buffer=0;return 1;}
    return 0;
}

void ProcessConfigName(void){
s8 j;
u8 i;

    for(j=7;j>=0;j--){                                                      //remove trailing spaces and underscores for config file display
      if((ConfigFileName[j]==32)||(ConfigFileName[j]==95))ConfigFileName[j]=0;else break;
    } 
    for(i=0;i<8;i++)if(ConfigFileName[i]==95)ConfigFileName[i]=32;          //change all other underscores to spaces for vertical display 
    for(i=0;i<8;i++){
      switch(ConfigFileName[i]){                                          //process unavailable chars to display dot
        case 39:              //'
        case 64:              //@
          ConfigFileName[i]=63;
          break;
        default:  
        if(((ConfigFileName[i]>32)&&(ConfigFileName[i]<37))||(ConfigFileName[i]>124))ConfigFileName[i]=63;    //limit to char array bounds 
      } 
    }

}

void UpdateFileMenu(void){
u8 i;
  Update_View_Area();
  if(Current==FILE){
    for(i=0;i<3;i++) Title[FILE][i].Flag |= UPDAT;  
  }
} 

/******************************** END OF FILE *********************************/

