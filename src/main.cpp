#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>
#include "log.h"

static void clock_setup(void) 
{
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_reset_pulse(RST_TIM2);
	rcc_periph_clock_enable(RCC_TIM3);
    rcc_periph_reset_pulse(RST_TIM3);
	rcc_periph_clock_enable(RCC_TIM4);
    rcc_periph_reset_pulse(RST_TIM4);

	rcc_periph_clock_enable(RCC_USART2);
}

static void timer_setup(void)
{
    /**
     * Motor PWM signals
     * enable these before we instantiate the motors
     * Timer global mode:
	 * No divider
	 * Alignment edge
	 * Direction up
	 * Prescaler 84 and period 999 allows for 0-1000 value for PWM duty cycle
	 */
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM2, 84);
    timer_set_period(TIM2, 999);
    timer_enable_oc_output(TIM2, TIM_OC3);
    timer_enable_oc_output(TIM2, TIM_OC4);
    timer_set_oc_mode(TIM2, TIM_OC3, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM2, TIM_OC4, TIM_OCM_PWM1);
    timer_enable_counter(TIM2);

	timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM3, 84);
    timer_set_period(TIM3, 999);
    timer_enable_oc_output(TIM3, TIM_OC1);
    timer_enable_oc_output(TIM3, TIM_OC2);
    timer_enable_oc_output(TIM3, TIM_OC3);
    timer_enable_oc_output(TIM3, TIM_OC4);
    timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM3, TIM_OC2, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM3, TIM_OC3, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM3, TIM_OC4, TIM_OCM_PWM1);
    timer_enable_counter(TIM3);

	timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM4, 84);
    timer_set_period(TIM4, 999);
    timer_enable_oc_output(TIM4, TIM_OC2);
    timer_enable_oc_output(TIM4, TIM_OC3);
    timer_set_oc_mode(TIM4, TIM_OC2, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM4, TIM_OC3, TIM_OCM_PWM1);
    timer_enable_counter(TIM4);
}

static void usart_setup(void)
{
	usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX_RX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	usart_enable(USART2);
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
            GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);
    gpio_clear(GPIOD, GPIO12 | GPIO13 | GPIO14 | GPIO15);

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
	
	/* Debug output USART */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP,
			GPIO2 | GPIO3);
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2 | GPIO3);
}

int main(void) 
{
    clock_setup();
    timer_setup();
	usart_setup();
    gpio_setup();

	const uint32_t pulse = 1000/3;
	
	timer_set_oc_value(TIM2, TIM_OC3, pulse);

    log_Init(USART2);
    log_SetEnable(true);

    for (;;) {

    }

    return 0;
}
