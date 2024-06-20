
#include <stdio.h>
#include "memory.h"

void memory_init() {
    printf("Memory: Initializing memory...\n");
}

void memory_allocate(int size) {
    printf("Memory: Allocating %d bytes of memory...\n", size);
}

void memory_free(void* ptr) {
    printf("Memory: Freeing memory at %p...\n", ptr);
}