#ifndef STM32_OS_ELF_HEAP_H
#define STM32_OS_ELF_HEAP_H
#define HEAP_BEGIN BSS_END
#define HEAP_END STM32_SRAM_END

void heap_init();

void *kernel_alloc(uint32_t size);

void kernel_free(void *position);

#endif //STM32_OS_ELF_HEAP_H
