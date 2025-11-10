
#include <avr/interrupt.h>
#include "ext_int.h"
#include "leds.h"
#define INT1_ALERT	2
#define INT2_ALERT	3


ISR(INT0_vect)
{
		//led_on_off_fast();
}

void init_int0(void)
{
	EICRA |= (1 << ISC00); //interruption on any logical change
	EIMSK |= (1 << INT0);
	
}
void init_ext_int12(void){
	//configurar INT1
	EICRA |= (1 << ISC11);
	EICRA &= ~(1 << ISC10);
	
	// configurar INT2 
	
	//habilitar INT1 e INT2
	EIMSK |= (1 << INT1);
	PCICR |= (1 << PCIE0);
	
	//habilitar la mascara para el pin especifico;
	PCMSK0 |= (1 << PCINT2);
}



ISR(INT1_vect){
	g_state = INT1_ALERT;
	g_timer = 0; //5s
}

ISR(PCINT0_vect)
{
	//interrupcion en PB2
	
	if ( !(PINB & (1 << PB2)) ){
		
		g_state = INT2_ALERT;
		g_timer = 0; //5 segundos
	}
}

