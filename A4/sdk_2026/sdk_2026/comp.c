#include <avr/interrupt.h>
#include <avr/io.h>

volatile uint8_t g_flag;

ISR(ANALOG_COMP_vect)
{
	
	
		//if (ACSR & (1 << ACO)){
			//// ACO = 1 significa AIN0 > AIN1 (Temp > 25°C)
			//g_flag = 1;
		//}
		//else{
			//// ACO = 0 significa AIN0 <= AIN1 
			//g_flag = 0;
		//
	//}
	
}


void init_comp(void)
{
	ACSR |= (1 << ACIS0 | 1 << ACIS1); //interrupt when Input > Vref
	ACSR |= (1 << ACIE); //enable interrupt
}