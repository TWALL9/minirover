#ifndef MOTOR_DELEGATE_H
#define MOTOR_DELEGATE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    NEUTRAL = 0, // counts as forward and reverse.  Direction is done through duty cycle
    DRIVE,
    BRAKE,
} drive_mode_t;

class MotorDelegate
{
    public:
        virtual void drive(void);
        virtual void set_mode(drive_mode_t new_mode);
        virtual drive_mode_t get_mode(void);
        virtual void set_duty(int16_t duty);
        virtual int16_t get_duty(void);
    
    protected:
        int16_t duty_cycle = 0;
        drive_mode_t drive_mode = NEUTRAL;
};

#endif /* MOTOR_DELEGATE_H */
