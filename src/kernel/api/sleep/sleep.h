#ifndef STM32_OS_SLEEP_H
#define STM32_OS_SLEEP_H

#include <sched.h>
#include "../../thread/scheduler/waiting_queue/waiting_queue.h"
#include "../../thread/scheduler/scheduler.h"


void sleep_until(uint32_t tick);

void sleep(uint32_t micro_seconds);


#endif //STM32_OS_SLEEP_H
