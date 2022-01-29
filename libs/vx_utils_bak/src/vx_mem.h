#pragma once
#include "vx_types.h"

#define VX_SIZE_OF_CVECTOR(_CVEC) (sizeof((_CVEC)) / sizeof(*(_CVEC)))

void* vx_smalloc(usize);
void* vx_srealloc(void*, usize);
void vx_memory_print_state();
