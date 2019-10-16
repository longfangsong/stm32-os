#ifndef STM32_OS_ELF_PANIC_H
#define STM32_OS_ELF_PANIC_H


void panic_with_message(char *message);

void panic();

void should_never_reach_here();

#endif //STM32_OS_ELF_PANIC_H
