#include <stdbool.h>
#include <stdint.h>
#include "ultrasonic.h"
#include "gpio.h"
#include "timer.h"
#include "delay.h"

#define CM_US_CONVERSION ((float)0.0171821)

UltrasonicSensor::UltrasonicSensor(uint32_t t_port, uint16_t t_pin, uint32_t e_port, uint16_t e_pin)
{
    trig_port = t_port;
    trig_pin = t_pin;
    echo_port = e_port;
    echo_pin = e_pin;
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
            gpio_set(trig_port, trig_pin);
            delay_us(10);
            gpio_clear(trig_port, trig_pin);
            
            start_timer = timer_get_system_ms();

            state = WAIT_FOR_RESPONSE;
            break;
        }
        case (WAIT_FOR_RESPONSE):
        {
            if (gpio_get_pin(echo_port, echo_pin))
            {
                echo_timer = timer_get_system_us();
                state = WAIT_FOR_CALC;
            }
            break;
        }
        case (WAIT_FOR_CALC):
        {
            if (gpio_get_pin(echo_port, echo_pin))
            {
                uint16_t round_trip = timer_get_system_us() - echo_timer;
                *distance = round_trip * CM_US_CONVERSION;
                state = COMPLETE;
            }
            break;
        }
        default:
        {
            state = IDLE;
            gpio_clear(trig_port, trig_pin);
            gpio_clear(echo_port, echo_pin);
            start_timer = 0;
            echo_timer = 0;

            break;
        }
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
    gpio_mode_setup(trig_port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, trig_pin);
    gpio_set_output_options(trig_port, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, trig_pin);
    gpio_clear(trig_port, trig_pin);
}

void UltrasonicSensor::configure_echo(void)
{
    // pulldown
    // input
    gpio_mode_setup(echo_port, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, echo_pin);
}
