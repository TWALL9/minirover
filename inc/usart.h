#ifndef USART_H
#define USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libopencm3/stm32/usart.h>
void usart_setup(void);
void usart_send_buf(const char * buf, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif /* USART_H */
