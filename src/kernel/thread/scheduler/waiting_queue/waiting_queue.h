#ifndef STM32_OS_WAITING_QUEUE_H
#define STM32_OS_WAITING_QUEUE_H

#include "../../thread.h"
#include "../../../heap/heap.h"

void thread_waiting(Thread *thread, uint32_t waiting_for_tick);

void update_waiting_queue(uint32_t now_tick);

#endif //STM32_OS_WAITING_QUEUE_H
