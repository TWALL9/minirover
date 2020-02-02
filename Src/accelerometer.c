#include "accelerometer.h"
#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define ACCEL_SMOOTHED 6
static accelerometer_t smoothedAccel = {0};

void accelerometer_Update(accelerometer_t * updatedAccelValues)
{
    smoothedAccel.accel_x = updatedAccelValues->accel_x / ACCEL_SMOOTHED;
    smoothedAccel.accel_y = updatedAccelValues->accel_y / ACCEL_SMOOTHED;
    smoothedAccel.accel_z = updatedAccelValues->accel_z / ACCEL_SMOOTHED;
}

accelerometer_t accelerometer_GetSmoothed(void)
{
    return smoothedAccel;
}

// TODO: smooth input/Kalman Filter?
