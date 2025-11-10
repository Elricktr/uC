/*
 * ports.c
 *
 * Created: 9/22/2025 4:32:22 PM
 *  Author: josel
 */
#include <avr/io.h>
void init_ports(void){
	
	DDRB = (1 << DDB5);
	PORTB = (1 << PB2); //activa pull-up en INT2
	
	// PD3 entradad 
	// PD6 entrada sensor temp
	// PD7 entrada referencia V
	DDRD = 0;
	PORTD = (1 << PD3); //activa pull-up en INT2
	
}