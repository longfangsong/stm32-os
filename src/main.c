#include <stm32f1xx_hal.h>
#include "device/led/led.h"
#include "kernel/thread/scheduler/scheduler.h"


void green_control_thread(void *_) {
    while (1) {
        green_led(LED_ON);
        HAL_Delay(500);
        schedule();
        green_led(LED_OFF);
        HAL_Delay(500);
        schedule();
    }
}

void red_control_thread(void *_) {
    while (1) {
        red_led(LED_ON);
        HAL_Delay(500);
        schedule();
        red_led(LED_OFF);
        HAL_Delay(500);
        schedule();
    }
}

int main(void) {
    Scheduler scheduler = create_scheduler();
    push_thread(&scheduler, create_thread(green_control_thread, NULL));
    push_thread(&scheduler, create_thread(red_control_thread, NULL));
    start(&scheduler);
    while (1) {
        __WFI();
    }
    return 0;
}

