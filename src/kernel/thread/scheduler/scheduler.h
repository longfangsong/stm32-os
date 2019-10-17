#ifndef STM32_OS_ELF_SCHEDULER_H
#define STM32_OS_ELF_SCHEDULER_H

#include "../thread.h"
#include "../../heap/heap.h"
#include "./context_switch/context_switch.h"
#include "../../panic/panic.h"

#define PRIORITY_COUNT 16

typedef struct ThreadListNode {
    struct ThreadListNode *prev;
    struct ThreadListNode *next;
    Thread thread;
} ThreadListNode;

typedef struct {
    ThreadListNode *head;
    ThreadListNode *next_run;
} PriorityThreadGroup;

typedef struct {
    PriorityThreadGroup priorityThreadGroup[PRIORITY_COUNT];
    ThreadListNode *current_running;
} Scheduler;

void init_scheduler();

void push_thread(Thread thread);

void start_schedule();

void schedule();

#endif //STM32_OS_ELF_SCHEDULER_H
