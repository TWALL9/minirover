#ifdef __cplusplus
extern "C" {
#endif

#include "timer.h"
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

volatile uint32_t system_ms;

void timer_setup(void)
{
    timer_pwm_setup(TIM2, TIM_OC3);
    timer_pwm_setup(TIM2, TIM_OC4);

	timer_pwm_setup(TIM3, TIM_OC1);
    timer_pwm_setup(TIM3, TIM_OC2);
    timer_pwm_setup(TIM3, TIM_OC3);
    timer_pwm_setup(TIM3, TIM_OC4);

	timer_pwm_setup(TIM4, TIM_OC2);
    timer_pwm_setup(TIM4, TIM_OC3);

    // Microsecond timer for ultrasonic sensors
    timer_set_drive_mode(TIM14, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM14, 84 - 1); // TIM14 gets 84 MHz clock, so set prescaler to 84 - 1 to get 1Mhz count
    timer_set_period(TIM14, 0xFFFF);
    timer_continuous_mode(TIM14);
    timer_enable_counter(TIM14);
}

/**
 * Motor PWM signals
 * enable these before we instantiate the motors
 * Timer global mode:
 * No divider
 * Alignment edge
 * Direction up
 * Prescaler 84 and period 999 allows for 0-1000 value for PWM duty cycle
 * Does this mean that there's a 1ms overflow?
 */
void timer_pwm_setup(uint32_t time_base, uint8_t channel)
{
    timer_set_drive_mode(time_base, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(time_base, 84 - 1);
    timer_disable_preload(time_base);
    timer_continuous_mode(time_base);
    timer_set_period(time_base, 999);
    timer_enable_oc_output(time_base, channel);
    timer_set_oc_mode(time_base, channel, TIM_OCM_PWM1);
    timer_enable_counter(time_base);
}

void sys_tick_handler(void)
{
    system_ms++;
}

uint32_t timer_get_system_ms(void)
{
    return system_ms;
}

uint16_t timer_get_system_us(void)
{
    return (timer_get_system_ms() * 1000) + timer_get_counter(TIM2);
}

#ifdef __cplusplus
}
#endif
