/******************************************************************************
 * File        : error.h
 * Project     : ChronosRTOS
 * Module      : Error
 * Group       :
 * Description : Error codes and status definitions.
 ******************************************************************************/

#ifndef ERROR_H
#define ERROR_H

#include "types.h"

typedef enum Error {
    ERR_OK                      = 0,
    ERR_MEM_EXHAUSTED           = 1,
    ERR_MEM_INVALID_SLOT        = 2,
    ERR_MEM_SLOT_NOT_ALLOCATED  = 3,
    ERR_INVALID_PARAMETER       = 4,
    ERR_NOT_INITIALIZED         = 5,
    ERR_TASK_LIMIT_REACHED      = 6,
    ERR_SCHEDULER_ERROR         = 7,
} Error;

#endif /* ERROR_H */