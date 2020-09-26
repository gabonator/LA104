/*
	Microchip 24LC256 I2C 256K EEPROM Arduino Library - Header File
	Made by Gonçalo Passos (more info @ http://diogopassos.pt) 2018
	BSD 2-Clause License
*/

#ifndef _24LC256_H
#define _24LC256_H

#define WRITE_SUCESS 1
#define WRITE_ERROR -1
#define E24LC256_SIZE 0X7D00

#include <stdint.h>

typedef uint8_t byte;

class E24LC256 
{
public:
	void begin();
    void end();
	bool detect();
	E24LC256(byte address);
	bool writeByte(int address, byte data);
	int8_t writePage(int address,int size, byte *buffer);
	bool readByte(int address, byte& value);
	bool readPage(int address,int size, byte *buffer);
    void ack_pooling();

private:
	byte address;
};

#endif
