/******************************************************************************
 * @file uart_prase_task.c
 *
 * @par dependencies
 * - uart_prase_task.h
 *
 * @author Author | R&D Dept. | EternalChip
 *
 * @brief Brief description of this module.
 *
 * @version V1.0 2026-11-01
 *
 * @note 1 tab == 4 spaces!
 *
 *****************************************************************************/

#include "uart_parse_task.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <elog.h>
#include "stm32f4xx_hal.h"
#include "bsp_uart_driver.h"
#include "mid_circular_buffer.h"
//******************************** Includes *********************************//

//******************************** Includes *********************************//

//******************************** Defines **********************************//
QueueHandle_t queue_rec_A;
extern UART_HandleTypeDef huart1;

circular_buffer_t *p_circular_buffer = NULL;
static int parse_state = 0;
static uint8_t temp_buffer[2] = {0, 0};

#define FRAM_HEAD 0xfe
#define FRAM_END 0xff
//******************************** Defines **********************************//

//******************************** Declaring ********************************//

/* USER CODE BEGIN Header_uart_recv_A_func */
/**
 * @brief Function implementing the Task_uart_recv_ thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_uart_recv_A_func */
void uart_recv_A_func(void *argument)
{
    /* USER CODE BEGIN uart_recv_A_func */
    queue_rec_A = xQueueCreate(1, 4);
    if (NULL == queue_rec_A)
    {
        log_w("queue create failed");
    }
    else
    {
        log_i("queue create succesfully");
    }

    p_circular_buffer = get_circular_buffer();
    if (NULL == p_circular_buffer)
    {
        log_w("circular buffer not ready");
        return; // 或等待驱动初始化完成的信号
    }

    /* Infinite loop */
    for (;;)
    {
        BaseType_t ret_QueueReceive;
        uint32_t recv = 0;
        ret_QueueReceive = xQueueReceive(queue_rec_A, &recv, portMAX_DELAY);
        if (pdPASS != ret_QueueReceive)
        {
            log_w("xQueueReceive queue_rec_A failed");
            return;
        }

        while (BUFFER_EMPTY != if_circular_buffer_empty(p_circular_buffer))
        {
            circular_buffer_status_t ret_read_circular_buffer;
            uint8_t buffer = 0;
            ret_read_circular_buffer = read_circular_buffer(p_circular_buffer, &buffer);
            if (BUFFER_SUCCESS != ret_read_circular_buffer)
            {
                log_w("ret_read_circular_buffer failed");
                return;
            }

            switch (parse_state)
            {
            case 0:
                if (FRAM_HEAD == buffer)
                {
                    parse_state = 1;
                    log_i("parse start");
                }
                break;

            case 1:
                if (FRAM_END == buffer)
                {
                    parse_state = 2;
                    log_i("parse end");

                    if (temp_buffer[0] == (temp_buffer[1] + temp_buffer[1]))
                    {
                        log_i("parse successfully");
                    }
                    else
                    {
                        log_w("parse failed");
                    }
                    temp_buffer[0] = 0;
                    temp_buffer[1] = 0;

                    parse_state = 0;
                }
                else
                {
                    temp_buffer[0] += buffer;
                    temp_buffer[1] = buffer;
                    log_i("data is %x",buffer);
                }
                break;

            default:
                break;
            }
        }

        // vTaskDelay(pdMS_TO_TICKS(10));
    }
    /* USER CODE END uart_recv_A_func */
}

//******************************** Declaring ********************************//
