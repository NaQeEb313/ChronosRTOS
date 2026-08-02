#!/bin/bash

create_c() {
cat > "$1" << EOF
/******************************************************************************
 * File        : $(basename "$1")
 * Project     : ChronosRTOS
 * Module      : $2
 * Group       :
 * Description : $3
 ******************************************************************************/

EOF
}

create_h() {

GUARD=$(basename "$1" | tr '[:lower:].' '[:upper:]_')

cat > "$1" << EOF
/******************************************************************************
 * File        : $(basename "$1")
 * Project     : ChronosRTOS
 * Module      : $2
 * Group       :
 * Description : $3
 ******************************************************************************/

#ifndef ${GUARD}
#define ${GUARD}



#endif /* ${GUARD} */
EOF
}

create_s() {
cat > "$1" << EOF
/******************************************************************************
 * File        : $(basename "$1")
 * Project     : ChronosRTOS
 * Module      : $2
 * Group       :
 * Description : $3
 ******************************************************************************/

EOF
}

###############################################################################
# Core
###############################################################################

create_h Core/Inc/kernel.h      "Kernel"      "Public kernel API and definitions."
create_h Core/Inc/task.h        "Task"        "Task management API and Task Control Block."
create_h Core/Inc/scheduler.h   "Scheduler"   "Scheduler API and scheduling definitions."
create_h Core/Inc/memory.h      "Memory"      "Memory management API."
create_h Core/Inc/config.h      "Configuration" "System configuration parameters."
create_h Core/Inc/types.h       "Types"       "Common data types and enumerations."
create_h Core/Inc/error.h       "Error"       "Error codes and status definitions."

create_c Core/Src/kernel.c      "Kernel"      "Kernel initialization and startup."
create_c Core/Src/task.c        "Task"        "Task creation and task management."
create_c Core/Src/scheduler.c   "Scheduler"   "Round-robin scheduler implementation."
create_c Core/Src/memory.c      "Memory"      "Static memory allocation."
create_c Core/Src/idle.c        "Idle Task"   "Idle task implementation."

###############################################################################
# Port
###############################################################################

create_h Port/ARM_CM3/Inc/port.h      "Port"      "ARM Cortex-M3 port layer."
create_h Port/ARM_CM3/Inc/critical.h  "Critical"  "Critical section API."
create_h Port/ARM_CM3/Inc/systick.h   "SysTick"   "System tick API."

create_c Port/ARM_CM3/Src/port.c      "Port"      "ARM Cortex-M3 port implementation."
create_c Port/ARM_CM3/Src/critical.c  "Critical"  "Critical section implementation."
create_c Port/ARM_CM3/Src/systick.c   "SysTick"   "System tick implementation."

create_s Port/ARM_CM3/ASM/startup.s   "Startup"   "Reset handler and system startup."
create_s Port/ARM_CM3/ASM/vectors.s   "Vector Table" "Interrupt vector table."
create_s Port/ARM_CM3/ASM/faults.s    "Fault Handlers" "Exception and fault handlers."

create_s Port/ARM_CM3/ASM/context.s   "Context Switch" "Save and restore CPU context."
create_s Port/ARM_CM3/ASM/pendsv.s    "PendSV"    "PendSV context switching routine."
create_s Port/ARM_CM3/ASM/svc.s       "SVC"       "Supervisor Call handler."
create_s Port/ARM_CM3/ASM/atomic.s    "Atomic"    "Atomic instruction wrappers."

###############################################################################
# Drivers
###############################################################################

create_h Drivers/Inc/gpio.h "GPIO" "GPIO driver interface."
create_h Drivers/Inc/uart.h "UART" "UART driver interface."

create_c Drivers/Src/gpio.c "GPIO" "GPIO driver implementation."
create_c Drivers/Src/uart.c "UART" "UART driver implementation."

###############################################################################
# Demo
###############################################################################

create_h Demo/demo_tasks.h "Demo Tasks" "Demo task declarations."

create_c Demo/demo_tasks.c "Demo Tasks" "Demo task implementations."
create_c Demo/main.c "Main" "ChronosRTOS demonstration application."

echo ""
echo "==============================================="
echo " ChronosRTOS file headers generated successfully"
echo "==============================================="
