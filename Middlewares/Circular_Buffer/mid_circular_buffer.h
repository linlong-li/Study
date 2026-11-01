/******************************************************************************
 * @file mid_circular_buffer.h
 *
 * @par dependencies
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

#ifndef __MID_CIRCULAR_BUFFER_H__
#define __MID_CIRCULAR_BUFFER_H__

//******************************** Includes *********************************//
#include <stdint.h>
//******************************** Includes *********************************//

//******************************** Defines **********************************//
typedef struct
{
    uint8_t *buffer;
    int rd_index;
    int wr_index;
    int buffer_len;
} circular_buffer_t;

typedef enum
{
    BUFFER_ERROR = 0,
    BUFFER_FULL = 1,
    BUFFER_EMPTY = 2,
    BUFFER_NOT_FULL = 3,
    BUFFER_NOT_EMPTY = 4,
    BUFFER_SUCCESS = 5,
    BUFFER_FAILED = 6,
} circular_buffer_status_t;

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
circular_buffer_status_t cicular_buffer_init(circular_buffer_t *circular_buffer, uint8_t *p_buffer, int len);

/**
 * @brief : write_circular_buffer
 *
 * @param[in] :
 *
 * @return :
 *
 * */
circular_buffer_status_t write_circular_buffer(circular_buffer_t *circular_buffer, uint8_t data);

/**
 * @brief : read_circular_buffer
 *
 * @param[in] :
 *
 * @return :
 *
 * */
circular_buffer_status_t read_circular_buffer(circular_buffer_t *circular_buffer,uint8_t *data);

/**
 * @brief : if_circular_buffer_empty
 *
 * @param[in] :
 *
 * @return :
 *
 * */
circular_buffer_status_t if_circular_buffer_empty(circular_buffer_t *circular_buffer);

/**
 * @brief : if_circular_buffer_full
 *
 * @param[in] :
 *
 * @return :
 *
 * */
circular_buffer_status_t if_circular_buffer_full(circular_buffer_t *circular_buffer);
//******************************** Declaring ********************************//

#endif // End of __MID_CIRCULAR_BUFFER_H__
