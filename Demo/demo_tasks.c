/******************************************************************************
 * File        : demo_tasks.c
 * Project     : ChronosRTOS
 * Module      : Demo Tasks
 * Group       :
 * Description : Demo task implementations.
 ******************************************************************************/
#include "demo_tasks.h"
#include "uart.h"

void TASK_A(void)
{
    USART_SendString(USART1, "Task A: RUNNING\r\n");
}

void TASK_B(void)
{
    USART_SendString(USART1, "Task B: RUNNING\r\n");
}

void TASK_C(void)
{
    USART_SendString(USART1, "Task C: RUNNING\r\n");
}

void TASK_D(void)
{
    USART_SendString(USART1, "Task D: RUNNING\r\n");
}

void TASK_E(void)
{
    USART_SendString(USART1, "Task E: RUNNING\r\n");
}