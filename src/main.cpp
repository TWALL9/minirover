#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include "timer.h"
#include "delay.h"
#include "usart.h"
#include "log.h"
#include "ultrasonic.h"

#define GREEN GPIO12
#define ORANGE GPIO13
#define RED GPIO14
#define BLUE GPIO15

static void clock_setup(void) 
{
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
    rcc_periph_clock_enable(RCC_TIM2);
	rcc_periph_clock_enable(RCC_TIM3);
	rcc_periph_clock_enable(RCC_TIM4);
    rcc_periph_clock_enable(RCC_TIM14);

	rcc_periph_clock_enable(RCC_USART2);
}

/* Set up a timer to create 1mS ticks. */
static void systick_setup(void)
{
	/* clock rate / 1000 to get 1mS interrupt rate */
	systick_set_reload(168000);
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_counter_enable();
	/* this done last */
	systick_interrupt_enable();
}

static void gpio_setup(void) 
{
	/* clocks */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOD);

    /* LEDS on discovery board */
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT,
            GPIO_PUPD_NONE, GREEN | ORANGE | RED | BLUE);
    gpio_clear(GPIOD, GREEN | ORANGE | RED | BLUE);

    /* Motor PWM outputs */
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, 
			GPIO7 | GPIO8 |GPIO10 | GPIO11);
    gpio_clear(GPIOB, GPIO7 | GPIO8 | GPIO10 | GPIO11);

    /* AF1 is timer 2 channel 2/3 output for pin 10 and 11 */
    gpio_set_af(GPIOB, GPIO_AF1, GPIO10 | GPIO11);
	/* AF2 is timer 4 channel 2/3 output on pin 7 and 8 */
    gpio_set_af(GPIOB, GPIO_AF2, GPIO7 | GPIO8);

	gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, 
			GPIO6 | GPIO7 | GPIO8 | GPIO9);
    gpio_clear(GPIOC, GPIO6 | GPIO7 | GPIO8 | GPIO9);
    gpio_set_af(GPIOC, GPIO_AF2, GPIO6 | GPIO7 | GPIO8 | GPIO9);

	/* ultrasonic sensors */
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, 
			GPIO9 | GPIO10);
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN,
			GPIO8 | GPIO9 | GPIO10 | GPIO13);
}

int main(void) 
{
    clock_setup();
    systick_setup();
    timer_setup();
    gpio_setup();
	usart_setup();

	const uint32_t pulse = 1000/3;
	
	timer_set_oc_value(TIM2, TIM_OC3, pulse);

    log_init();
    gpio_reference_t trig = 
    {
        .port = GPIOD,
        .pins = GPIO9
    };

    gpio_reference_t echo = 
    {
        .port = GPIOD,
        .pins = GPIO10
    };

    UltrasonicSensor us = UltrasonicSensor(trig, echo);
    float distance = 0.0;

    for (;;) 
    {
        ultrasonic_state_t us_state = us.read(&distance, 1000);
        switch (us_state)
        {
            case COMPLETE:
            {
                gpio_clear(GPIOD, GREEN | ORANGE | RED | BLUE);
                gpio_set(GPIOD, ORANGE);
                //DEBUG("%f", distance);
                //delay_ms(1000);
                break;
            }
            case IDLE:
            {
                gpio_clear(GPIOD, GREEN | ORANGE | RED | BLUE);
                gpio_set(GPIOD, GREEN);
                break;
            }
            case WAIT_FOR_RESPONSE:
            {
                gpio_clear(GPIOD, GREEN | ORANGE | RED | BLUE);
                gpio_set(GPIOD, RED);
                break;
            }
            case WAIT_FOR_CALC:
            {
                gpio_clear(GPIOD, GREEN | ORANGE | RED | BLUE);
                gpio_set(GPIOD, BLUE);
                break;
            }
        }
    }

    return 0;
}
