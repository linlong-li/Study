/******************************************************************************
 * @file bsp_led_hanler.h
 * 
 * @par dependencies 
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

#ifndef __BSP_LED_HANLER_H__
#define __BSP_LED_HANLER_H__

//******************************** Includes *********************************//
/*********************** C **********************/
#include <stdint.h>
#include <stdio.h>
/********************** MCU *********************/
/* CPU */

/* Core */

/* OS */

/* BSP */
#include "bsp_led_driver.h"
//******************************** Includes *********************************//

//******************************** Defines **********************************//
#define HANDLER_INSTED 1
#define HANDLER_NOT_INSTED 0

#define DEBUG_ENABLE 
#define DEBUG_OUT printf

#define OS_SUPPORT 

typedef enum
{
	LED_HANDLER_OK             = 0,      /* LED Operation completed successfully.    */
	LED_HANDLER_ERROR          = 1,      /* LED Run-time error without case matched  */
	LED_HANDLER_ERRORTIMEOUT   = 2,      /* LED Operation failed with timeout        */
	LED_HANDLER_ERRORRESOURCE  = 3,      /* LED Resource not available.              */
	LED_HANDLER_ERRORPARAMETER = 4,      /* LED Parameter error.                     */
	LED_HANDLER_ERRORNOMEMORY  = 5,      /* LED Out of memory.                       */
	LED_HANDLER_ERRORISR       = 6,      /* LED Not allowed in ISR context           */
	LED_HANDLER_RESERVED       = 0xFF,   /* LED Reserved                             */
} led_handler_status_t;

typedef struct led_handler led_handler_t;             /* LED driver structure. */
/*********************************Interface***********************************/
/* Internal */
typedef struct
{
    led_handler_status_t (*pf_led_get_time_ms)(void);  
}led_get_time_ms_t;                 /* Get the current time in milliseconds. */


#ifdef OS_SUPPORT
typedef struct
{
    led_handler_status_t (*pf_led_os_delay)(const uint32_t ms);
}led_os_delay_t;                                  /* LED OS delay interface. */

//thread
typedef struct
{
    led_handler_status_t (*pf_led_create_thread)(                void * const                   task_code,/* Defined. Internal*/
                    const char * const             task_name,/* Defined. external*/
                    const uint32_t               stack_depth,/* Defined. external*/
                    void * const                  parameters,/* Defined. Internal*/
                    uint32_t                        priority,/* Defined. external*/
                    void ** const               task_handler /* Defined. Internal*/);
}led_create_thread_t;                             /* LED create thread interface. */

typedef struct
{
    led_handler_status_t (*pf_led_delete_thread)(void *thread_handle);
}led_delete_thread_t;                             /* LED delete thread interface. */

typedef struct
{
    led_create_thread_t *led_create_thread;       /* LED create thread interface. */
    led_delete_thread_t *led_delete_thread;       /* LED delete thread interface. */
}led_os_thread_t;                                     /* LED thread interface. */

//queue
typedef struct
{
    led_handler_status_t (*pf_led_create_queue)(void **queue_handle, 
                                                uint32_t queue_length, 
                                                uint32_t item_size);
}led_create_queue_t;                              /* LED create queue interface. */

typedef struct
{
    led_handler_status_t (*pf_led_delete_queue)(void *queue_handle);
}led_delete_queue_t;                              /* LED delete queue interface. */

typedef struct
{
    led_handler_status_t (*pf_led_send_queue)(void *queue_handle, 
                                              const void *item, 
                                              uint32_t timeout);
}led_send_queue_t;                                /* LED send queue interface. */

typedef struct
{
    led_handler_status_t (*pf_led_receive_queue)(void *queue_handle, 
                                                 void *item, 
                                                 uint32_t timeout);
}led_receive_queue_t;                             /* LED receive queue interface. */

typedef struct
{
    led_create_queue_t  *led_create_queue;        /* LED create queue interface. */
    led_delete_queue_t  *led_delete_queue;        /* LED delete queue interface. */
    led_send_queue_t    *led_send_queue;          /* LED send queue interface.   */
    led_receive_queue_t *led_receive_queue;       /* LED receive queue interface.*/ 
}led_os_queue_t;                                     /* LED queue interface. */


//cratical
typedef struct
{
    led_handler_status_t (*pf_led_enter_critical)(void);
}led_enter_critical_t;                             /* LED enter critical interface. */

typedef struct
{
    led_handler_status_t (*pf_led_exit_critical)(void);
}led_exit_critical_t;                              /* LED exit critical interface. */

typedef struct
{
    led_enter_critical_t *led_enter_critical;     /* LED enter critical interface. */
    led_exit_critical_t  *led_exit_critical;      /* LED exit critical interface.  */
}led_os_critical_t;                                  /* LED critical interface. */

#endif

typedef enum
{
    LED_1 = 0,      /* LED 1. */
    LED_2,          /* LED 2. */
    LED_3,          /* LED 3. */
    LED_4,          /* LED 4. */
    LED_MAX,        /* LED max. */
}led_index_t;

typedef struct
{
    led_driver_t* led_instance[LED_MAX];             /* LED driver instance. */
    uint8_t led_instance_count;                /* LED driver instance count. */
}led_handler_instance_t;

/* External */
typedef led_handler_status_t (*pf_led_control)(led_handler_t *const self);  
                                                   /* LED control interface. */
/*********************************Interface***********************************/

struct led_handler
{
    /*********************************if-insted*******************************/
    uint8_t if_insted;                  /* LED if-insted flag.               */

    /*********************************Features********************************/

    /*********************************Features********************************/

    /*********************************Item************************************/
    led_handler_instance_t led_instance;              /* LED driver instance.*/
    void *led_queue_handle;                          /* LED queue handle.   */
    void *led_thread_handle;                         /* LED thread handle.  */
    /*********************************Item************************************/

    /*********************************Interface*******************************/
    /* Internal */
    led_get_time_ms_t *led_get_time_ms;       /* Get the current time in ms. */
#ifdef OS_SUPPORT    
    led_os_delay_t    *led_os_delay;          /* LED OS delay interface.     */
    led_os_thread_t   *led_os_thread;         /* LED OS thread interface.    */
    led_os_queue_t    *led_os_queue;          /* LED OS queue interface.     */
    led_os_critical_t *led_os_critical;       /* LED OS critical interface.  */
#endif

    /* External */
    pf_led_control    led_control;            /* LED control interface.      */
    /*********************************Interface*******************************/
};
//******************************** Defines **********************************//

//******************************** Declaring ********************************//
/**
 * @brief : func
 *  
 * @param[in] : 
 * 
 * @return : 
 * 
 * */
led_handler_status_t led_handler_inst(led_handler_t     *const self, 
#ifdef OS_SUPPORT
                                      led_os_delay_t    *const led_os_delay,
                                      led_os_thread_t   *const led_os_thread,
                                      led_os_queue_t    *const led_os_queue,
                                      led_os_critical_t *const led_os_critical,
#endif
                                      led_get_time_ms_t *const led_get_time_ms
                                      );          /* LED handler  interface. */
//******************************** Declaring ********************************//

#endif // End of __BSP_LED_HANLER_H__
