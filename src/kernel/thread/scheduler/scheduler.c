#include <stm32f1xx_hal.h>
#include "scheduler.h"
#include "waiting_queue/waiting_queue.h"

Scheduler scheduler;

static PriorityThreadGroup create_priority_thread_group() {
    PriorityThreadGroup result = {
            .head = NULL,
            .next_run = NULL
    };
    return result;
}

void update_next_run(PriorityThreadGroup *group) {
    if (group->next_run == NULL) {
        group->next_run = group->head;
    }
    ThreadOwnedDoubleListNode *current_run = group->next_run;
    ThreadOwnedDoubleListNode *looking_at = current_run->next;
    while (looking_at != current_run) {
        if (looking_at->thread.state == Ready) {
            group->next_run = looking_at;
            break;
        }
        looking_at = looking_at->next;
    }
    if (looking_at == current_run && looking_at->thread.state != Ready) {
        group->next_run = NULL;
    }
}

__weak void idle_thread(void *_) {
    while (1) {
        __WFI();
        schedule();
    }
}

void scheduler_init() {
    for (size_t i = 0; i < PRIORITY_COUNT; ++i) {
        scheduler.priorityThreadGroup[i] = create_priority_thread_group();
    }
    push_thread(create_thread(idle_thread, NULL, PRIORITY_COUNT - 1));
    scheduler.current_running = NULL;
}

void start_schedule() {
    ThreadOwnedDoubleListNode *to = NULL;
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
    ThreadOwnedDoubleListNode *node = kernel_alloc(sizeof(ThreadOwnedDoubleListNode));
    node->thread = thread;
    if (group->head == NULL) {
        node->next = node;
        node->prev = node;
        group->head = node;
        group->next_run = node;
    } else {
        ThreadOwnedDoubleListNode *prev = group->head->prev;
        node->prev = prev;
        prev->next = node;
        node->next = group->head;
        group->head->prev = node;
    }
}

void remove_thread(Thread *thread) {
    PriorityThreadGroup *group = &scheduler.priorityThreadGroup[thread->priority];
    ThreadOwnedDoubleListNode *current = group->head;
    do {
        if (&current->thread == thread) {
            if (current->next == current->prev) {
                kernel_free(group->head);
                group->head = NULL;
                group->next_run = NULL;
            } else {
                current->next->prev = current->prev;
                current->prev->next = current->next;
                update_next_run(group);
                kernel_free(current);
            }
            break;
        } else {
            current = current->next;
        }
    } while (current != group->head);
}

void schedule() {
    ThreadOwnedDoubleListNode *from = scheduler.current_running;
    ThreadOwnedDoubleListNode *to = NULL;
    for (size_t i = 0; i < PRIORITY_COUNT; ++i) {
        if (scheduler.priorityThreadGroup[i].next_run != NULL) {
            to = scheduler.priorityThreadGroup[i].next_run;
            update_next_run(&scheduler.priorityThreadGroup[i]);
            break;
        }
    }
    scheduler.current_running = to;
    if (to != NULL && to != from) {
        context_switch(&from->thread.stack_top, &to->thread.stack_top);
    }
}

void SysTick_Handler(void) {
    HAL_IncTick();
    update_waiting_queue(HAL_GetTick());
    schedule();
}
