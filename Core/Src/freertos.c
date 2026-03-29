/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for DataprocessTask */
osThreadId_t DataprocessTaskHandle;
const osThreadAttr_t DataprocessTask_attributes = {
  .name = "DataprocessTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Usart1Task */
osThreadId_t Usart1TaskHandle;
const osThreadAttr_t Usart1Task_attributes = {
  .name = "Usart1Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for BlueTask */
osThreadId_t BlueTaskHandle;
const osThreadAttr_t BlueTask_attributes = {
  .name = "BlueTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for EC200task */
osThreadId_t EC200taskHandle;
const osThreadAttr_t EC200task_attributes = {
  .name = "EC200task",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Adcqueue01 */
osMessageQueueId_t Adcqueue01Handle;
const osMessageQueueAttr_t Adcqueue01_attributes = {
  .name = "Adcqueue01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDataprocessTask(void *argument);
void StartUsart1Task(void *argument);
void StartBlueTask(void *argument);
void StartEC200task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of Adcqueue01 */
  Adcqueue01Handle = osMessageQueueNew (10, sizeof(float), &Adcqueue01_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of DataprocessTask */
  DataprocessTaskHandle = osThreadNew(StartDataprocessTask, NULL, &DataprocessTask_attributes);

  /* creation of Usart1Task */
  Usart1TaskHandle = osThreadNew(StartUsart1Task, NULL, &Usart1Task_attributes);

  /* creation of BlueTask */
  BlueTaskHandle = osThreadNew(StartBlueTask, NULL, &BlueTask_attributes);

  /* creation of EC200task */
  EC200taskHandle = osThreadNew(StartEC200task, NULL, &EC200task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDataprocessTask */
/**
  * @brief  Function implementing the DataprocessTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDataprocessTask */
__weak void StartDataprocessTask(void *argument)
{
  /* USER CODE BEGIN StartDataprocessTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDataprocessTask */
}

/* USER CODE BEGIN Header_StartUsart1Task */
/**
* @brief Function implementing the Usart1Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUsart1Task */
__weak void StartUsart1Task(void *argument)
{
  /* USER CODE BEGIN StartUsart1Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartUsart1Task */
}

/* USER CODE BEGIN Header_StartBlueTask */
/**
* @brief Function implementing the BlueTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBlueTask */
__weak void StartBlueTask(void *argument)
{
  /* USER CODE BEGIN StartBlueTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartBlueTask */
}

/* USER CODE BEGIN Header_StartEC200task */
/**
* @brief Function implementing the EC200task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartEC200task */
__weak void StartEC200task(void *argument)
{
  /* USER CODE BEGIN StartEC200task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartEC200task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

