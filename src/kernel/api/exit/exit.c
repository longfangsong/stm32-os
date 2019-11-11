#include <cmsis_compiler.h>
#include "exit.h"

void exit() {
    __disable_irq();
    remove_thread(&scheduler.current_running->thread);
    scheduler.current_running = NULL;
    schedule();
    __enable_irq();
}
