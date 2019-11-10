#include "cmsis_compiler.h"
#include "waiting_queue.h"
#include "../scheduler.h"

typedef struct ThreadUnownedSingleListNode {
    struct ThreadUnownedSingleListNode *next;
    Thread *thread;
} ThreadUnownedSingleListNode;

static void insert_after_tail(ThreadUnownedSingleListNode *node, Thread *thread) {
    while (node->next != NULL)
        node = node->next;
    node->next = kernel_alloc(sizeof(ThreadUnownedSingleListNode));
    node->next->thread = thread;
    node->next->next = NULL;
}

static void free_and_awake_all(ThreadUnownedSingleListNode *node) {
    while (node != NULL) {
        node->thread->state = Ready;
        update_next_run(&scheduler.priorityThreadGroup[node->thread->priority]);
        ThreadUnownedSingleListNode *next_node = node->next;
        kernel_free(node);
        node = next_node;
    }
}

typedef struct WaitingNode {
    uint32_t waiting_for_tick;
    struct WaitingNode *next;
    ThreadUnownedSingleListNode *head;
} WaitingNode;

WaitingNode *waitingList = NULL;

void thread_waiting(Thread *thread, uint32_t waiting_for_tick) {
    __disable_irq();
    thread->state = Block;
    if (waitingList == NULL) {
        waitingList = kernel_alloc(sizeof(WaitingNode));
        waitingList->waiting_for_tick = waiting_for_tick;
        waitingList->head = kernel_alloc(sizeof(ThreadUnownedSingleListNode));
        waitingList->head->thread = thread;
        waitingList->head->next = NULL;
        waitingList->next = NULL;
    } else {
        WaitingNode *current_searching = waitingList;
        while (current_searching->next != NULL && current_searching->next->waiting_for_tick <= waiting_for_tick) {
            current_searching = current_searching->next;
        }
        if (current_searching->waiting_for_tick == waiting_for_tick) {
            insert_after_tail(current_searching->head, thread);
        } else {
            WaitingNode *old_next = current_searching->next;
            current_searching->next = kernel_alloc(sizeof(WaitingNode));
            current_searching->next->waiting_for_tick = waiting_for_tick;
            current_searching->next->head = kernel_alloc(sizeof(ThreadUnownedSingleListNode));
            current_searching->next->head->thread = thread;
            current_searching->next->head->next = NULL;
            current_searching->next->next = old_next;
        }
    }
    update_next_run(&scheduler.priorityThreadGroup[thread->priority]);
    schedule();
    __enable_irq();
}

void update_waiting_queue(uint32_t now_tick) {
    while (waitingList != NULL && waitingList->waiting_for_tick <= now_tick) {
        WaitingNode *next = waitingList->next;
        free_and_awake_all(waitingList->head);
        kernel_free(waitingList);
        waitingList = next;
    }
}
