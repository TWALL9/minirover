#include "accelerometer.c"

static struct 
{
    ADC_HandleTypeDef * adc_x = NULL;
    ADC_HandleTypeDef * adc_y = NULL;
    ADC_HandleTypeDef * adc_z = NULL;
    accelerometer_t accel = {0};
    bool init = false;
} accelerometerHandle;

bool accelerometer_Init(ADC_HandleTypeDef *adc_x,
                        ADC_HandleTypeDef *adc_y, ADC_HandleTypeDef *adc_z)
{
    memset(&accelerometerHandle, 0, sizeof(accelerometerHandle));

    if (HAL_ADC_Start(adc_x) == HAL_ADC_Start(adc_y) == HAL_ADC_Start(adc_z) == HAL_OK)
    {
        accelerometerHandle.adc_x = adc_x;
        accelerometerHandle.adc_y = adc_y;
        accelerometerHandle.adc_z = adc_z;
        accelerometerHandle.init = true;
    }

    return accelerometerHandle;
}

accelerometer_t accelerometer_Update(void)
{
    accelerometer_t accel = {0};
    if (accelerometerHandle.init)
    {
        accel = accelerometerHandle.accel;
    }

    return accel;
}

// TODO: add the 3 ADC interrupt functions
// TODO: find a better way to get the acceleration data, rather than polling for it.
// TODO: smooth input/Kalman Filter?