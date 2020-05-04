#include <stdbool.h>
#include <stdint.h>
#include "hc_sr04.h"
#include "main.h"
#include "tim.h"

#define CM_US_CONVERSION ((float)0.0171821)

static void hc_sr04_ConfigureTrig(GPIO_reference_t *reference);
static void hc_sr04_ConfigureEcho(GPIO_reference_t *reference);

void hc_sr04_Init(hc_sr04_t *handle)
{
    hc_sr04_ConfigureTrig(&handle->trigPin);
    hc_sr04_ConfigureEcho(&handle->echoPin);

    handle->state = IDLE;
    handle->responseTimer = 0;
}

float hc_sr04_Read(hc_sr04_t *handle)
{
    switch (handle->state)
    {
        case(IDLE):
        case(COMPLETE):
        {
            // Ensure pin is reset before triggering
            HAL_GPIO_WritePin(handle->trigPin.port, handle->trigPin.pin, GPIO_PIN_RESET);
            TIM_MicrosecondDelay(2);

            HAL_GPIO_WritePin(handle->trigPin.port, handle->trigPin.pin, GPIO_PIN_SET);
            TIM_MicrosecondDelay(10);
            HAL_GPIO_WritePin(handle->trigPin.port, handle->trigPin.pin, GPIO_PIN_RESET);
            
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
                return distance;
            }
            break;
        }
        default:
            break;
    }
    return 0;
}

static void hc_sr04_ConfigureTrig(GPIO_reference_t * reference)
{
    // Pulldown
    // output
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(reference->port, reference->pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = reference->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(reference->port, &GPIO_InitStruct);
}

static void hc_sr04_ConfigureEcho(GPIO_reference_t *reference)
{
    // pulldown
    // input

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(reference->port, reference->pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = reference->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(reference->port, &GPIO_InitStruct);
}
