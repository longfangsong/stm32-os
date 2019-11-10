#ifndef STM32_OS_ELF_INIT_H
#define STM32_OS_ELF_INIT_H

#include "../section.h"

typedef void (*init_function)(void);

#define EXPORT_INIT(func, level) \
    __attribute__((used)) const init_function init_##func SECTION(".init_fn."level) = func

#define EXPORT_BOARD_INIT(func) EXPORT_INIT(func, "1")
#define EXPORT_DEVICE_INIT(func) EXPORT_INIT(func, "2")
#define EXPORT_KERNEL_INIT_1(func) EXPORT_INIT(func, "3")
#define EXPORT_KERNEL_INIT_2(func) EXPORT_INIT(func, "4")

void init_all();

#endif //STM32_OS_ELF_INIT_H
