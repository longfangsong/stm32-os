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
    push_thread(create_thread(green_control_thread, NULL, 2));
    push_thread(create_thread(red_control_thread, NULL, 1));
    start_schedule();
    while (1) {
        __WFI();
    }
    return 0;
}

