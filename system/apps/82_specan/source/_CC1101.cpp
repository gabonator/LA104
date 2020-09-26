/*

	This library was originally copyright of Michael at elechouse.com but permision was
    granted by Wilson Shen on 2016-10-23 for me (Simon Monk) to uodate the code for Arduino 1.0+
    and release the code on github under the MIT license.

	
*/
#include "CC1101.h"
#include "SmartResponseXE.h"

/****************************************************************/
#define 	WRITE_BURST     	0x40						//write burst
#define 	READ_SINGLE     	0x80						//read single
#define 	READ_BURST      	0xC0						//read burst
#define 	BYTES_IN_RXFIFO     0x7F  						//byte number in RXfifo

/****************************************************************/
byte PaTabel[8] = {0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60};

/****************************************************************
*FUNCTION NAME:SpiInit
*FUNCTION     :spi communication initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void CC1101::SpiInit(void)
{
  // initialize the SPI pins
  pinMode(SCK_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(SS_PIN, OUTPUT);

  // enable SPI Master, MSB, SPI mode 0, FOSC/4
  SpiMode(0);
}
/****************************************************************
*FUNCTION NAME:SpiMode
*FUNCTION     :set spi mode
*INPUT        :        config               mode
			   (0<<CPOL) | (0 << CPHA)		 0
			   (0<<CPOL) | (1 << CPHA)		 1
			   (1<<CPOL) | (0 << CPHA)		 2
			   (1<<CPOL) | (1 << CPHA)		 3
*OUTPUT       :none
****************************************************************/
void CC1101::SpiMode(byte config)
{
#if 0
  byte tmp;

  // enable SPI master with configuration byte specified
  SPCR = 0;
  SPCR = (config & 0x7F) | (1<<SPE) | (1<<MSTR);
  tmp = SPSR;
  tmp = SPDR;
#endif
}


/****************************************************************
*FUNCTION NAME:Suspend
*FUNCTION     :suspend radio
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void CC1101::Suspend(void){
  digitalWrite(SS_PIN, LOW);
  // only volatile state is lost
  SpiTransfer(CC1101_SPWD);
  digitalWrite(SS_PIN, HIGH);
}


/****************************************************************
*FUNCTION NAME:Resume
*FUNCTION     :resume radio
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void CC1101::Resume(void){
        digitalWrite(SS_PIN, LOW);
        delay(1);   
        digitalWrite(SS_PIN, HIGH);
}


/****************************************************************
*FUNCTION NAME:SpiTransfer
*FUNCTION     :spi transfer
*INPUT        :value: data to send
*OUTPUT       :data to receive
****************************************************************/
/*
byte CC1101::SpiTransfer(byte value)
{
  SPDR = value;
  while (!(SPSR & (1<<SPIF))) ;
  return SPDR;
}
*/
/****************************************************************
*FUNCTION NAME: GDO_Set()
*FUNCTION     : set GDO0,GDO2 pin
*INPUT        : none
*OUTPUT       : none
****************************************************************/
/*
void CC1101::GDO_Set (void)
{
	pinMode(GDO0, INPUT);
	//pinMode(GDO2, INPUT);
}
*/
/****************************************************************
*FUNCTION NAME:Reset
*FUNCTION     :CC1101 reset //details refer datasheet of CC1101/CC1100//
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void CC1101::Reset (void)
{
	digitalWrite(SS_PIN, LOW);
	delay(1);
	digitalWrite(SS_PIN, HIGH);
	delay(1);
	digitalWrite(SS_PIN, LOW);


	//while(digitalRead(MISO_PIN)){Serial.println(1);};

	SpiTransfer(CC1101_SRES);

	//while(digitalRead(MISO_PIN)){Serial.println(2);};
	digitalWrite(SS_PIN, HIGH);
}

/****************************************************************
*FUNCTION NAME:Init
*FUNCTION     :CC1101 initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void CC1101::Init(void)
{
	//SpiInit();										//spi initialization
	GDO_Set();	
	digitalWrite(SS_PIN, HIGH);
	digitalWrite(SCK_PIN, HIGH);
	digitalWrite(MOSI_PIN, LOW);

	Reset();	
									//CC1101 reset
	//RegConfigSettings(F_433);						//CC1101 register config
	SpiWriteBurstReg(CC1101_PATABLE,PaTabel,8);		//CC1101 PATABLE config
}

/****************************************************************
*FUNCTION NAME:Init
*FUNCTION     :CC1101 initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void CC1101::Init(byte f)
{
	SpiInit();										//spi initialization
	GDO_Set();										//GDO set
	digitalWrite(SS_PIN, HIGH);
	digitalWrite(SCK_PIN, HIGH);
	digitalWrite(MOSI_PIN, LOW);
	Reset();										//CC1101 reset
	//RegConfigSettings(f);							//CC1101 register config
	SpiWriteBurstReg(CC1101_PATABLE,PaTabel,8);		//CC1101 PATABLE config
}


void CC1101::RadioConfigure(const uint8_t *config){
    uint8_t i = 0;
    while(config[i] != 0 || config[i+1] != 0){
        SpiWriteReg(config[i],config[i+1]);
        i+=2; 
    }
}

/****************************************************************
*FUNCTION NAME:SpiWriteReg
*FUNCTION     :CC1101 write data to register
*INPUT        :addr: register address; value: register value
*OUTPUT       :none
****************************************************************/
void CC1101::SpiWriteReg(byte addr, byte value)
{
	digitalWrite(SS_PIN, LOW);
	//while(digitalRead(MISO_PIN)){Serial.println(3);};
	SpiTransfer(addr);
	SpiTransfer(value);
	digitalWrite(SS_PIN, HIGH);
}

/****************************************************************
*FUNCTION NAME:SpiWriteBurstReg
*FUNCTION     :CC1101 write burst data to register
*INPUT        :addr: register address; buffer:register value array; num:number to write
*OUTPUT       :none
****************************************************************/
void CC1101::SpiWriteBurstReg(byte addr, byte *buffer, byte num)
{
	byte i, temp;

	temp = addr | WRITE_BURST;
    digitalWrite(SS_PIN, LOW);
    //while(digitalRead(MISO_PIN)){Serial.println(4);};
    SpiTransfer(temp);
    for (i = 0; i < num; i++)
 	{
        SpiTransfer(buffer[i]);
    }
    digitalWrite(SS_PIN, HIGH);
}

/****************************************************************
*FUNCTION NAME:SpiStrobe
*FUNCTION     :CC1101 Strobe
*INPUT        :strobe: command; //refer define in CC1101.h//
*OUTPUT       :none
****************************************************************/
void CC1101::SpiStrobe(byte strobe)
{
	digitalWrite(SS_PIN, LOW);
	//while(digitalRead(MISO_PIN)){Serial.println(5);};
	SpiTransfer(strobe);
	digitalWrite(SS_PIN, HIGH);
}

/****************************************************************
*FUNCTION NAME:SpiReadReg
*FUNCTION     :CC1101 read data from register
*INPUT        :addr: register address
*OUTPUT       :register value
****************************************************************/
byte CC1101::SpiReadReg(byte addr) 
{
	byte temp, value;

    temp = addr|READ_SINGLE;
	digitalWrite(SS_PIN, LOW);
	//while(digitalRead(MISO_PIN)){Serial.println(6);};
	SpiTransfer(temp);
	value=SpiTransfer(0);
	digitalWrite(SS_PIN, HIGH);

	return value;
}

/****************************************************************
*FUNCTION NAME:SpiReadBurstReg
*FUNCTION     :CC1101 read burst data from register
*INPUT        :addr: register address; buffer:array to store register value; num: number to read
*OUTPUT       :none
****************************************************************/
void CC1101::SpiReadBurstReg(byte addr, byte *buffer, byte num)
{
	byte i,temp;

	temp = addr | READ_BURST;
	digitalWrite(SS_PIN, LOW);
	//while(digitalRead(MISO_PIN)){Serial.println(7);};
	SpiTransfer(temp);
	for(i=0;i<num;i++)
	{
		buffer[i]=SpiTransfer(0);
	}
	digitalWrite(SS_PIN, HIGH);
}

/****************************************************************
*FUNCTION NAME:SpiReadStatus
*FUNCTION     :CC1101 read status register
*INPUT        :addr: register address
*OUTPUT       :status value
****************************************************************/
byte CC1101::SpiReadStatus(byte addr) 
{
	byte value,temp;

	temp = addr | READ_BURST;
	digitalWrite(SS_PIN, LOW);
	//while(digitalRead(MISO_PIN)){Serial.println(8);};
	SpiTransfer(temp);
	value=SpiTransfer(0);
	digitalWrite(SS_PIN, HIGH);

	return value;
}

/****************************************************************
*FUNCTION NAME:RegConfigSettings
*FUNCTION     :CC1101 register config //details refer datasheet of CC1101/CC1100//
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void CC1101::RegConfigSettings(byte f) 
{
    SpiWriteReg(CC1101_FSCTRL1,  0x08);
    SpiWriteReg(CC1101_FSCTRL0,  0x00);
	
    switch(f)
    {
      case F_868:
      	SpiWriteReg(CC1101_FREQ2,    F2_868);
      	SpiWriteReg(CC1101_FREQ1,    F1_868);
      	SpiWriteReg(CC1101_FREQ0,    F0_868);
        break;
      case F_915:
        SpiWriteReg(CC1101_FREQ2,    F2_915);
        SpiWriteReg(CC1101_FREQ1,    F1_915);
        SpiWriteReg(CC1101_FREQ0,    F0_915);
        break;
	  case F_433:
        SpiWriteReg(CC1101_FREQ2,    F2_433);
        SpiWriteReg(CC1101_FREQ1,    F1_433);
        SpiWriteReg(CC1101_FREQ0,    F0_433);
        break;
	  default: // F must be set
	  	break;
	}
	
    SpiWriteReg(CC1101_MDMCFG4,  0x5B);
    SpiWriteReg(CC1101_MDMCFG3,  0xF8);
    SpiWriteReg(CC1101_MDMCFG2,  0x03);
    SpiWriteReg(CC1101_MDMCFG1,  0x22);
    SpiWriteReg(CC1101_MDMCFG0,  0xF8);
    SpiWriteReg(CC1101_CHANNR,   0x00);
    SpiWriteReg(CC1101_DEVIATN,  0x47);
    SpiWriteReg(CC1101_FREND1,   0xB6);
    SpiWriteReg(CC1101_FREND0,   0x10);
    SpiWriteReg(CC1101_MCSM0 ,   0x18);
    SpiWriteReg(CC1101_FOCCFG,   0x1D);
    SpiWriteReg(CC1101_BSCFG,    0x1C);
    SpiWriteReg(CC1101_AGCCTRL2, 0xC7);
	SpiWriteReg(CC1101_AGCCTRL1, 0x00);
    SpiWriteReg(CC1101_AGCCTRL0, 0xB2);
    SpiWriteReg(CC1101_FSCAL3,   0xEA);
	SpiWriteReg(CC1101_FSCAL2,   0x2A);
	SpiWriteReg(CC1101_FSCAL1,   0x00);
    SpiWriteReg(CC1101_FSCAL0,   0x11);
    SpiWriteReg(CC1101_FSTEST,   0x59);
    SpiWriteReg(CC1101_TEST2,    0x81);
    SpiWriteReg(CC1101_TEST1,    0x35);
    SpiWriteReg(CC1101_TEST0,    0x09);
    SpiWriteReg(CC1101_IOCFG2,   0x0B); 	//serial clock.synchronous to the data in synchronous serial mode
    SpiWriteReg(CC1101_IOCFG0,   0x06);  	//asserts when sync word has been sent/received, and de-asserts at the end of the packet 
    SpiWriteReg(CC1101_PKTCTRL1, 0x04);		//two status bytes will be appended to the payload of the packet,including RSSI LQI and CRC OK
											//No address check
    SpiWriteReg(CC1101_PKTCTRL0, 0x05);		//whitening off;CRC Enable
    SpiWriteReg(CC1101_ADDR,     0x00);		//address used for packet filtration.
    SpiWriteReg(CC1101_PKTLEN,   0x3D); 	//61 bytes max length
}

/****************************************************************
*FUNCTION NAME:SendData
*FUNCTION     :use CC1101 send data
*INPUT        :txBuffer: data array to send; size: number of data to send, no more than 61
*OUTPUT       :none
****************************************************************/
void CC1101::SendData(byte *txBuffer,byte size)
{
	SpiWriteReg(CC1101_TXFIFO,size);
	SpiWriteBurstReg(CC1101_TXFIFO,txBuffer,size);			//write data to send
	SpiStrobe(CC1101_STX);									//start send	
	while (!digitalRead(GDO0));								// Wait for GDO0 to be set -> sync transmitted  
	while (digitalRead(GDO0));								// Wait for GDO0 to be cleared -> end of packet
	SpiStrobe(CC1101_SFTX);									//flush TXfifo
}

/****************************************************************
*FUNCTION NAME:SetReceive
*FUNCTION     :set CC1101 to receive state
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void CC1101::SetReceive(void)
{
	SpiStrobe(CC1101_SRX);
}

/****************************************************************
*FUNCTION NAME:CheckReceiveFlag
*FUNCTION     :check receive data or not
*INPUT        :none
*OUTPUT       :flag: 0 no data; 1 receive data 
****************************************************************/
byte CC1101::CheckReceiveFlag(void)
{
	if(digitalRead(GDO0))			//receive data
	{
		while (digitalRead(GDO0));
		return 1;
	}
	else							// no data
	{
		return 0;
	}
}


/****************************************************************
*FUNCTION NAME:ReceiveData
*FUNCTION     :read data received from RXfifo
*INPUT        :rxBuffer: buffer to store data
*OUTPUT       :size of data received
****************************************************************/
byte CC1101::ReceiveData(byte *rxBuffer)
{
	byte size;
	byte status[2];

	if(SpiReadStatus(CC1101_RXBYTES) & BYTES_IN_RXFIFO)
	{
		size=SpiReadReg(CC1101_RXFIFO);
		SpiReadBurstReg(CC1101_RXFIFO,rxBuffer,size);
		SpiReadBurstReg(CC1101_RXFIFO,status,2);
		SpiStrobe(CC1101_SFRX);
		return size;
	}
	else
	{
		SpiStrobe(CC1101_SFRX);
		return 0;
	}
	
}

CC1101 cc1101;




