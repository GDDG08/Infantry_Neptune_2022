/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IST8310_DRDY_Pin GPIO_PIN_13
#define IST8310_DRDY_GPIO_Port GPIOC
#define IST8310_DRDY_EXTI_IRQn EXTI15_10_IRQn
#define IST8310_RST_Pin GPIO_PIN_14
#define IST8310_RST_GPIO_Port GPIOC
#define Infantry_code3_Pin GPIO_PIN_15
#define Infantry_code3_GPIO_Port GPIOC
#define VCC_Voltage_Pin GPIO_PIN_0
#define VCC_Voltage_GPIO_Port GPIOC
#define Cap_Voltage_Pin GPIO_PIN_1
#define Cap_Voltage_GPIO_Port GPIOC
#define Out_Voltage_Pin GPIO_PIN_2
#define Out_Voltage_GPIO_Port GPIOC
#define Func_Pin GPIO_PIN_3
#define Func_GPIO_Port GPIOC
#define Func_EXTI_IRQn EXTI3_IRQn
#define Left_encoderA_Pin GPIO_PIN_0
#define Left_encoderA_GPIO_Port GPIOA
#define Left_encoderB_Pin GPIO_PIN_1
#define Left_encoderB_GPIO_Port GPIOA
#define Referee_Pin GPIO_PIN_2
#define Referee_GPIO_Port GPIOA
#define RefereeA3_Pin GPIO_PIN_3
#define RefereeA3_GPIO_Port GPIOA
#define Charge_Set_Pin GPIO_PIN_4
#define Charge_Set_GPIO_Port GPIOA
#define BMI088_Pin GPIO_PIN_5
#define BMI088_GPIO_Port GPIOA
#define BMI088A6_Pin GPIO_PIN_6
#define BMI088A6_GPIO_Port GPIOA
#define BMI088A7_Pin GPIO_PIN_7
#define BMI088A7_GPIO_Port GPIOA
#define Buck_CSAccel_Pin GPIO_PIN_4
#define Buck_CSAccel_GPIO_Port GPIOC
#define Cap_CSGyro_Pin GPIO_PIN_5
#define Cap_CSGyro_GPIO_Port GPIOC
#define Bullet_Charge_Pin GPIO_PIN_0
#define Bullet_Charge_GPIO_Port GPIOB
#define Aim_Pin GPIO_PIN_1
#define Aim_GPIO_Port GPIOB
#define BMI088_INT1_Pin GPIO_PIN_2
#define BMI088_INT1_GPIO_Port GPIOB
#define BMI088_INT1_EXTI_IRQn EXTI2_IRQn
#define IST8310_Pin GPIO_PIN_10
#define IST8310_GPIO_Port GPIOB
#define IST8310B11_Pin GPIO_PIN_11
#define IST8310B11_GPIO_Port GPIOB
#define BMI088_INT2_Pin GPIO_PIN_14
#define BMI088_INT2_GPIO_Port GPIOB
#define BMI088_INT2_EXTI_IRQn EXTI15_10_IRQn
#define Heater_Pin GPIO_PIN_15
#define Heater_GPIO_Port GPIOB
#define IMU_CAP_Pin GPIO_PIN_6
#define IMU_CAP_GPIO_Port GPIOC
#define IMU_CAPC7_Pin GPIO_PIN_7
#define IMU_CAPC7_GPIO_Port GPIOC
#define Infantry_code1_Pin GPIO_PIN_8
#define Infantry_code1_GPIO_Port GPIOC
#define Infantry_code2_Pin GPIO_PIN_9
#define Infantry_code2_GPIO_Port GPIOC
#define Servo_Pin GPIO_PIN_8
#define Servo_GPIO_Port GPIOA
#define Dbus_Pin GPIO_PIN_9
#define Dbus_GPIO_Port GPIOA
#define DbusA10_Pin GPIO_PIN_10
#define DbusA10_GPIO_Port GPIOA
#define Back_Pin GPIO_PIN_15
#define Back_GPIO_Port GPIOA
#define Back_EXTI_IRQn EXTI15_10_IRQn
#define MiniPC_Pin GPIO_PIN_10
#define MiniPC_GPIO_Port GPIOC
#define MiniPCC11_Pin GPIO_PIN_11
#define MiniPCC11_GPIO_Port GPIOC
#define Reserved_Pin GPIO_PIN_12
#define Reserved_GPIO_Port GPIOC
#define ReservedD2_Pin GPIO_PIN_2
#define ReservedD2_GPIO_Port GPIOD
#define Boost_Pin GPIO_PIN_3
#define Boost_GPIO_Port GPIOB
#define Snail_A_Pin GPIO_PIN_4
#define Snail_A_GPIO_Port GPIOB
#define Snail_B_Pin GPIO_PIN_5
#define Snail_B_GPIO_Port GPIOB
#define Right_encoderA_Pin GPIO_PIN_6
#define Right_encoderA_GPIO_Port GPIOB
#define Right_encoderB_Pin GPIO_PIN_7
#define Right_encoderB_GPIO_Port GPIOB
#define OLED_Pin GPIO_PIN_8
#define OLED_GPIO_Port GPIOB
#define OLEDB9_Pin GPIO_PIN_9
#define OLEDB9_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
