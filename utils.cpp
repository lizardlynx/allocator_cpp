#include <iostream>
#include <allocator.h>
#include <utils.h>

using namespace std;

void buf_fill(char *c, size_t size)
{
    while (size--)
    {
        *c++ = (unsigned char)rand();
    }
}

void* buf_alloc(Allocator allocator, size_t size)
{
    void *ptr;

    ptr = allocator.mem_alloc(size);
    if (ptr != NULL)
        buf_fill((char*)ptr, size);
    return ptr;
}