#include <stdio.h>
#include <stdlib.h>

#include "kernel.h"
#include "task.h"

/*
 * Host versions of the demo tasks.
 *
 * The actual RTOS task management, scheduler,
 * dispatcher and termination logic are unchanged.
 */

void TASK_A(void)
{
    printf("[Task] TID=1 RUNNING\n");
    printf("[Task] TID=1 FINISHED\n");
}

void TASK_B(void)
{
    printf("[Task] TID=2 RUNNING\n");
    printf("[Task] TID=2 FINISHED\n");
}

void TASK_C(void)
{
    printf("[Task] TID=3 RUNNING\n");
    printf("[Task] TID=3 FINISHED\n");
}

void TASK_D(void)
{
    printf("[Task] TID=4 RUNNING\n");
    printf("[Task] TID=4 FINISHED\n");
}

void TASK_E(void)
{
    printf("[Task] TID=5 RUNNING\n");
    printf("[Task] TID=5 FINISHED\n");
}


/*
 * Host version of Idle_Function.
 *
 * The real STM32 project can keep its normal
 * Idle implementation. This exists only so
 * the host build does not require hardware code.
 */
void Idle_Function(void)
{
    static int idle_count = 0;

    printf("Idle Task: RUNNING\n");

    idle_count++;

    if (idle_count >= 10)
    {
        printf("Host demonstration complete.\n");
        exit(0);
    }
}


int main(void)
{
    printf("====================================\n");
    printf("      ChronosRTOS HOST DEMO\n");
    printf("====================================\n\n");

    printf("Kernel_Init()\n");

    /*
     * ACTUAL ChronosRTOS kernel initialization.
     *
     * This calls:
     *   Mem_Init()
     *   Task_Init()
     *   Scheduler_Init()
     */
    Kernel_Init();

    printf("Kernel initialized.\n\n");


    printf("Creating tasks...\n");

    /*
     * ACTUAL ChronosRTOS Task_Create().
     *
     * Same priorities as the STM32 demo.
 
    */
Task_Create(TASK_A, 1);
printf("[Task] Created TID=1 | Priority=1\n");

Task_Create(TASK_B, 5);
printf("[Task] Created TID=2 | Priority=5\n");

Task_Create(TASK_C, 3);
printf("[Task] Created TID=3 | Priority=3\n");

Task_Create(TASK_D, 2);
printf("[Task] Created TID=4 | Priority=2\n");

Task_Create(TASK_E, 4);
printf("[Task] Created TID=5 | Priority=4\n");
    printf("5 tasks created.\n\n");


    printf("Starting Kernel...\n");
    printf("------------------------------------\n");

    /*
     * ACTUAL ChronosRTOS Kernel_Start().
     *
     * This never returns in the real RTOS.
     */
    Kernel_Start();

    return 0;
}
