#ifndef STM32_OS_ELF_SCHEDULER_H
#define STM32_OS_ELF_SCHEDULER_H

#include "../thread.h"
#include "../../heap/heap.h"
#include "./context_switch/context_switch.h"
#include "../../panic/panic.h"

#define PRIORITY_COUNT 16
typedef struct ThreadOwnedDoubleListNode {
    struct ThreadOwnedDoubleListNode *prev;
    struct ThreadOwnedDoubleListNode *next;
    Thread thread;
} ThreadOwnedDoubleListNode;

typedef struct {
    ThreadOwnedDoubleListNode *head;
    ThreadOwnedDoubleListNode *next_run;
} PriorityThreadGroup;

typedef struct {
    PriorityThreadGroup priorityThreadGroup[PRIORITY_COUNT];
    ThreadOwnedDoubleListNode *current_running;
} Scheduler;

extern Scheduler scheduler;

void scheduler_init();

void push_thread(Thread thread);

void remove_thread(Thread *thread);

void start_schedule();

void schedule();

void update_next_run(PriorityThreadGroup *group);

#endif //STM32_OS_ELF_SCHEDULER_H
