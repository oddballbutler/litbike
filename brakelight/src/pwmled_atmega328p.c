/*
 * pwmled_atmega328p.c
 *
 *  Created on: Jan 25, 2014
 *      Author: jeremybutler
 */

#include "pwmled.h"

#include <avr/io.h>

bool pwmled_initialize(PWMled_t * led, volatile uint8_t * dutyCycleReg)
{
	switch ((uintptr_t) dutyCycleReg)
	{
	case ((uintptr_t) &OCR0A):
		// SETUP PWM
		// Set to Fast PWM
		// Set compare output to non-inverting mode
		// Set timer clock selection to be the I/O clock without prescaling
		TCCR0A |= _BV(COM0A1) | _BV(WGM01) | _BV(WGM00);
		TCCR0A &= ~_BV(COM0A0);
		TCCR0B = _BV(CS00);
		DDRD |= _BV(DDD6); // Set led port to be output
		break;
	case ((uintptr_t) &OCR0B):
		// SETUP PWM
		// Set to Fast PWM
		// Set compare output to non-inverting mode
		// Set timer clock selection to be the I/O clock without prescaling
		TCCR0A |= _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
		TCCR0A &= ~_BV(COM0B0);
		TCCR0B = _BV(CS00);
		DDRD |= _BV(DDD5); // Set led port to be output
		break;
	default:
		//duty cycle register not recognized.
		return (false);
		break;
	}
	led->dutyCycleReg = dutyCycleReg;
	return (true);
}

void pwmled_off(PWMled_t * led)
{
	switch ((uintptr_t) (led->dutyCycleReg))
	{
	case ((uintptr_t) &OCR0A):
		if ((TCCR0A & (_BV(COM0A1) | _BV(COM0A0))) != _BV(COM0A1))
		{
			TCCR0A |= _BV(COM0A1);
			TCCR0A &= ~_BV(COM0A0);
		}
		OCR0A = 255;
		break;
	case ((uintptr_t) &OCR0B):
		if ((TCCR0A & (_BV(COM0B1) | _BV(COM0B0))) != _BV(COM0B1))
		{
			TCCR0A |= _BV(COM0B1);
			TCCR0A &= ~_BV(COM0B0);
		}
		OCR0B = 255;
		break;
	default:
		break;
	}
}

void pwmled_on_full(PWMled_t * led)
{
	switch ((uintptr_t) (led->dutyCycleReg))
	{
	case ((uintptr_t) &OCR0A):
		if ((TCCR0A & (_BV(COM0A1) | _BV(COM0A0)))
				!= (_BV(COM0A1) | _BV(COM0A0)))
		{
			TCCR0A |= _BV(COM0A1) | _BV(COM0A0);
		}
		OCR0A = 255;
		break;
	case ((uintptr_t) &OCR0B):
		if ((TCCR0A & (_BV(COM0B1) | _BV(COM0B0)))
				!= (_BV(COM0B1) | _BV(COM0B0)))
		{
			TCCR0A |= _BV(COM0B1) | _BV(COM0B0);
		}
		OCR0B = 255;
		break;
	default:
		break;
	}
}

/**
 * Set the brightness of the led.
 * @param led A pointer to the led option
 * @param brightness A number between 0-255. 0 being 0% duty cycle and 255
 * being 100% duty cycle.
 */
void pwmled_on(PWMled_t * led, const uint8_t brightness)
{
	if (brightness == 0)
	{
		pwmled_off(led);
	}
	else
	{
		switch ((uintptr_t) (led->dutyCycleReg))
		{
		case ((uintptr_t) &OCR0A):
			if ((TCCR0A & (_BV(COM0A1) | _BV(COM0A0)))
					!= (_BV(COM0A1) | _BV(COM0A0)))
			{
				TCCR0A |= _BV(COM0A1) | _BV(COM0A0);
			}
			if (OCR0A != brightness)
			{
				OCR0A = brightness;
			}
			break;
		case ((uintptr_t) &OCR0B):
			if ((TCCR0A & (_BV(COM0B1) | _BV(COM0B0)))
					!= (_BV(COM0B1) | _BV(COM0B0)))
			{
				TCCR0A |= _BV(COM0B1) | _BV(COM0B0);
			}
			if (OCR0B != brightness)
			{
				OCR0B = brightness;
			}
			break;
		default:
			break;
		}
	}
}
