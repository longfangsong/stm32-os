#include <sched.h>
#include "heap.h"
#include "../align.h"
#include "../init/init.h"

extern int __HEAP_START;
extern int __HEAP_END;

typedef struct {
    void *begin;
    void *end;
} AllocatedMemoryRange;

uint32_t is_empty(AllocatedMemoryRange range) {
    return range.begin == NULL && range.end == NULL;
}

#define ALLOCATED_INFO_LENGTH 128

typedef struct {
    AllocatedMemoryRange *allocated_info;
    void *buffer_begin;
    void *buffer_end;
} MemoryControllerBlock;
#define HEAP_CONTROL_SPACE (ALLOCATED_INFO_LENGTH*sizeof(AllocatedMemoryRange))
MemoryControllerBlock memoryControllerBlock;

void heap_init() {
    void *begin_addr = &__HEAP_START;
    void *end_addr = &__HEAP_END;
    uint32_t begin_aligned = ALIGN((uint32_t) begin_addr, 4u);
    uint32_t end_aligned = ALIGN((uint32_t) end_addr, 4u);
    uint32_t real_free_space = begin_aligned + HEAP_CONTROL_SPACE;
    for (uint32_t i = begin_aligned; i < real_free_space; i += 4) {
        *((uint32_t *) (i)) = 0;
    }
    MemoryControllerBlock block = {
            (AllocatedMemoryRange *) begin_aligned,
            (void *) real_free_space,
            (void *) end_aligned
    };
    memoryControllerBlock = block;
}

EXPORT_KERNEL_INIT_1(heap_init);

static uint32_t find_alloc_info_pos(uint32_t size) {
    if (is_empty(memoryControllerBlock.allocated_info[0])) {
        return 0;
    }
    for (size_t i = 1; i < ALLOCATED_INFO_LENGTH; ++i) {
        if (is_empty(memoryControllerBlock.allocated_info[i])) {
            return i;
        } else if ((u_int8_t *) memoryControllerBlock.allocated_info[i].begin -
                   (u_int8_t *) memoryControllerBlock.allocated_info[i - 1].end > size) {
            return i;
        }
    }
    return (uint32_t) -1;
}

void *kernel_alloc(uint32_t size) {
    uint32_t alloc_info_pos = find_alloc_info_pos(size + 4);
    if (alloc_info_pos == (uint32_t) -1)
        return NULL;
    if (!is_empty(memoryControllerBlock.allocated_info[alloc_info_pos])) {
        for (size_t i = ALLOCATED_INFO_LENGTH - 1; i > alloc_info_pos; --i) {
            if (!is_empty(memoryControllerBlock.allocated_info[i - 1])) {
                memoryControllerBlock.allocated_info[i] = memoryControllerBlock.allocated_info[i - 1];
                *((uint32_t *) (memoryControllerBlock.allocated_info[i].begin)) += 1;
            }
        }
    }
    void *begin;
    if (alloc_info_pos == 0) {
        begin = memoryControllerBlock.buffer_begin;
    } else {
        begin = memoryControllerBlock.allocated_info[alloc_info_pos - 1].end;
    }
    void *end = ((uint8_t *) (begin) + size + 4);
    memoryControllerBlock.allocated_info[alloc_info_pos].begin = begin;
    memoryControllerBlock.allocated_info[alloc_info_pos].end = end;
    *((uint32_t *) (memoryControllerBlock.allocated_info[alloc_info_pos].begin)) = alloc_info_pos;
    return (void *) ((unsigned char *) (begin) + 4);
}

void kernel_free(void *position) {
    if (position == NULL) {
        return;
    }
    void *real_begin = ((unsigned char *) (position) - 4);
    for (uint32_t index = *((uint32_t *) (real_begin)); index < ALLOCATED_INFO_LENGTH - 1; ++index) {
        if (is_empty(memoryControllerBlock.allocated_info[index + 1])) {
            memoryControllerBlock.allocated_info[index] = memoryControllerBlock.allocated_info[index + 1];
            break;
        } else {
            memoryControllerBlock.allocated_info[index] = memoryControllerBlock.allocated_info[index + 1];
            *((uint32_t *) (memoryControllerBlock.allocated_info[index].begin)) -= 1;
        }
    }
}
