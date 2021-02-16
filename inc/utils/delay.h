#ifndef DELAY_H
#define DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void delay_us(uint16_t period);
void delay_ms(uint32_t period);

#ifdef __cplusplus
}
#endif

#endif /* DELAY_H */
