#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libopencm3/stm32/timer.h>
#include <stdint.h>

void timer_setup(void);

void timer_pwm_setup(uint32_t time_base, uint8_t channel);

uint32_t timer_get_system_ms(void);
uint16_t timer_get_system_us(void);

#ifdef __cplusplus
}
#endif

#endif /* TIMER_H */
