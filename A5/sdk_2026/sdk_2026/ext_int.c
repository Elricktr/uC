
#include <avr/interrupt.h>
#include "control.h"

ISR(INT0_vect)
{
		app_emergency_stop();
}

void init_int0(void)
{
	EICRA |= (1 << ISC01); // Interrupción en flanco descendente 
	// ISC01=1, ISC00=0 -> falling edge
	EIMSK |= (1 << INT0);  // Habilita INT0
	
}