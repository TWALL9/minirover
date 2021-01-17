#ifndef H_BRIDGE_H
#define H_BRIDGE_H

#include <stdint.h>
#include <stdbool.h>
#include "timer.h"

typedef enum
{
    NEUTRAL = 0, // counts as forward and reverse.  Direction is done through duty cycle
    DRIVE,
    BRAKE,
} drive_mode_t;

class HBridge
{
    public:
        HBridge(uint32_t input_1_timer_base, uint8_t input_1_channel, uint32_t input_2_timer_base, uint8_t input_2_channel);
        void drive(void);
        void set_mode(drive_mode_t new_mode);
        drive_mode_t get_mode(void);
        void set_duty(int16_t duty);
        int16_t get_duty(void);
    
    private:
        uint32_t timebase_1 = 0;
        uint8_t channel_1 = TIM_OC1;
        uint32_t timebase_2 = 0;
        uint8_t channel_2 = TIM_OC1;
        int16_t duty_cycle = 0;
        drive_mode_t drive_mode = NEUTRAL;
};

#endif /* H_BRIDGE_H */
