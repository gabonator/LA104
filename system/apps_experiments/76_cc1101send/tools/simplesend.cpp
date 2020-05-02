bool send()
{
  struct packet_t
  {
    int length{4};
    uint8_t data[32];
  } packet;

  packet.data[0] = 0xff;
  packet.data[1] = 0x00;
  packet.data[2] = 0xff;
  packet.data[3] = 0x00;
  packet.data[4] = 0xaa;
  packet.data[5] = 0x00;
  packet.data[6] = 0xaa;
  packet.data[7] = 0xff;

  uint8_t marcState;
  int res = 0;

  if (packet.length > 0)
  {
    CONSOLE::Print("%d bytes ", packet.length);
    gModem.SetDataRate(1000);
    gModem.SetOutputPower(0x50);
    gModem.SetPacketLength(packet.length);
    gModem.Write(CC1101_TXFIFO, packet.data, packet.length); // max 64 bytes

    // CCA enabled: will enter TX state only if the channel is clear
    gModem.SetTxState();
  }

  // Check that TX state is being entered (state = RXTX_SETTLING)
  do {
    // add delay, enabling TX goes through calibration, or disable automatic calibration
    marcState = gModem.Read(CC1101_MARCSTATE | CC1101_STATUS_REGISTER) & 0x1F;
  } while ((marcState != 0x13) && (marcState != 0x14) && (marcState != 0x15));

  if((marcState != 0x13) && (marcState != 0x14) && (marcState != 0x15))
  {
    CONSOLE::Color(RGB565(ff0000));
    CONSOLE::Print("(no tx mode, 0x%02x)", marcState);
    CONSOLE::Color(RGB565(ffffff));

    gModem.SetIdleState();
    gModem.FlushTxFifo();

    return false;
  }

// p. 39
  CONSOLE::Color(RGB565(b0b0b0));
  int txbytes = 0;
bool cont = true;
  do {
    // add delay, or sync with GDO0
    txbytes = gModem.Read(CC1101_TXBYTES | CC1101_STATUS_REGISTER) & 0x7F;
    CONSOLE::Print("%d ", txbytes);
    if (cont && txbytes < 2)
    {
      CONSOLE::Print("!");
      cont = false;
    }
    BIOS::SYS::DelayMs(8);
  } while (txbytes > 0);
  CONSOLE::Color(RGB565(ffffff));

  if(txbytes == 0)
    res = true;
  else {
    CONSOLE::Color(RGB565(ff0000));
    CONSOLE::Print("(not sent, tx bytes=%d)", txbytes);
    CONSOLE::Color(RGB565(ffffff));
  }

  return res;
}

