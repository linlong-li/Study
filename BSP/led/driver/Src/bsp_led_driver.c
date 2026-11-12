/******************************************************************************
 * @file bsp_led_driver.c
 * 
 * @par dependencies 
 * - bsp_led_driver.h
 * 
 * @author Author | R&D Dept. | EternalChip
 * 
 * @brief Brief description of this module.
 * 
 * @version V1.0 2026-11-11
 *
 * @note 1 tab == 4 spaces!
 * 
 *****************************************************************************/

//******************************** Includes *********************************//
#include "bsp_led_driver.h"
/*********************** C **********************/
#include <Stddef.h>
/********************** MCU *********************/
/* CPU */

/* Core */

/* OS */

/* BSP */

//******************************** Includes *********************************//

//******************************** Defines **********************************//

/**
 * @brief : led_driver_init
 *  
 * @param[in] : 
 * 
 * @return : 
 * 
 * */
led_status_t led_driver_init(led_driver_t *const led_instance)
{
#ifdef DEBUG_ENABLE
    DEBUG_OUT("led_driver_init: Initializing LED driver...\r\n");
#endif
    /* Check for NULL pointer */
    if (NULL == led_instance)
    {
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_driver_init: Error - NULL pointer passed to led_driver_init.\r\n");
#endif  
        return LED_ERRORPARAMETER;  /* Return error status for NULL pointer */
    }

#ifdef DEBUG_ENABLE
    DEBUG_OUT("led_driver_init: LED driver initialized successfully.\r\n");
#endif

    led_instance->led_operation->pf_led_off();
    return LED_OK;  /* Return success status */
}

/**
 * @brief : func
 *  
 * @param[in] : 
 * 
 * @return : 
 * 
 * */
led_status_t led_driver_inst(led_driver_t      *const self, 
                             led_operation_t   *const led_operation                   
                            )
{
#ifdef DEBUG_ENABLE
    DEBUG_OUT("led_driver_inst: Instantiating LED driver...\r\n");
#endif
    /* Check for NULL pointers */
    if (NULL == self || NULL == led_operation)
    {
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_driver_inst: Error - NULL pointer passed to led_driver_inst.\r\n");
#endif  
        return LED_ERRORPARAMETER;  /* Return error status for NULL pointers */
    }

    /*Check led_driver if initialized */
    if (INSTED == self->if_insted)
    {
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_driver_inst: Error - LED driver already instantiated.\r\n");
#endif
        return LED_ERRORPARAMETER;  /* Return error status for initialized   */
    }

    /* Initialize the LED driver structure */
    self->led_cycle_time = 0;
    self->led_blink_count = 0;
    self->led_proportion = PROPORTIONN_x_x;

    /* Assign the function pointers to the LED driver structure */
    self->led_operation   = led_operation;

    /* Initialize LED driver */
    led_status_t ret = LED_OK;
    ret = led_driver_init(self);
    if (LED_OK != ret)
    {
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_driver_inst: Error - Failed to initialize LED driver.\r\n");
#endif

        self->led_operation   = NULL;

        return ret;  /* Return error status if initialization fails */
    }

    /* Mark the LED driver as instantiated */
    self->if_insted = INSTED;
#ifdef DEBUG_ENABLE
    DEBUG_OUT("led_driver_inst: LED driver instantiated successfully.\r\n");
#endif  

    /* Return success status */
    return LED_OK;
}

//******************************** Defines **********************************//

//******************************** Declaring ********************************//

//******************************** Declaring ********************************//
