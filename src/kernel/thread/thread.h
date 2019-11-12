#ifndef STM32_OS_ELF_THREAD_H
#define STM32_OS_ELF_THREAD_H

#include <sched.h>
#include "scheduler/context_switch/context_switch.h"
#include "../types.h"

#define STACK_SIZE 512

typedef enum {
    Ready,
    Block
} State;

typedef struct {
    Address stack_top;
    StackPointer stack_begin;

    void (*entry)(void *);

    void *params;
    size_t priority;
    State state;
} Thread;

Thread create_thread(void (*entry)(void *params), void *params, size_t priority);

#endif //STM32_OS_ELF_THREAD_H
