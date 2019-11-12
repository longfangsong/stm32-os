#include <sched.h>
#include "heap.h"
#include "../align.h"
#include "../init/init.h"
#include "../types.h"

static Address begin_addr;
static Address end_addr;

void heap_init() {
    extern int __HEAP_START;
    extern int __HEAP_END;
    begin_addr = &__HEAP_START;
    end_addr = &__HEAP_END;
    Address begin_aligned = (Address) ALIGN((uint32_t) begin_addr, 4u);
    Address end_aligned = (Address) ALIGN((uint32_t) end_addr, 4u);
#ifndef NDEBUG
    for (uint32_t *i = begin_aligned; i < (uint32_t *) end_aligned; ++i) {
        *i = 0xdeadbeef;
    }
#endif
}

EXPORT_KERNEL_INIT_1(heap_init);

typedef struct AllocatedMemory {
    struct AllocatedMemory *prev;
    struct AllocatedMemory *next;
    size_t size;
} AllocatedMemory;

static AllocatedMemory *allocated_list;

static inline Address allocated_memory_start(AllocatedMemory *this) {
    return (Address) ((byte *) this + sizeof(AllocatedMemory));
}

static inline Address allocated_memory_end(AllocatedMemory *this) {
    return (Address) ((byte *) allocated_memory_start(this) + this->size);
}

byte is_free_space_after_large_enough(AllocatedMemory *this, size_t space_request) {
    AllocatedMemory *next = this->next;
    if (next == NULL) {
        return 1;
    } else {
        Address next_start = (Address) next;
        return (byte *) next_start - (byte *) allocated_memory_end(this) >= space_request;
    }
}

AllocatedMemory *find_first_before_large_enough_space(size_t space_request) {
    if (allocated_list == NULL) {
        return NULL;
    }
    AllocatedMemory *current_looking = allocated_list;
    while (!is_free_space_after_large_enough(current_looking, space_request)) {
        current_looking = current_looking->next;
    }
    return current_looking;
}

void *kernel_alloc(size_t size) {
    if (allocated_list == NULL) {
        AllocatedMemory *new_allocated = begin_addr;
        new_allocated->next = new_allocated;
        new_allocated->prev = new_allocated;
        new_allocated->size = size;
        allocated_list = new_allocated;
        return allocated_memory_start(new_allocated);
    }
    if ((byte *) begin_addr - (byte *) allocated_list >= size + sizeof(AllocatedMemory)) {
        AllocatedMemory *new_allocated = begin_addr;
        AllocatedMemory *old_prev = allocated_list->prev;
        old_prev->next = new_allocated;
        new_allocated->prev = old_prev;
        allocated_list = new_allocated;
        return allocated_memory_start(new_allocated);
    }
    AllocatedMemory *before_new_allocated = find_first_before_large_enough_space(size);
    AllocatedMemory *old_next = before_new_allocated->next;
    AllocatedMemory *new_allocated = allocated_memory_end(before_new_allocated);

    before_new_allocated->next = new_allocated;
    new_allocated->prev = before_new_allocated;

    new_allocated->next = old_next;
    old_next->prev = new_allocated;

    new_allocated->size = size;
    return allocated_memory_start(new_allocated);
}

void kernel_free(void *position) {
    if (position == NULL) {
        return;
    }
    AllocatedMemory *control_block = (AllocatedMemory *) ((byte *) position - sizeof(AllocatedMemory));
    if (control_block->prev == control_block->next) {
        allocated_list = NULL;
    } else {
        AllocatedMemory *old_prev = control_block->prev;
        AllocatedMemory *old_next = control_block->next;
        old_prev->next = old_next;
        old_next->prev = old_prev;
    }
}
