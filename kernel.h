#pragma once
#include <block.h>

class Kernel
{
private:
  static void* startAlloc;
public:
  void *malloc(size_t size);
  void mrelease(void* ptr, size_t size);
  void notifyBlankPage(void* ptr, size_t size);
  void copyData(void*, void*, size_t);
};
