#include "h_bridge.h"
#include <stdlib.h>

HBridge::HBridge(uint32_t input_1_timer_base, uint8_t input_1_channel, uint32_t input_2_timer_base, uint8_t input_2_channel)
{
    timebase_1 = input_1_timer_base;
    channel_1 = input_1_channel;
    timebase_2 = input_2_timer_base;
    channel_2 = input_2_channel;

    // Because alternate function registers differ between GPIO bank/port for PWM output,
    // this constructor counts on the GPIO being initialized first
    timer_pwm_setup(timebase_1, (tim_oc_id)channel_1);
    timer_pwm_setup(timebase_2, (tim_oc_id)channel_2);
}

void HBridge::drive(void)
{
    switch(drive_mode)
    {
        case (NEUTRAL):
        {
            timer_set_oc_value(timebase_1, (tim_oc_id)channel_1, 0);
            timer_set_oc_value(timebase_2, (tim_oc_id)channel_2, 0);
            break;
        }
        case(DRIVE):
        {
            if (duty_cycle >= 0)
            {
                timer_set_oc_value(timebase_1, (tim_oc_id)channel_1, duty_cycle);
                timer_set_oc_value(timebase_2, (tim_oc_id)channel_2, 0);
            }
            else
            {
                timer_set_oc_value(timebase_1, (tim_oc_id)channel_1, 0);
                timer_set_oc_value(timebase_2, (tim_oc_id)channel_2, abs(duty_cycle));
            }
            break;
        }
        case (BRAKE):
        {
            timer_set_oc_value(timebase_1, (tim_oc_id)channel_1, 1000);
            timer_set_oc_value(timebase_2, (tim_oc_id)channel_2, 1000);
            break;
        }
        default:
            break;
    }
}

void HBridge::set_mode(drive_mode_t new_mode)
{
    drive_mode = new_mode;
}

drive_mode_t HBridge::get_mode(void)
{
    return drive_mode;
}

void HBridge::set_duty(int16_t duty)
{
    // default motor timers have a 0-1000 duty cycle
    duty = duty < -1000? -1000 : duty;
    duty = duty > 1000? 1000 : duty;
    
    duty_cycle = duty;
}

int16_t HBridge::get_duty(void)
{
    return duty_cycle;
}