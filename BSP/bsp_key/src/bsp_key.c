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

#include "bsp_key.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stdio.h"
#include "stm32f4xx_hal.h"

//********************************************includes**********************//

 
//********************************************Defines***********************//
QueueHandle_t key2led_queue;

//********************************************Defines***********************//


//********************************************Declaring*********************//
/**
 * @brief : 初始化按键任务：创建消息队列并创建 keyTask 任务
 *
 * @param[in] : 无
 *
 * @return : 1 表示成功，0 表示失败
 *
 * */
int key_task_init(void)
{
    BaseType_t  ret = NULL;

    key2led_queue = xQueueCreate(5,sizeof(key_press_state_t));
    if(NULL == key2led_queue)
    {
        return 0;
    }

    ret = xTaskCreate(keyTask,"keyTask",128*4,NULL,1,NULL);

    if(pdPASS == ret)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief : 扫描按键电平，判断当前是否被按下
 *
 * @param[in] : 无
 *
 * @return : KEY_PRESSED 表示按下，KEY_NOT_PRESSED 表示未按下
 *
 * */
key_press_state_t key_scan(void)
{
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
    {
        return KEY_PRESSED;
    }
    return KEY_NOT_PRESSED;
}

/**
 * @brief : 检测按键短按/长按（阻塞式）
 *
 * @param[in] : ms 长按判定时间阈值（毫秒）
 *
 * @return : KEY_LONG_PRESSED 长按 / KEY_SHORT_PRESSED 短按 / KEY_NOT_PRESSED 未按下
 *
 * */
key_press_state_t key_scan_short_long(int ms)
{
    key_press_state_t ret = KEY_NOT_PRESSED;
    ret = key_scan();
    float time = 0;

    if(KEY_PRESSED == ret)
    {
        time = HAL_GetTick();
        while (HAL_GetTick()<time + ms)
        ;

        ret = key_scan();

        if(KEY_PRESSED == ret)
        {
            while(KEY_PRESSED == key_scan())
            ;
            return KEY_LONG_PRESSED;
        }
        else
        {
            return KEY_SHORT_PRESSED;
        }
    }
    else
    {
        return KEY_NOT_PRESSED;
    }
}

/**
 * @brief : 从按键消息队列中读取按键状态
 *
 * @param[in] : key_state 用于接收按键状态的指针
 *
 * @return : KEY_OK 表示成功，KEY_ERROR 表示失败
 *
 * */
key_state_t get_key_state(key_press_state_t *key_state)
{
    BaseType_t ret = NULL;
    ret = xQueueReceive(key2led_queue,key_state,0xffff);

    if(pdTRUE == ret)
    {
        return KEY_OK;
    }

    return KEY_ERROR;
}

/**
 * @brief : 按键任务：周期性扫描按键并将结果发送到队列
 *
 * @param[in] : Parameters FreeRTOS 任务参数（未使用）
 *
 * @return : 无
 *
 * */
void keyTask(void *Parameters)
{
  /* USER CODE BEGIN keyTask */
  key_press_state_t key_state = KEY_NOT_PRESSED;
  
  /* Infinite loop */
  for(;;)
  {
    // key_state = key_scan();
    key_state = key_scan_short_long(500);

    printf("%d\r\n",key_state);

    xQueueSend(key2led_queue,&key_state,0xffff);

    vTaskDelay(pdMS_TO_TICKS(50));
  }
  /* USER CODE END keyTask */
}

//********************************************Declaring*********************//
