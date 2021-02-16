#ifdef __cplusplus
extern "C" {
#endif

#include "peripherals/usart.h"
#include <libopencm3/stm32/gpio.h>

void usart_setup(void)
{
    /* Debug output USART */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP,
			GPIO2 | GPIO3);
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2 | GPIO3);

	usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX_RX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	usart_enable(USART2);
}

void usart_send_buf(const char * buf, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++)
    {
        char c = buf[i];
        usart_send_blocking(USART2, c);
    }
}

#ifdef __cplusplus
}
#endif