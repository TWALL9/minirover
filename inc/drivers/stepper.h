#ifndef STEPPER_H
#define STEPPER_H

#include <stdint.h>
#include <stdbool.h>
#include "drivers/motor_delegate.h"
#include "peripherals/timer.h"

namespace motors
{
    class Stepper : public MotorDelegate
    {
        public:
            Stepper(uint32_t drive_timer_base, uint32_t direction_port, uint16_t direction_pin, uint8_t drive_timer_channel);
            void drive(void);
        
        private:
            uint32_t drive_base;
            uint32_t dir_port;
            uint16_t dir_pin;
            uint8_t drive_channel;
    };
}

#endif /* STEPPER_H */
