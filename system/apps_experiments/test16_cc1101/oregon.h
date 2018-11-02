#include "spi.h"
#include "cc1101.h"

class C1101Receiver : public CC1101
{
  const BIOS::GPIO::EPin GDO0 = BIOS::GPIO::P4;
   
public:
  C1101Receiver()
  {
  }
  
bool Init()
  {    
//    BIOS::GPIO::PinMode(GDO0, BIOS::GPIO::Input);

    CC1101::init(CFREQ_433, 0);

    // http://ygorshko.blogspot.sk/2015/04/finally-set-up-c1101-to-receive-oregon.html
    CC1101::writeReg(CC1101_FIFOTHR, 0x0f); // FIFOTHR: RX FIFO and TX FIFO Thresholds
    CC1101::writeReg(CC1101_SYNC1, 0x55); // SYNC1: Sync Word, High Byte
    CC1101::writeReg(CC1101_SYNC0, 0x99); // SYNC0: Sync Word, Low Byte
    CC1101::writeReg(CC1101_PKTLEN, 255); // PKTLEN: Packet Length
    CC1101::writeReg(CC1101_PKTCTRL0, 0x04); // PKTCTRL0: Packet Automation Control
    CC1101::writeReg(CC1101_FSCTRL1, 0x06); // FSCTRL1: Frequency Synthesizer Control
    CC1101::writeReg(CC1101_FREQ2, 0x10); // FREQ2: Frequency Control Word, High Byte
    CC1101::writeReg(CC1101_FREQ1, 0xb0); // FREQ1: Frequency Control Word, Middle Byte
    CC1101::writeReg(CC1101_FREQ0, 0x71); // FREQ0: Frequency Control Word, Low Byte
    CC1101::writeReg(CC1101_MDMCFG4, 0xf6); // MDMCFG4: Modem Configuration
    CC1101::writeReg(CC1101_MDMCFG3, 0x4a); // MDMCFG3: Modem Configuration
    CC1101::writeReg(CC1101_MDMCFG2, 0x3e); // MDMCFG2: Modem Configuration
    CC1101::writeReg(CC1101_DEVIATN, 0x15); // DEVIATN: Modem Deviation Setting
    CC1101::writeReg(CC1101_MCSM0, 0x18); // MCSM0: Main Radio Control State Machine Configuration
    CC1101::writeReg(CC1101_FOCCFG, 0x16); // FOCCFG: Frequency Offset Compensation Configuration
    CC1101::writeReg(CC1101_AGCCTRL2, 0x05); // AGCCTRL2: AGC Control
    CC1101::writeReg(CC1101_AGCCTRL1, 0x00); // AGCCTRL1: AGC Control
    CC1101::writeReg(CC1101_WORCTRL, 0xfb); // WORCTRL: Wake On Radio Control
    CC1101::writeReg(CC1101_FREND0, 0x11); // FREND0: Front End TX Configuration
    CC1101::writeReg(CC1101_FSCAL3, 0xe9); // FSCAL3: Frequency Synthesizer Calibration
    CC1101::writeReg(CC1101_FSCAL2, 0x2a); // FSCAL2: Frequency Synthesizer Calibration
    CC1101::writeReg(CC1101_FSCAL1, 0x00); // FSCAL1: Frequency Synthesizer Calibration
    CC1101::writeReg(CC1101_FSCAL0, 0x1f); // FSCAL0: Frequency Synthesizer Calibration
    CC1101::writeReg(CC1101_TEST2, 0x81); // TEST2: Various Test Settings
    CC1101::writeReg(CC1101_TEST1, 0x35); // TEST1: Various Test Settings
    CC1101::writeReg(CC1101_TEST0, 0x09); // TEST0: Various Test Settings


    // 0x33, 0xcc
    CC1101::writeReg(CC1101_SYNC1, 0x3c); // SYNC0: Sync Word, Low Byte
    CC1101::writeReg(CC1101_SYNC0, 0x33); // SYNC1: Sync Word, High Byte
    CC1101::writeReg(CC1101_PKTCTRL0, 0x00); // PKTCTRL0: Packet Automation Control
    CC1101::writeReg(CC1101_PKTCTRL1, 0x00); // PKTCTRL0: Packet Automation Control
    CC1101::writeReg(CC1101_PKTLEN, 34+25); // PKTCTRL0: Packet Automation Control

    
    CC1101::writeReg(CC1101_MDMCFG4, 0xc7); // 8400 8421
    CC1101::writeReg(CC1101_MDMCFG3, 0x4A);
    CC1101::writeReg(CC1101_MDMCFG2, 0x34); // 4 = no preamble, 5 = 15/16, 6 = 16/16
    //CC1101::writeReg(CC1101_MDMCFG1, 0x22); // netreba 0x42 0x22

    CC1101::setRxState();

  
    CC1101::writeReg(CC1101_PKTCTRL0, 0x30); // asynchronous serial mode
    CC1101::writeReg(CC1101_IOCFG0, 0x0d);   // GD0 output
    uint8_t check = CC1101::readReg(CC1101_TEST1, CC1101_CONFIG_REGISTER);
    BIOS::DBG::Print("chk=%02x ", check);
    return check == 0x35;
  }

  byte read()
  {
    return BIOS::GPIO::DigitalRead(GDO0);
  }
};

class COregon
{
public:
  C1101Receiver Receiver;

public:
  enum 
  {
    RxPin = 7, 
    sDelay = 230,
    lDelay = 460,
    PacketLength = 10,
    HeaderBits = 20
  };

  bool WaitHeader()
  {   
    return Receiver.read() == 0;
  }

  bool GetHeader()
  {
    byte headerHits = 0;
    long lTimeout = BIOS::SYS::GetTick() + 100;
    
    while (1)
    {
      delayMicroseconds(sDelay);
      
      if (Receiver.read() == 0)
      { 
        delayMicroseconds(lDelay);         
        
        if (Receiver.read() == 0)
          return false;
        
        headerHits ++; 
        if (headerHits == HeaderBits)
          return true; 
      }
      
      do
      {
        if ( (int32_t)BIOS::SYS::GetTick() - lTimeout > 0 ) // POZOR!
          return false;
          
      } while (Receiver.read()==1);
    }
    
    return false; // make sure we look for another header
  }

  byte ReceivePacket(byte* manchester)
  {
    // https://github.com/robwlakes/ArduinoWeatherOS/blob/master/Arduino_OS_WeatherV26.ino
    boolean logic = 1; // look for rest of header 1's, these must be soaked up intil first 0 arrives to denote start of data
    byte signal = 0; //RF Signal is at 0 after 1's 1->0 transition, inverted Manchester (see Wiki, it is a matter of opinion)
    boolean firstZero = false; //The first zero is not immediately found, but is flagged when found
    boolean test230 = false;
    boolean test460 = false;
    int     nosBytes = 0; //counter for the data bytes required
    byte    dataByte = 0; //accumulates the bits of the signal  
    byte    dataMask = 16; //rotates, so allows nybbles to be reversed
    byte    nosBits = 0; //counts the shifted bits in the dataByte
    int     maxBytes = 10; //sets the limits of how many data bytes will be required

    while (1)
    {
      //now get last of the header, and then store the data after trigger bit 0 arrives, and data train timing remains valid 
      long lTimeout = BIOS::SYS::GetTick() + 100;
      while (Receiver.read()!=signal)
      { //halt here while signal matches inverse of logic, if prev=1 wait for sig=0
        if ( (int32_t)BIOS::SYS::GetTick() - lTimeout > 0 )
        {
          return 0;
        }
      }//exits when signal==logic

      delayMicroseconds(sDelay); //wait for first 1/4 of a bit pulse
      test230 = Receiver.read();//snapshot of the input
      if ((test230 == signal)&&(nosBytes < maxBytes)){  //after a wait the signal level is the same, so all good, continue!
        delayMicroseconds(lDelay); //wait for second 1/2 of a bit pulse
        test460=Receiver.read();//snapshot of the input
        if (test230==test460){  // finds a long pulse, so the logic to look for will change, so flip the logic value 
          //Assuming the manchester encoding, a long pulse means data flips, otherwise data stays the same
          logic = logic^1;
          signal = signal^1;
          if (!firstZero){ //if this is the first 0-1 data transition then is the sync 0
            firstZero = true; //flag that legit data has begun
            //VIP OS Seems to put the Synch '0' bit into the data, as it causes the rest to align onto byte boundaries
            dataByte = 0b00000000; // set the byte as 1's (just reflects the header bit that have preceded the trigger bit=0)
            dataMask = 0b00010000; // set the byte as 1's (just reflects the header bit that have preceded the trigger bit=0)
            nosBits = 0;  // preset bit counter so we have 7 bits counted already
          }
        }

        //data stream has been detected begin packing bits into bytes
        if (firstZero){
          if (logic){
            dataByte = dataByte | dataMask; //OR the data bit into the dataByte
          }
          dataMask = dataMask << 1;//rotate the data bit
          if (dataMask==0){
            dataMask=1;//make it roll around, is there a cleaner way than this? eg dataMask *=2?
          }
          nosBits++;
          if (nosBits == 8){ //one byte created, so move onto the next one
            manchester[nosBytes] = dataByte; //store this byte
            nosBits = 0;     //found 8, rezero and get another 8
            dataByte = 0;    //hold the bits in this one
            dataMask = 16;   //mask to do reversed nybbles on the fly
            nosBytes++;      //keep a track of how many bytes we have made
          }
        }
      }  
      else {
        //non valid data found, or maxBytes equalled by nosBytes, reset all pointers and exit the while loop
        return 0;
      }

      if (nosBytes == maxBytes)
      { 
        return nosBytes;
      }
    }
    return 0;
  }
};
