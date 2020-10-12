/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdint.h>
#include <stdio.h>
#include "motor.h"
#include "log.h"
#include "ultrasonic.h"

/* Private variables ---------------------------------------------------------*/
uint32_t backLeftMotorEnc = 0;
uint32_t backRightMotorEnc = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* MCU Configuration--------------------------------------------------------*/
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
  
    /* Configure the system clock */
    SystemClock_Config();
    SystemCoreClockUpdate();
  
    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_TIM4_Init();
    MX_ADC1_Init();
    MX_USART2_UART_Init();
    MX_TIM3_Init();
    MX_TIM2_Init();
    MX_TIM14_Init();
  
    log_Init();
    log_SetEnable(true);
    log_SetLevel(LOG_DEBUG);
  
    HAL_TIM_Base_Start(&htim2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
  
    HAL_TIM_Base_Start_IT(&htim14);
    
    MotorHandle_t backLeftMotor = 
    {
        .input1Timer = &htim3,
        .input2Timer = &htim3,
        .input1 = TIM_CHANNEL_1,
        .input2 = TIM_CHANNEL_2,
        .direction = MOTOR_NEUTRAL,
        .dutyCycle = 0
    };
  
    MotorHandle_t backRightMotor = 
    {
        .input1Timer = &htim3,
        .input2Timer = &htim3,
        .input1 = TIM_CHANNEL_3,
        .input2 = TIM_CHANNEL_4,
        .direction = MOTOR_NEUTRAL,
        .dutyCycle = 0
    };
  
    MotorHandle_t frontLeftMotor = 
    {
        .input1Timer = &htim4,
        .input2Timer = &htim4,
        .input1 = TIM_CHANNEL_2,
        .input2 = TIM_CHANNEL_3,
        .direction = MOTOR_NEUTRAL,
        .dutyCycle = 0
    };
    
    MotorHandle_t frontRightMotor = 
    {
        .input1Timer = &htim2,
        .input2Timer = &htim2,
        .input1 = TIM_CHANNEL_3,
        .input2 = TIM_CHANNEL_4,
        .direction = MOTOR_NEUTRAL,
        .dutyCycle = 0
    };
    
    motor_Init(REAR_LEFT, backLeftMotor);
    motor_Init(REAR_RIGHT, backRightMotor);
    motor_Init(FRONT_LEFT, frontLeftMotor);
    motor_Init(FRONT_RIGHT, frontRightMotor);
  
    motor_SetDirection(REAR_LEFT, MOTOR_FORWARD);
    motor_SetDirection(REAR_RIGHT, MOTOR_FORWARD);
    motor_SetDirection(FRONT_LEFT, MOTOR_FORWARD);
    motor_SetDirection(FRONT_RIGHT, MOTOR_FORWARD);
  
    HAL_ADC_Start(&hadc1);
  
    uint32_t msTicks = 0;
    uint8_t dutyCycle = 0;
  
    GPIO_reference_t trigPin = 
    {
        .port = GPIOD,
        .pin = GPIO_PIN_14,
    };
    GPIO_reference_t echoPin = 
    {
        .port = GPIOD,
        .pin = GPIO_PIN_15,
    };

    GPIO_reference_t pingPin = 
    {
        .port = GPIOD,
        .pin = GPIO_PIN_13,
    };
    
    UltrasonicHandle_t hcsr04Handle = ultrasonic_Init(trigPin, echoPin, HC_SR04);
    UltrasonicHandle_t pingHandle = ultrasonic_Init(pingPin, pingPin, PING);

    /* Infinite loop */
    while (1)
    {
        if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK)
        {
            dutyCycle = HAL_ADC_GetValue(&hadc1);
            motor_SetSpeed(REAR_LEFT, dutyCycle);
            motor_SetSpeed(REAR_RIGHT, dutyCycle);
            motor_SetSpeed(FRONT_LEFT, dutyCycle);
            motor_SetSpeed(FRONT_RIGHT, dutyCycle);
        }
        else
        {
            Error_Handler();
        }
  
        if (HAL_GetTick() - msTicks > 500)
        {
            // print the encoder speed.
            //log_DEBUG("%d, %lu", dutyCycle, backRightMotorEnc);
            msTicks = HAL_GetTick();
            backLeftMotorEnc = 0;
            backRightMotorEnc = 0;
        }
  
        uint16_t distance = ultrasonic_Read(&hcsr04Handle, 1000);
        uint16_t pingDistance = ultrasonic_Read(&pingHandle, 1000);
        if (hcsr04Handle.state == COMPLETE && distance < 1000)
        {
            log_DEBUG("HCSR04 %d", distance);
        }
        if (pingHandle.state == COMPLETE && pingDistance < 1000)
        {
            log_DEBUG("Ping %d", pingDistance);
        }
    }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** 
     * Configure the main internal regulator output voltage 
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** 
   * Initializes the CPU, AHB and APB busses clocks 
   */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** 
     * Initializes the CPU, AHB and APB busses clocks 
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief EXTI line detection callbacks
 * @param GPIO_Pin: Specifies the pins connected EXTI line
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) 
{
    switch (GPIO_Pin) 
    {
        case GPIO_PIN_0: 
        {
            break;
        }

        case GPIO_PIN_4: 
        {
            if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4) == GPIO_PIN_SET)
            {
                backRightMotorEnc++;
            }
            break;
        }

        case GPIO_PIN_5: 
        {
            backLeftMotorEnc++;
            break;
        }

        default: { break; }
    }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{

}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{ 
    /**
     * User can add his own implementation to report the file name and line number,
     * tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)
     */
}
#endif /* USE_FULL_ASSERT */
