#ifdef __cplusplus
extern "C" {
#endif

#include "timer.h"
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

volatile uint32_t system_ms;

void sys_tick_handler(void)
{
    system_ms++;
}

/**
 * delay uses timer 14 due to it not being used/modified by anyone else
 */ 
void timer_delay_us(uint32_t period)
{
    uint32_t current = 0;
    uint32_t start = timer_get_counter(TIM14);
    do
    {
        current = timer_get_counter(TIM14);
    } while (current - start <= period);
}

uint16_t timer_get_system_ms(void)
{
    return system_ms;
}

uint16_t timer_get_system_us(void)
{
    return (timer_get_system_ms() * 1000) + timer_get_counter(TIM14);
}

#ifdef __cplusplus
}
#endif
