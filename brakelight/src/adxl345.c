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
	reg |= _BV(ADXL345_PCTL_AUTOSLEEP_BIT);
	adxl345_setRegister(dev, ADXL345_RA_POWER_CTL, reg);
	reg |= _BV(ADXL345_PCTL_MEASURE_BIT);
	adxl345_setRegister(dev, ADXL345_RA_POWER_CTL, reg);
}

TWI_Action_t adxl345_nextAction(const void *dev)
{
	ADXL345_t *device = (ADXL345_t *) dev;
	switch (device->state)
	{
	case REQUEST_REG:
		device->slave.messageBuffer[0] |= TRUE << TWI_READ_BIT;
		device->slave.messageSize = 2;
		device->state = COLLECT_REG;
		return (REPEAT_START);
	case COLLECT_REG:
		TWI_Get_Data_From_Transceiver(&(device->slave));
		device->status_reg.dataReady = TRUE;
		return (STOP);
	case SET_REG:
		return (STOP);
	default:
		break;
	}
	return (STOP);
}

void adxl345_setRegister(ADXL345_t *dev, const uint8_t regAddr,
		const uint8_t data)
{
	dev->state = SET_REG;
	dev->slave.messageBuffer[0] = (dev->slave.address << TWI_ADR_BITS)
			| (FALSE << TWI_READ_BIT);
	dev->slave.messageBuffer[1] = regAddr;
	dev->slave.messageBuffer[2] = data;
	dev->slave.messageSize = 3;
	TWI_Start_Transceiver_With_Data(&(dev->slave));
}

bool adxl345_dataReay(const ADXL345_t *dev)
{
	return (dev->status_reg.dataReady);
}

void adxl345_requestRegister(ADXL345_t *dev, const uint8_t regAddr)
{
	dev->state = REQUEST_REG;
	dev->slave.messageBuffer[0] = (dev->slave.address << TWI_ADR_BITS)
			| (FALSE << TWI_READ_BIT);
	dev->slave.messageBuffer[1] = regAddr;
	dev->slave.messageSize = 2;
	TWI_Start_Transceiver_With_Data(&(dev->slave));
}

void adxl345_readRegister(ADXL345_t *dev, uint8_t * data)
{
	*data = dev->slave.messageBuffer[1];
	dev->status_reg.dataReady = FALSE;
}
