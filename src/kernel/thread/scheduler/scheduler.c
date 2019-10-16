#include <stm32f103xe.h>
#include <stm32f1xx_hal.h>
#include "scheduler.h"

Scheduler *default_scheduler;

Scheduler create_scheduler() {
    Scheduler result = {NULL, 0, (uint32_t) (-1)};
    return result;
}

void push_thread(Scheduler *scheduler, Thread thread) {
    Thread *new_thread_buffer = kernel_alloc(sizeof(Thread) * (scheduler->thread_count + 1));
    size_t i;
    for (i = 0; i < scheduler->thread_count; ++i) {
        new_thread_buffer[i] = scheduler->threads[i];
    }
    new_thread_buffer[i] = thread;
    kernel_free(scheduler->threads);
    scheduler->threads = new_thread_buffer;
    ++scheduler->thread_count;
}

void start(Scheduler *scheduler) {
    default_scheduler = scheduler;
    Thread *thread = &scheduler->threads[0];
    scheduler->current_running = 0;
    context_switch_to(&thread->stack_top);
}

void schedule() {
    size_t current_running = default_scheduler->current_running;
    size_t next_running = (default_scheduler->current_running + 1) % default_scheduler->thread_count;
    Thread *from_thread = &default_scheduler->threads[current_running];
    Thread *to_thread = &default_scheduler->threads[next_running];
    default_scheduler->current_running = next_running;
    context_switch(&from_thread->stack_top, &to_thread->stack_top);
}


