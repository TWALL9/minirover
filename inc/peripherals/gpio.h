#ifndef GPIO_H
#define GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libopencm3/stm32/gpio.h>

// All LED's are on GPIOD
#define LED_GREEN GPIO12
#define LED_ORANGE GPIO13
#define LED_RED GPIO14
#define LED_BLUE GPIO15

bool gpio_get_pin(uint32_t gpioport, uint16_t gpios);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_H */
