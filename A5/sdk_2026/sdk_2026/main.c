#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ports.h"
#include "ext_int.h"
#include "timers.h"
#include "lcd_i2c.h"
#include "adc.h"
#include "usart.h"
#include "control.h"

int main(void)
{
	// Inicializacion general
	init_ports();           // configra puertos 
	init_int0();            // INT0 
	adc_continuo();			// ADC cotninuo 
	lcd_i2c_init();         // LCD I2C
	init_usart(207);        // UART 
	timer1_10seg();			// Timer1 
	
	//LCD
	lcd_i2c_clr();
	lcd_i2c_col_row(1, 1);
	lcd_i2c_write_string("Sistema de riego");
	lcd_i2c_col_row(1, 2);
	lcd_i2c_write_string("Nocturno");
	_delay_ms(2000);
	
	//sistema
	app_init();
	
	// HM10 
	usart_transmit_string("AT\r\n");
	_delay_ms(100);
	
	sei(); 
	
	lcd_i2c_clr();
	lcd_i2c_col_row(1, 1);
	lcd_i2c_write_string("Esperando");
	lcd_i2c_col_row(1, 2);
	lcd_i2c_write_string("noche...");
	
	while (1)
	{
		app_process(); 
		_delay_ms(100); 
	}
}