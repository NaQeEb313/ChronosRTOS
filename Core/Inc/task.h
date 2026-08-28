/******************************************************************************
 * File        : task.h
 * Project     : ChronosRTOS
 * Module      : Task
 * Group       :
 * Description : Task management API and Task Control Block.
 ******************************************************************************/

#ifndef TASK_H
#define TASK_H
#include <stdint.h>

typedef enum TaskState
{
    TASK_NEW,
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_TERMINATED
} TaskState;

typedef void (*TaskFunction)(void);

typedef struct TCB
{
    uint32_t tid;               // Task ID
    uint32_t pc;                // Program counter
    TaskState task_state;       // state of the task
    uint8_t task_priority;      // priority of task
    uint32_t *sp;               // stack pointer
    uint32_t cpu_ticks;         // Total CPU execution ticks used by the task
    uint32_t run_count;         // Number of times the task has been scheduled
    TaskFunction task_function; // pointer for demo task function using typedef
} TCB;

void Task_Create();
void Task_Init(void);
void Task_Terminate();

#endif /* TASK_H */
