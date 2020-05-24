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
  BIOS::DBG::Print("(p:%d,%d,n:%d,P:%d)", pulse[0], pulse[1], pulse.GetSize(), bitstream.GetSize());
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
	gModem.PrepareTxState();
	gModem.FlushRxFifo();
	if (length <= 255)
	{
		gModem.SetFinitePacketLength();
		gModem.SetPacketLength(length);
	} else
	{
		gModem.SetInfinitePacketLength();
		gModem.SetPacketLength(length & 255);
	}

	int toWrite = min(64, length);
	gModem.Write(CC1101_TXFIFO, buffer, toWrite); // max 64 bytes
	length -= toWrite;
	buffer += toWrite;

	gModem.SetTxState();

	long lBase = BIOS::SYS::GetTick();
	int marcState = 0;
	do {
		// add delay, enabling TX goes through calibration, or disable automatic calibration
		// TODO: measure time required for calibration
		marcState = gModem.Read(CC1101_MARCSTATE | CC1101_STATUS_REGISTER) & 0x1F;
		BIOS::SYS::DelayMs(1);
		if (BIOS::SYS::GetTick() - lBase > 1000)
		{
			BIOS::DBG::Print("--TX fail 1-- marc=%d", marcState);
			gModem.FlushTxFifo();
			gModem.SetIdleState();
			return false;
		}
	} while ((marcState != 0x13) && (marcState != 0x14) && (marcState != 0x15));

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
	BIOS::DBG::Print("Send=%dms, ", BIOS::SYS::GetTick() - lBase);
	// TODO: fix sync!
	BIOS::SYS::DelayMs(20);
	gModem.LeaveTxState();
	return true;
}
