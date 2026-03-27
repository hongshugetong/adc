/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os2.h"
#include "FreeRTOS.h"
/* Private includes --
/* USER CODE END Includes */
#include "loop.h"
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
#define Reset_4G_Pin GPIO_PIN_3
#define Reset_4G_GPIO_Port GPIOA
#define Pwr_4G_Pin GPIO_PIN_1
#define Pwr_4G_GPIO_Port GPIOB
#define W25Q64_CS_Pin GPIO_PIN_15
#define W25Q64_CS_GPIO_Port GPIOG
#define OTA_VERSION_SIZE 32

/* USER CODE BEGIN Private defines */
extern uint32_t ADC_data[10];
extern uint16_t Data;
extern osSemaphoreId_t processHandle;
extern DMA_HandleTypeDef hdma_adc1;
extern osMessageQueueId_t Adcqueue01Handle;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern osMessageQueueId_t ReciveQueue02Handle;
extern osThreadId_t DataprocessTaskHandle;
extern uint8_t AT_Recive[1024];
extern PacketQueue_t message;
typedef struct{
  uint8_t Update_Buffer[1024];
  uint32_t W25Q64_Block_num;
  uint32_t XMODE_TIMER;
  uint32_t XMODE_NUMBER;
  uint32_t XMODE_CRC;
}UPDATEA_INFO;
enum OTA_FLAG {
    Ready  = 1,
    Unredy = 0
};
typedef struct
{
    enum OTA_FLAG flag;
    uint32_t FileLen[5];
    uint8_t OTA_VERSION[32];
} OTA_INFO;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
