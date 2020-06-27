#include <stdbool.h>
#include <stdint.h>
#include "ultrasonic.h"
#include "main.h"
#include "tim.h"

#define CM_US_CONVERSION ((float)0.0171821)

static void ultrasonic_ConfigureTrig(GPIO_reference_t reference);
static void ultrasonic_ConfigureEcho(GPIO_reference_t reference);

static uint16_t hc_sr04_Read(UltrasonicHandle_t *handle, uint16_t maxDelay);
static uint16_t ping_Read(UltrasonicHandle_t *handle, uint16_t maxDelay);

UltrasonicHandle_t ultrasonic_Init(GPIO_reference_t trigPin, GPIO_reference_t echoPin, UltrasonicType_t type)
{
    switch(type)
    {
        case(HC_SR04):
        {
            ultrasonic_ConfigureTrig(trigPin);
            ultrasonic_ConfigureEcho(echoPin);
            break;
        }
        case(PING):
        {
            // trigger configuration shared with HC-SR04
            // Parallax Ping uses a shared trigger/echo pin.
            ultrasonic_ConfigureTrig(trigPin);
            echoPin = trigPin;
            break;
        }
        default:
            break;
    }

    UltrasonicHandle_t handle = 
    {
        .trigPin = trigPin,
        .echoPin = echoPin,
        .state = IDLE,
        .responseTimer = 0,
        .type = type
    };

    return handle;
}

uint16_t ultrasonic_Read(UltrasonicHandle_t *handle, uint16_t maxDelay)
{
    switch(handle->type)
    {
        case(HC_SR04):
        {
            return hc_sr04_Read(handle, maxDelay);
        }
        case(PING):
        {
            return ping_Read(handle, maxDelay);
        }
        default:
            return 0;
    }

    return 0;
}

static uint16_t hc_sr04_Read(UltrasonicHandle_t *handle, uint16_t maxDelay)
{
    switch (handle->state)
    {
        case(IDLE):
        case(COMPLETE):
        {
            HAL_GPIO_WritePin(handle->trigPin.port, handle->trigPin.pin, GPIO_PIN_SET);
            TIM_MicrosecondDelay(10);
            HAL_GPIO_WritePin(handle->trigPin.port, handle->trigPin.pin, GPIO_PIN_RESET);
            
            handle->startTimer = TIM_GetMilliseconds();

            handle->state = WAIT_FOR_RESPONSE;
            break;
        }
        case (WAIT_FOR_RESPONSE):
        {
            if (HAL_GPIO_ReadPin(handle->echoPin.port, handle->echoPin.pin) == GPIO_PIN_SET)
            {
                handle->responseTimer = TIM_GetMicroseconds();
                handle->state = WAIT_FOR_CALC;
                return 0;
            }
            break;
        }
        case (WAIT_FOR_CALC):
        {
            if (HAL_GPIO_ReadPin(handle->echoPin.port, handle->echoPin.pin) == GPIO_PIN_RESET)
            {
                uint16_t roundTrip = TIM_GetMicroseconds() - handle->responseTimer;
                float distance = roundTrip * CM_US_CONVERSION;
                handle->state = COMPLETE;
                return (uint16_t)distance;
            }
            break;
        }
        default:
            break;
    }

    if (TIM_GetMilliseconds() - handle->startTimer >= maxDelay)
    {
        // we've timed out.  Reset.
        handle->state = IDLE;
    }

    return 0;
}

static uint16_t ping_Read(UltrasonicHandle_t *handle, uint16_t maxDelay)
{
    switch (handle->state)
    {
        case (IDLE):
        case (COMPLETE):
        {
            // Need to set trigger to output
            ultrasonic_ConfigureTrig(handle->trigPin);

            HAL_GPIO_WritePin(handle->trigPin.port, handle->trigPin.pin, GPIO_PIN_SET);
            TIM_MicrosecondDelay(10);
            HAL_GPIO_WritePin(handle->trigPin.port, handle->trigPin.pin, GPIO_PIN_RESET);

            handle->startTimer = TIM_GetMilliseconds();

            // Receive ping back on same pin.  Convert to echo mode
            ultrasonic_ConfigureEcho(handle->echoPin);

            handle->state = WAIT_FOR_RESPONSE;
            break;
        }
        case (WAIT_FOR_RESPONSE):
        {
            if (HAL_GPIO_ReadPin(handle->echoPin.port, handle->echoPin.pin) == GPIO_PIN_SET)
            {
                handle->responseTimer = TIM_GetMicroseconds();
                handle->state = WAIT_FOR_CALC;
                return 0;
            }
            break;
        }
        case (WAIT_FOR_CALC):
        {
            if (HAL_GPIO_ReadPin(handle->echoPin.port, handle->echoPin.pin) == GPIO_PIN_RESET)
            {
                uint16_t roundTrip = TIM_GetMicroseconds() - handle->responseTimer;
                float distance = roundTrip * CM_US_CONVERSION;
                handle->state = COMPLETE;
                return (uint16_t)distance;
            }
            break;
        }
        default:
            break;
    }

    if (TIM_GetMilliseconds() - handle->startTimer >= maxDelay)
    {
        // we've timed out.  Reset.
        handle->state = IDLE;
    }

    return 0;
}

static void ultrasonic_ConfigureTrig(GPIO_reference_t reference)
{
    // Pulldown
    // output
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = reference.pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(reference.port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(reference.port, reference.pin, GPIO_PIN_RESET);
}

static void ultrasonic_ConfigureEcho(GPIO_reference_t reference)
{
    // pulldown
    // input

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = reference.pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(reference.port, &GPIO_InitStruct);
}
