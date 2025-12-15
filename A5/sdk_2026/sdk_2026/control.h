#ifndef CONTROL_H_
#define CONTROL_H_

#include <stdint.h>

// states
typedef enum {
    STATE_WAITING,      
    STATE_WATERING,     
    STATE_STOPPED,      
    STATE_COMPLETED     
} system_state_t;

 
// THRESHOLDS
#define LIGHT_THRESHOLD_DARK 300   
#define LIGHT_THRESHOLD_BRIGHT 500 

//segundos
#define WATERING_TIME 10


void app_init(void);
void app_process(void);
void app_start_watering(void);
void app_stop_watering(void);
uint16_t app_get_light_level(void);
uint8_t app_get_remaining_time(void);
system_state_t app_get_state(void);

#endif /* APP_CONTROL_H_ */