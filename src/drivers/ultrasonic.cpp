#include <stdbool.h>
#include <stdint.h>
#include "ultrasonic.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "timer.h"
#include "delay.h"

#define CM_US_CONVERSION ((float)0.0171821)

UltrasonicSensor::UltrasonicSensor(gpio_reference_t trig_pin, gpio_reference_t echo_pin, uint32_t start_timer, uint32_t response_timer)
{
    trig = trig_pin;
    echo = echo_pin;
    start_timer = start_timer;
    echo_timer = response_timer;
    state = IDLE;
    configure_trig();
    configure_echo();
}

ultrasonic_state_t UltrasonicSensor::read(float * distance, uint16_t max_delay)
{
    switch (state)
    {
        case(IDLE):
        case(COMPLETE):
        {
            gpio_set(trig.port, trig.pins);
            delay_us(10);
            gpio_clear(trig.port, trig.pins);
            
            start_timer = timer_get_system_ms();

            state = WAIT_FOR_RESPONSE;
            break;
        }
        case (WAIT_FOR_RESPONSE):
        {
            if (gpio_get(echo.port, echo.pins) == echo.pins)
            {
                echo_timer = timer_get_system_us();
                state = WAIT_FOR_CALC;
            }
            break;
        }
        case (WAIT_FOR_CALC):
        {
            if (gpio_get(echo.port, echo.pins) != echo.pins)
            {
                uint16_t round_trip = timer_get_system_us() - echo_timer;
                *distance = round_trip * CM_US_CONVERSION;
                state = COMPLETE;
            }
            break;
        }
        default:
            break;
    }

    if (timer_get_system_ms() - start_timer >= max_delay)
    {
        // we've timed out.  Reset.
        state = IDLE;
    }

    return state;
}

void UltrasonicSensor::configure_trig(void)
{
    // Pulldown
    // output
    gpio_mode_setup(trig.port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, trig.pins);
    gpio_set_output_options(trig.port, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, trig.pins);
    gpio_clear(trig.port, trig.pins);
}

void UltrasonicSensor::configure_echo(void)
{
    // pulldown
    // input
    gpio_mode_setup(echo.port, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, echo.pins);
}
