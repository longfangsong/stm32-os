#include <stm32f103xe.h>
#include <sched.h>
#include <stm32f1xx_hal.h>
#include "context_switch.h"

StackPointer *interrupt_from_thread_stack_pointer = NULL;
StackPointer *interrupt_to_thread_stack_pointer = NULL;
uint32_t context_switch_interrupt_flag = 0;

void context_switch_to(StackPointer *to_stack_pointer) {
    interrupt_to_thread_stack_pointer = to_stack_pointer;
    interrupt_from_thread_stack_pointer = NULL;
    context_switch_interrupt_flag = 1;
    // HAL seems put NVIC in wrong position
    // NVIC_SetPendingIRQ(PendSV_IRQn); won't work
    // so write directly
    *((uint32_t *) 0xE000ED04) = 0x10000000;
    __enable_fault_irq();
    __enable_irq();
}

void context_switch(StackPointer *from_stack_pointer, StackPointer *to_stack_pointer) {
    if (context_switch_interrupt_flag != 1) {
        context_switch_interrupt_flag = 1;
        interrupt_from_thread_stack_pointer = from_stack_pointer;
    }
    interrupt_to_thread_stack_pointer = to_stack_pointer;
    // HAL seems put NVIC in wrong position
    // NVIC_SetPendingIRQ(PendSV_IRQn); won't work
    // so write directly
    *((uint32_t *) 0xE000ED04) = 0x10000000;
}