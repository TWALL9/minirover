#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "ble.h"
#include "main.h"
#include "usart.h"

const char * atPrefix = "AT+";
const char * atSuccess = "OK";

BluetoothHandle_t ble_Setup(UART_HandleTypeDef * huart)
{
    // Set UART peripheral to HC06 defaults
    huart->Init.BaudRate = 9600;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;

    BluetoothHandle_t handle = {};
    memcpy(handle.uart, huart, sizeof(huart));
    handle.parity = NONE;
    handle.baud = BAUD4;
    handle.PIN = 1234;
    strcpy(handle.name, "linvor");

    return handle;
}

bool ble_SetPIN(uint32_t pin)
{
    return true;
}

bool ble_SetBaud(BaudRates_t baud)
{
    return true;
}

void ble_Transmit(char * buffer, uint16_t length)
{
    
}