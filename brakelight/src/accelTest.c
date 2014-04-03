/*
 * accelTest.c
 *
 *  Created on: Feb 7, 2014
 *      Author: jeremybutler
 */

#include "adxl345.h"
#include "TWI_Master.h"
#include "avr/interrupt.h"

#include <avr/io.h>

int main(void)
{
	ADXL345_t accel;
	uint8_t data[6] =
	{ 0, 0, 0, 0, 0, 0 };
	int16_t xAccel = 0;
	int16_t yAccel = 0;
	int16_t zAccel = 0;

	DDRD |= _BV(DDD6) | _BV(DDD5); // Set led ports to be outputs
	DDRC &= ~(_BV(DDC5) | _BV(DDC4)); // Set I2C bus to inputs
	TWI_Master_Initialize();
	sei();
	adxl345_initialize(&accel, 0);

	while (1)
	{
		if (adxl345_readRegisters(&accel, data, 6))
		{
			xAccel = (data[1] << 8) | data[0];
			yAccel = (data[3] << 8) | data[2];
			zAccel = (data[5] << 8) | data[4];
		}
		if (accel.state == ADXL345_IDLE)
		{
			adxl345_requestRegisters(&accel, ADXL345_RA_DATAX0, 6);
		}
		if(xAccel > 100)
		{
			PORTD = 0xFF;
		}
		else
		{
			PORTD = 0x00;
		}

	}
	return (0);
}
