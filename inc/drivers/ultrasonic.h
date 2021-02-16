#ifndef HC_SR04_H
#define HC_SR04_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    IDLE = 0,
    WAIT_FOR_RESPONSE,
    WAIT_FOR_CALC,
    COMPLETE,
} ultrasonic_state_t;

class UltrasonicSensor 
{
    public:
        UltrasonicSensor(uint32_t t_port, uint16_t t_pin, uint32_t e_port, uint16_t e_pin);
        ultrasonic_state_t read(float * distance, uint16_t max_delay);
    
    protected:
        void configure_trig(void);
        void configure_echo(void);

    private:
        ultrasonic_state_t state = IDLE;
        uint32_t trig_port = 0;
        uint16_t trig_pin = 0;
        uint32_t echo_port = 0;
        uint16_t echo_pin = 0;
        uint32_t start_timer = 0;
        uint32_t echo_timer = 0;
};

#endif /* HC_SR04_H */
