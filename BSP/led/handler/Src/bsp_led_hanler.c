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
    self->if_insted = HANDLER_NOT_INSTED;
    for (uint8_t i = 0; i < LED_MAX; i++)
    {
        self->led_instance.led_instance[i] = NULL;
    }

    return LED_HANDLER_OK;

}

/**
 * @brief perform the detailed behavior of  the bsp_led_driver_t target.
 * 
 * Steps:
 *  1, analyze the target paras..
 *  2, do the appropriate operations of led.
 *  
 * 
 * @param[in] self              : TBD:AI to add comments
 * 
 * @return led_status_t : The status of running
 * 
 * */
led_handler_status_t led_blink( led_handler_t * self ,
                                led_handler_message_t *led_message)
{
    led_handler_status_t ret = LED_HANDLER_OK;
    //1. check if the target has been instantiated
    if( NULL       == self               || 
        NOT_INSTED == self->if_insted    ||
        NULL       == led_message         ) 
    {
        //2. if not instantiated, return error to caller 
        //TBD:3.option - mutex to upgrade low priority task to init target ASAP
#ifdef DEBUG
    DEBUG_OUT("LED_ERRORPARAMETER in led blink\r\n");
#endif  // DEBUG
        ret = LED_HANDLER_ERRORPARAMETER;
        return ret;
    }
    //2. analyze the features
    {
        //2.1. define the time value for saving the features
        uint8_t                    cycle_time_local;
        uint8_t                   blink_times_local;
        proportion_t                     proportion_local;
        uint32_t                     led_toggle_time;

        cycle_time_local  =  led_message->led_cycle_time;
        blink_times_local =  led_message->led_blink_count;
        proportion_local  =  led_message->led_proportion;

        //2.2. calculate the time for each led to toggle
        if( PROPORTIONN_1_1     == proportion_local)
        {
            led_toggle_time = cycle_time_local / 2;
        }
        else if(PROPORTIONN_1_2 == proportion_local)
        {
            led_toggle_time = cycle_time_local / 3;
        }
        else if(PROPORTIONN_1_3 == proportion_local)
        {
            led_toggle_time = cycle_time_local / 4;
        }
        else 
        {
#ifdef DEBUG
    DEBUG_OUT("LED_ERRORPARAMETER in led blink\r\n");
#endif  // DEBUG
            ret = LED_HANDLER_ERRORPARAMETER;
            return ret;
        }

        //3. do the operation
        //TBD:4.option - use mutex to protect the critical section
        for(uint32_t i = 0; i < blink_times_local; i++)
        {
            //TBD:5.option - use mutex to protect the critical section
            for(uint32_t j = 0; j < cycle_time_local; j++)
            {
               self->led_os_delay->pf_led_os_delay(1); // Delay for 1 ms
                //TBD:6.option - use mutex to protect the critical section
                if( j < led_toggle_time )
                {
                    //TBD:7.option - LED on
                    self->led_instance.led_instance[led_message->led_index]->led_operation->pf_led_on();
                }
                else
                {
                    //TBD:8.option - LED off
                    self->led_instance.led_instance[led_message->led_index]->led_operation->pf_led_off();
                }
            }
        }

    }

    return ret;
}

/**
 * @brief Control the bsp_led_driver_t target.
 * 
 * Steps:
 *  1, set the internal values of target, and do the action.
 *  
 * 
 * @param[in] self              : TBD:AI to add comments
 * @param[in] Cycle_time        : TBD:AI to add comments
 * @param[in] blink_times       : TBD:AI to add comments
 * @param[in] proportion_on_off : TBD:AI to add comments
 * 
 * @return led_status_t : The status of running
 * 
 * */
static led_handler_status_t led_control  (led_handler_t *const self,
                                               led_index_t led_index,    
                                               uint8_t cycle_time,
                                                uint8_t blink_times,
                                                proportion_t proportion)
{
    /*************1.Checing the input parameters**********/
    led_handler_status_t ret = LED_HANDLER_OK;
    //1.check if the target has been instantiated
    if(     NULL       == self                    ||  
         !((cycle_time  < 10000 )                 &&
           (blink_times < 1000  )                 && 
           (PROPORTIONN_1_1 >= proportion))       ||
            NOT_INSTED == self->if_insted         ||
            (led_index  >= LED_MAX)              )
    {
        //2.if not instantiated, return error to caller 
        //TBD:3.option - mutex to upgrade low priority task to init target ASAP
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_control: Error - LED driver not instantiated.\r\n");
#endif
        ret = LED_HANDLER_ERRORPARAMETER;
        return ret;
    }

    /*************2.Adding the data in target*************/
    led_handler_message_t led_message;
    led_message.led_index      = led_index;
    led_message.led_cycle_time = cycle_time;
    led_message.led_blink_count= blink_times;
    led_message.led_proportion = proportion;

    /************3.Send Queue To Thread*************/

    ret = self->led_os_queue->led_send_queue->pf_led_send_queue(self->led_queue_handle, &led_message, 0);
    if (LED_HANDLER_OK != ret)
    {
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_control: Error - Failed to send message to LED handler queue.\r\n");
#endif
        return ret;  /* Return error status for queue send failure */
    }

    return ret;
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

        led_handler_status_t ret = LED_HANDLER_OK;
        led_handler_message_t led_message;
        ret = self->led_os_queue->led_receive_queue->pf_led_receive_queue(self->led_queue_handle, &led_message, portMAX_DELAY);
        if (LED_HANDLER_OK != ret)
        {
#ifdef DEBUG_ENABLE
            DEBUG_OUT("led_handler_thread_func: Error - Failed to receive message from LED handler queue.\r\n");
#endif
            continue;  /* Continue to the next iteration of the loop */
        }

        led_handler_status_t ret1 = LED_HANDLER_OK;
        ret1 = led_blink(self,&led_message);
        if(LED_HANDLER_OK != ret1)
        {
#ifdef DEBUG_ENABLE
            DEBUG_OUT("led_blink: Error - Failed to blink.\r\n");
#endif            
        }

        //self->led_os_delay->pf_led_os_delay(100);  /* Delay for 100 milliseconds */

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
    led_handler->led_control     = led_control;

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
        512,
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

/**
 * @brief : led_register
 *  
 * @param[in] : 
 * 
 * @return : 
 * 
 * */
led_handler_status_t led_register(led_handler_t *const led_handler, 
                                  led_driver_t  *const led_driver,
                                  led_index_t*           led_index
                                  )          /* LED register interface. */
{
    /* Check for NULL pointers */
    if (NULL == led_handler || NULL == led_driver)
    {
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_register: Error - NULL pointer passed to led_register.\r\n");
#endif
        return LED_HANDLER_ERRORPARAMETER;  /* Return error status for NULL pointers */
    }

    /* Check if the LED handler is instantiated */
    if (HANDLER_INSTED != led_handler->if_insted)
    {
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_register: Error - LED handler not instantiated.\r\n");
#endif
        return LED_HANDLER_ERRORPARAMETER;  /* Return error status for uninitialized handler */
    }

    /* Check if the LED driver is instantiated */
    if (INSTED != led_driver->if_insted)
    {
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_register: Error - LED driver not instantiated.\r\n");
#endif
        return LED_HANDLER_ERRORPARAMETER;  /* Return error status for uninitialized driver */
    }

    /* Check if the maximum number of LED instances has been reached */
    if (led_handler->led_instance.led_instance_count >= LED_MAX)
    {   
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_register: Error - Maximum number of LED instances reached.\r\n");
#endif
        return LED_HANDLER_ERRORPARAMETER;  /* Return error status for maximum instances reached */
    }

    *led_index = (led_index_t)led_handler->led_instance.led_instance_count;  /* Assign the current index to the output parameter */
    led_handler->led_instance.led_instance[led_handler->led_instance.led_instance_count] = led_driver;
    led_handler->led_instance.led_instance_count++;  /* Increment the instance count */
    return LED_HANDLER_OK;

}
//******************************** Defines **********************************//

//******************************** Declaring ********************************//

//******************************** Declaring ********************************//
