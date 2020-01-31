#include "accelerometer.h"
#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static struct 
{
    ADC_HandleTypeDef * adc_x;
    ADC_HandleTypeDef * adc_y;
    ADC_HandleTypeDef * adc_z;
    accelerometer_t accel;
    accel_status_t status;
} accelerometer_handle;

void accelerometer_Init(ADC_HandleTypeDef *adc_x,
                        ADC_HandleTypeDef *adc_y, ADC_HandleTypeDef *adc_z)
{
    memset(&accelerometer_handle, 0, sizeof(accelerometer_handle));
    accelerometer_handle.status = ACCEL_ERR_NO_INIT;

        if ((HAL_ADC_Start(adc_x) == HAL_OK) && (HAL_ADC_Start(adc_y) == HAL_OK) && (HAL_ADC_Start(adc_z) == HAL_OK))
    {
        accelerometer_handle.adc_x = adc_x;
        accelerometer_handle.adc_y = adc_y;
        accelerometer_handle.adc_z = adc_z;
        accelerometer_handle.status = ACCEL_OK;
    }
}

accelerometer_t accelerometer_Update(void)
{
    accelerometer_t accel = {0};
    if (accelerometer_handle.status == ACCEL_OK)
    {
        accel = accelerometer_handle.accel;
    }

    return accel;
}

// TODO: add the 3 ADC interrupt functions
// TODO: find a better way to get the acceleration data, rather than polling for it.
// TODO: smooth input/Kalman Filter?
