#include <stm32f1xx_hal.h>
#include "device/led/led.h"
#include "kernel/thread/scheduler/scheduler.h"
#include "kernel/api/sleep/sleep.h"
#include "kernel/api/exit/exit.h"
#include "device/uart/uart.h"


void green_control_thread(void *_) {
    for (size_t i = 0; i < 5; ++i) {
        green_led(LED_ON);
        sleep(500);
        green_led(LED_OFF);
        sleep(500);
    }
    exit();
}

void red_control_thread(void *_) {
    for (size_t i = 0; i < 10; ++i) {
        red_led(LED_ON);
        sleep(500);
        red_led(LED_OFF);
        sleep(500);
    }
    exit();
}

void uart_thread(void *_) {
    while (1) {
        uart_send("hello!\r\n", 8);
        sleep(1000);
    }
}

int main(void) {
    push_thread(create_thread(green_control_thread, NULL, 1));
    push_thread(create_thread(red_control_thread, NULL, 1));
    push_thread(create_thread(uart_thread, NULL, 1));
    start_schedule();
    while (1) {
        __WFI();
    }
    return 0;
}
