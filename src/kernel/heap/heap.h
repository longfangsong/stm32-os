#ifndef STM32_OS_ELF_HEAP_H
#define STM32_OS_ELF_HEAP_H

void *kernel_alloc(size_t size);

void kernel_free(void *position);

#endif //STM32_OS_ELF_HEAP_H
