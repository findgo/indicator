/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#include "stm32f0xx_ll_adc.h"
#include "stm32f0xx_ll_dma.h"
#include "stm32f0xx_ll_crs.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_system.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_cortex.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_usart.h"
#include "stm32f0xx_ll_wwdg.h"
#include "stm32f0xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

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
#define DAT3_KEY_RIGHT_Pin LL_GPIO_PIN_13
#define DAT3_KEY_RIGHT_GPIO_Port GPIOC
#define DAT4_KEY_ENTER_Pin LL_GPIO_PIN_14
#define DAT4_KEY_ENTER_GPIO_Port GPIOC
#define DAT5_LCD_RS_Pin LL_GPIO_PIN_15
#define DAT5_LCD_RS_GPIO_Port GPIOC
#define LED_A1_Pin LL_GPIO_PIN_0
#define LED_A1_GPIO_Port GPIOC
#define LED_A2_Pin LL_GPIO_PIN_1
#define LED_A2_GPIO_Port GPIOC
#define LED_B1_Pin LL_GPIO_PIN_2
#define LED_B1_GPIO_Port GPIOC
#define LED_B2_Pin LL_GPIO_PIN_3
#define LED_B2_GPIO_Port GPIOC
#define HAL_KEY_Pin LL_GPIO_PIN_0
#define HAL_KEY_GPIO_Port GPIOA
#define LEDEnergyStorage_Pin LL_GPIO_PIN_1
#define LEDEnergyStorage_GPIO_Port GPIOA
#define MAX485_TX_Pin LL_GPIO_PIN_2
#define MAX485_TX_GPIO_Port GPIOA
#define MAX485_RX_Pin LL_GPIO_PIN_3
#define MAX485_RX_GPIO_Port GPIOA
#define AD_TEM1_Pin LL_GPIO_PIN_4
#define AD_TEM1_GPIO_Port GPIOA
#define AD_HUM1_Pin LL_GPIO_PIN_5
#define AD_HUM1_GPIO_Port GPIOA
#define AD_TEM2_Pin LL_GPIO_PIN_6
#define AD_TEM2_GPIO_Port GPIOA
#define AD_HUM2_Pin LL_GPIO_PIN_7
#define AD_HUM2_GPIO_Port GPIOA
#define LED_C1_Pin LL_GPIO_PIN_4
#define LED_C1_GPIO_Port GPIOC
#define LED_C2_Pin LL_GPIO_PIN_5
#define LED_C2_GPIO_Port GPIOC
#define LED_D1_Pin LL_GPIO_PIN_0
#define LED_D1_GPIO_Port GPIOB
#define LED_D2_Pin LL_GPIO_PIN_1
#define LED_D2_GPIO_Port GPIOB
#define LED_E1_Pin LL_GPIO_PIN_2
#define LED_E1_GPIO_Port GPIOB
#define LED_E2_Pin LL_GPIO_PIN_10
#define LED_E2_GPIO_Port GPIOB
#define LIVEJUDGMENT_Pin LL_GPIO_PIN_11
#define LIVEJUDGMENT_GPIO_Port GPIOB
#define DAT6_LCD_MOSI_Pin LL_GPIO_PIN_15
#define DAT6_LCD_MOSI_GPIO_Port GPIOB
#define IN_Ground_Pin LL_GPIO_PIN_9
#define IN_Ground_GPIO_Port GPIOC
#define IN_TestStatus_Pin LL_GPIO_PIN_8
#define IN_TestStatus_GPIO_Port GPIOA
#define IN_WorkStatus_Pin LL_GPIO_PIN_11
#define IN_WorkStatus_GPIO_Port GPIOA
#define IN_SwitchOff_Pin LL_GPIO_PIN_12
#define IN_SwitchOff_GPIO_Port GPIOA
#define SWDIO_Pin LL_GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin LL_GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define IN_SwitchOn_Pin LL_GPIO_PIN_15
#define IN_SwitchOn_GPIO_Port GPIOA
#define IN_EnergyStorage_Pin LL_GPIO_PIN_10
#define IN_EnergyStorage_GPIO_Port GPIOC
#define RELAY_HOT1_Pin LL_GPIO_PIN_11
#define RELAY_HOT1_GPIO_Port GPIOC
#define RELAY_HOT2_Pin LL_GPIO_PIN_12
#define RELAY_HOT2_GPIO_Port GPIOC
#define RELAY_ALARM1_Pin LL_GPIO_PIN_2
#define RELAY_ALARM1_GPIO_Port GPIOD
#define RELAY_ALARM2_Pin LL_GPIO_PIN_3
#define RELAY_ALARM2_GPIO_Port GPIOB
#define ALARM_TEST1_Pin LL_GPIO_PIN_4
#define ALARM_TEST1_GPIO_Port GPIOB
#define ALARM_TEST2_Pin LL_GPIO_PIN_5
#define ALARM_TEST2_GPIO_Port GPIOB
#define RELAY_CLOSURE_Pin LL_GPIO_PIN_6
#define RELAY_CLOSURE_GPIO_Port GPIOB
#define DAT0_KEY_UP_Pin LL_GPIO_PIN_7
#define DAT0_KEY_UP_GPIO_Port GPIOB
#define DAT1_KEY_DOWN_Pin LL_GPIO_PIN_8
#define DAT1_KEY_DOWN_GPIO_Port GPIOB
#define DAT2_KEY_LEFT_Pin LL_GPIO_PIN_9
#define DAT2_KEY_LEFT_GPIO_Port GPIOB
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
