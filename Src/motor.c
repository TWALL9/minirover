#include "motor.h"
#include "tim.h"
#include "stm32f4xx_hal_def.h"

MotorHandleTypeDef motors[NUM_MOTORS] = {};

HAL_StatusTypeDef motor_Init(MotorLocation location, MotorHandleTypeDef motor)
{
    HAL_StatusTypeDef pwmStatus = HAL_OK;
    
    if (location < NUM_MOTORS)
    {
        motors[location] = motor;
        pwmStatus = HAL_TIM_Base_Start(motor.input1Timer);
        pwmStatus |= HAL_TIM_Base_Start(motor.input2Timer);
        pwmStatus |= HAL_TIM_PWM_Start(motor.input2Timer, motor.input1);
        pwmStatus |= HAL_TIM_PWM_Start(motor.input2Timer, motor.input2);
    }
    else
    {
        pwmStatus = HAL_ERROR;
    }
    
    return pwmStatus;
}

void motor_SetDirection(MotorLocation location, MotorDirection newDirection)
{
    if (location < NUM_MOTORS)
    {
        MotorHandleTypeDef motor = motors[location];
        motor.direction = newDirection;
        motor_SetSpeed(location, motor.dutyCycle);
    }
}

void motor_SetSpeed(MotorLocation location, uint8_t dutyCycle)
{
    if (location < NUM_MOTORS)
    {
        MotorHandleTypeDef motor = motors[location];
        motor.dutyCycle = dutyCycle;
        switch (motor.direction)
        {
            case (MOTOR_FORWARD):
            {
                PWM_SetDutyCycle(motor.input1Timer, motor.input1, dutyCycle);
                PWM_SetDutyCycle(motor.input2Timer, motor.input2, 0);
                break;
            }

            case (MOTOR_REVERSE):
            {
                PWM_SetDutyCycle(motor.input1Timer, motor.input1, 0);
                PWM_SetDutyCycle(motor.input2Timer, motor.input2, dutyCycle);
                break;
            }

            case (MOTOR_NEUTRAL):
            {
                PWM_SetDutyCycle(motor.input1Timer, motor.input1, 0);
                PWM_SetDutyCycle(motor.input2Timer, motor.input2, 0);
                break;
            }

            case (MOTOR_BRAKE):
            {
                PWM_SetDutyCycle(motor.input1Timer, motor.input1, 0xFF);
                PWM_SetDutyCycle(motor.input2Timer, motor.input2, 0xFF);
                break;
            }
        }
    }
}
