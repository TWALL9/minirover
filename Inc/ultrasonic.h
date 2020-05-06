#ifndef HC_SR04_H
#define HC_SR04_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    IDLE = 0,
    WAIT_FOR_RESPONSE,
    WAIT_FOR_CALC,
    COMPLETE,
    MAX_STATE
} UltrasonicState_t;

typedef enum
{
    HC_SR04 = 0,
    PING,
} UltrasonicType_t;

typedef struct
{
    GPIO_reference_t trigPin;
    GPIO_reference_t echoPin;
    uint32_t responseTimer;
    UltrasonicState_t state;
    UltrasonicType_t type;
} UltrasonicHandle_t;

UltrasonicHandle_t ultrasonic_Init(GPIO_reference_t trigPin, GPIO_reference_t echoPin, UltrasonicType_t type);
uint16_t ultrasonic_Read(UltrasonicHandle_t *handle, uint16_t maxDelay);

#ifdef __cplusplus
}
#endif

#endif /* HC_SR04_H */
