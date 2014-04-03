/*
 * brakelight.c
 *
 *  Created on: Jan 22, 2014
 *      Author: jeremybutler
 */

#include "brakelight.h"
#include "uart.h"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <math.h>

#define UART_BAUD_RATE 9600
#define GRAVITY 240

uint16_t BRAKELIGHT_THRESHOLD = 3; // Values above this are considered braking
uint16_t BRAKELIGHT_FIRST_WEIGHT = 1024; //make this a power of 2.
uint16_t BRAKELIGHT_SECOND_WEIGHT = 16; //make this a power of 2.
char rxBuf[7];
uint8_t rxBufIndex = 0;
uint8_t commandReceived = 0;
uint16_t counter = 0;
uint16_t counter1 = 0;
uint16_t brakeDelay = 1000;
uint8_t recentlyBraked = 0;

void brakelight_initialize(Brakelight_t * brakelight)
{
	// SETUP PORTS

	DDRC &= ~(_BV(DDC5) | _BV(DDC4)); // Set I2C bus to inputs
	//PORTC |= _BV(PORTC5) | _BV(PORTC4); // Put pull-up resistors on the I2C bus

	// SETUP I2C
	TWI_Master_Initialize();
	pwmled_initialize(&(brakelight->led1), &LED1_DUTY_CYCLE_REG);
	pwmled_initialize(&(brakelight->led2), &LED2_DUTY_CYCLE_REG);
	sei();
	adxl345_initialize(&(brakelight->accelerometer), 0);
	cli();
	brakelight->state = BRAKELIGHT_IDLE;
	brakelight->xAccel = 0;
	brakelight->yAccel = 0;
	brakelight->zAccel = 0;
	brakelight->xAccelPast = 0;
	brakelight->yAccelPast = 0;
	brakelight->zAccelPast = 0;
//	brakelight->accelBufIndex = 0;
//	brakelight->xAccelSum = 0;
//	brakelight->yAccelSum = 0;
//	brakelight->zAccelSum = 0;
//
//	for (uint8_t i = 0; i < BRAKELIGHT_BUFSIZE; i++)
//	{
//		brakelight->xAccelBuf[i] = 0;
//	}
//	for (uint8_t i = 0; i < BRAKELIGHT_BUFSIZE; i++)
//	{
//		brakelight->yAccelBuf[i] = 0;
//	}
//	for (uint8_t i = 0; i < BRAKELIGHT_BUFSIZE; i++)
//	{
//		brakelight->zAccelBuf[i] = 0;
//	}

	//Setup UART
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
	uart_puts_P("UART OK\n\r");
}

void brakelight_run(Brakelight_t * brakelight)
{
//#ifdef DEBUG
//	uart_puts_P("BL_Run\n\r");
//#endif
	//TODO Implement Brakelight state machine.
	//int16_t grav_y;
	//int16_t grav_x;
	if (!commandReceived)
	{
		uint16_t c = uart_getc();
		if (!(c & UART_NO_DATA))
		{
			switch ((char) c)
			{
			case ('\n'):
				rxBuf[7] = 0;
				rxBufIndex = 0;
				commandReceived = 1;
				break;
			default:
				if (rxBufIndex < 6)
				{
					rxBuf[rxBufIndex++] = (char) c;
				}
				break;
			}
		}
	}
	else
	{
		uint16_t newVal = atoi(&rxBuf[1]);
		switch (rxBuf[0])
		{
		case ('T'):
			BRAKELIGHT_THRESHOLD = newVal;
			uart_puts_P("Threshold changed: ");
			uart_puts(&rxBuf[1]);
			uart_putc('\n');
			uart_putc('\r');
			break;
		case ('F'):
			BRAKELIGHT_FIRST_WEIGHT = newVal;
			uart_puts_P("First Weight changed: ");
			uart_puts(&rxBuf[1]);
			uart_putc('\n');
			uart_putc('\r');
			break;
		case ('S'):
			BRAKELIGHT_SECOND_WEIGHT = newVal;
			uart_puts_P("Second Weight changed: ");
			uart_puts(&rxBuf[1]);
			uart_putc('\n');
			uart_putc('\r');
			break;
		case ('B'):
			brakeDelay = newVal;
			uart_puts_P("Brake delay changed: ");
			uart_puts(&rxBuf[1]);
			uart_putc('\n');
			uart_putc('\r');
			break;
		default:
			uart_puts_P("Unknown Command\n\r");
			break;
		}
		commandReceived = 0;
	}
	switch (brakelight->state)
	{
	case (BRAKELIGHT_SLEEP):
		break;
	case (BRAKELIGHT_IDLE):
		brakelight_updateLights(brakelight);
		if (brakelight_updateAcceleration(brakelight))
		{
			if (!(counter++ % 1024))
			{
				char buf[7];
				itoa(brakelight->zAccel, buf, 10);
				uart_puts(buf);
				uart_putc(' ');
				itoa(brakelight->zAccelPast, buf, 10);
				uart_puts(buf);
				uart_putc('\n');
				uart_putc('\r');
			}
			//New acceleration data received
//			grav_y =
//					(abs(brakelight->yAccel) > GRAVITY) ?
//							GRAVITY : abs(brakelight->yAccel);
//			grav_x = sqrt(square(GRAVITY) - square(grav_y));
//			if ((brakelight->xAccel < 0)
//					&& (abs(brakelight->xAccel) > (BRAKING_THRESHOLD + grav_x)))
			if (recentlyBraked)
			{
				if (counter1++ > brakeDelay)
				{
					recentlyBraked = 0;
					counter1 = 0;
				}
			}
			else if ((brakelight->zAccelPast - brakelight->zAccel)
					> BRAKELIGHT_THRESHOLD)
			{
				uart_puts_P("BRAKE!\n\r");
				//Acceleration indicates braking
				brakelight->state = BRAKELIGHT_BRAKE;
			}
		}
		break;
	case (BRAKELIGHT_BRAKE):
		brakelight_updateLights(brakelight);
		char buf[7];
		itoa(brakelight->zAccel, buf, 10);
		uart_puts(buf);
		uart_putc(' ');
		itoa(brakelight->zAccelPast, buf, 10);
		uart_puts(buf);
		uart_putc('\n');
		uart_putc('\r');
		_delay_ms(500);
		recentlyBraked = 1;
//		if (brakelight_updateAcceleration(brakelight))
//		{
//			//New acceleration data received
//			grav_y =
//					(abs(brakelight->yAccel) > GRAVITY) ?
//							GRAVITY : abs(brakelight->yAccel);
//			grav_x = sqrt(square(GRAVITY) - square(grav_y));
//			if (abs(brakelight->xAccel) < (BRAKING_THRESHOLD + grav_x))
//			{
//				//Acceleration indicates not braking anymore
//				brakelight->state = BRAKELIGHT_IDLE;
//			}
//		}
		brakelight->state = BRAKELIGHT_IDLE;
		break;
	default:
		break;
	}
#ifdef GUBED
	char buf[7];
	itoa(grav_x, buf, 10);
	uart_puts(buf);
	uart_putc(' ');
	itoa(grav_y, buf, 10);
	uart_puts(buf);
	uart_putc('\n');
	uart_putc('\r');
#endif
}

bool brakelight_updateAcceleration(Brakelight_t * brakelight)
{
//#ifdef DEBUG
//	uart_puts_P("BL_UpdateAccel\n\r");
//#endif
	bool ret = false;
	uint8_t buf[6];
	if (adxl345_readRegisters(&(brakelight->accelerometer), buf, 6))
	{
		brakelight->xAccelPast = ((brakelight->xAccelPast
				* BRAKELIGHT_SECOND_WEIGHT) + brakelight->xAccel
				- brakelight->xAccelPast) / BRAKELIGHT_SECOND_WEIGHT;
		brakelight->yAccelPast = ((brakelight->yAccelPast
				* BRAKELIGHT_SECOND_WEIGHT) + brakelight->yAccel
				- brakelight->yAccelPast) / BRAKELIGHT_SECOND_WEIGHT;
		brakelight->zAccelPast = ((brakelight->zAccelPast
				* BRAKELIGHT_SECOND_WEIGHT) + brakelight->zAccel
				- brakelight->zAccelPast) / BRAKELIGHT_SECOND_WEIGHT;

		brakelight->xAccel = ((brakelight->xAccel * BRAKELIGHT_FIRST_WEIGHT)
				+ ((buf[1] << 8) | buf[0]) - brakelight->xAccel)
				/ BRAKELIGHT_FIRST_WEIGHT;
		brakelight->yAccel = ((brakelight->yAccel * BRAKELIGHT_FIRST_WEIGHT)
				+ ((buf[3] << 8) | buf[2]) - brakelight->yAccel)
				/ BRAKELIGHT_FIRST_WEIGHT;
		brakelight->zAccel = ((brakelight->zAccel * BRAKELIGHT_FIRST_WEIGHT)
				+ ((buf[5] << 8) | buf[6]) - brakelight->zAccel)
				/ BRAKELIGHT_FIRST_WEIGHT;

//		brakelight->xAccelBuf[brakelight->accelBufIndex] = (buf[1] << 8)
//				| buf[0];
//		brakelight->yAccelBuf[brakelight->accelBufIndex] = (buf[3] << 8)
//				| buf[2];
//		brakelight->zAccelBuf[brakelight->accelBufIndex] = (buf[5] << 8)
//				| buf[4];
//
//		brakelight->xAccelSum +=
//				brakelight->xAccelBuf[brakelight->accelBufIndex];
//		brakelight->yAccelSum +=
//				brakelight->yAccelBuf[brakelight->accelBufIndex];
//		brakelight->zAccelSum +=
//				brakelight->zAccelBuf[brakelight->accelBufIndex];
//
//		brakelight->xAccel = brakelight->xAccelSum / BRAKELIGHT_BUFSIZE;
//		brakelight->yAccel = brakelight->yAccelSum / BRAKELIGHT_BUFSIZE;
//		brakelight->zAccel = brakelight->zAccelSum / BRAKELIGHT_BUFSIZE;
//
//		brakelight->accelBufIndex++;
//
//		if (brakelight->accelBufIndex >= BRAKELIGHT_BUFSIZE)
//		{
//			brakelight->accelBufIndex = 0;
//		}

#ifdef GUBED
		char buf[7];
//		uart_puts_P("accelbufindex: ");
//		itoa(brakelight->accelBufIndex, buf, 10);
//		uart_puts(buf);
//		uart_puts_P("\n\r");
//
//		uart_puts_P("xAccel: ");
//		itoa(brakelight->xAccel, buf, 10);
//		uart_puts(buf);
//		uart_puts_P("\n\r");
		itoa(brakelight->xAccel, buf, 10);
		uart_puts(buf);

		uart_putc(' ');
		itoa(brakelight->yAccel, buf, 10);
		uart_puts(buf);

		uart_putc(' ');
		itoa(brakelight->zAccel, buf, 10);
		uart_puts(buf);
		uart_putc('\n');
		uart_putc('\r');
#endif

		ret = true;
	}
	if (brakelight->accelerometer.state == ADXL345_IDLE)
	{
		adxl345_requestRegisters(&(brakelight->accelerometer),
		ADXL345_RA_DATAX0, 6);
	}

	return (ret);
}

void brakelight_updateLights(Brakelight_t * brakelight)
{
//static uint8_t counter = 0;

//#ifdef DEBUG
//	uart_puts_P("BL_UpdateLights\n\r");
//#endif
//counter++;
	switch (brakelight->state)
	{
	case (BRAKELIGHT_SLEEP):
		break;
	case (BRAKELIGHT_IDLE):
//		if (counter % 2)
//		{
//			pwmled_off(&(brakelight->led2));
//			pwmled_on(&(brakelight->led1), IDLE_BRIGHTNESS);
//		}
//		else
//		{
//			pwmled_off(&(brakelight->led1));
//			pwmled_on(&(brakelight->led2), IDLE_BRIGHTNESS);
//		}
		pwmled_on(&(brakelight->led1), IDLE_BRIGHTNESS);
		pwmled_on(&(brakelight->led2), IDLE_BRIGHTNESS);
		break;
	case (BRAKELIGHT_BRAKE):
//		if (counter % 2)
//		{
//			pwmled_off(&(brakelight->led2));
//			pwmled_on(&(brakelight->led1), BRAKE_BRIGHTNESS);
//		}
//		else
//		{
//			pwmled_off(&(brakelight->led1));
//			pwmled_on(&(brakelight->led2), BRAKE_BRIGHTNESS);
//		}
		pwmled_on(&(brakelight->led1), BRAKE_BRIGHTNESS);
		pwmled_on(&(brakelight->led2), BRAKE_BRIGHTNESS);
		break;
	default:
		break;
	}
}
