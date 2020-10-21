/*
* MFRC522_SPI.cpp - Library to use ARDUINO RFID MODULE KIT 13.56 MHZ WITH TAGS SPI W AND R BY COOQROBOT.
* NOTE: Please also check the comments in MFRC522_SPI.h - they provide useful hints and background information.
* Released into the public domain.
*/

#include "MFRC522.h"
#include "../spi/spi.h"

CSpi SPI;

/**
 * Writes a byte to the specified register in the MFRC522_SPI chip.
 * The interface is described in the datasheet section 8.1.2.
 */
void MFRC522_SPI::PCD_WriteRegister(	MFRC522::PCD_Register reg,	///< The register to write to. One of the PCD_Register enums.
									byte value			///< The value to write.
								) {
	SPI.select();
	SPI.fixtransfer(reg << 1);
	SPI.fixtransfer(value);
	SPI.deselect();
} // End PCD_WriteRegister()

/**
 * Writes a number of bytes to the specified register in the MFRC522_SPI chip.
 * The interface is described in the datasheet section 8.1.2.
 */
void MFRC522_SPI::PCD_WriteRegister(	MFRC522::PCD_Register reg,	///< The register to write to. One of the PCD_Register enums.
									byte count,			///< The number of bytes to write to the register
									byte *values		///< The values to write. Byte array.
								) {
	SPI.select();
	SPI.fixtransfer(reg << 1);
	for (byte index = 0; index < count; index++) {
		SPI.fixtransfer(values[index]);
	}
	SPI.deselect();
} // End PCD_WriteRegister()

/**
 * Reads a byte from the specified register in the MFRC522_SPI chip.
 * The interface is described in the datasheet section 8.1.2.
 */
byte MFRC522_SPI::PCD_ReadRegister(	MFRC522::PCD_Register reg	///< The register to read from. One of the PCD_Register enums.
								) {
	byte value;
	SPI.select();
	SPI.fixtransfer(0x80 | (reg << 1));					// MSB == 1 is for reading. LSB is not used in address. Datasheet section 8.1.2.3
	value = SPI.transfer(0);					// Read the value back. Send 0 to stop reading.
	SPI.deselect();
	return value;
} // End PCD_ReadRegister()

/**
 * Reads a number of bytes from the specified register in the MFRC522_SPI chip.
 * The interface is described in the datasheet section 8.1.2.
 */
void MFRC522_SPI::PCD_ReadRegister(	MFRC522::PCD_Register reg,	///< The register to read from. One of the PCD_Register enums.
								byte count,			///< The number of bytes to read
								byte *values,		///< Byte array to store the values in.
								byte rxAlign		///< Only bit positions rxAlign..7 in values[0] are updated.
								) {
	if (count == 0) {
		return;
	}
	//Serial.print(F("Reading ")); 	Serial.print(count); Serial.println(F(" bytes from register."));
	byte address = 0x80 | (reg << 1);				// MSB == 1 is for reading. LSB is not used in address. Datasheet section 8.1.2.3.
	byte index = 0;							// Index in values array.
	SPI.select();
	count--;								// One read is performed outside of the loop
	SPI.fixtransfer(address);					// Tell MFRC522_SPI which address we want to read
	if (rxAlign) {		// Only update bit positions rxAlign..7 in values[0]
		// Create bit mask for bit positions rxAlign..7
		byte mask = (0xFF << rxAlign) & 0xFF;
		// Read value and tell that we want to read the same address again.
		byte value = SPI.transfer(address);
		// Apply mask to both current value of values[0] and the new data in value.
		values[0] = (values[0] & ~mask) | (value & mask);
		index++;
	}
	while (index < count) {
		values[index] = SPI.transfer(address);	// Read value and tell that we want to read the same address again.
		index++;
	}
	values[index] = SPI.transfer(0);			// Read the final byte. Send 0 to stop reading.
	SPI.deselect(); // Stop using the SPI bus
} // End PCD_ReadRegister()

bool MFRC522_SPI::PCD_Init() {
        // Set the chipSelectPin as digital output, do not select the slave yet
	SPI.begin();

/*
        // If a valid pin number has been set, pull device out of power down / reset state.
        if (_resetPowerDownPin != UNUSED_PIN) {
                // Set the resetPowerDownPin as digital output, do not reset or power down.
                pinMode(_resetPowerDownPin, OUTPUT);

                if (digitalRead(_resetPowerDownPin) == LOW) {   // The MFRC522 chip is in power down mode.
                        digitalWrite(_resetPowerDownPin, HIGH);         // Exit power down mode. This triggers a hard reset.
                        // Section 8.8.2 in the datasheet says the oscillator start-up time is the start up time of the crystal + 37,74μs. Let us be generous: 50ms.
                        delay(50);
			return true;
                }
        }*/
	return SPI.reset();
}

