#include "../lcd/lcd_basic.h"
#include "../led/led.h"

void devices_init() {
    lcd_Init();
    green_led_init();
    red_led_init();
}