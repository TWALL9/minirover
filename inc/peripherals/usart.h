#ifndef USART_H
#define USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libopencm3/stm32/usart.h>

void usart_setup(
    uint32_t usart_base, 
    uint32_t baud_rate, 
    uint32_t data_bits, 
    uint32_t stop_bits, 
    uint32_t mode, 
    uint32_t parity, 
    uint32_t flowcontrol
    );
void usart_send_buf(uint32_t usart_base, const char * buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* USART_H */
