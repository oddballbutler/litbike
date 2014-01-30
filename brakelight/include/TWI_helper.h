/*
 * TWI_helper.h
 *
 *	Helper functions for TWI_Master
 *
 *  Created on: Jan 27, 2014
 *      Author: jeremybutler
 */

#include <inttypes.h>
#include <stdbool.h>

#ifndef TWI_HELPER_H_
#define TWI_HELPER_H_

uint8_t i2c_readBit(const uint8_t devAddr, const uint8_t byteAddr,
		const uint8_t bitNum, bool *data);
uint8_t i2c_writeBit(const uint8_t devAddr, const uint8_t byteAddr,
		const uint8_t bitNum, const bool data);

void i2c_requestByte(const uint8_t devAddr, const uint8_t byteAddr);
uint8_t i2c_readByte(uint8_t *data);
void i2c_writeByte(const uint8_t devAddr, const uint8_t byteAddr,
		const uint8_t data);

void i2c_requestBytes(const uint8_t devAddr, const uint8_t byteAddr,
		const uint8_t size);
uint8_t i2c_readBytes(const uint8_t size, uint8_t *data);
void i2c_writeBytes(const uint8_t devAddr, const uint8_t byteAddr,
		const uint8_t size, const uint8_t *data);

#endif /* TWI_HELPER_H_ */
