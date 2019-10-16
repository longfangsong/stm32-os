#ifndef STM32_OS_ELF_SCHEDULER_H
#define STM32_OS_ELF_SCHEDULER_H

#include "../thread.h"
#include "../../heap/heap.h"
#include "./context_switch/context_switch.h"
#include "../../panic/panic.h"

typedef struct {
    Thread *threads;
    uint32_t thread_count;
    uint32_t current_running;
} Scheduler;

Scheduler create_scheduler();

void push_thread(Scheduler *scheduler, Thread thread);

void start(Scheduler *scheduler);

void schedule();

#endif //STM32_OS_ELF_SCHEDULER_H
