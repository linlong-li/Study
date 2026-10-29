/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "queue.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void Task_TreadA(void *argument);
void Task_TreadB(void *argument);

#define BUFFER_SIZE 1
#define ADC_ConvCplt 0x11
#define BUFEER1_READY 0x01
#define BUFEER2_READY 0x02
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint32_t *Buffer1 = NULL;
uint32_t *Buffer2 = NULL;

QueueHandle_t mailbox_it2A;
QueueHandle_t mailbox_A2B;

int index = 0;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  BaseType_t ret;

  ret = xTaskCreate(Task_TreadA, "Task_TreadA", 128, NULL, 1, NULL);
  if (errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY == ret)
  {
    printf("TaskA create failed");
  }

  ret = xTaskCreate(Task_TreadB, "Task_TreadB", 128, NULL, 1, NULL);
  if (errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY == ret)
  {
    printf("TaskB create failed");
  }

  Buffer1 = pvPortMalloc(sizeof(uint32_t) * BUFFER_SIZE);
  if (NULL == Buffer1)
  {
    printf("Buffer1 create failed\r\n");
  }

  Buffer2 = pvPortMalloc(sizeof(uint32_t) * BUFFER_SIZE);
  if (NULL == Buffer2)
  {
    printf("Buffer2 create failed\r\n");
  }

  mailbox_it2A = xQueueCreate(1, sizeof(uint32_t *));
  if (NULL == mailbox_it2A)
  {
    printf("Bmailbox_it2A create failed\r\n");
  }

  mailbox_A2B = xQueueCreate(1, sizeof(uint32_t *));
  if (NULL == mailbox_A2B)
  {
    printf("mailbox_A2B create failed\r\n");
  }

  index = 1;
  HAL_ADC_Start_DMA(&hadc1, Buffer1, BUFFER_SIZE);

  /* Infinite loop */
  for (;;)
  {
    printf("Default\r\n");
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void Task_TreadA(void *argument)
{
  for (;;)
  {
    uint32_t info = 0;
    BaseType_t ret;

    ret = xQueueReceive(mailbox_it2A, &info, portMAX_DELAY);
    if (pdPASS != ret)
    {
      printf("TaskA reciv from it failed");
    }

    if (1 == index)
    {
      /* 1st: point DMA at the other buffer, 2nd: hand Buffer1 to Task B */
      index = 2;
      HAL_ADC_Start_DMA(&hadc1, Buffer2, BUFFER_SIZE);

      uint32_t info = BUFEER1_READY;
      ret = xQueueSend(mailbox_A2B, &info, portMAX_DELAY);
      if (pdPASS != ret)
      {
        printf("TaskA send to B failed\r\n");
      }
    }
    else
    {
      /* 1st: point DMA at the other buffer, 2nd: hand Buffer2 to Task B */
      index = 1;
      HAL_ADC_Start_DMA(&hadc1, Buffer1, BUFFER_SIZE);

      uint32_t info = BUFEER2_READY;
      ret = xQueueSend(mailbox_A2B, &info, portMAX_DELAY);
      if (pdPASS != ret)
      {
        printf("TaskA send to B failed\r\n");
      }
    }
  }
}

void Task_TreadB(void *argument)
{
  for (;;)
  {
    uint32_t info;
    xQueueReceive(mailbox_A2B, &info, portMAX_DELAY);

    if (BUFEER1_READY == info)
    {
      printf("buffer1 vol:%f\r\n", Buffer1[0] / 4095.f * 3.3f);
    }
    if (BUFEER2_READY == info)
    {
      printf("buffer2 vol:%f\r\n", Buffer2[0] / 4095.f * 3.3f);
    }
    
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_ADC_ConvCpltCallback could be implemented in the user file
   */
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  uint32_t info = ADC_ConvCplt;

  xQueueSendFromISR(mailbox_it2A, &info, &xHigherPriorityTaskWoken);

  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_ADC_ErrorCallback could be implemented in the user file
   */

  printf("ADC error\r\n");
}
/* USER CODE END Application */
