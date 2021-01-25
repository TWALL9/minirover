#ifndef H_BRIDGE_H
#define H_BRIDGE_H

#include <stdint.h>
#include <stdbool.h>
#include "motor_delegate.h"
#include "timer.h"

namespace motors
{
    class HBridge : public MotorDelegate
    {
        public:
            HBridge(uint32_t input_1_timer_base, uint8_t input_1_channel, uint32_t input_2_timer_base, uint8_t input_2_channel);
            void drive(void);
        
        private:
            uint32_t timebase_1 = 0;
            uint8_t channel_1 = TIM_OC1;
            uint32_t timebase_2 = 0;
            uint8_t channel_2 = TIM_OC1;
    };
}

#endif /* H_BRIDGE_H */
