#include "demo_tasks.h"
#include "uart.h"
#include "gpio.h"
#include "stm32f1xx_hal.h"

void TASK_A(void)
{
    USART_SendString(USART1, "Task A: RUNNING\r\n");

    GPIO_ON();
    HAL_Delay(500);
    GPIO_OFF();

    HAL_Delay(5000);

    USART_SendString(USART1, "Task A: FINISHED\r\n");
}

void TASK_B(void)
{
    USART_SendString(USART1, "Task B: RUNNING\r\n");

    GPIO_ON();
    HAL_Delay(500);
    GPIO_OFF();
    HAL_Delay(500);

    GPIO_ON();
    HAL_Delay(500);
    GPIO_OFF();

    HAL_Delay(5000);

    USART_SendString(USART1, "Task B: FINISHED\r\n");
}

void TASK_C(void)
{
    USART_SendString(USART1, "Task C: RUNNING\r\n");

    for (int i = 0; i < 3; i++)
    {
        GPIO_ON();
        HAL_Delay(500);
        GPIO_OFF();
        HAL_Delay(500);
    }

    HAL_Delay(5000);

    USART_SendString(USART1, "Task C: FINISHED\r\n");
}

void TASK_D(void)
{
    USART_SendString(USART1, "Task D: RUNNING\r\n");

    for (int i = 0; i < 4; i++)
    {
        GPIO_ON();
        HAL_Delay(500);
        GPIO_OFF();
        HAL_Delay(500);
    }

    HAL_Delay(5000);

    USART_SendString(USART1, "Task D: FINISHED\r\n");
}

void TASK_E(void)
{
    USART_SendString(USART1, "Task E: RUNNING\r\n");

    for (int i = 0; i < 5; i++)
    {
        GPIO_ON();
        HAL_Delay(500);
        GPIO_OFF();
        HAL_Delay(500);
    }

    HAL_Delay(5000);

    USART_SendString(USART1, "Task E: FINISHED\r\n");
}