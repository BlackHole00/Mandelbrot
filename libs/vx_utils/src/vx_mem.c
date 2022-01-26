#include "vx_mem.h"

#include <stdio.h>
#include <malloc.h>
#include "vx_panic.h"

static u32 allocation_number = 0;
static u32 deallocation_number = 0;

void* vx_smalloc(usize size) {
    void* ptr = malloc(size);
    VX_ASSERT("Could not allocate memory!", ptr != 0 || size == 0);

    allocation_number++;

    return ptr;
}

void* vx_srealloc(void* mem_adr, usize size) {
    void* ptr = realloc(mem_adr, size);
    VX_ASSERT("Could not reallocate memory!", ptr != 0 || size == 0);

    deallocation_number++;

    return ptr;
}

void vx_memory_print_state() {
    printf("There has been %d allocations and %d deallocations. There are still %d blocks to be deallocated!\n", allocation_number, deallocation_number, allocation_number - deallocation_number);
}
