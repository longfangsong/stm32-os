.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb
.extern interrupt_from_thread_stack_pointer
.extern interrupt_to_thread_stack_pointer
.extern context_switch_interrupt_flag

.global PendSV_Handler
.thumb_func
PendSV_Handler:
    mrs r2, primask
    cpsid I

    ldr r0, =context_switch_interrupt_flag
    ldr r1, [r0]
    cbz r1, pendsv_end
    mov r1, #0x00
    str r1, [r0]

    ldr r0, =interrupt_from_thread_stack_pointer
    ldr r1, [r0]
    cbz r1, switch_to_thread

        mrs r1, psp
        stmfd r1!, {r4 - r11}
        ldr r0, [r0]
        str r1, [r0]

    switch_to_thread:
        ldr r1, =interrupt_to_thread_stack_pointer
        ldr r1, [r1]
        ldr r1, [r1]
        ldmfd r1!, {r4 - r11}
        msr psp, r1
    pendsv_end:
    msr primask, r2
    orr lr, lr, #0x04
    bx lr

