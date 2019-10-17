#include <stm32f103xe.h>
#include <stm32f1xx_hal.h>
#include "scheduler.h"

static Scheduler scheduler;

static PriorityThreadGroup create_priority_thread_group() {
    PriorityThreadGroup result = {
            .head = NULL,
            .next_run = NULL
    };
    return result;
}

void init_scheduler() {
    for (size_t i = 0; i < PRIORITY_COUNT; ++i) {
        scheduler.priorityThreadGroup[i] = create_priority_thread_group();
    }
    scheduler.current_running = NULL;
}

void start_schedule() {
    ThreadListNode *to = NULL;
    for (size_t i = 0; i < PRIORITY_COUNT; ++i) {
        if (scheduler.priorityThreadGroup[i].head != NULL) {
            to = scheduler.priorityThreadGroup[i].next_run;
            scheduler.priorityThreadGroup[i].next_run = scheduler.priorityThreadGroup[i].next_run->next;
            break;
        }
    }
    scheduler.current_running = to;
    if (to != NULL) {
        context_switch_to(&to->thread.stack_top);
    }
}

void push_thread(Thread thread) {
    if (thread.priority >= PRIORITY_COUNT) {
        panic_with_message("No such priority!");
    }
    PriorityThreadGroup *group = &scheduler.priorityThreadGroup[thread.priority];
    ThreadListNode *node = kernel_alloc(sizeof(ThreadListNode));
    node->thread = thread;
    if (group->head == NULL) {
        node->next = node;
        node->prev = node;
        group->head = node;
        group->next_run = node;
    } else {
        ThreadListNode *prev = group->head->prev;
        node->prev = prev;
        prev->next = node;
        node->next = group->head;
        group->head->prev = node;
    }
}

void schedule() {
    ThreadListNode *from = scheduler.current_running;
    ThreadListNode *to = NULL;
    for (size_t i = 0; i < PRIORITY_COUNT; ++i) {
        if (scheduler.priorityThreadGroup[i].head != NULL) {
            to = scheduler.priorityThreadGroup[i].next_run;
            scheduler.priorityThreadGroup[i].next_run = scheduler.priorityThreadGroup[i].next_run->next;
            break;
        }
    }
    scheduler.current_running = to;
    if (to != NULL && to != from) {
        context_switch(&from->thread.stack_top, &to->thread.stack_top);
    }
}

