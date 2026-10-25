/*****************************************************************************
 * @file
 * @par_dependencies
 * @author
 * @brief
 * @version
 * @date
 * @note
 ****************************************************************************/
#ifndef _BSP_KEY_H
#define _BSP_KEY_H

//********************************************includes**********************//

//********************************************includes**********************//

//********************************************Defines***********************//

typedef enum
{
    FALLING_EDGE = 0,
    RISING_EDGE = 1,
} key_edge_t;

typedef struct
{
    float          time; /* 触发时间 */
    key_edge_t key_edge; /* 触发类型 */
} key_exti_ifo_t;

/**
 * @brief 按键操作返回状态
 */
typedef enum
{
    KEY_OK = 0,    /*!< 操作成功 */
    KEY_ERROR = 1, /*!< 操作失败 */
} key_state_t;

/**
 * @brief 按键按压状态
 */
typedef enum
{
    KEY_NOT_PRESSED   = 0,   /*!< 未按下 */
    KEY_PRESSED       = 1,   /*!< 按下   */
    KEY_SHORT_PRESSED = 2,   /*!< 短按   */
    KEY_LONG_PRESSED  = 3,   /*!< 长按   */
} key_press_state_t;

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
int key_task_init(void);

/**
 * @brief : 扫描按键电平，判断当前是否被按下
 *
 * @param[in] : 无
 *
 * @return : KEY_PRESSED 表示按下，KEY_NOT_PRESSED 表示未按下
 *
 * */
key_press_state_t key_scan(void);

/**
 * @brief : 检测按键短按/长按（阻塞式）
 *
 * @param[in] : ms 长按判定时间阈值（毫秒）
 *
 * @return : KEY_LONG_PRESSED 长按 / KEY_SHORT_PRESSED 短按 / KEY_NOT_PRESSED 未按下
 *
 * */
key_press_state_t key_scan_short_long(int ms);

/**
 * @brief : 从按键消息队列中读取按键状态
 *
 * @param[in] : key_state 用于接收按键状态的指针
 *
 * @return : KEY_OK 表示成功，KEY_ERROR 表示失败
 *
 * */
key_state_t get_key_state(key_press_state_t *key_state);

/**
 * @brief : 按键任务：周期性扫描按键并将结果发送到队列
 *
 * @param[in] : Parameters FreeRTOS 任务参数（未使用）
 *
 * @return : 无
 *
 * */
void keyTask(void *Parameters);
//********************************************Declaring*********************//

#endif
