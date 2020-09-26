#include "CC1101.h"
#include "spi.h"

CSpi mSpi;
CC1101 cc1101;

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

void CC1101::SpiStrobe(byte strobe) 
{
        mSpi.select();
        mSpi.fixtransfer(strobe);
        mSpi.deselect();
}

byte CC1101::SpiReadStatus(byte addr) 
{
#ifdef __APPLE__
    return random() & 127;
#endif
        enum {READ_BURST = 0xC0};

	byte value,temp;

	temp = addr | READ_BURST;
	mSpi.select();
	mSpi.fixtransfer(temp);
	value = mSpi.transfer(0);
	mSpi.deselect();

	return value;

}

void CC1101::SpiWriteReg(byte addr, byte value) 
{
        mSpi.select();
        mSpi.wait();
        mSpi.fixtransfer(addr);
        mSpi.fixtransfer(value);
        mSpi.deselect();
}

byte CC1101::SpiReadReg(byte addr) 
{
        enum {READ_SINGLE = 0x80};
        
        mSpi.select();
        mSpi.wait();
        mSpi.transfer(addr | READ_SINGLE);
        uint_fast8_t aux = mSpi.transfer(0);
        mSpi.deselect();
        return aux;
}

void CC1101::RadioConfigure(const uint8_t *config) 
{
    uint8_t i = 0;
    while(config[i] != 0 || config[i+1] != 0){
        SpiWriteReg(config[i],config[i+1]);
        i+=2; 
    }
}

void CC1101::Reset()
{
    mSpi.deselect();                    // Deselect CC1101
    BIOS::SYS::DelayMs(1);
    mSpi.select();                      // Select CC1101
    BIOS::SYS::DelayMs(1);
    mSpi.deselect();                    // Deselect CC1101
    BIOS::SYS::DelayMs(1);
    mSpi.select();                      // Select CC1101
    mSpi.wait();                          // Wait until MISO goes low
    SpiStrobe(CC1101_SRES);
    //mSpi.transfer(CC1101_SRES);                // Send reset command strobe
//BIOS::DBG::Print("C");
    mSpi.wait();                          // Wait until MISO goes low
//BIOS::DBG::Print("D");

    mSpi.deselect();                    // Deselect CC1101
}

    void SpiWriteBurstReg(uint_fast8_t reg, uint8_t* buffer, int len)
    {
	enum { WRITE_BURST = 0x40 };

        mSpi.select();
        mSpi.wait();
        mSpi.transfer(reg | WRITE_BURST);
        while (len--)
          mSpi.transfer(*buffer++);
        mSpi.deselect();
    }

void CC1101::Init(void) 
{
  byte PaTabel[8] = {0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60};
  mSpi.begin();
  Reset();										//CC1101 reset
  SpiWriteBurstReg(CC1101_PATABLE,PaTabel,8);		//CC1101 PATABLE config
}



//void CC1101::Init(byte f) {}
//void CC1101::SendData(byte *txBuffer, byte size) {}
void CC1101::SetReceive(void) 
{
	SpiStrobe(CC1101_SRX);
}

//byte CC1101::CheckReceiveFlag(void) { return 0; }
//byte CC1101::ReceiveData(byte *rxBuffer) {return 0;}
//void CC1101::Suspend(void) { }
//void CC1101::Resume(void) { }
