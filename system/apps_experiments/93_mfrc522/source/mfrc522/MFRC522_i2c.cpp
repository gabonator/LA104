#include "MFRC522.h"

/////////////////////////////////////////////////////////////////////////////////////
// Basic interface functions for communicating with the MFRC522
/////////////////////////////////////////////////////////////////////////////////////

/**
 * Writes a byte to the specified register in the MFRC522_I2C chip.
 * The interface is described in the datasheet section 8.1.2.
 */
void MFRC522_I2C::PCD_WriteRegister(        MFRC522::PCD_Register reg,               ///< The register to write to. One of the PCD_Register enums.
                                            byte value              ///< The value to write.
) {
        Wire.beginTransmission(_chipAddress);
        Wire.write(reg);
        Wire.write(value);
        Wire.endTransmission();
} // End PCD_WriteRegister()

/**
 * Writes a number of bytes to the specified register in the MFRC522_I2C chip.
 * The interface is described in the datasheet section 8.1.2.
 */
void MFRC522_I2C::PCD_WriteRegister(        MFRC522::PCD_Register reg,               ///< The register to write to. One of the PCD_Register enums.
                                                                        byte count,             ///< The number of bytes to write to the register
                                                                        byte *values    ///< The values to write. Byte array.
                                                                ) {
        Wire.beginTransmission(_chipAddress);
        Wire.write(reg);
        for (byte index = 0; index < count; index++) {
                Wire.write(values[index]);
        }
        Wire.endTransmission();
} // End PCD_WriteRegister()

/**
 * Reads a byte from the specified register in the MFRC522_I2C chip.
 * The interface is described in the datasheet section 8.1.2.
 */
byte MFRC522_I2C::PCD_ReadRegister( MFRC522::PCD_Register reg        ///< The register to read from. One of the PCD_Register enums.
                                                                ) {
        byte value;
        //digitalWrite(_chipSelectPin, LOW);                    // Select slave
        Wire.beginTransmission(_chipAddress);
        Wire.write(reg);
        Wire.endTransmission();

        Wire.requestFrom(_chipAddress, (uint8_t)1 /* bytes to request */);
        value = Wire.read();

        return value;
} // End PCD_ReadRegister()

/**
 * Reads a number of bytes from the specified register in the MFRC522_I2C chip.
 * The interface is described in the datasheet section 8.1.2.
 */
void MFRC522_I2C::PCD_ReadRegister( MFRC522::PCD_Register reg,               ///< The register to read from. One of the PCD_Register enums.
                                                                byte count,             ///< The number of bytes to read
                                                                byte *values,   ///< Byte array to store the values in.
                                                                byte rxAlign    ///< Only bit positions rxAlign..7 in values[0] are updated.
                                                                ) {
        if (count == 0) {
                return;
        }
        byte address = reg;
        byte index = 0;                                                 // Index in values array.
        Wire.beginTransmission(_chipAddress);
        Wire.write(address);
        Wire.endTransmission();
        Wire.requestFrom(_chipAddress, count);
for (int i=0; i<count; i++) {
//        while (Wire.available()) {
                if (index == 0 && rxAlign) {            // Only update bit positions rxAlign..7 in values[0]
                        // Create bit mask for bit positions rxAlign..7
                        byte mask = 0;
                        for (byte i = rxAlign; i <= 7; i++) {
                                mask |= (1 << i);
                        }
                        // Read value and tell that we want to read the same address again.
                        byte value = Wire.read();
if (Wire.error()) return;
                        // Apply mask to both current value of values[0] and the new data in value.
                        values[0] = (values[index] & ~mask) | (value & mask);
                }
                else { // Normal case
                        values[index] = Wire.read();
if (Wire.error()) return;
                }
                index++;
        }
} // End PCD_ReadRegister()


bool MFRC522_I2C::PCD_Init() {
	Wire.begin();
/*
        // Set the chipSelectPin as digital output, do not select the slave yet

        // Set the resetPowerDownPin as digital output, do not reset or power down.
        pinMode(_resetPowerDownPin, OUTPUT);

        if (digitalRead(_resetPowerDownPin) == LOW) {   //The MFRC522 chip is in power down mode.
                digitalWrite(_resetPowerDownPin, HIGH);         // Exit power down mode. This triggers a hard reset.
                // Section 8.8.2 in the datasheet says the oscillator start-up time is the start up time of the crystal + 37,74�s. Let us be generous: 50ms.
                delay(50);
		return true;
        }
	return false;
*/
	BIOS::GPIO::DigitalWrite(BIOS::GPIO::EPin::P3, false);
	BIOS::GPIO::DigitalWrite(BIOS::GPIO::EPin::P3, true);
        BIOS::SYS::DelayMs(50);
	return true;
};

