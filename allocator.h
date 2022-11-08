#pragma once
#include <kernel.h>
#include <tree.h>

class Allocator 
{
  Kernel kernel;
  Block arena;
  void return_memory();
  bool released;
  RedBlackTree tree;
  int A_PAGES_NUMBER;
  int A_PAGE_SIZE;
  int A_SIZE;
public:
  Allocator();
  Allocator(size_t var_int, std::string var_name);
  void *mem_alloc(size_t size);
  void mem_free(void *ptr);
  void* mem_realloc(void *ptr, size_t size);
  void mem_show();
  void mem_show_tree();
  void blockSplitHelper(Block block, size_t size);
  void* blockMergeHelper(Block block1);
  bool checkBlocks();
};
