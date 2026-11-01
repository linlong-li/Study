/******************************************************************************
 * @file bsp_uart_driver.h
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

#ifndef __BSP_UART_DRIVER_H__
#define __BSP_UART_DRIVER_H__

//******************************** Includes *********************************//
#include "mid_circular_buffer.h"
//******************************** Includes *********************************//

//******************************** Defines **********************************//
void uart_driv_func(void *argument);
circular_buffer_t *get_circular_buffer(void);
//******************************** Defines **********************************//

//******************************** Declaring ********************************//

//******************************** Declaring ********************************//

#endif // End of __BSP_UART_DRIVER_H__
