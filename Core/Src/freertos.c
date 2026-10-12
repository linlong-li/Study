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
#include "bsp_key.h"
#include "bsp_led.h"
#include "queue.h"
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

/* Definitions for keyTask */
osThreadId_t keyTaskHandle;
const osThreadAttr_t keyTask_attributes = {
  .name = "keyTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Definitions for ledTask */
osThreadId_t ledTaskHandle;
const osThreadAttr_t ledTask_attributes = {
  .name = "ledTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

QueueHandle_t key2led_queue = NULL;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void keyTask(void *argument);

void ledTask(void *argument);

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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  keyTaskHandle     = osThreadNew(keyTask         , NULL, &keyTask_attributes    );

  ledTaskHandle     = osThreadNew(ledTask         , NULL, &ledTask_attributes    );

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
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

void keyTask(void *argument)
{
  /* USER CODE BEGIN keyTask */
  key_state_t key_state = KEY_NOT_PRESSED;
  BaseType_t  ret       = NULL           ;

  key2led_queue = xQueueCreate(10,sizeof(key_state_t));
  // if(NULL == key2led_queue)
  // {
  //   printf("create failed");
  // }
  // if(NULL != key2led_queue)
  // {
  //   printf("create successfully");
  // }
  /* Infinite loop */
  for(;;)
  {
    //printf("key\n");

    key_state = key_scan();
    // if(KEY_PRESSED == key_state)
    // {
    //   printf("PRESSED\r\n");
    // }
    // if(KEY_NOT_PRESSED == key_state)
    // {
    //   printf("NOT_PRESSED\r\n");
    // }
    // if(KEY_ERROR == key_state)
    // {
    //   printf("ERROR\r\n");
    // }

    ret = xQueueSend(key2led_queue,&key_state,0xFFFF);
    // if(errQUEUE_FULL == ret)
    // {
    //   printf("send failed");
    // }
    // if(pdPASS == ret)
    // {
    //   printf("send successfully");
    // }

    osDelay(10);
  }
  /* USER CODE END keyTask */
}

void ledTask(void *argument)
{
  /* USER CODE BEGIN ledTask */
  key_state_t key_state = KEY_NOT_PRESSED;
  BaseType_t  ret       = NULL           ;

  /* Infinite loop */
  for(;;)
  {
    //printf("led\r\n");

    if(NULL != key2led_queue)
    {
      ret = xQueueReceive(key2led_queue,&key_state,0xFFFF);
      if(pdPASS == ret)
      {
        if(KEY_PRESSED == key_state)
        {
          led_turn_on();
        }
        if(KEY_NOT_PRESSED == key_state||KEY_ERROR == key_state)
        {
          led_turn_off();
        }
        //printf("key_state=%d\r\n",key_state);
      }
      if(errQUEUE_EMPTY == ret)
      {
        //printf("receive failed\r\n");
      }
      
    }

    osDelay(10);
  }
  /* USER CODE END ledTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

