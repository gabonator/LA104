#define CC1101_MARCSTATE         0x35        // Main Radio Control State Machine State
#define WRITE_BURST              0x40
#define READ_SINGLE              0x80
#define READ_BURST               0xC0

/**
 * Type of register
 */
#define CC1101_CONFIG_REGISTER   READ_SINGLE
#define CC1101_STATUS_REGISTER   READ_BURST
#define CC1101_TXFIFO            0x3F        // TX FIFO address
#define CC1101_RXFIFO            0x3F        // RX FIFO address
#define CC1101_TXBYTES           0x3A        // Underflow and Number of Bytes
#define CC1101_RXBYTES           0x3B        // Overflow and Number of Bytes

uint8_t bitstreamData[500];
CArray<uint8_t> bitstream(bitstreamData, COUNT(bitstreamData));

bool sendPacket(uint8_t* buffer, int length);
void PulseToBitstream(const CArray<uint16_t>& pulse, CArray<uint8_t>& bitstream, int interval);

bool SendPulses(const CArray<uint16_t>& pulse, int divisor)
{
  bitstream.SetSize(0);
  PulseToBitstream(pulse, bitstream, divisor);
  //BIOS::DBG::Print("(p:%d,%d,n:%d,P:%d)", pulse[0], pulse[1], pulse.GetSize(), bitstream.GetSize());
  return sendPacket(bitstream.GetData(), bitstream.GetSize());
}

void PulseToBitstream(const CArray<uint16_t>& pulse, CArray<uint8_t>& bitstream, int interval)
{
  int n = 0;

  for (int i=0; i<pulse.GetSize(); i++)
  {
    for (int j=0; j<pulse[i]; j+=interval, n++)
    {
      int bit = 1-(i&1);
      int byteIndex = n >> 3;
      int bitIndex = n & 7;
      if (bitIndex == 0)
        bitstream.Add(0);

      bitstream[byteIndex] |= bit << (7-bitIndex);
    }
  }
}

bool sendPacket(uint8_t* buffer, int length)
{
	//_ASSERT(length < 64);
	//_ASSERT(length < 256);
	long lBase0 = BIOS::SYS::GetTick();

	gModem.PrepareTxState();          // 8 ms
	gModem.FlushRxFifo();
	long lBase01 = BIOS::SYS::GetTick();     
	if (length <= 255)                      // 6 ms
	{
		gModem.SetFinitePacketLength();
		gModem.SetPacketLength(length);
	} else
	{
		gModem.SetInfinitePacketLength();
		gModem.SetPacketLength(length & 255);
	}
	long lBase02 = BIOS::SYS::GetTick();

	int toWrite = min(64, length);
	gModem.Write(CC1101_TXFIFO, buffer, toWrite); // max 64 bytes
	long lBase03 = BIOS::SYS::GetTick();
	                                         // 4ms
	length -= toWrite;
	buffer += toWrite;

	gModem.SetTxState();
	long lBase1 = BIOS::SYS::GetTick();

	long lBase = BIOS::SYS::GetTick();
int temp = 0;
	int marcState = 0;
	do {
temp++;
		// add delay, enabling TX goes through calibration, or disable automatic calibration
		// TODO: measure time required for calibration
		marcState = gModem.Read(CC1101_MARCSTATE | CC1101_STATUS_REGISTER) & 0x1F;
		if (marcState == 0 || marcState == 1) // sleep/idle
		{
			BIOS::DBG::Print("QuickSent? ");
			gModem.LeaveTxState();
			return true;
		}

		BIOS::SYS::DelayMs(1);
		if (BIOS::SYS::GetTick() - lBase > 500)
		{                  
		        // check MCSM1.CCA clear channel assesment
			BIOS::DBG::Print("--TX fail 1-- marc=%d", marcState);
			gModem.FlushTxFifo();
			gModem.SetIdleState();
			return false;
		}
	} while ((marcState != 0x13) && (marcState != 0x14) && (marcState != 0x15));

	long lBase2 = BIOS::SYS::GetTick();
	streamerFlag = 0x3000;
	lBase = BIOS::SYS::GetTick();

	int txbytes = 0;
	do {
		// add delay, or sync with GDO0, or check device state? After transmission it should go to idle
		txbytes = gModem.Read(CC1101_TXBYTES | CC1101_STATUS_REGISTER) & 0x7F;
//		BIOS::DBG::Print("%d ", txbytes);
		if (txbytes < 32 && length > 0)
		{
			toWrite = min(32, length);
			gModem.Write(CC1101_TXFIFO, buffer, toWrite); // max 64 bytes
//			BIOS::DBG::Print("(+%d) ", toWrite);
			length -= toWrite;
			buffer += toWrite;
			if (length == 0)
				gModem.SetFinitePacketLength();

			continue;
		}

//		BIOS::SYS::DelayMs(5); // at 20kbps too long
		BIOS::SYS::DelayMs(1);
		if (BIOS::SYS::GetTick() - lBase > 15000)
		{
			marcState = gModem.Read(CC1101_MARCSTATE | CC1101_STATUS_REGISTER) & 0x1F;
			BIOS::DBG::Print("--TX fail 2 (cnt=%d,marc=%d)--", txbytes, marcState);
			return false;
		}
	} while (txbytes > 0);

	long lBase3 = BIOS::SYS::GetTick();

	lBase = BIOS::SYS::GetTick();

	do {
		marcState = gModem.Read(CC1101_MARCSTATE | CC1101_STATUS_REGISTER) & 0x1F;
		BIOS::SYS::DelayMs(1);
		if (BIOS::SYS::GetTick() - lBase > 20)
		{
			BIOS::DBG::Print("--TX fail 3-- marc=%d", marcState);
			return false;
		}
	} while (marcState != 1); // wait for idle

	long lBase4 = BIOS::SYS::GetTick();

//	BIOS::DBG::Print("Send=%dms, ", BIOS::SYS::GetTick() - lBase0);
	BIOS::DBG::Print("Send=%dms, ", BIOS::SYS::GetTick() - lBase0);
	BIOS::DBG::Print("Send[%d,%d[%d,%d,%d],%d,%d,%d]", temp, lBase1-lBase0, 
          lBase01-lBase0, lBase02-lBase01, lBase03-lBase02,
          lBase2-lBase1, lBase3-lBase2, lBase4-lBase3);
// 1, 19,4, 4, 3
// 1, 20[8,6,4], 6, 4

	gModem.LeaveTxState();
	return true;
}
