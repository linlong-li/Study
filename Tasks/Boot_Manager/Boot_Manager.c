#include "Boot_Manager.h"
#include "main.h"

uint32_t JumpAddress;
int32_t FileSize;
void JumpToApp(void)
{
    /* 检查栈顶地址是否合法 */
    uint32_t JumpAddress;
    pFunction Jump_To_Application;

    /* 检查栈顶地址是否合法 */
    if (((*(__IO uint32_t *)ApplicationAddress) & 0x2FFE0000) == 0x20000000)
    {
        /* 屏蔽所有中断，防止在跳转过程中，中断干扰出现异常 */
        __disable_irq();
        NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000);
        RCC_DeInit();
        /* 用户代码区第二个 字 为程序开始地址(复位地址) */
        JumpAddress = *(__IO uint32_t *)(ApplicationAddress + 4);

        /* Initialize user application's Stack Pointer */
        /* 初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址) */
        __set_MSP(*(__IO uint32_t *)ApplicationAddress);

        /* 类型转换 */
        Jump_To_Application = (pFunction)JumpAddress;

        /* 跳转到 APP */
        Jump_To_Application();
    }
}

int8_t BackToApp(void)
{
    int32_t j;
    uint32_t FlashDestination = ApplicationAddress;
    uint32_t BackSource = BackAppAddress;

    if (FileSize < 0 || FileSize > 18000 - 1)
    {
        return -1;
    }

    for (j = 0; j < FileSize; j += 4)
    {
        /* Program the data received into STM32F10x Flash */
        // FLASH_ProgramWord(FlashDestination, *(uint32_t*)RamSource);
        Flash_Write(FlashDestination, *(uint32_t *)BackSource);
        if (*(uint32_t *)FlashDestination != *(uint32_t *)BackSource)
        {
            return 0;
        }
        FlashDestination += 4;
        BackSource += 4;
    }

    return 1;
}
