/*
 * comp.c
 *
 * Created: 10/22/2025 4:28:49 PM
 *  Author: josel
 */ 
#include "leds.h"
#include <avr/interrupt.h>
#include <avr/io.h>

ISR(ANALOG_COMP_vect)
{
	led_on_off_fast();
}


void init_comp(void)
{
	ACSR |= (1 << ACIS0 | 1 << ACIS1); //interrupt when Input > Vref
	ACSR |= (1 << ACIE); //enable intarrupt
}