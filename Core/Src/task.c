/******************************************************************************
 * File        : task.c
 * Project     : ChronosRTOS
 * Module      : Task
 * Group       :
 * Description : Task creation and task management.
 ******************************************************************************/

#define MAX_TASKS 6
#include "task.h"
#include "idle.h"
#include <stddef.h>

static int task_count = 0;

static TCB tcb_pool[MAX_TASKS];

static TCB *TCB0 = &tcb_pool[0];
static TCB *curr = NULL;
static TCB *prev = NULL;
static TCB *next = NULL;

typedef struct Free_Stack {

  int8_t top;
  int8_t arr[6];
} Free_Stack;

static Free_Stack free_slot;

void Free_Stack_Init(void) { free_slot.top = -1; }

void Free_Stack_Push(int8_t free) {

  if (free_slot.top < 5) {
    free_slot.top++;
    free_slot.arr[free_slot.top] = free;
  } else
    return;
}
int8_t Free_Stack_Empty() {
  if (free_slot.top == -1) {
    return 1;
  } else {
    return 0;
  }
}

int8_t Free_STack_Top() {
  if (free_slot.top >= 0)
    return free_slot.arr[free_slot.top];
  else
    return -1;
}

void Free_Stack_Pop(void) {
  if (free_slot.top >= 0) {
    free_slot.top--;
  } else
    return;
}

void Task_Create(TaskFunction task_function, uint8_t priority) {
  if (task_count >= MAX_TASKS - 1) {
    return;
  }
  task_count++;
  TCB *tcb = &tcb_pool[task_count];
  tcb->task_priority = priority;
  tcb->task_function = task_function;
  tcb->tid = task_count;
  tcb->task_state = TASK_READY;

  // logic
  curr->next = tcb;
  prev = curr;
  curr = tcb;
  curr->next = TCB0;
}

void Task_Init(void) {
  // initialise the task management
  task_count = 0;
  TCB0->next = TCB0;
  prev = TCB0;
  curr = TCB0;
  next = TCB0;

  TCB0->task_priority = 0;
  TCB0->task_function = Idle_Function;
  TCB0->tid = 0;
  TCB0->task_state = TASK_READY;

  // initialise the task
}

void Task_Terminate(TCB *temp) {
  if (temp == TCB0) {
    // it will destroy the entire list
    return;
  }

  if (task_count > 0) {
    while (curr->next != temp) {
      curr = curr->next;
    }
    prev = curr;
    curr = curr->next;
    next = curr->next;
    prev->next = next;
    temp->task_state = TASK_TERMINATED;
    task_count--;
    curr = prev->next;
    next = curr->next;
  }

  else
    return;
}
