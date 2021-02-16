#include "drivers/motor_delegate.h"

namespace motors
{
    MotorDelegate::MotorDelegate(void)
    :
        duty_cycle(0),
        drive_mode(NEUTRAL)
    {}

    void MotorDelegate::set_drive_mode(drive_mode_t new_mode)
    {
        drive_mode = new_mode;
    }

    drive_mode_t MotorDelegate::get_drive_mode(void)
    {
        return drive_mode;
    }

    void MotorDelegate::set_duty_cycle(int16_t new_duty)
    {
        // default motor timers have a -1000 <= x <= 1000 duty cycle
        new_duty = new_duty < min_duty? min_duty : new_duty;
        new_duty = new_duty > max_duty? max_duty : new_duty;
    
        duty_cycle = new_duty;
    }

    int16_t MotorDelegate::get_duty_cycle(void)
    {
        return duty_cycle;
    }
}