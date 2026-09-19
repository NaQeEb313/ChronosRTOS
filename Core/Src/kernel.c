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

void Kernel_Init(void) {
  Mem_Init();
  Task_Init();
  Scheduler_Init();
}

void Kernel_Start(void) {
  TCB *selected_task;

  selected_task = Scheduler_Select_Next();
  Scheduler_Set_Current(selected_task);
  Task_Set_Current(selected_task);
}

void Kernel_Tick(void) {}
