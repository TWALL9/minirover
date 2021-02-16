#ifndef MOTOR_DELEGATE_H
#define MOTOR_DELEGATE_H

#include <stdint.h>
#include <stdbool.h>

namespace motors
{
    const int16_t max_duty = 1000;
    const int16_t min_duty = -1000;

    typedef enum
    {
        NEUTRAL = 0, // counts as forward and reverse.  Direction is done through duty cycle
        DRIVE,
        BRAKE,
    } drive_mode_t;

    class MotorDelegate
    {
        public:
            MotorDelegate(drive_mode_t mode, int16_t duty);
            MotorDelegate(void);
            virtual void drive(void) = 0;
            void set_drive_mode(drive_mode_t new_mode);
            drive_mode_t get_drive_mode(void);
            void set_duty_cycle(int16_t new_duty);
            int16_t get_duty_cycle(void);
        
        private:
            int16_t duty_cycle = 0;
            drive_mode_t drive_mode = NEUTRAL;
    };
}

#endif /* MOTOR_DELEGATE_H */
