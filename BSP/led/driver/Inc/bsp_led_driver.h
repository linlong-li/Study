/******************************************************************************
 * @file bsp_led_driver.h
 * 
 * @par dependencies:
 *                  stdint.h 
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

#ifndef __BSP_LED_DRIVER_H__
#define __BSP_LED_DRIVER_H__

//******************************** Includes *********************************//
/*********************** C **********************/
#include <stdint.h>
#include <stdio.h>
/********************** MCU *********************/
/* CPU */

/* Core */

/* OS */

/* BSP */
//******************************** Includes *********************************//

//******************************** Defines **********************************//
#define INSTED 1
#define NOT_INSTED 0

#define DEBUG_ENABLE 
#define DEBUG_OUT printf

#define OS_SUPPORT 

typedef enum
{
	LED_OK             = 0,      /* LED Operation completed successfully.    */
	LED_ERROR          = 1,      /* LED Run-time error without case matched  */
	LED_ERRORTIMEOUT   = 2,      /* LED Operation failed with timeout        */
	LED_ERRORRESOURCE  = 3,      /* LED Resource not available.              */
	LED_ERRORPARAMETER = 4,      /* LED Parameter error.                     */
	LED_ERRORNOMEMORY  = 5,      /* LED Out of memory.                       */
	LED_ERRORISR       = 6,      /* LED Not allowed in ISR context           */
	LED_RESERVED       = 0xFF,   /* LED Reserved                             */
} led_status_t;

typedef enum
{
    PROPORTIONN_1_3    = 0,      /* LED Operation completed successfully.    */
    PROPORTIONN_1_2    = 1,      /* LED Operation completed successfully.    */
    PROPORTIONN_1_1    = 2,      /* LED Operation completed successfully.    */
    PROPORTIONN_x_x    = 0xFF,   /* LED Operation completed successfully.    */
} proportion_t;

typedef struct led_driver led_driver_t;             /* LED driver structure. */
/*********************************Interface***********************************/
/* Internal */
typedef struct
{
    led_status_t (*pf_led_get_time_ms)(void);  
}led_get_time_ms_t;                 /* Get the current time in milliseconds. */

typedef struct
{
    led_status_t (*pf_led_on)(void);  
    led_status_t (*pf_led_off)(void);
}led_operation_t;                                /* LED operation interface. */


#ifdef OS_SUPPORT
typedef struct
{
    led_status_t (*pf_led_os_delay)(const uint32_t ms);
}led_os_delay_t;                                  /* LED OS delay interface. */
#endif

/* External */
typedef led_status_t (*pf_led_control)(led_driver_t *const self,
                                       const uint8_t       cycle_time,
                                       const uint8_t       blink_count, 
                                       const proportion_t  proportion);  
                                                   /* LED control interface. */

/*********************************Interface***********************************/

struct led_driver
{
    /*********************************if-insted*******************************/
    uint8_t if_insted;                  /* LED if-insted flag.               */
    /*********************************Features********************************/
    uint8_t led_cycle_time;          /* LED cycle time in milliseconds.      */
    uint8_t led_blink_count;         /* LED blink count.                     */
    proportion_t led_proportion;          /* LED on/off proportion.          */ 
    /*********************************Features********************************/

    /*********************************Interface*******************************/
    /* Internal */
    led_get_time_ms_t *led_get_time_ms;       /* Get the current time in ms. */
    led_operation_t   *led_operation;         /* LED operation interface.    */
#ifdef OS_SUPPORT    
    led_os_delay_t    *led_os_delay;          /* LED OS delay interface.     */
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
led_status_t led_driver_inst(led_driver_t      *const self, 
                             led_get_time_ms_t *const led_get_time_ms,
                             led_operation_t   *const led_operation,
#ifdef OS_SUPPORT
                             led_os_delay_t    *const led_os_delay
#endif                       
                            );                   /* LED instance interface. */
//******************************** Declaring ********************************//

#endif // End of __BSP_LED_DRIVER_H__
