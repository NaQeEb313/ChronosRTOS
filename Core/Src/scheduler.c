/******************************************************************************
 * File        : scheduler.c
 * Project     : ChronosRTOS
 * Module      : Scheduler
 * Group       :
 * Description : Priority-based scheduler — selects the highest-priority
 *runnable task.
 ******************************************************************************/

#include "scheduler.h"
#include "task.h"
#include <stddef.h>

TCB *current_task = NULL;
TCB *next_task = NULL;
TCB *idle_task = NULL;

void Scheduler_Init(void) {
  if (!Task_Is_Initialized())
    return;

  current_task = Task_Get_Current();
  idle_task = Task_Get_Idle();
}
void Scheduler_Set_Current(TCB *task) { current_task = task; }

TCB *Scheduler_Select_Next(void) {
  next_task = NULL;

  if (current_task == NULL || idle_task == NULL) {
    return NULL;
  }

  TCB *start = current_task;
  TCB *node = current_task->next;
  TCB *highest = current_task;

  do {
    if (node->task_priority > highest->task_priority) {
      highest = node;
    }

    node = node->next;

  } while (node != start);

  next_task = highest;

  return next_task;
}