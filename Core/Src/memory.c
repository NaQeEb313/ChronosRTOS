/******************************************************************************
 * File        : memory.c
 * Project     : ChronosRTOS
 * Module      : Memory
 ******************************************************************************/

#include "memory.h"
#include <stddef.h>

#define MEM_FREE_SLOT_CAPACITY (MAX_TASKS - 1U)

typedef enum Slot_State {
  SLOT_NEVER_USED,
  SLOT_ALLOCATED,
  SLOT_FREE
} Slot_State;

typedef struct {
  int8_t top;
  int8_t arr[MEM_FREE_SLOT_CAPACITY];
} Free_Slot_Stack;

static Slot_State slot_state[MAX_TASKS];
static Free_Slot_Stack free_slots;
static int8_t next_new_slot;
static uint8_t used_count;

static uint32_t task_stacks[MAX_TASKS][TASK_STACK_SIZE_WORDS];

Error Mem_Init(void)
{
  free_slots.top = -1;
  next_new_slot = 1;
  used_count = 0;

  for (uint8_t i = 0; i < MAX_TASKS; i++) {
    slot_state[i] = SLOT_NEVER_USED;
  }

  slot_state[MEM_IDLE_SLOT] = SLOT_ALLOCATED;

  return ERR_OK;
}

Error Mem_Slot_Alloc(int8_t *out_slot)
{
  int8_t slot;

  if (free_slots.top >= 0) {
    slot = free_slots.arr[free_slots.top];
    free_slots.top--;
  }
  else if (next_new_slot < MAX_TASKS) {
    slot = next_new_slot;
    next_new_slot++;
  }
  else {
    return ERR_MEM_EXHAUSTED;
  }

  slot_state[slot] = SLOT_ALLOCATED;
  used_count++;
  *out_slot = slot;

  return ERR_OK;
}

Error Mem_Slot_Release(int8_t slot)
{
  if (slot <= MEM_IDLE_SLOT || slot >= MAX_TASKS) {
    return ERR_MEM_INVALID_SLOT;
  }

  if (slot_state[slot] != SLOT_ALLOCATED) {
    return ERR_MEM_SLOT_NOT_ALLOCATED;
  }

  slot_state[slot] = SLOT_FREE;

  free_slots.top++;
  free_slots.arr[free_slots.top] = slot;

  used_count--;

  return ERR_OK;
}

uint32_t *Mem_Stack_Base(int8_t slot)
{
  if (slot < 0 || slot >= MAX_TASKS) {
    return NULL;
  }

  return &task_stacks[slot][0];
}

uint32_t Mem_Stack_Size_Words(void)
{
  return TASK_STACK_SIZE_WORDS;
}

uint8_t Mem_Slots_Used(void)
{
  return used_count;
}

uint8_t Mem_Slots_Free(void)
{
  return (uint8_t)((MAX_TASKS - 1U) - used_count);
}