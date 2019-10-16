#ifndef STM32_OS_ELF_THREAD_H
#define STM32_OS_ELF_THREAD_H

#include <sched.h>
#include "scheduler/context_switch/context_switch.h"

#define STACK_SIZE 512
typedef struct {
    void *stack_top;
    StackPointer stack_begin;

    void (*entry)(void *);

    void *params;
} Thread;

Thread create_thread(void (*entry)(void *params), void *params);

#endif //STM32_OS_ELF_THREAD_H
