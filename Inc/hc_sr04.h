#ifndef HC_SR04_H
#define HC_SR04_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

typedef enum
{
    IDLE = 0,
    WAIT_FOR_RESPONSE,
    WAIT_FOR_CALC,
    COMPLETE
} UltrasonicState_t;

    typedef struct
    {
        GPIO_reference_t trigPin;
        GPIO_reference_t echoPin;
        uint32_t responseTimer;
        UltrasonicState_t state;
} hc_sr04_t;

void hc_sr04_Init(hc_sr04_t * handle);
uint16_t hc_sr04_Read(hc_sr04_t *handle, uint16_t maxDelay);

#ifdef __cplusplus
}
#endif

#endif /* HC_SR04_H */
