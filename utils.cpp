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

void* buf_alloc(Allocator* allocator, size_t size)
{
    void *ptr;

    ptr = allocator->mem_alloc(size);
    if (ptr != NULL)
        buf_fill((char*)ptr, size);
    return ptr;
}

void manualCall()
{
    //allocate arena with two busy blocks
    Allocator a = Allocator(4000, "arena_size");
    Allocator* aPtr = &a;
    cout << "\n------1-------" << endl;
    void* ptr = buf_alloc(aPtr, 45);
    void* ptr2 = buf_alloc(aPtr, 44);
    a.mem_show_tree();
    a.mem_show();

    //free first block
    cout << "\n------2-------" << endl;
    a.mem_free(ptr);
    a.mem_show();
    a.mem_show_tree();

    //now free second block
    cout << "\n------3-------" << endl;
    a.mem_free(ptr2);
    a.mem_show_tree();
    a.mem_show();

    //allocate new two blocks
    cout << "\n------4-------" << endl;
    buf_alloc(aPtr, 600);
    void* ptr3 = buf_alloc(aPtr, 60);
    void* ptr4 = buf_alloc(aPtr, 3);
    a.mem_show();

    // realloc second block
    cout << "\n------5-------" << endl;
    a.mem_realloc(ptr3, 300);
    a.mem_show();
}
