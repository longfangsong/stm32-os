#include "thread.h"
#include "../align.h"
#include "../heap/heap.h"

typedef struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    void *link_register;
    void *program_counter;
    uint32_t program_states_register;
} ExceptionStackFrame;

typedef struct {
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;

    ExceptionStackFrame exception_stack_frame;
} StackFrame;

void *init_stack(void *stack_memory, void (*entry_point)(void *), void *params) {
    uint8_t *stack_top = (uint8_t *) ALIGN_DOWN((uint32_t) stack_memory, 8u);
    stack_top -= sizeof(StackFrame);
    StackFrame *frame = (StackFrame *) (stack_top);
    frame->exception_stack_frame.r0 = (uint32_t) params;
    frame->exception_stack_frame.r1 = 0;
    frame->exception_stack_frame.r2 = 0;
    frame->exception_stack_frame.r3 = 0;
    frame->exception_stack_frame.r12 = 0;
    frame->exception_stack_frame.link_register = NULL;
    frame->exception_stack_frame.program_counter = entry_point;
    frame->exception_stack_frame.program_states_register = 0x01000000;
    frame->r4 = 0xdeadbeef;
    frame->r5 = 0xdeadbeef;
    frame->r6 = 0xdeadbeef;
    frame->r7 = 0xdeadbeef;
    frame->r8 = 0xdeadbeef;
    frame->r9 = 0xdeadbeef;
    frame->r10 = 0xdeadbeef;
    frame->r11 = 0xdeadbeef;
    return stack_top;
}

Thread create_thread(void (*entry)(void *params), void *params, size_t priority) {
    void *stack_memory = kernel_alloc(STACK_SIZE);
    StackPointer stack_top = init_stack(stack_memory + STACK_SIZE, entry, params);
    Thread result = {
            .entry = entry,
            .params = params,
            .stack_begin = stack_memory,
            .stack_top=stack_top,
            .priority=priority
    };
    return result;
}