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

bool sendPacket(uint32_t frequency, uint32_t bps, uint8_t* buffer, int length)
{
#ifdef __APPLE__
	return true;
#endif
	gModem.SetFrequency(frequency);
	gModem.SetDataRate(bps);
	//  gModem.SetOutputPower(0x50);
	gModem.SetOutputPower(0xb0);
	_ASSERT(length < 64);
	gModem.SetPacketLength(length);
	gModem.Write(CC1101_TXFIFO, buffer, length); // max 64 bytes
	gModem.SetTxState();

long lBase = BIOS::SYS::GetTick();
	int marcState = 0;
	do {
		// add delay, enabling TX goes through calibration, or disable automatic calibration
		// TODO: measure time required for calibration
		marcState = gModem.Read(CC1101_MARCSTATE | CC1101_STATUS_REGISTER) & 0x1F;
		BIOS::SYS::DelayMs(5);
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
		//CONSOLE::Print("%d ", txbytes);
		BIOS::SYS::DelayMs(5);
if (BIOS::SYS::GetTick() - lBase > 1000)
{
  BIOS::DBG::Print("--TX fail 2--");
  return false;
}

	} while (txbytes > 0);
	BIOS::SYS::DelayMs(20);
	return true;
}
