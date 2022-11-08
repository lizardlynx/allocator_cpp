#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <block.h>
#include <unistd.h>
#include <kernel.h>
#include <config.h>
#include <sys/mman.h>
#include <utils.h>

using namespace std;

void* Kernel::startAlloc = 0;

// функція отримання пам’яті з наявного алокатору пам’яті
void* Kernel::malloc(size_t size)
{
  void* ptr;
  #if OS == 2
    ptr = mmap(startAlloc, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  #elif (OS == 1)
    ptr = VirtualAlloc(startAlloc, size, MEM_RESERVE|MEM_COMMIT|MEM_RESET, PAGE_READWRITE);
  #endif
  return ptr;
}

void Kernel::notifyBlankPage(void* ptr, size_t size) 
{
  cout << "notify" << endl;
  #if OS == 2
    madvise(ptr, size, MADV_FREE);
  #elif (OS == 1)
    // PrefetchVirtualMemory(GetCurrentProcess(), )
  #endif
}

void Kernel::mrelease(void* ptr, size_t size)
{
  #if OS == 2
    munmap(ptr, size);
  #elif (OS == 1)
    VirtualFree(ptr, size, MEM_RELEASE);
  #endif
}

void Kernel::copyData(void* dest, void* src, size_t size)
{
  memcpy(dest, src, size);
}
