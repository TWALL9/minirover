#ifndef HC_06_H
#define HC_06_H

#include <stdint.h>
#include "peripherals/usart.h"

/**
 * This driver is for the HC-06 USART/Bluetooth module.
 * Some _interesting_ quirks about this thing:
 * 1. Slave only
 * 2. Commands aren't queued with a nice little "\r\n", 
 * but by putting them on the bus and waiting
 * 3. AT commands only work when it's not connected to BT...
 * but there's no command to get Bluetooth connection status
 */

#define HC06_PIN_LEN 4
#define HC06_DEFAULT_BAUD_RATE 38400
#define HC06_NAME_LEN 40
#define HC06_MAX_CMD_LEN 10 // AT+VERSION
#define HC06_WRITE_WAIT 1200

class HC06
{
    public:
        HC06(uint32_t usart_base);
        ~HC06(void);
        
        void start(void);
        void reset(void);
        bool check_connect(void);

        void write_blocking(const char * msg, uint16_t len);
        uint16_t read_blocking(char * msg);

        void set_pin(const char * pin);
        void set_name(const char * name);

        void get_pin(char * pin);
        void get_name(char * name);
    
    private:
        uint32_t _usart = 0;
        bool _init = false;
        char _pin[HC06_PIN_LEN + 1];
        char _name[HC06_NAME_LEN + 1];
        char _rx_queue[1024];
};

#endif /* HC_O6_H */
