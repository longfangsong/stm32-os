#ifndef STM32_OS_ELF_CONTEXT_SWITCH_H
#define STM32_OS_ELF_CONTEXT_SWITCH_H
typedef void *StackPointer;

extern StackPointer *interrupt_from_thread_stack_pointer;
extern StackPointer *interrupt_to_thread_stack_pointer;
extern uint32_t context_switch_interrupt_flag;

void context_switch_to(StackPointer *to_stack_pointer);

void context_switch(StackPointer *from_stack_pointer, StackPointer *to_stack_pointer);

#endif //STM32_OS_ELF_CONTEXT_SWITCH_H
