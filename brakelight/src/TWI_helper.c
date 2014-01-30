/*
 * TWI_helper.c
 *
 *  Created on: Jan 27, 2014
 *      Author: jeremybutler
 */

#include "TWI_helper.h"
#include "TWI_Master.h"

uint8_t i2c_readBit(const uint8_t bitNum, bool *data)
{
	uint8_t b;
	if (i2c_readByte(&b))
		return (1);
	*data = (b & (1 << bitNum)) ? true : false;
	return (0);
}

uint8_t i2c_writeBit(const uint8_t devAddr, const uint8_t byteAddr,
		const uint8_t bitNum, const bool data)
{
	uint8_t b;
	if (i2c_readByte(devAddr, byteAddr, &b))
		return (1);
	b = data ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	i2c_writeByte(devAddr, byteAddr, b);
	return (0);
}

void i2c_requestByte(const uint8_t devAddr, const uint8_t byteAddr)
{
	i2c_requestBytes();
}

uint8_t i2c_readByte(uint8_t *data)
{
	return (i2c_readBytes(1, data));
}

void i2c_writeByte(const uint8_t devAddr, const uint8_t byteAddr,
		const uint8_t data)
{
	i2c_writeBytes(devAddr, byteAddr, 1, &data);
}

uint8_t i2c_readBytes(const uint8_t size, uint8_t *data)
{
	uint8_t buf[size + 1];
	uint8_t l = size + 1;
	if (!TWI_Get_Data_From_Transceiver(buf, l))
		return (0);
	for (int i = 0; i < size; i++)
	{
		data[i] = buf[i + 1];
	}
	return (1);
}

void i2c_requestBytes(const uint8_t devAddr, const uint8_t byteAddr,
		const uint8_t size)
{
	uint8_t messagebuf[];
}

void i2c_writeBytes(const uint8_t devAddr, const uint8_t byteAddr,
		const uint8_t size, const uint8_t *data)
{
	uint8_t messageBuf[2 + size];
	messageBuf[0] = (devAddr << TWI_ADR_BITS) | (FALSE << TWI_READ_BIT);
	messageBuf[1] = byteAddr;
	for (uint8_t i = 0; i < size; i++)
	{
		messageBuf[i + 2] = data[i];
	}
	TWI_Start_Transceiver_With_Data(messageBuf, size + 2);
}
