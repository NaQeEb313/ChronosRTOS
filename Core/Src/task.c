/******************************************************************************
 * File        : task.c
 * Project     : ChronosRTOS
 * Module      : Task
 * Group       :
 * Description : Task creation and task management.
 ******************************************************************************/

// TASK_RESUME CODE REMAINING

#define MAX_TASKS 6
#include "task.h"
#include "idle.h"
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

int8_t Free_Stack_Top() {
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
  if (Free_Stack_Empty() || !init || task_function == NULL) {
    return;
  } else {
    task_count++;
    TCB *tcb = &tcb_pool[Free_Stack_Top()];

    tcb->task_priority = priority;
    tcb->task_function = task_function;
    tcb->tid = Free_Stack_Top();
    tcb->task_state = TASK_READY;

    // logic
    tail->next = tcb;
    tcb->next = TCB0;
    tail = tcb;
    curr = tail;
    Free_Stack_Pop();
  }
}

void Task_Init(void) {
  // initialise the task management
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

  // initialise the task
}

void Task_Terminate(TCB *temp) {
  if (temp == NULL || temp == TCB0 || !init) {
    return;
  }

  if (task_count > 0) {
    TCB *check;
    check = curr;
    int8_t count = 0;

    while (curr->next != temp) {
      if (curr == check && count != 0)
        return;
      curr = curr->next;
      count++;
    }

    Free_Stack_Push(temp->tid);

    prev = curr;
    curr = curr->next;
    next = curr->next;

    if (temp == tail)
      tail = prev;

    prev->next = next;

    temp->task_state = TASK_TERMINATED;
    task_count--;

    curr = prev->next;
    next = curr->next;
  } else {
    return;
  }
}

void Task_Suspend(TCB *temp) {
  if (temp == NULL || temp == TCB0 || !init) {
    return;
  }

  if (task_count > 0) {
    TCB *check;
    check = curr;
    int8_t count = 0;

    while (curr->next != temp) {
      if (curr == check && count != 0)
        return;
      curr = curr->next;
      count++;
    }

    prev = curr;
    curr = curr->next;
    next = curr->next;

    if (temp == tail)
      tail = prev;

    prev->next = next;

    temp->task_state = TASK_BLOCKED;

    curr = prev->next;
    next = curr->next;
  } else {
    return;
  }
}

void Task_Resume(TCB *temp) {
  if (temp == NULL || temp == TCB0 || !init) {
    return;
  }

  if (task_count > 0) {
    tail->next = temp ;
    temp->next = TCB0 ;
    tail = temp ;
    
    tail->task_state = TASK_READY ;
  }
  else {
    return;
  }
}

TCB *Task_Get_Current() {
  return curr ;
}

TCB *Task_Get_Idle() {
  return TCB0 ;
}