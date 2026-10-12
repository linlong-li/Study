/*****************************************************************************
 * @file     
 * @par_dependencies
 * @author
 * @brief    
 * @version  
 * @date
 * @note 
 ****************************************************************************/
 
//********************************************includes**********************//

#include "bsp_key.h"

#include "stdio.h"
#include "stm32f4xx_hal.h"

//********************************************includes**********************//

 
//********************************************Defines***********************//
//********************************************Defines***********************//


//********************************************Declaring*********************//

key_state_t key_scan(void)
{
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0))
    {
        return KEY_PRESSED;
    }

    if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0))
    {
        return KEY_NOT_PRESSED;
    }

    return KEY_ERROR;
}

//********************************************Declaring*********************//
