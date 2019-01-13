// https://github.com/nitinsane/PCF8591/blob/master/PCF8591.cpp

class CPCF8591
{
  int mAddress;
  const int PCF8591_ADDRESS{0x48};
public:
    
void begin (uint8_t addr)
{
	// i2cAddr cannot be higher than 7
	if (addr > 7)
		addr = 7;
	
	
	mAddress = PCF8591_ADDRESS | addr;
	
	// initialize i2c library
      BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::I2c);
      BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::I2c);
}

uint8_t analogRead (const uint8_t pin)
{
	// there are only 4 pins. so value cannot be more than 3
	if (pin > 3)
		return 0;
	
      BIOS::GPIO::I2C::BeginTransmission(mAddress);

      BIOS::GPIO::I2C::Write(pin);
	
      BIOS::GPIO::I2C::EndTransmission();
	

      BIOS::GPIO::I2C::RequestFrom(mAddress, 2);

      BIOS::GPIO::I2C::Read();
      int aux = BIOS::GPIO::I2C::Read();
      BIOS::GPIO::I2C::EndTransmission();
        return aux;
}
};
