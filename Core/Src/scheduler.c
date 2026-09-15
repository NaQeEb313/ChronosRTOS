/******************************************************************************
 * File        : scheduler.c
 * Project     : ChronosRTOS
 * Module      : Scheduler
 * Group       :
 * Description : Round-robin scheduler implementation.
 ******************************************************************************/


#include "task.h"
#include "scheduler.h"
#include <stddef.h>

TCB *current_task = NULL ;
TCB *next_task = NULL ;
TCB *idle_task = NULL ;


void Scheduler_Init(void) {
    current_task = Task_Get_Current() ;
    idle_task = Task_Get_Idle() ;
} ;

TCB *Scheduler_Select_Next(void) {
    TCB* start = NULL ;
    TCB* node = NULL ;
    TCB *highest = NULL ;

    start = current_task ;
    node = start->next ;
    highest = start->next ;

    do {
        if((highest->task_priority) < (node->task_priority)) {
            highest = node ;
            next_task = highest ;
            node = node->next ;
            continue ;
        }

        if (node == idle_task && start == idle_task) {
            highest = idle_task ;
            next_task = highest ;
            return next_task ;
        }
        node = node->next ;
    } while(node != start) ;
    
    if(next_task == NULL) {
        next_task = highest ;
    }
    return next_task ;
} ;