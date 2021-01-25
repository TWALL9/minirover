#include "stepper.h"
#include "gpio.h"
#include <stdlib.h>

namespace motors
{
    Stepper::Stepper(uint32_t drive_timer_base, uint8_t drive_timer_channel, uint32_t direction_port, uint16_t direction_pin)
    :
        drive_base(drive_timer_base),
        drive_channel(drive_timer_channel),
        dir_port(direction_port),
        dir_pin(direction_pin)
    {
        timer_pwm_setup(this->drive_base, (tim_oc_id)this->drive_channel);
        gpio_mode_setup(dir_port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, dir_pin);
        gpio_clear(dir_port, dir_pin);
    }

    void Stepper::drive(void)
    {
        switch(get_drive_mode())
        {
            case (NEUTRAL):
            {
                // TODO check if this can be done via enable pin
                break;
            }
            case (DRIVE):
            {
                int16_t dc = get_duty_cycle();
                if (dc >= 0)
                {
                    gpio_clear(dir_port, dir_pin);
                }
                else
                {
                    gpio_set(dir_port, dir_pin);
                }
                timer_set_oc_value(this->drive_base, (tim_oc_id)this->drive_channel, abs(dc));

                break;
            }
            case (BRAKE):
            {
                timer_set_oc_value(this->drive_base, (tim_oc_id)this->drive_channel, 0);
                break;
            }
        }
    }
}