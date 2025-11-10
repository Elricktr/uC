/*
 * timers.h
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_
extern volatile uint32_t	g_counter;
extern volatile uint8_t		g_state;
extern volatile uint8_t		g_update_display;
extern volatile uint16_t	g_timer;

void timer0_normalmode_noprescaler(void);
void timer0_normalmode_prescaler(void);
void timer0_overflow_interrupt(void);
void timer1_normalmode_noprescaler(void);
void timer1_CTCmode_interrupt(void);
void timer1_CTCmode_nonPWM(void);
void timer1_fastPWM_inverting(void);
void timer2_CTCmode(void);
void Timer2_PhaseCorrectPWMMode_Inverting(void);

void timer1_CTC_1ms_init(void);

#endif /* TIMERS_H_ */