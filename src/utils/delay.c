#ifdef __cplusplus
extern "C" {
#endif

#include "peripherals/timer.h"
#include "utils/delay.h"

/**
 * delay uses timer 14 due to it not being used/modified by anyone else
 */ 
void delay_us(uint16_t period)
{
    // Timer has a 16-bit count
    period = period >= 0xFFFF? 0xFFFE : period;

    timer_set_counter(TIM14, 0);
    while(timer_get_counter(TIM14) < period);
}

void delay_ms(uint32_t period)
{
    uint32_t current = 0;
    uint32_t start = timer_get_system_ms();
    do
    {
        current = timer_get_system_ms();
    } while (current - start <= period);
    
}

#ifdef __cplusplus
}
#endif