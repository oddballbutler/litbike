/*
 * main.c
 *
 *  Created on: Jan 17, 2014
 *      Author: jeremybutler
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "TWI_Master.h"

#include "brakelight.h"

int main(void)
{
	Brakelight_t Brakelight1;

	brakelight_initialize(&Brakelight1);

	sei();
	while (1)
	{
		brakelight_run(&Brakelight1);
	}

	return (0);
}
