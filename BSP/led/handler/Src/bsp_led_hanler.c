/******************************************************************************
 * @file bsp_led_hanler.c
 * 
 * @par dependencies 
 * - bsp_led_hanler.h
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

#include "bsp_led_hanler.h"
#include "freertos.h"
//******************************** Includes *********************************//

//******************************** Includes *********************************//

//******************************** Defines **********************************//
typedef struct
{
    led_index_t led_index;                  /* LED index.                    */
    uint8_t led_cycle_time;          /* LED cycle time in milliseconds.      */
    uint8_t led_blink_count;         /* LED blink count.                     */
    proportion_t led_proportion;          /* LED on/off proportion.          */ 
}led_handler_message_t;                    /* LED handler message structure. */

/**
 * @brief : led_handler_init
 *  
 * @param[in] : 
 * 
 * @return : 
 * 
 * */
led_handler_status_t led_handler_init(led_handler_t *const self)
{
    self->led_instance.led_instance_count = 0;
    for (uint8_t i = 0; i < LED_MAX; i++)
    {
        self->led_instance.led_instance[i] = NULL;
    }

    return LED_HANDLER_OK;

}

void led_handler_thread_func(void *arg)
{
     led_handler_t *self = (led_handler_t *)arg;
#ifdef DEBUG_ENABLE
    DEBUG_OUT("led_handler_thread_func: LED handler thread started.\r\n");
#endif

    //self->led_os_delay->pf_led_os_delay(100);
    for(;;)
    {
       
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_handler_thread_func: LED handler thread running.\r\n");
#endif
        self->led_os_delay->pf_led_os_delay(100);  /* Delay for 100 milliseconds */

    }
}   


/**
 * @brief : func
 *  
 * @param[in] : 
 * 
 * @return : 
 * 
 * */
led_handler_status_t led_handler_inst(led_handler_t     *const led_handler, 
#ifdef OS_SUPPORT
                                      led_os_delay_t    *const led_os_delay,
                                      led_os_thread_t   *const led_os_thread,
                                      led_os_queue_t    *const led_os_queue,
                                      led_os_critical_t *const led_os_critical,
#endif
                                      led_get_time_ms_t *const led_get_time_ms
                                      )
{
#ifdef DEBUG_ENABLE
    DEBUG_OUT("led_handler_inst: Instantiating LED handler...\r\n");
#endif
    /* Check for NULL pointers */
    if (NULL == led_handler || NULL == led_get_time_ms
#ifdef OS_SUPPORT
        || NULL == led_os_delay || NULL == led_os_thread 
        || NULL == led_os_queue //|| NULL == led_os_critical
#endif
        )
    {
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_handler_inst: Error - NULL pointer passed to led_handler_inst.\r\n");
#endif
        return LED_HANDLER_ERRORPARAMETER;  /* Return error status for NULL pointers */
    }

    /*Check led_handler if initialized */
    if (HANDLER_INSTED == led_handler->if_insted)
    {
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_handler_inst: Error - LED handler already instantiated.\r\n");
#endif
        return LED_HANDLER_ERRORPARAMETER;  /* Return error status for initialized   */
    }

    /* Assign the function pointers to the LED handler structure */
    led_handler->led_get_time_ms = led_get_time_ms;
#ifdef OS_SUPPORT
    led_handler->led_os_delay    = led_os_delay;   
    led_handler->led_os_thread   = led_os_thread;
    led_handler->led_os_queue    = led_os_queue;
    led_handler->led_os_critical = led_os_critical;
#endif

    /* Initialize the LED handler */
    if (LED_HANDLER_OK != led_handler_init(led_handler))
    {
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_handler_inst: Error - Failed to initialize LED handler.\r\n");
#endif
            led_handler->led_get_time_ms = NULL;
#ifdef OS_SUPPORT
            led_handler->led_os_delay    = NULL;   
            led_handler->led_os_thread   = NULL;
            led_handler->led_os_queue    = NULL;
            led_handler->led_os_critical = NULL;
#endif
        return LED_HANDLER_ERROR;  /* Return error status for initialization failure */
    }

#ifdef OS_SUPPORT
    /*Create Thread And Queue*/
    led_handler_status_t ret = LED_HANDLER_OK;
    ret = led_handler->led_os_thread->led_create_thread->pf_led_create_thread(
        led_handler_thread_func,
        "led_handler_thread",
        256,
        (void *)led_handler,
        0,
        &led_handler->led_thread_handle
    );

    if (LED_HANDLER_OK != ret)
    {
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_handler_inst: Error - Failed to create LED handler thread.\r\n");            
#endif
        return ret;  /* Return error status for thread creation failure */
    }

    ret = led_handler->led_os_queue->led_create_queue->pf_led_create_queue(
        (void **)&led_handler->led_queue_handle, 
        10, 
        sizeof(led_handler_message_t)
    );

    if (LED_HANDLER_OK != ret)
    {
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_handler_inst: Error - Failed to create LED handler queue.\r\n");
#endif
        led_handler->led_os_thread->led_delete_thread->pf_led_delete_thread(led_handler->led_thread_handle);
        return ret;  /* Return error status for queue creation failure */
    }
#endif

    /* Mark the LED handler as instantiated */
    led_handler->if_insted = HANDLER_INSTED;

#ifdef DEBUG_ENABLE
    DEBUG_OUT("led_handler_inst: LED handler instantiated successfully.\r\n");
#endif

    return LED_HANDLER_OK;  /* Return success status */
}
//******************************** Defines **********************************//

//******************************** Declaring ********************************//

//******************************** Declaring ********************************//
