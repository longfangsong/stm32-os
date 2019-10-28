#include <stm32f1xx_hal.h>
#include "sleep.h"

void sleep_until(uint32_t tick) {
    thread_waiting(&scheduler.current_running->thread, tick);
}

void sleep(uint32_t micro_seconds) {
    sleep_until(HAL_GetTick() + micro_seconds);
}
