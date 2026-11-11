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
led_status_t led_blink( led_driver_t * self )
{
    led_status_t ret = LED_OK;
    //1. check if the target has been instantiated
    if( NULL       == self               || 
        NOT_INSTED == self->if_insted      ) 
    {
        //2. if not instantiated, return error to caller 
        //TBD:3.option - mutex to upgrade low priority task to init target ASAP
#ifdef DEBUG
    DEBUG_OUT("LED_ERRORPARAMETER in led blink\r\n");
#endif  // DEBUG
        ret = LED_ERRORPARAMETER;
        return ret;
    }
    //2. analyze the features
    {
        //2.1. define the time value for saving the features
        uint8_t                    cycle_time_local;
        uint8_t                   blink_times_local;
        proportion_t                     proportion_local;
        uint32_t                     led_toggle_time;

        cycle_time_local  =  self->led_cycle_time;
        blink_times_local =  self->led_blink_count;
        proportion_local  =  self->led_proportion;

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
            ret = LED_ERRORPARAMETER;
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
                    self->led_operation->pf_led_on();
                }
                else
                {
                    //TBD:8.option - LED off
                    self->led_operation->pf_led_off();
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
static led_status_t led_control   (
                                led_driver_t * const       self, // target
                                uint8_t      cycle_time,//     Cycle_time[ms]
                                uint8_t      blink_times,//blink_times[times]
                                proportion_t  proportion//  proportion_on_off
                                )
{
    /*************0.Checing the target statues  **********/
    led_status_t ret = LED_OK;
    //1.check if the target has been instantiated
    if( NULL       == self               || 
        NOT_INSTED == self->if_insted      ) 
    {
        //2.if not instantiated, return error to caller 
        //TBD:3.option - mutex to upgrade low priority task to init target ASAP
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_control: Error - LED driver not instantiated.\r\n");
#endif
        ret = LED_ERRORPARAMETER;
        return ret;
    }

    /*************1.Checing the input parameters**********/
    //1.cycle time checking
    if( 
        !( 
           (cycle_time  < 10000 )                 &&
           (blink_times < 1000  )                 &&
           ((PROPORTIONN_1_3 <= proportion)       &&  
           (PROPORTIONN_1_1 >= proportion))        
        ) // end of !(
      )   // end of if(
    {
#ifdef DEBUG_ENABLE
        DEBUG_OUT("led_control: Error - Invalid input parameters.\r\n");
#endif
        ret = LED_ERRORPARAMETER;
        return ret;
    }

    /*************2.Adding the data in target*************/
    self->led_cycle_time    =  cycle_time;
    self->led_blink_count   = blink_times;
    self->led_proportion    =  proportion;

    /************3. run the operations of led*************/
    //1.call the fuction to blink
    ret = led_blink(self);

    return ret;
}


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
                             led_get_time_ms_t *const led_get_time_ms,
                             led_operation_t   *const led_operation,
#ifdef OS_SUPPORT
                             led_os_delay_t    *const led_os_delay
#endif                       
                            )
{
#ifdef DEBUG_ENABLE
    DEBUG_OUT("led_driver_inst: Instantiating LED driver...\r\n");
#endif
    /* Check for NULL pointers */
    if (NULL == self || NULL == led_get_time_ms || NULL == led_operation
#ifdef OS_SUPPORT
        || NULL == led_os_delay
#endif
        )
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
    self->led_get_time_ms = led_get_time_ms;
    self->led_operation   = led_operation;
#ifdef OS_SUPPORT
    self->led_os_delay    = led_os_delay;
#endif
    self->led_control     = led_control;

    /* Initialize LED driver */
    led_status_t ret = LED_OK;
    ret = led_driver_init(self);
    if (LED_OK != ret)
    {
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
