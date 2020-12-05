// Bitbanged implementation of i2c bus, because hardware transceiver cannot recover from
// sudden removal of device and is stuck in BUSY state. Only restart helps

namespace i2c 
{
// https://github.com/todbot/SoftI2CMaster/blob/master/SoftI2CMaster.cpp
void sda_hi() { BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::EMode(BIOS::GPIO::Input | BIOS::GPIO::PullUp)); }
void scl_hi() { BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::EMode(BIOS::GPIO::Input | BIOS::GPIO::PullUp)); }
void sda_lo() { BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Output); BIOS::GPIO::DigitalWrite(BIOS::GPIO::P2, false); }
void scl_lo() { BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Output); BIOS::GPIO::DigitalWrite(BIOS::GPIO::P1, false); }
void delay() { for (volatile int i=0; i<1000; i++); }
bool sda() { return BIOS::GPIO::DigitalRead(BIOS::GPIO::P2); }

bool writebit( int c )
{
    if (c)
    {
        sda_hi();
        delay();
        if (!sda())
            return false;
        scl_hi();
        delay();
        scl_lo();
        delay();
    } else 
    {
        sda_lo();
        delay();
        if (sda())
            return false;
        scl_hi();
        delay();
        scl_lo();
        delay();
    }
    return true;
}

int readbit(void)
{
    sda_hi();
    delay();
    scl_hi();
    delay();
    int ack = sda();
    scl_lo();
    delay();
    return ack == 0;
}

bool start()
{
    sda_hi();
    scl_hi();
    delay();

    if (!sda())
        return false;

    sda_lo();
    delay();

    if (sda())
        return false;

    scl_lo();
    delay();
    return true;
}

int write( int c )
{
    bool ok = true;
    for ( uint8_t i=0;i<8;i++) {
        if (!writebit( c & 128 ))
            ok = false;
        c<<=1;
    }

    return ok && readbit();
}

void stop()
{
    scl_hi();
    delay();
    sda_hi();
    delay();
}

bool testPresence(int address)
{
  if (!start())
  {
    stop();
    return false;
  }
  if (!write(address<<1))
  {
    stop();
    return false;
  }
  stop();
  return true;
}

}