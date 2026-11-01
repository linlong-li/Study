/******************************************************************************
 * @file mid_circular_buffer.c
 *
 * @par dependencies
 * - mid_circular_buffer.h
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

#include "mid_circular_buffer.h"

//******************************** Includes *********************************//
#include <stddef.h>
//******************************** Includes *********************************//

//******************************** Defines **********************************//

//******************************** Defines **********************************//

//******************************** Declaring ********************************//

/**
 * @brief : cicular_buffer_init
 *
 * @param[in] :
 *
 * @return :
 *
 * */
circular_buffer_status_t cicular_buffer_init(circular_buffer_t *circular_buffer, uint8_t *p_buffer, int len)
{
    if (NULL == p_buffer || NULL == circular_buffer)
    {
        return BUFFER_ERROR;
    }

    circular_buffer->buffer = p_buffer;
    circular_buffer->buffer_len = len;
    circular_buffer->wr_index = 0;
    circular_buffer->rd_index = 0;

    return BUFFER_SUCCESS;
}

/**
 * @brief : if_circular_buffer_empty
 *
 * @param[in] :
 *
 * @return :
 *
 * */
circular_buffer_status_t if_circular_buffer_empty(circular_buffer_t *circular_buffer)
{
    if (NULL == circular_buffer)
    {
        return BUFFER_ERROR;
    }

    if (circular_buffer->wr_index == circular_buffer->rd_index)
    {
        return BUFFER_EMPTY;
    }
    else
    {
        return BUFFER_NOT_EMPTY;
    }
}

/**
 * @brief : if_circular_buffer_full
 *
 * @param[in] :
 *
 * @return :
 *
 * */
circular_buffer_status_t if_circular_buffer_full(circular_buffer_t *circular_buffer)
{
    if (NULL == circular_buffer)
    {
        return BUFFER_ERROR;
    }

    if ((circular_buffer->wr_index + 1) % circular_buffer->buffer_len == circular_buffer->rd_index)
    {
        return BUFFER_FULL;
    }
    else
    {
        return BUFFER_NOT_FULL;
    }
}

/**
 * @brief : write_circular_buffer
 *
 * @param[in] :
 *
 * @return :
 *
 * */
circular_buffer_status_t write_circular_buffer(circular_buffer_t *circular_buffer, uint8_t data)
{
    if (NULL == circular_buffer)
    {
        return BUFFER_ERROR;
    }

    if (BUFFER_FULL == if_circular_buffer_full(circular_buffer))
    {
        return BUFFER_FULL;
    }

    if (circular_buffer->buffer_len == circular_buffer->wr_index)
    {
        circular_buffer->wr_index = 0;
    }
    circular_buffer->buffer[circular_buffer->wr_index] = data;
    circular_buffer->wr_index++;

    return BUFFER_SUCCESS;
}

/**
 * @brief : read_circular_buffer
 *
 * @param[in] :
 *
 * @return :
 *
 * */
circular_buffer_status_t read_circular_buffer(circular_buffer_t *circular_buffer,uint8_t *data)
{
    if (NULL == circular_buffer)
    {
        return BUFFER_ERROR;
    }

    if (BUFFER_EMPTY == if_circular_buffer_empty(circular_buffer))
    {
        return BUFFER_EMPTY;
    }

    if(circular_buffer->buffer_len == circular_buffer->rd_index)
    {
        circular_buffer->rd_index = 0;
    }
    *data = circular_buffer->buffer[circular_buffer->rd_index];
    circular_buffer->rd_index++;

    return BUFFER_SUCCESS;
}
//******************************** Declaring ********************************//
