#ifdef __cplusplus
extern "C" {
#endif

#include "peripherals/gpio.h"

bool gpio_get_pin(uint32_t gpioport, uint16_t gpios)
{
    return (gpio_get(gpioport, gpios) == gpios);
}

#ifdef __cplusplus
}
#endif