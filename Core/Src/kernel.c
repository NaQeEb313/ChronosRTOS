/******************************************************************************
 * File        : kernel.c
 * Project     : ChronosRTOS
 * Module      : Kernel
 * Group       :
 * Description : Kernel initialization and startup.
 ******************************************************************************/

#include "kernel.h"
#include "memory.h"
#include "scheduler.h"
#include "task.h"
#include "dispatcher.h"
#include "debug.h"

void Kernel_Init(void)
{
  Debug_Print("[Kernel] Initializing memory...\n");

  Mem_Init();

  Debug_Print("[Kernel] Initializing task manager...\n");

  Task_Init();

  Debug_Print("[Kernel] Initializing scheduler...\n");

  Scheduler_Init();

  Debug_Print("[Kernel] Initialization complete.\n");
}

void Kernel_Start(void)
{
  TCB *selected_task;

  Debug_Print("[Kernel] Starting scheduler...\n");

  while(1)
  {
    selected_task = Scheduler_Select_Next();

    if (selected_task == NULL)
    {
      Debug_Scheduler_Null();
      continue;
    }

    Debug_Task_Scheduled(selected_task->tid,
                         selected_task->task_priority);

    Scheduler_Set_Current(selected_task);

    Task_Set_Current(selected_task);

    Dispatcher_Run(selected_task);

    if (selected_task != Task_Get_Idle())
    {
      Task_Terminate(selected_task);

      Scheduler_Set_Current(Task_Get_Current());
    }
  }
}

void Kernel_Tick(void)
{
}