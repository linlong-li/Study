/*****************************************************************************
 * @file
 * @par_dependencies
 * @author
 * @brief
 * @version
 * @date
 * @note
 ****************************************************************************/

//********************************************includes**********************//

#include "bsp_led.h"
#include "bsp_key.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stdio.h"
#include "stm32f4xx_hal.h"
//********************************************includes**********************//

//********************************************Defines***********************//
QueueHandle_t led_queue;
static int toggle_times = 0;
//********************************************Defines***********************//

//********************************************Declaring*********************//
/**
 * @brief : 初始化 LED 任务：创建消息队列并创建 ledTask 任务
 *
 * @param[in] : 无
 *
 * @return : 1 表示成功，0 表示失败
 *
 * */
int led_task_init(void)
{
  led_queue = xQueueCreate(5, sizeof(led_operation_t));
  if (NULL == led_queue)
  {
    while (1)
      ;
  }

  BaseType_t ret = NULL;
  ret = xTaskCreate(ledTask, "ledTask", 128 * 4, NULL, 1, NULL);

  if (pdPASS == ret)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief : 打开 LED（PC13 低电平点亮）
 *
 * @param[in] : 无
 *
 * @return : 无
 *
 * */
void led_turn_on(void)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

/**
 * @brief : 关闭 LED（PC13 高电平熄灭）
 *
 * @param[in] : 无
 *
 * @return : 无
 *
 * */
void led_turn_off(void)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

/**
 * @brief : 翻转 LED 电平状态
 *
 * @param[in] : 无
 *
 * @return : 无
 *
 * */
void led_toggle(void)
{
  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

/**
 * @brief : LED 闪烁指定次数
 *
 * @param[in] : times 闪烁次数
 *
 * @return : 无
 *
 * */
void led_blink(int times)
{

  for (int i = 0; i < times; i++)
  {
    led_turn_on();
    vTaskDelay(pdMS_TO_TICKS(100));
    led_turn_off();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

/**
 * @brief : 向 LED 消息队列发送一个操作
 *
 * @param[in] : led_opt 指向待发送 LED 操作的指针
 *
 * @return : LED_OK 表示成功，LED_ERROR 表示失败
 *
 * */
led_state_t led_send_operation(led_operation_t *led_opt)
{
  BaseType_t ret;
  ret = xQueueSend(led_queue, led_opt, 0xffff);

  if (pdTRUE == ret)
  {
    return LED_OK;
  }
  return LED_ERROR;
}

/**
 * @brief : 从 LED 消息队列读取一个操作
 *
 * @param[in] : led_opt 用于接收 LED 操作的指针
 *
 * @return : LED_OK 表示成功，LED_ERROR 表示失败
 *
 * */
led_state_t led_get_operation(led_operation_t *led_opt)
{
  BaseType_t ret;
  ret = xQueueReceive(led_queue, led_opt, 0xffff);

  if (pdTRUE == ret)
  {
    return LED_OK;
  }
  return LED_ERROR;
}

/**
 * @brief : 根据操作来得到led反转次数
 *
 * @param[in] : led_opt 用于接收 LED 操作
 *
 * @return : LED_OK 表示成功，LED_ERROR 表示失败
 *
 * */
led_state_t led_get_operation_isr(led_operation_t *led_opt)
{
  BaseType_t ret;
  ret = xQueueReceive(led_queue, led_opt, 0xffff);

  if (LED_BLINK == *led_opt)
  {
    toggle_times = 20;
  }
  if (LED_TOGGLE == *led_opt)
  {
    toggle_times = 1;
  }

  if (pdTRUE == ret)
  {
    return LED_OK;
  }
  return LED_ERROR;
}

/**
 * @brief : 执行指定的 LED 操作
 *
 * @param[in] : led_opt 要执行的 LED 操作
 *
 * @return : 无
 *
 * */
void led_operation(led_operation_t led_opt)
{
  if (LED_TURN_ON == led_opt)
  {
    led_turn_on();
  }
  if (LED_TURN_OFF == led_opt)
  {
    // led_turn_off();
  }
  if (LED_TOGGLE == led_opt)
  {
    led_toggle();
  }
  if (LED_BLINK == led_opt)
  {
    led_blink(3);
  }
}

/**
 * @brief : 中断中执行指定的 LED 操作
 *
 * @param[in] : led_opt 要执行的 LED 操作
 *
 * @return : 无
 *
 * */
void led_operation_isr(void)
{
  if (toggle_times > 0)
  {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    toggle_times -= 1;
  }
}

/**
 * @brief : LED 任务：循环读取队列中的操作并执行
 *
 * @param[in] : Parameters FreeRTOS 任务参数（未使用）
 *
 * @return : 无
 *
 * */
void ledTask(void *Parameters)
{
  /* USER CODE BEGIN ledTask */
  led_operation_t led_opt = LED_TURN_OFF;
  BaseType_t ret = NULL;

  /* Infinite loop */
  for (;;)
  {
    ret = led_get_operation_isr(&led_opt);
    if (LED_ERROR == ret)
    {
      while (1)
        ;
    }

    vTaskDelay(pdMS_TO_TICKS(50));
  }
  /* USER CODE END ledTask */
}

//********************************************Declaring*********************//
