#ifndef STM32_OS_ELF_ALIGN_H
#define STM32_OS_ELF_ALIGN_H
#define ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))
#define ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

#endif //STM32_OS_ELF_ALIGN_H
