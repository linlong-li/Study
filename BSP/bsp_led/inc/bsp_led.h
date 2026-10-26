/*****************************************************************************
 * @file     
 * @par_dependencies
 * @author
 * @brief    
 * @version  
 * @date
 * @note 
 ****************************************************************************/
#ifndef _BSP_LED_H_
#define _BSP_LED_H_

 
//********************************************includes**********************//

//********************************************includes**********************//

 
//********************************************Defines***********************//
/**
 * @brief LED 操作返回状态
 */
typedef enum
{
    LED_OK    = 0, /*!< 操作成功 */
    LED_ERROR = 1, /*!< 操作失败 */
}led_state_t;

/**
 * @brief LED 操作类型
 */
typedef enum
{
    LED_TURN_OFF = 0, /*!< 关闭 */
    LED_TURN_ON  = 1, /*!< 打开 */
    LED_TOGGLE   = 2, /*!< 翻转 */
    LED_BLINK    = 3, /*!< 闪烁 */
}led_operation_t;

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
int led_task_init(void);

/**
 * @brief : 打开 LED（PC13 低电平点亮）
 *
 * @param[in] : 无
 *
 * @return : 无
 *
 * */
void led_turn_on(void);

/**
 * @brief : 关闭 LED（PC13 高电平熄灭）
 *
 * @param[in] : 无
 *
 * @return : 无
 *
 * */
void led_turn_off(void);

/**
 * @brief : 翻转 LED 电平状态
 *
 * @param[in] : 无
 *
 * @return : 无
 *
 * */
void led_toggle(void);

/**
 * @brief : LED 闪烁指定次数
 *
 * @param[in] : times 闪烁次数
 *
 * @return : 无
 *
 * */
void led_blink(int times);

/**
 * @brief : 向 LED 消息队列发送一个操作
 *
 * @param[in] : led_opt 指向待发送 LED 操作的指针
 *
 * @return : LED_OK 表示成功，LED_ERROR 表示失败
 *
 * */
led_state_t led_send_operation(led_operation_t *led_opt);

/**
 * @brief : 从 LED 消息队列读取一个操作
 *
 * @param[in] : led_opt 用于接收 LED 操作的指针
 *
 * @return : LED_OK 表示成功，LED_ERROR 表示失败
 *
 * */
led_state_t led_get_operation(led_operation_t *led_opt);

/**
 * @brief : 根据操作来得到led反转次数
 *
 * @param[in] : led_opt 用于接收 LED 操作
 *
 * @return : LED_OK 表示成功，LED_ERROR 表示失败
 *
 * */
led_state_t led_get_operation_isr(led_operation_t *led_opt);

/**
 * @brief : 执行指定的 LED 操作
 *
 * @param[in] : led_opt 要执行的 LED 操作
 *
 * @return : 无
 *
 * */
void led_operation(led_operation_t led_opt);

/**
 * @brief : 中断中执行指定的 LED 操作
 *
 * @param[in] : led_opt 要执行的 LED 操作
 *
 * @return : 无
 *
 * */
void led_operation_isr(void);

/**
 * @brief : LED 任务：循环读取队列中的操作并执行
 *
 * @param[in] : Parameters FreeRTOS 任务参数（未使用）
 *
 * @return : 无
 *
 * */
void ledTask(void *Parameters);
//********************************************Declaring*********************//

#endif
