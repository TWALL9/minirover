#ifndef CONTINUOUS_SERVO_H
#define CONTINUOUS_SERVO_H

#include <stdint.h>
#include <stdbool.h>
#include "motor_delegate.h"
#include "timer.h"

namespace motors
{
    class ContinuousServo : public MotorDelegate
    {
        public:
            ContinuousServo(uint32_t timer_base, uint8_t timer_channel);
            void drive(void);
        
        private:
            uint32_t timebase = 0;
            uint8_t channel_1 = TIM_OC1;
    };
}

#endif /* CONTINUOUS_SERVO_H */
