#include "exit.h"

void exit() {
    remove_thread(&scheduler.current_running->thread);
    scheduler.current_running = NULL;
    schedule();
}
