#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <block.h>

#include <kernel.h>
#include <allocator.h>
#include <allocator_impl.h>
#include <config.h>

using namespace std;

Allocator::Allocator() 
{
    A_PAGES_NUMBER = ALLOCATOR_ARENA_PAGES;
    A_PAGE_SIZE = ALLOCATOR_PAGE_SIZE;
    A_SIZE = ARENA_SIZE;

    released = false;
    size_t size = arena.getCurrSize();
    block* arena_ptr = arena.getPtr();
    tree.insert(size, arena_ptr);
    cout << "constructor" << endl;
}

Allocator::Allocator(size_t var_int, std::string var_name) 
{
    A_PAGES_NUMBER = ALLOCATOR_ARENA_PAGES;
    A_PAGE_SIZE = ALLOCATOR_PAGE_SIZE;
    A_SIZE = ARENA_SIZE;

    released = false;
    if (var_name == "arena_size")
    {
        if((PAGE_TYPE_OS == 0 && var_int > 1) ||
          PAGE_TYPE_OS == 1)
            A_SIZE = var_int;
    }
    else if (var_name == "page_size")
    {
        if (PAGE_TYPE_OS == 1)
        {
            if (var_int > 0 && var_int%2 != 0) 
                var_int+= 1;
            A_SIZE = var_int * A_PAGES_NUMBER;
        }
    }

    arena = Block(var_int, var_name);
    size_t size = arena.getCurrSize();
    block* arena_ptr = arena.getPtr();
    tree.insert(size, arena_ptr);
    cout << "constructor params" << endl;
}

void Allocator::blockSplitHelper(Block block1, size_t size)
{
    size_t block_size = block1.getCurrSize();
    block* block2_ptr = block1.block_split(size);    
    if (block2_ptr)
    {
        block* nnn = tree.findCorrectBlock(block_size);
        tree.deleteNode(block_size, block1.getPtr());
        tree.insert(size, block1.getPtr());
        Block block2 = block2_ptr;
        block2.setBusy(0);
        size_t size2 = block2.getCurrSize();
        tree.insert(size2, block2_ptr);
    }
}

void* Allocator::blockMergeHelper(Block block1)
{
    std::vector<size_t> sizes(3, NULL);
    sizes[1] = block1.getCurrSize();

    if (!block1.isFirst())
        sizes[0] = block1.getPrevSize();
    if (!block1.isLast())
        sizes[2] = Block(block1.block_next()).getCurrSize();

    block1.setBusy(0);
    std::vector<block*> merged_blocks_pointers = block1.block_merge();

    for (int i = 0; i< merged_blocks_pointers.size(); i++)
        if (merged_blocks_pointers[i] != NULL)
        {
            tree.deleteNode(sizes[i], merged_blocks_pointers[i]);
        }

    block* pointer1 = merged_blocks_pointers[0];
    Block pointer1Block = Block(pointer1);
    if (pointer1)
    {
        tree.insert(pointer1Block.getCurrSize(), pointer1);
        kernel.copyData(pointer1Block.block_to_payload(), block1.block_to_payload(), sizes[1]);
    }
    else
        tree.insert(block1.getCurrSize(), block1.getPtr());
    
    //check for free pages
    int numPageCounts = floor((BLOCK_STRUCT_SIZE + block1.getCurrSize())/A_PAGE_SIZE);
    if (numPageCounts >= 1 && PAGE_TYPE_OS == 1)
    {
        size_t page_num = ceil(block1.getOffset()/A_PAGE_SIZE);
        void* ptr = (void*) (BLOCK_STRUCT_SIZE + block1.getCurrSize() + block1.getOffset());
        // kernel.notifyBlankPage(ptr, numPageCounts*A_PAGE_SIZE);
    }
    if (pointer1)
        return pointer1Block.block_to_payload();
    else 
        return block1.block_to_payload();
}

void *Allocator::mem_alloc(size_t size)
{
    if (size == 0)
        return NULL;
    size = alignment(size);
    if (released)
    {
        arena = Block(size, "arena_size");
        released = false;
        size_t size = arena.getCurrSize();
        block* arena_ptr = arena.getPtr();
        tree.insert(size, arena_ptr);
        return arena.block_to_payload();
    }

    //call function for searching for correct block size
    block* block_ptr = tree.findCorrectBlock(size);
    if (block_ptr)
    {
        Block block1 = block_ptr;
        blockSplitHelper(block1, size);
        block1.setBusy(1);
        return block1.block_to_payload();
    }
    return NULL;
}

void Allocator::mem_free(void *ptr) {
    if (released)
    {
        cout << "No memory allocated" << endl;
        return;
    }

    if (!ptr)
        return;

    Block block1 = arena.payload_to_block(ptr);
    
    blockMergeHelper(block1);

    bool all_free = tree.checkAllFree();
    if (all_free)
        return_memory();
}

void* Allocator::mem_realloc(void *ptr, size_t size) 
{
    size = alignment(size);
    if (!ptr || released)
        return mem_alloc(size);

    void* resultPtr = ptr;
    Block block = arena.payload_to_block(ptr);
    size_t currSize = block.getCurrSize();
    void* ptrBl = tree.findCorrectBlock(currSize);
    Block next = block.block_next();
    size_t n_c_size = next.getCurrSize();
    size_t n_size = n_c_size + currSize + BLOCK_STRUCT_SIZE;
    bool n_busy = next.isBusy();
    if (ptrBl == NULL)
    {
        return NULL;
    }
    if (currSize < size && !n_busy && n_size >= size)
    {
        blockSplitHelper(next, (size - currSize)); 
        block.setMergeOnlyNext(true);
        resultPtr = blockMergeHelper(block);
        block.setMergeOnlyNext(false);
        block.setBusy(1);
    }
    else if (size + BLOCK_STRUCT_SIZE < currSize)
        blockSplitHelper(block, size); 
    else 
    {
        resultPtr = mem_alloc(size);
        kernel.copyData(resultPtr, ptr, currSize);
        mem_free(ptr);
    }

    return resultPtr;
}

void Allocator::mem_show() 
{
    cout << "\n########MEM_SHOW#########" << endl;
    if (released)
    {
        cout << "No memory allocated" << endl;
        return;
    }

    for (Block block = arena;; block = block.block_next()) 
    {
        printf("%s %10zu %10zu %10zu%s%s\n",
            block.isBusy() ? "busy" : "free",
            block.getCurrSize(), block.getPrevSize(), block.getOffset(),
            block.isFirst() ? " first" : "",
            block.isLast() ? " last" : "");
        if (block.isLast())
            break;
    }
}

void Allocator::mem_show_tree() 
{
    cout << "\n########MEM_SHOW_TREE#########" << endl;
    if (released)
    {
        cout << "No memory allocated" << endl;
        return;
    }

    tree.printTree();
}

void Allocator::return_memory()
{
    cout << "return memory" << endl;
    released = true;
    arena.return_memory();
} 

bool Allocator::checkBlocks()
{
    bool res = true;
    for (Block block = arena;; block = block.block_next()) 
    {
        res = res && block.checkBlock();
        if (block.isLast())
            break;
    }
    if (!res)
        cout << "Testing failed!" << endl;
    else
        cout << "Testing successful!" << endl;
    return res;
}

