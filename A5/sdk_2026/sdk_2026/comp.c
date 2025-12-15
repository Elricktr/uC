 
#include "leds.h"
#include <avr/interrupt.h>
#include <avr/io.h>
volatile uint8_t flag=0;
ISR(ANALOG_COMP_vect)
{
	flag = 1;
	
}


void init_comp(void)
{
	ACSR |= (1 << ACIS0 | 1 << ACIS1); //interrupt when Input > Vref
	ACSR |= (1 << ACIE); //enable intarrupt
}