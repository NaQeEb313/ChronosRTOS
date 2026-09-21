/******************************************************************************
 * File        : task.c
 * Project     : ChronosRTOS
 * Module      : Task
 * Group       :
 * Description : Task creation and task management.
 ******************************************************************************/

// TASK_RESUME CODE REMAINING

#include "task.h"
#include "config.h"
#include "idle.h"
#include "memory.h"
#include "debug.h"
#include <stdbool.h>
#include <stddef.h>

static int task_count = 0;

static TCB tcb_pool[MAX_TASKS];

static TCB *TCB0 = &tcb_pool[0];
static TCB *curr = NULL;
static TCB *prev = NULL;
static TCB *next = NULL;
static TCB *tail = NULL;
static bool init = 0;

typedef struct Free_Stack {

  int8_t top;
  int8_t arr[6];
} Free_Stack;

static Free_Stack free_slot;

void Free_Stack_Init(void)
{
  free_slot.top = -1;
}

void Free_Stack_Push(int8_t free)
{
  if (free_slot.top < 5) {
    free_slot.top++;
    free_slot.arr[free_slot.top] = free;
  } else {
    return;
  }
}

int8_t Free_Stack_Empty()
{
  if (free_slot.top == -1) {
    return 1;
  } else {
    return 0;
  }
}

int8_t Free_Stack_Top()
{
  if (free_slot.top >= 0)
    return free_slot.arr[free_slot.top];
  else
    return -1;
}

void Free_Stack_Pop(void)
{
  if (free_slot.top >= 0) {
    free_slot.top--;
  } else {
    return;
  }
}

void Task_Exit_Handler(void)
{
  Task_Terminate(curr);
}

void Task_Set_Current(TCB *task)
{
  curr = task;
}

void Task_Create(TaskFunction task_function, uint8_t priority)
{
  if (Free_Stack_Empty() || !init || task_function == NULL) {

    if (Free_Stack_Empty())
      Debug_Edge_Invalid_Allocation();

    if (!init)
      Debug_Edge_Not_Initialized("Task_Create");

    if (task_function == NULL)
      Debug_Edge_Null_Task("Task_Create");

    return;
  }
  else {

    task_count++;

    int8_t slot = Free_Stack_Top();

    TCB *tcb = &tcb_pool[slot];

    uint32_t *stack_base = Mem_Stack_Base(&slot);

    uint32_t *sp =
        stack_base + Mem_Stack_Size_Words();

    sp--;
    *sp = 0x01000000;

    sp--;
    *sp = (uint32_t)task_function;

    sp--;

    /* LR: return address used if the task function returns */
    *sp = (uint32_t)Task_Exit_Handler;

    --sp;
    *sp = 0x00000000;

    --sp;
    *sp = 0x00000000;

    --sp;
    *sp = 0x00000000;

    --sp;
    *sp = 0x00000000;

    --sp;
    *sp = 0x00000000;

    /* Software saved frame */

    --sp;
    *sp = 0x00000000;

    --sp;
    *sp = 0x00000000;

    --sp;
    *sp = 0x00000000;

    --sp;
    *sp = 0x00000000;

    --sp;
    *sp = 0x00000000;

    --sp;
    *sp = 0x00000000;

    --sp;
    *sp = 0x00000000;

    --sp;
    *sp = 0x00000000;

    tcb->task_priority = priority;
    tcb->task_function = task_function;
    tcb->tid = slot;
    tcb->sp = sp;
    tcb->task_state = TASK_READY;

    tail->next = tcb;
    tcb->next = TCB0;
    tail = tcb;

    Free_Stack_Pop();

    Debug_Task_Created(tcb->tid, tcb->task_priority);
  }
}

void Task_Init(void)
{
  init = 1;
  task_count = 0;

  TCB0->next = TCB0;

  prev = TCB0;
  curr = TCB0;
  next = TCB0;
  tail = TCB0;

  TCB0->task_priority = 0;
  TCB0->task_function = Idle_Function;
  TCB0->tid = 0;
  TCB0->task_state = TASK_READY;

  Free_Stack_Init();

  for (int8_t i = 5; i > 0; i--)
    Free_Stack_Push(i);
}

void Task_Terminate(TCB *temp)
{
  if (temp == NULL || temp == TCB0 || !init) {

    if (temp == NULL)
      Debug_Edge_Null_Task("Task_Terminate");

    if (temp == TCB0)
      Debug_Edge_Idle_Task("Task_Terminate");

    if (!init)
      Debug_Edge_Not_Initialized("Task_Terminate");

    return;
  }

  if (task_count > 0) {

    TCB *check = curr;
    TCB *walker = curr;
    int8_t count = 0;

    /* Find the node before temp */
    while (walker->next != temp) {

      if (walker == check && count != 0) {
        return;
      }

      walker = walker->next;
      count++;
    }

    Free_Stack_Push(temp->tid);

    prev = walker;
    next = temp->next;

    if (temp == tail) {
      tail = prev;
    }

    prev->next = next;

    if (temp == curr) {
      curr = next;
    }

    temp->task_state = TASK_TERMINATED;

    Debug_Task_Terminated(temp->tid);

    task_count--;

  }
  else {
    Debug_Edge_No_Tasks("Task_Terminate");
    return;
  }
}

void Task_Suspend(TCB *temp)
{
  if (temp == NULL || temp == TCB0 || !init) {

    if (temp == NULL)
      Debug_Edge_Null_Task("Task_Suspend");

    if (temp == TCB0)
      Debug_Edge_Idle_Task("Task_Suspend");

    if (!init)
      Debug_Edge_Not_Initialized("Task_Suspend");

    return;
  }

  if (task_count > 0) {

    TCB *check = curr;
    TCB *walker = curr;
    int8_t count = 0;

    /* Find the node before temp */
    while (walker->next != temp) {

      if (walker == check && count != 0) {
        return;
      }

      walker = walker->next;
      count++;
    }

    prev = walker;
    next = temp->next;

    if (temp == tail) {
      tail = prev;
    }

    prev->next = next;

    if (temp == curr) {
      curr = next;
    }

    temp->task_state = TASK_BLOCKED;

    Debug_Task_Blocked(temp->tid);

  }
  else {
    Debug_Edge_No_Tasks("Task_Suspend");
    return;
  }
}

void Task_Resume(TCB *temp)
{
  if (temp == NULL || temp == TCB0 || !init) {

    if (temp == NULL)
      Debug_Edge_Null_Task("Task_Resume");

    if (temp == TCB0)
      Debug_Edge_Idle_Task("Task_Resume");

    if (!init)
      Debug_Edge_Not_Initialized("Task_Resume");

    return;
  }

  if (temp->task_state != TASK_BLOCKED) {
    Debug_Edge_Invalid_Resume(temp->tid);
    return;
  }

  if (task_count > 0) {

    tail->next = temp;
    temp->next = TCB0;
    tail = temp;

    tail->task_state = TASK_READY;

    Debug_Task_Ready(temp->tid);
  }
  else {
    Debug_Edge_No_Tasks("Task_Resume");
    return;
  }
}

TCB *Task_Get_Current()
{
  return curr;
}

TCB *Task_Get_Idle()
{
  return TCB0;
}

bool Task_Is_Initialized(void)
{
  return init;
}