/******************************************************************************
 * @file bsp_uart_driver.c
 *
 * @par dependencies
 * - bsp_uart_driver.h
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

#include "bsp_uart_driver.h"

//******************************** Includes *********************************//
#include "stm32f4xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <elog.h>

//******************************** Includes *********************************//

//******************************** Defines **********************************//
extern QueueHandle_t queue_rec_A;
extern UART_HandleTypeDef huart1;

static int ringbuffer_len = 20;
// static uint8_t *ringbuffer = NULL;
static circular_buffer_t *circular_buffer = NULL;
uint8_t *ringbuffer = NULL;

QueueHandle_t queue_it2func;

#define RECV_MAX_LEN 20
//******************************** Defines **********************************//

//******************************** Declaring ********************************//
/* USER CODE BEGIN Header_uart_driv_func */
/**
 * @brief Function implementing the Task_uart_driv thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_uart_driv_func */
void uart_driv_func(void *argument)
{
    /* USER CODE BEGIN uart_driv_func */
    circular_buffer = pvPortMalloc(sizeof(circular_buffer_t));
    if (NULL == circular_buffer)
    {
        log_w("pvPortMalloc(sizeof(circular_buffer_t)) failed");
    }

    ringbuffer = pvPortMalloc(ringbuffer_len * sizeof(uint8_t));
    if (NULL == ringbuffer)
    {
        log_w("pvPortMalloc(10*sizeof(uint8_t)) failed");
    }

    circular_buffer_status_t ret_cb = BUFFER_FAILED;
    ret_cb = cicular_buffer_init(circular_buffer,
                                 ringbuffer,
                                 ringbuffer_len);
    if (BUFFER_SUCCESS != ret_cb)
    {
        log_w("cicular_buffer_init failed");
    }

    queue_it2func = xQueueCreate(1, 4);
    if (NULL == queue_it2func)
    {
        log_w("xQueueCreate(1,4) failed");
    }

    HAL_StatusTypeDef ret;
    // ret = HAL_UART_Receive_IT(&huart1, &g_data, 1);
    // ret = HAL_UARTEx_ReceiveToIdle_DMA(&huart1,
    //                                    circular_buffer->buffer,
    //                                    RECV_MAX_LEN);
    ret = HAL_UARTEx_ReceiveToIdle_DMA(&huart1,
                                       circular_buffer->buffer,
                                       RECV_MAX_LEN);
    if (HAL_OK != ret)
    {
        log_w("HAL_UART_Receive_DMA failed");
    }
    /* Infinite loop */
    for (;;)
    {
        uint32_t recv = 0;
        xQueueReceive(queue_it2func, &recv, portMAX_DELAY);

        uint32_t send = 0;
        xQueueGenericSend(queue_rec_A, &send, 0, queueOVERWRITE);
    }
    /* USER CODE END uart_driv_func */
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    uint32_t send = 0;
    BaseType_t ret;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (RECV_MAX_LEN / 2 == Size)
    {
        circular_buffer->wr_index += (RECV_MAX_LEN / 2 - circular_buffer->wr_index % RECV_MAX_LEN);

        ret = xQueueGenericSendFromISR(queue_rec_A, &send, &xHigherPriorityTaskWoken, queueOVERWRITE);
        if (pdPASS != ret)
        {
            log_w("xQueueGenericSendFromISR failed");
        }
    }
    else if (RECV_MAX_LEN == Size)
    {
        circular_buffer->wr_index += (RECV_MAX_LEN - circular_buffer->wr_index % RECV_MAX_LEN);

        ret = xQueueGenericSendFromISR(queue_rec_A, &send, &xHigherPriorityTaskWoken, queueOVERWRITE);
        if (pdPASS != ret)
        {
            log_w("xQueueGenericSendFromISR failed");
        }
    }
    else
    {
        circular_buffer->wr_index += (Size - circular_buffer->wr_index % RECV_MAX_LEN);

        ret = xQueueGenericSendFromISR(queue_rec_A, &send, &xHigherPriorityTaskWoken, queueOVERWRITE);
        if (pdPASS != ret)
        {
            log_w("xQueueGenericSendFromISR failed");
        }
    }

    if (circular_buffer->wr_index > circular_buffer->buffer_len)
    {
        circular_buffer->wr_index = circular_buffer->wr_index % circular_buffer->buffer_len;
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

circular_buffer_t *get_circular_buffer(void)
{
    if (circular_buffer == NULL)
    {
        return NULL;
    }
    return circular_buffer; // 没初始化就返回 NULL，由调用方判断
}

//******************************** Declaring ********************************//
