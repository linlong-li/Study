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
#include "queue.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_led_driver.h"
#include "bsp_led_hanler.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TEST_LED

#ifdef TEST_LED
led_handler_status_t led_get_time(void)
{
    HAL_GetTick();
    return LED_HANDLER_OK;
}

led_get_time_ms_t led_get_time_ms = {
    .pf_led_get_time_ms = led_get_time
};

led_handler_status_t led_os_delay(const uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
    return LED_HANDLER_OK;
}

led_os_delay_t led_os_delay_ms = {
    .pf_led_os_delay = led_os_delay
};

led_handler_status_t led_os_thread_create(void * const task_code,
                    const char * const task_name,
                    const uint32_t stack_depth,
                    void * const parameters,
                    uint32_t priority,
                    void ** const task_handler)
{
    /*Check parameters*/
    if (task_code == NULL || task_name == NULL || 
        task_handler == NULL|| stack_depth <= 0 )
    {
        return LED_HANDLER_ERRORPARAMETER;
    }

    BaseType_t ret = xTaskCreate((TaskFunction_t)task_code, 
                                        task_name, 
                                        stack_depth , 
                                        parameters, 
                                        priority, 
                                        (TaskHandle_t *)task_handler);

    if (ret == pdPASS)
    {
        return LED_HANDLER_OK;
    }
    else
    {
        return LED_HANDLER_ERROR;
    }

}

led_create_thread_t led_os_thread_create_t = {
    .pf_led_create_thread = &led_os_thread_create
};

led_handler_status_t led_os_thread_delete(void *thread_handle)
{
    if (thread_handle == NULL)
    {
        return LED_HANDLER_ERRORPARAMETER;
    }

    vTaskDelete((TaskHandle_t)thread_handle);
    return LED_HANDLER_OK;
}

led_delete_thread_t led_os_thread_delete_t = {
    .pf_led_delete_thread = &led_os_thread_delete
};

led_os_thread_t led_os_thread = {
    .led_create_thread = &led_os_thread_create_t,
    .led_delete_thread = &led_os_thread_delete_t
};

led_handler_status_t led_os_queue_create(void **queue_handle, 
                                    uint32_t queue_length, 
                                    uint32_t item_size)
{
    if (queue_handle == NULL || queue_length <= 0 || item_size <= 0)
    {
        return LED_HANDLER_ERRORPARAMETER;
    }

    *queue_handle = xQueueCreate(queue_length, item_size);
    if (*queue_handle != NULL)
    {
        return LED_HANDLER_OK;
    }
    else
    {
        return LED_HANDLER_ERRORNOMEMORY;
    }
}

led_create_queue_t led_os_queue_create_t = {
    .pf_led_create_queue = &led_os_queue_create
};

led_handler_status_t led_os_queue_delete(void *queue_handle)
{
    if (queue_handle == NULL)
    {
        return LED_HANDLER_ERRORPARAMETER;
    }

    vQueueDelete((QueueHandle_t)queue_handle);
    return LED_HANDLER_OK;
}

led_delete_queue_t led_os_queue_delete_t = {
    .pf_led_delete_queue = &led_os_queue_delete
};

led_handler_status_t led_os_queue_send(void *queue_handle, 
                                    const void *item, 
                                    uint32_t timeout)
{
    if (queue_handle == NULL || item == NULL)
    {
        return LED_HANDLER_ERRORPARAMETER;
    }

    BaseType_t ret = xQueueSend((QueueHandle_t)queue_handle, item, timeout);
    if (ret == pdPASS)
    {
        return LED_HANDLER_OK;
    }
    else
    {
        return LED_HANDLER_ERROR;
    }
}

led_send_queue_t led_os_queue_send_t = {
    .pf_led_send_queue = &led_os_queue_send
};

led_handler_status_t led_os_queue_receive(void *queue_handle, 
                                       void *item, 
                                       uint32_t timeout)
{
    if (queue_handle == NULL || item == NULL)
    {
        return LED_HANDLER_ERRORPARAMETER;
    }

    BaseType_t ret = xQueueReceive((QueueHandle_t)queue_handle, item, timeout);
    if (ret == pdPASS)
    {
        return LED_HANDLER_OK;
    }
    else
    {
        return LED_HANDLER_ERROR;
    }
}

led_receive_queue_t led_os_queue_receive_t = {
    .pf_led_receive_queue = &led_os_queue_receive
};

led_os_queue_t led_os_queue = {
    .led_create_queue  = &led_os_queue_create_t,
    .led_delete_queue  = &led_os_queue_delete_t,
    .led_send_queue    = &led_os_queue_send_t,
    .led_receive_queue = &led_os_queue_receive_t
};

void test()
{
  static led_handler_t led_handler;
  led_handler_status_t ret = led_handler_inst(&led_handler,   
                                              &led_os_delay_ms,
                                              &led_os_thread,
                                              &led_os_queue,
                                              NULL,
                                              &led_get_time_ms);
}
#endif

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

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
#ifdef TEST_LED
  test();
#endif

  /* Infinite loop */
  for(;;)
  {
    printf("helloworld\r\n");
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

