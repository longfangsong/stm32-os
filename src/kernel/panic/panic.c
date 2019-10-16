#include "./panic.h"
#include "../../device/lcd/lcd_basic.h"

void panic_with_message(char *message) {
    LCD_Clear(BLUE);
    LCD_ShowString(0, 0, 200, 24, 24, (uint8_t *) message);
    while (1) {
        __WFI();
    }
}

void panic() {
    panic_with_message("kernel panic!");
}

void should_never_reach_here() {
    panic_with_message("should never reach here!");
}