#ifndef MOTOR_H
#define MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/** 
 * @brief Motor direction enum
 */ 
typedef enum
{
    MOTOR_FORWARD = 0,
    MOTOR_REVERSE,
    MOTOR_NEUTRAL,
    MOTOR_BRAKE
} MotorDirection;

/**
 * @brief Motor placement
 */ 
typedef enum
{
    REAR_LEFT = 0,
    REAR_RIGHT,
    FRONT_LEFT,
    FRONT_RIGHT,
    NUM_MOTORS,
} MotorLocation;

/**
 * @brief Motor Handle definition
 */
typedef struct
{
    TIM_HandleTypeDef * input1Timer;
    TIM_HandleTypeDef * input2Timer;
    uint32_t input1;
    uint32_t input2;
    MotorDirection direction;
    uint8_t dutyCycle;
} MotorHandleTypeDef;

/**
 * @brief Add new motor to list of available motors
 * @retval HAL_StatusTypeDef indicating if PWM is available
 */ 
HAL_StatusTypeDef motor_Init(MotorLocation location, MotorHandleTypeDef motor);

/**
 * @brief Set direction of motor.  Acted upon instantaneously.
 * @param location location of motor
 * @param direction new direction to move
 * @retval none
 */ 
void motor_SetDirection(MotorLocation location, MotorDirection direction);

/**
 * @brief set dutyCycle of motor.
 * @param location location of motor
 * @param dutyCycle new motor speed
 * @retval none
 */ 
void motor_SetSpeed(MotorLocation location, uint8_t dutyCycle);

#ifdef __cplusplus
}
#endif

#endif /* MOTOR_H */
