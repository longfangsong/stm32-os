#include <stm32f1xx_hal.h>
#include "device/led/led.h"
#include "kernel/thread/scheduler/scheduler.h"
#include "kernel/api/sleep/sleep.h"


void green_control_thread(void *_) {
    while (1) {
        green_led(LED_ON);
        sleep(500);
        green_led(LED_OFF);
        sleep(500);
    }
}

void red_control_thread(void *_) {
    while (1) {
        red_led(LED_ON);
        sleep(500);
        red_led(LED_OFF);
        sleep(500);
    }
}

int main(void) {
    push_thread(create_thread(green_control_thread, NULL, 1));
    push_thread(create_thread(red_control_thread, NULL, 2));
    start_schedule();
    while (1) {
        __WFI();
    }
    return 0;
}

