#include "dispatcher.h"
#include "debug.h"

void Dispatcher_Run(TCB *task)
{
    if (task == NULL || task->task_function == NULL)
        return;

    Debug_Task_Running(task->tid);

    task->task_function();

    Debug_Task_Finished(task->tid);
}