/******************************************************************************
 * File        : memory.h
 * Project     : ChronosRTOS
 * Module      : Memory
 * Group       :
 * Description : Memory management API.
 ******************************************************************************/

#ifndef MEMORY_H
#define MEMORY_H

#include "config.h"
#include "error.h"
#include <stdint.h>

Error Mem_Init(void);
/*
Error Mem_Slot_Alloc(int8_t *out_slot);
Error Mem_Slot_Release(int8_t *slot);
uint8_t Mem_Slots_Used(void);
uint8_t Mem_Slots_Free(void);
*/

uint32_t *Mem_Stack_Base(int8_t *slot);
uint32_t Mem_Stack_Size_Words(void);

#endif /* MEMORY_H */