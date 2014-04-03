/*
 * adxl345.c
 *
 *  Created on: Jan 25, 2014
 *      Author: jeremybutler
 */

#include "adxl345.h"
#include "TWI_Master.h"

#include <avr/sfr_defs.h>
#include <stdbool.h>

/**
 * Initialization function for the ADXL345
 * This function should be called before any other ADXL345 functions are used.
 * @param dev A pointer to the ADXL345 object.
 * @param address The address of the ADXL345 on the I2C bus or NULL if the
 * default address is desired.
 */
void adxl345_initialize(ADXL345_t *dev, const uint8_t address)
{
	uint8_t reg = 0;
	if (address)
		dev->slave.address = address;
	else
	{
		dev->slave.address = ADXL345_DEFAULT_ADDRESS;
	}
	dev->status_reg.all = ADXL345_DEFAULT_STATUS;
	dev->slave.TWI_Next_Action = adxl345_nextAction;
	dev->slave.device = (void *) dev;
	reg |= _BV(ADXL345_PCTL_AUTOSLEEP_BIT) | _BV(ADXL345_PCTL_MEASURE_BIT);
	adxl345_setRegister(dev, ADXL345_RA_POWER_CTL, reg);
	while (!TWI_statusReg.lastTransOK)
		;
}

/**
 * Implementation of the callback function required by TWI_Master.
 * @param dev Pointer to the ADXL345 object.
 * @return The next action to be performed by TWI_Master.
 */
TWI_Action_t adxl345_nextAction(const void *dev)
{
	ADXL345_t *device = (ADXL345_t *) dev;
	switch (device->state)
	{
	case ADXL345_REQUEST_REGS:
		device->slave.messageBuffer[0] |= TRUE << TWI_READ_BIT;
		device->slave.messageSize = device->slave.readSize;
		device->state = ADXL345_COLLECT_REGS;
		return (TWI_REPEAT_START);
	case ADXL345_COLLECT_REGS:
		device->state = ADXL345_IDLE;
		device->status_reg.dataReady = TRUE;
		return (TWI_STOP);
	case ADXL345_SET_REG:
		device->state = ADXL345_IDLE;
		return (TWI_STOP);
	default:
		break;
	}
	return (TWI_STOP);
}

/**
 * Set a register in the ADXL345 to a value.
 * @param dev Pointer to the ADXL345 object.
 * @param regAddr The address of the register that is to be set.
 * @param data The data that is to be written to the register.
 */
void adxl345_setRegister(ADXL345_t *dev, const uint8_t regAddr,
		const uint8_t data)
{
	dev->status_reg.dataReady = FALSE;
	dev->state = ADXL345_SET_REG;
	dev->slave.messageBuffer[0] = (dev->slave.address << TWI_ADR_BITS)
			| (FALSE << TWI_READ_BIT);
	dev->slave.messageBuffer[1] = regAddr;
	dev->slave.messageBuffer[2] = data;
	dev->slave.messageSize = 3;
	TWI_Start_Transceiver_With_Data(&(dev->slave));
}

/**
 * Is there new data to be read from the ADXL345?
 * This function should be polled after a call to adxl_requestRegister() to
 * determine when the data has arrived from the ADXL345.
 * @param dev A pointer to the ADXL345 object.
 * @return TRUE if data is ready to be read. FALSE otherwise.
 */
bool adxl345_dataReady(const ADXL345_t *dev)
{
	return (dev->status_reg.dataReady);
}

/**
 * Request that a register be retrieved from the ADXL345 module.
 * This function should be called before adxl345_readRegister().
 * @param dev A pointer to the ADXL345 object.
 * @param regAddr The address of the register to be requested.
 */
void adxl345_requestRegister(ADXL345_t *dev, const uint8_t regAddr)
{
	adxl345_requestRegisters(dev, regAddr, 1);
}

/**
 * Read the last requested register.
 * Once adxl345_dataReady() returns TRUE, this function can be called to read
 * the data that was requested using adxl345_requestRegister().
 * As an alternative to polling adxl345_dataReady(), this function can be polled
 * to immediately read the data once it is ready.
 * @param dev A pointer to the ADXL345 object.
 * @param data A pointer to the location where the read data is to be stored.
 * @return 1 if data read successfully. 0 if data is not ready to be read.
 */
uint8_t adxl345_readRegister(ADXL345_t *dev, uint8_t * data)
{
	return (adxl345_readRegisters(dev, data, 1));
}

void adxl345_requestRegisters(ADXL345_t *dev, const uint8_t regAddr,
		const uint8_t length)
{
	dev->status_reg.dataReady = FALSE;
	dev->state = ADXL345_REQUEST_REGS;
	dev->slave.messageBuffer[0] = (dev->slave.address << TWI_ADR_BITS)
			| (FALSE << TWI_READ_BIT);
	dev->slave.messageBuffer[1] = regAddr;
	dev->slave.messageSize = 2;
	dev->slave.readSize = length + 1;
	TWI_Start_Transceiver_With_Data(&(dev->slave));
}

uint8_t adxl345_readRegisters(ADXL345_t *dev, uint8_t *data,
		const uint8_t length)
{
	if (!(dev->status_reg.dataReady))
	{
		return (0);
	}
	TWI_Get_Data_From_Transceiver(&(dev->slave));
	for (int i = 0; i < length; i++)
	{
		data[i] = dev->slave.messageBuffer[i + 1];
	}
	return (1);
}
