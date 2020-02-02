#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "adc.h"

typedef struct 
{
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
} accelerometer_t;

void accelerometer_Update(accelerometer_t * updatedAccelValues);

accelerometer_t accelerometer_GetSmoothed(void);

#ifdef __cplusplus
 }
#endif

#endif /* MOTOR_H */