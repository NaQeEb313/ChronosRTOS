/******************************************************************************
 * File        : scheduler.h
 * Project     : ChronosRTOS
 * Module      : Scheduler
 * Group       :
 * Description : Scheduler API and scheduling definitions.
 ******************************************************************************/

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

void Scheduler_Init(void) ;

TCB *Scheduler_Select_Next(void) ;

// void Scheduler_Start(void) ;

// void Scheduler_Tick(void) ;

#endif /* SCHEDULER_H */