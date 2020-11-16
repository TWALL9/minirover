#ifndef HC_SR04_H
#define HC_SR04_H

#include <stdint.h>
#include <stdbool.h>
#include "util_define.h"

typedef enum
{
    IDLE = 0,
    WAIT_FOR_RESPONSE,
    WAIT_FOR_CALC,
    COMPLETE,
    MAX_STATE
} ultrasonic_state_t;

class UltrasonicSensor 
{
    public:
        UltrasonicSensor(gpio_reference_t trig_pin, gpio_reference_t echo_pin, uint32_t start_timer, uint32_t response_timer);
        ultrasonic_state_t read(float * distance, uint16_t max_delay);
    
    protected:
        void configure_trig(void);
        void configure_echo(void);

    private:
        ultrasonic_state_t state;
        gpio_reference_t trig;
        gpio_reference_t echo;
        uint32_t start_timer;
        uint32_t echo_timer;
};

#endif /* HC_SR04_H */
