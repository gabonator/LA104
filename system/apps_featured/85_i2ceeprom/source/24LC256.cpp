/*
	Microchip 24LC256 I2C 256K EEPROM Arduino Library - Code File
	Made by Gonçalo Passos (more info @ http://diogopassos.pt) 2018
	BSD 2-Clause License
*/

#include "24LC256.h"
#include <library.h>

using namespace BIOS::GPIO;

E24LC256::E24LC256(byte address)
{
	this->address = address;
}

/*
	Function to detect if 24LC256 is present in bus with the address passed in constructor.
	Returns true if eeprom is present in i2c bus.
*/

void E24LC256::begin()
{
	  BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::I2c);
	  BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::I2c);
}

void E24LC256::end()
{
      BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Input);
      BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Input);
}

bool E24LC256::detect() {
	byte code;

	if (!I2C::BeginTransmission(address))
		return false;
	if (!I2C::EndTransmission())
		return false;
	return true;
}

/*
	Acknowledge Pooling Algoritihm recomended by Manufacturer	
*/

void E24LC256::ack_pooling()
{
	byte code = -1;
	do
	{
		I2C::BeginTransmission(this->address);
		I2C::Write((byte) 0);
		code = I2C::EndTransmission();
	}
	while(code == 0);
}

/*
	Function that writes a byte in EEPROM.
	address parameter: eeprom cell address between 0x0000 and 0x7CFF
	data parameter: byte to write to a eeprom cell.
	Returns the state of operation (WRITE_SUCESS or WRITE_ERROR)
*/

bool E24LC256::writeByte(int address, byte data)
{
	//ack_pooling();
	if (!I2C::BeginTransmission(this->address))
	        return false;
	I2C::Write((byte) (address>>8));
	I2C::Write((byte) (address & 0xFF));
	I2C::Write((byte) data);

    return I2C::EndTransmission();
}

/*
	Function that reads one byte from eeprom.
	address parameter: eeprom cell address between 0x0000 and 0x7CFF
	Returns the byte readed from eeprom.
*/

bool E24LC256::readByte(int address, byte& value)
{
	if (!I2C::BeginTransmission(this->address))
	{
		I2C::EndTransmission();
		return false;
	}

	I2C::Write((byte) (address>>8));
	I2C::Write((byte) (address&0xFF));
	if (!I2C::EndTransmission())
	{
		return false;
	}
	if (!I2C::RequestFrom(this->address,(int) 1))
	{
		return false;
	}
	value = I2C::Read();
	return I2C::EndTransmission();
}

/*
	Write multiple bytes into eeprom
	address parameter:	address of the first eeprom cell to write
	size parameter:	size of buffer
	buffer parameter: address of buffer in primary memory.
	Returns the state of operation (WRITE_ERROR or WRITE_SUCESS) 

	To do: Check memory size.
*/

int8_t E24LC256::writePage(int address,int size, byte *buffer)
{
	ack_pooling();

	bool error = false;

	if(size > 64)
	{

		int new_size = size;
		int current_buffer_loc;
		int base_addr = address;
		while(new_size > 64)
		{
			I2C::BeginTransmission(this->address);
			I2C::Write((byte) (base_addr>>8));
			I2C::Write((byte) (base_addr&0xFF));

			for(int i = current_buffer_loc; i<current_buffer_loc + 64;i++)
			{
				I2C::Write((byte) buffer[i]);
			}

			error = error || (I2C::EndTransmission() != 0);

			if(error)
			{
				return WRITE_ERROR;
			}

			ack_pooling();

			current_buffer_loc += 63;
			base_addr += 63;
			new_size -= 64;
		}

		I2C::BeginTransmission(this->address);
		I2C::Write((byte) (base_addr>>8));
		I2C::Write((byte) (base_addr&0xFF));

		for(int i = current_buffer_loc ; i<new_size;i++)
		{
			I2C::Write(buffer[i]);
		}

		I2C::EndTransmission();

	}
	else
	{
		I2C::BeginTransmission(this->address);
		I2C::Write((byte) (address>>8));
		I2C::Write((byte) (address&0xFF));
		for(int i = 0; i<size;i++)
		{
			I2C::Write((byte) buffer[i]);
		}

		I2C::EndTransmission();



	}


	if(error)
	{
		return WRITE_ERROR;
	}
	else
	{
		return WRITE_SUCESS;
	}




}

/*
	Read multiple eeprom cell address's and saves into a buffer
	address parameter:	address of the first eeprom cell to read
	size parameter:	size of buffer
	buffer parameter: address of buffer in primary memory.
	Returns the number of bytes readed

*/

bool E24LC256::readPage(int address,int size, byte *buffer)
{
	if (!I2C::BeginTransmission(this->address))
		return false;
	I2C::Write((byte) (address>>8));
	I2C::Write((byte) (address&0xFF));
	if (!I2C::EndTransmission())
		return false;

      I2C::RequestFrom(this->address,size);
	for(int i = 0;i<size;i++)
	{
	        buffer[i] = I2C::Read();
	}

	if (!I2C::EndTransmission())
		return false;
	return true;
}
