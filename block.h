#pragma once
#include <allocator_impl.h>
#include <config.h>
#include <vector>
#include <iostream>
using namespace std;

struct block {
    size_t currSize;
    size_t prevSize;
    size_t offset;
    bool busy;
    bool first;
    bool last;
};

#define BLOCK_STRUCT_SIZE alignment(sizeof(block))

class Block
{
private:
    static bool merge_i;
    bool merge_only_next = false;
    struct block* b;
    void create_block();
public:
    Block();
    Block(block* block);
    Block(block* block, bool ifnew);
    Block(size_t var_int, std::string var_name);
    void setArenaSize(int arena_size);
    void setPageSize(int page_size);
    size_t getCurrSize();
    void setCurrSize(size_t size);
    size_t getPrevSize();
    void setPrevSize(size_t size);
    void setMergeOnlyNext(bool mer);
    size_t getOffset();
    void setOffset(size_t size);
    bool isBusy();
    void setBusy(bool busyArg);
    bool isFirst();
    void setFirst(bool firstArg);
    bool isLast();
    void setLast(bool lastArg);
    block* block_next();
    block* block_prev();
    void* block_to_payload();
    block* block_split(size_t);
    Block payload_to_block(void* ptr);
    std::vector<block*> block_merge();
    void return_memory();
    block* getPtr();
    bool checkBlock();
    std::vector<block*> block_merge_helper(Block bl);
};

