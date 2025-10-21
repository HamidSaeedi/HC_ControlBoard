/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f3xx_hal.h"

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
#define TACHO1_Pin GPIO_PIN_13
#define TACHO1_GPIO_Port GPIOC
#define TACHO1_EXTI_IRQn EXTI15_10_IRQn
#define FAN1_Pin GPIO_PIN_14
#define FAN1_GPIO_Port GPIOC
#define EN1_Pin GPIO_PIN_0
#define EN1_GPIO_Port GPIOB
#define DC_OKAY_ERROR_LED_Pin GPIO_PIN_1
#define DC_OKAY_ERROR_LED_GPIO_Port GPIOB
#define FAN_LED_Pin GPIO_PIN_2
#define FAN_LED_GPIO_Port GPIOB
#define ENABLE_LED_Pin GPIO_PIN_10
#define ENABLE_LED_GPIO_Port GPIOB
#define COM_LED_Pin GPIO_PIN_11
#define COM_LED_GPIO_Port GPIOB
#define HEAT_SINK_Pin GPIO_PIN_13
#define HEAT_SINK_GPIO_Port GPIOB
#define RELAY_INRUSH_Pin GPIO_PIN_14
#define RELAY_INRUSH_GPIO_Port GPIOB
#define PFC_EN_Pin GPIO_PIN_15
#define PFC_EN_GPIO_Port GPIOB
#define TACHO2_Pin GPIO_PIN_6
#define TACHO2_GPIO_Port GPIOC
#define TACHO2_EXTI_IRQn EXTI9_5_IRQn
#define RELAY_AC_Pin GPIO_PIN_7
#define RELAY_AC_GPIO_Port GPIOC
#define PING_Pin GPIO_PIN_8
#define PING_GPIO_Port GPIOC
#define DC_OK_Pin GPIO_PIN_8
#define DC_OK_GPIO_Port GPIOA
#define HEAT_SINK2_Pin GPIO_PIN_9
#define HEAT_SINK2_GPIO_Port GPIOA
#define DERE_Pin GPIO_PIN_5
#define DERE_GPIO_Port GPIOB
#define FAN_Pin GPIO_PIN_8
#define FAN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
