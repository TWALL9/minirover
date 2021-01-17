#ifndef GPIO_H
#define GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libopencm3/stm32/gpio.h>

bool gpio_get_pin(uint32_t gpioport, uint16_t gpios);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_H */
