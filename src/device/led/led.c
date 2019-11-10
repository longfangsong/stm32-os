#include "led.h"
#include "../../kernel/init/init.h"

void green_led_init() {
    GPIO_InitTypeDef GPIO_InitStruct = {
            GPIO_PIN_2,
            GPIO_MODE_OUTPUT_PP,
            GPIO_NOPULL,
            GPIO_SPEED_FREQ_LOW,
    };
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

EXPORT_DEVICE_INIT(green_led_init);

void red_led_init() {
    GPIO_InitTypeDef GPIO_InitStruct = {
            GPIO_PIN_8,
            GPIO_MODE_OUTPUT_PP,
            GPIO_NOPULL,
            GPIO_SPEED_FREQ_LOW,
    };
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

EXPORT_DEVICE_INIT(red_led_init);

void green_led(uint8_t state) {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, state);
}

void red_led(uint8_t state) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, state);
}
