#ifndef HC06_H
#define HC06_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "usart.h"

#define NAME_LENGTH 20

typedef enum
{
    BAUD1 = 1200,
    BAUD2 = 2400,
    BAUD3 = 4800,
    BAUD4 = 9600,
    BAUD5 = 19200,
    BAUD6 = 38400,
    BAUD7 = 57600,
    BAUD8 = 115200,
} BaudRates_t;

typedef enum
{
    NONE = 0,
    ODD,
    EVEN
} Parity_t;

typedef struct
{
    UART_HandleTypeDef * uart;
    Parity_t parity;
    BaudRates_t baud;
    uint32_t PIN;
    char name[NAME_LENGTH];
} BluetoothHandle_t;


void ble_Setup(UART_HandleTypeDef * huart);
bool ble_SetName(char * name);
bool ble_SetPIN(uint32_t pin);
bool ble_SetBaud(BaudRates_t baud);
void ble_Transmit(char * buffer, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif /* HC06_H */