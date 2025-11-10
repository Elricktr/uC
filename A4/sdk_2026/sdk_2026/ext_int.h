
#ifndef EXT_INT_H_
#define EXT_INT_H_
extern volatile uint8_t g_state;
extern volatile uint16_t g_timer;

void init_int0(void);

void init_ext_int12(void);

#endif /* EXT_INT_H_ */