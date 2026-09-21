/******************************************************************************
 * File        : debug.c
 * Project     : ChronosRTOS
 * Module      : Debug
 * Description : Diagnostic output implementation.
 ******************************************************************************/

#include "debug.h"

#ifdef CHRONOS_HOST

#include <stdio.h>

void Debug_Init(void)
{
}

void Debug_Print(const char *message)
{
    printf("%s", message);
}

void Debug_Task_Created(uint32_t tid, uint8_t priority)
{
    printf("[Task] Created TID=%lu | Priority=%u\n",
           (unsigned long)tid,
           priority);
}

void Debug_Task_Scheduled(uint32_t tid, uint8_t priority)
{
    printf("[Scheduler] Scheduled TID=%lu | Priority=%u\n",
           (unsigned long)tid,
           priority);
}

void Debug_Task_Running(uint32_t tid)
{
    printf("[Task] TID=%lu RUNNING\n",
           (unsigned long)tid);
}

void Debug_Task_Finished(uint32_t tid)
{
    printf("[Task] TID=%lu FINISHED\n",
           (unsigned long)tid);
}

void Debug_Task_Terminated(uint32_t tid)
{
    printf("[Task] TID=%lu -> TERMINATED\n",
           (unsigned long)tid);
}

void Debug_Task_Blocked(uint32_t tid)
{
    printf("[Task] TID=%lu -> BLOCKED\n",
           (unsigned long)tid);
}

void Debug_Task_Ready(uint32_t tid)
{
    printf("[Task] TID=%lu -> READY\n",
           (unsigned long)tid);
}

void Debug_Edge_Null_Task(const char *operation)
{
    printf("[Edge Case] %s rejected: NULL task\n",
           operation);
}

void Debug_Edge_Idle_Task(const char *operation)
{
    printf("[Edge Case] %s rejected: TCB0/Idle task cannot be used\n",
           operation);
}

void Debug_Edge_Not_Initialized(const char *operation)
{
    printf("[Edge Case] %s rejected: task system not initialized\n",
           operation);
}

void Debug_Edge_No_Tasks(const char *operation)
{
    printf("[Edge Case] %s rejected: no user tasks available\n",
           operation);
}

void Debug_Edge_Invalid_Resume(uint32_t tid)
{
    printf("[Edge Case] Resume rejected: TID=%lu is not BLOCKED\n",
           (unsigned long)tid);
}

void Debug_Edge_Invalid_Allocation(void)
{
    printf("[Edge Case] Task creation rejected: no free task slot\n");
}

void Debug_Scheduler_Null(void)
{
    printf("[Scheduler] No task selected\n");
}

#else

#include "uart.h"

void Debug_Init(void)
{
}

void Debug_Print(const char *message)
{
    USART_SendString(USART1, (char *)message);
}

static void Debug_Print_Number(uint32_t value)
{
    char buffer[12];
    int index = 0;

    if (value == 0)
    {
        buffer[index++] = '0';
    }
    else
    {
        char reverse[12];
        int reverse_index = 0;

        while (value > 0)
        {
            reverse[reverse_index++] =
                (char)('0' + (value % 10));

            value /= 10;
        }

        while (reverse_index > 0)
        {
            buffer[index++] = reverse[--reverse_index];
        }
    }

    buffer[index] = '\0';

    USART_SendString(USART1, buffer);
}

static void Debug_Print_TID_Priority(uint32_t tid,
                                     uint8_t priority)
{
    USART_SendString(USART1, "TID=");

    Debug_Print_Number(tid);

    USART_SendString(USART1, " | Priority=");

    Debug_Print_Number(priority);

    USART_SendString(USART1, "\r\n");
}

void Debug_Task_Created(uint32_t tid, uint8_t priority)
{
    USART_SendString(USART1, "[Task] Created ");

    Debug_Print_TID_Priority(tid, priority);
}

void Debug_Task_Scheduled(uint32_t tid, uint8_t priority)
{
    USART_SendString(USART1, "[Scheduler] Scheduled ");

    Debug_Print_TID_Priority(tid, priority);
}

void Debug_Task_Running(uint32_t tid)
{
    USART_SendString(USART1, "[Task] TID=");

    Debug_Print_Number(tid);

    USART_SendString(USART1, " RUNNING\r\n");
}

void Debug_Task_Finished(uint32_t tid)
{
    USART_SendString(USART1, "[Task] TID=");

    Debug_Print_Number(tid);

    USART_SendString(USART1, " FINISHED\r\n");
}

void Debug_Task_Terminated(uint32_t tid)
{
    USART_SendString(USART1, "[Task] TID=");

    Debug_Print_Number(tid);

    USART_SendString(USART1, " -> TERMINATED\r\n");
}

void Debug_Task_Blocked(uint32_t tid)
{
    USART_SendString(USART1, "[Task] TID=");

    Debug_Print_Number(tid);

    USART_SendString(USART1, " -> BLOCKED\r\n");
}

void Debug_Task_Ready(uint32_t tid)
{
    USART_SendString(USART1, "[Task] TID=");

    Debug_Print_Number(tid);

    USART_SendString(USART1, " -> READY\r\n");
}

void Debug_Edge_Null_Task(const char *operation)
{
    USART_SendString(USART1, "[Edge Case] ");

    USART_SendString(USART1, (char *)operation);

    USART_SendString(USART1,
                     " rejected: NULL task\r\n");
}

void Debug_Edge_Idle_Task(const char *operation)
{
    USART_SendString(USART1, "[Edge Case] ");

    USART_SendString(USART1, (char *)operation);

    USART_SendString(USART1,
                     " rejected: TCB0/Idle task cannot be used\r\n");
}

void Debug_Edge_Not_Initialized(const char *operation)
{
    USART_SendString(USART1, "[Edge Case] ");

    USART_SendString(USART1, (char *)operation);

    USART_SendString(USART1,
                     " rejected: task system not initialized\r\n");
}

void Debug_Edge_No_Tasks(const char *operation)
{
    USART_SendString(USART1, "[Edge Case] ");

    USART_SendString(USART1, (char *)operation);

    USART_SendString(USART1,
                     " rejected: no user tasks available\r\n");
}

void Debug_Edge_Invalid_Resume(uint32_t tid)
{
    USART_SendString(USART1,
                     "[Edge Case] Resume rejected: TID=");

    Debug_Print_Number(tid);

    USART_SendString(USART1,
                     " is not BLOCKED\r\n");
}

void Debug_Edge_Invalid_Allocation(void)
{
    USART_SendString(USART1,
                     "[Edge Case] Task creation rejected: "
                     "no free task slot\r\n");
}

void Debug_Scheduler_Null(void)
{
    USART_SendString(USART1,
                     "[Scheduler] No task selected\r\n");
}

#endif