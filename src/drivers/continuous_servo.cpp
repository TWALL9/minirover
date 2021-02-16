#include "drivers/continuous_servo.h"
#include "peripherals/gpio.h"
#include "utils/math.h"
#include <stdlib.h>

namespace motors
{
    ContinuousServo::ContinuousServo(uint32_t timer_base, uint8_t timer_channel)
    :
        timebase(timer_base),
        channel_1(timer_channel)
    {
        /** 
         * Servos use a 50Hz counter (20ms)
         * With minimum angle/full-reverse of 0.5ms high
         * Maxmimum angle/full-forward 2.5ms high
         */
        timer_set_mode(this->timebase, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
        timer_set_prescaler(this->timebase, 1680-1);
        timer_disable_preload(this->timebase);
        timer_continuous_mode(this->timebase);
        timer_set_period(this->timebase, 999);
        timer_enable_oc_output(this->timebase, (tim_oc_id)this->channel_1);
        timer_set_oc_mode(this->timebase, (tim_oc_id)this->channel_1, TIM_OCM_PWM1);
        timer_enable_counter(this->timebase);
    }

    void ContinuousServo::drive(void)
    {
        /**
         * rescale duty cycle
         * prog duty cycle: -1000 <= x <= 1000
         * servo duty cycle: 2.5% <= x <= 12.5%
         *                   25 <= x <= 125
         */
        int16_t dc = get_duty_cycle();

        switch(get_drive_mode())
        {
            case (NEUTRAL):
            case (BRAKE):
            {
                // set to even point
                dc = 75;        
                break;
            }
            case (DRIVE):
            {
                dc = rescale_range(dc, min_duty, max_duty, 25, 125);
                break;
            }
            default:
            {
                dc = 75;
                break;
            }
        }
        timer_set_oc_value(this->timebase, (tim_oc_id)this->channel_1, dc);
    }
}