/******************************************************************************
 * File        : debug.h
 * Project     : ChronosRTOS
 * Module      : Debug
 * Description : Diagnostic output interface.
 ******************************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>

void Debug_Init(void);

void Debug_Print(const char *message);

void Debug_Task_Created(uint32_t tid, uint8_t priority);

void Debug_Task_Scheduled(uint32_t tid, uint8_t priority);

void Debug_Task_Running(uint32_t tid);

void Debug_Task_Finished(uint32_t tid);

void Debug_Task_Terminated(uint32_t tid);

void Debug_Task_Blocked(uint32_t tid);

void Debug_Task_Ready(uint32_t tid);

void Debug_Edge_Null_Task(const char *operation);

void Debug_Edge_Idle_Task(const char *operation);

void Debug_Edge_Not_Initialized(const char *operation);

void Debug_Edge_No_Tasks(const char *operation);

void Debug_Edge_Invalid_Resume(uint32_t tid);

void Debug_Edge_Invalid_Allocation(void);

void Debug_Scheduler_Null(void);

#endif /* DEBUG_H */