#ifndef STM32_OS_ELF_LED_H
#define STM32_OS_ELF_LED_H

#include <stm32f1xx_hal.h>

#define LED_ON GPIO_PIN_RESET
#define LED_OFF GPIO_PIN_SET

void green_led_init();

void red_led_init();

void green_led(uint8_t state);

void red_led(uint8_t state);

#endif //STM32_OS_ELF_LED_H
