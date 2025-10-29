#include <avr/io.h>
#include <util/delay.h>
//#include <avr/interrupt.h>
#include "ports.h"
//#include "leds.h"
//#include "ext_int.h"
#include "timers.h"
//#include "comp.h"
//#include "lcd_4b.h"

#define SERVO_POS_0     900
#define SERVO_POS_90    1500
#define SERVO_POS_180   2000
#define SERVO_MOVE_DELAY 1000

int main(void) {
    ///* Replace with your application code */
    //init_ports();
	//init_int0();
	////timer0_normalmode_noprescaler();
	////timer0_overflow_interrupt();
	////timer1_CTCmode_interrupt();
	////timer1_CTCmode_nonPWM();
	////timer1_fastPWM_inverting();
	////Timer2_PhaseCorrectPWMMode_Inverting();
	//init_comp();
	//lcd_init();
	//lcd_col_row(5,1);
	//lcd_write_string("reprobados");
	//sei();
	//while (1) 
    //{
	 ////led_on_off();
    //}
	
	
	init_ports();           // Inicializa los puertos
	timer1_servos_init();   // Configura Timer1 para PWM en OC1A y OC1B
	
	//0° al inicio
	servo1_set_pos(SERVO_POS_0);
	servo2_set_pos(SERVO_POS_0);
	_delay_ms(1000); 
	
	while (1){
		
		//Secuencia 1: Servo 1 (0 - 90 - 180) 
		
		servo1_set_pos(SERVO_POS_90);
		_delay_ms(SERVO_MOVE_DELAY);
		servo1_set_pos(SERVO_POS_180);
		_delay_ms(SERVO_MOVE_DELAY);

		//Secuencia 2: Servo 2 (0 -> 90 -> 180) 
		
		servo2_set_pos(SERVO_POS_90);
		_delay_ms(SERVO_MOVE_DELAY);
		servo2_set_pos(SERVO_POS_180);
		_delay_ms(SERVO_MOVE_DELAY);

		_delay_ms(SERVO_MOVE_DELAY); // Pausa 

		//Secuencia 3: Servo 1 (180 -> 90 -> 0) 
		servo1_set_pos(SERVO_POS_90);
		_delay_ms(SERVO_MOVE_DELAY);
		servo1_set_pos(SERVO_POS_0);
		_delay_ms(SERVO_MOVE_DELAY);

		//Secuencia 4: Servo 2 (180 -> 90 -> 0) 
		servo2_set_pos(SERVO_POS_90);
		_delay_ms(SERVO_MOVE_DELAY);
		servo2_set_pos(SERVO_POS_0);
		_delay_ms(SERVO_MOVE_DELAY);
		
		_delay_ms(SERVO_MOVE_DELAY);
	}
}

