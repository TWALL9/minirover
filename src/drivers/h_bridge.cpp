#include "h_bridge.h"
#include <stdlib.h>

namespace motors
{
    HBridge::HBridge(uint32_t input_1_timer_base, uint8_t input_1_channel, uint32_t input_2_timer_base, uint8_t input_2_channel)
    :
        timebase_1(input_1_timer_base),
        channel_1(input_1_channel),
        timebase_2(input_2_timer_base),
        channel_2(input_2_channel)
    {
        // Because alternate function registers differ between GPIO bank/port for PWM output,
        // this constructor counts on the GPIO being initialized first
        timer_pwm_setup(this->timebase_1, (tim_oc_id)this->channel_1);
        timer_pwm_setup(this->timebase_2, (tim_oc_id)this->channel_2);
    }

    void HBridge::drive(void)
    {
        switch(get_drive_mode())
        {
            case (NEUTRAL):
            {
                timer_set_oc_value(this->timebase_1, (tim_oc_id)this->channel_1, 0);
                timer_set_oc_value(this->timebase_2, (tim_oc_id)this->channel_2, 0);
                break;
            }
            case(DRIVE):
            {
                int16_t dc = get_duty_cycle();
                if (dc >= 0)
                {
                    timer_set_oc_value(this->timebase_1, (tim_oc_id)this->channel_1, dc);
                    timer_set_oc_value(this->timebase_2, (tim_oc_id)this->channel_2, 0);
                }
                else
                {
                    timer_set_oc_value(this->timebase_1, (tim_oc_id)this->channel_1, 0);
                    timer_set_oc_value(this->timebase_2, (tim_oc_id)this->channel_2, abs(dc));
                }
                break;
            }
            case (BRAKE):
            {
                timer_set_oc_value(this->timebase_1, (tim_oc_id)this->channel_1, 1000);
                timer_set_oc_value(this->timebase_2, (tim_oc_id)this->channel_2, 1000);
                break;
            }
            default:
                break;
        }
    }
}