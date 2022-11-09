#include <iostream>
#include <vector>
#include <string>
#include <block.h>
#include <kernel.h>
#include <config.h>

using namespace std;

int arenaSize = ARENA_SIZE;

bool Block::merge_i = false;

void Block::create_block()
{
    Kernel k;
    size_t size = arenaSize;
    b = (struct block*)k.malloc(size);

    setCurrSize(size - BLOCK_STRUCT_SIZE);
    setPrevSize(0);
    setOffset(BLOCK_STRUCT_SIZE);
    setFirst(1);
    setLast(1);
    setBusy(0);
}

Block::Block()
{
    create_block();
}

Block::Block(block* block)
{
    b = block;
}

// Якщо пам’ять комп’ютера сторінкова, тоді в
// ядра можливо запитати цілу кількість сторінок, інакше в ядра можливо запитати 1 байт або
// більше.
Block::Block(size_t var_int, std::string var_name)
{
    if (var_name == "arena_size")
    {
        if((PAGE_TYPE_OS == 0 && var_int > 1) ||
          PAGE_TYPE_OS == 1)
            arenaSize = var_int;
    }
    else if (var_name == "page_size")
    {
        if (PAGE_TYPE_OS == 1)
            arenaSize = var_int * ALLOCATOR_ARENA_PAGES;
    }
    
    create_block();
}

Block::Block(block* block, bool ifnew)
{
    if (ifnew)
    {
        b = block;
        setCurrSize(0);
        setPrevSize(0);
        setOffset(BLOCK_STRUCT_SIZE);
        setFirst(0);
        setLast(0);
        setBusy(0);
    }
    else
        block = b;
}

std::vector<block*> Block::block_merge_helper(Block bl)
{
    merge_i = true;
    std::vector<block*> pointers = bl.block_merge();
    merge_i = false;
    return pointers;
} 

// об’єднання блоку та його правого сусіда, якщо обидва вільні, треба для mem_free()
std::vector<block*> Block::block_merge() 
{
    std::vector<block*> merged_blocks_pointers(3, NULL);
    if (isBusy()) 
        return merged_blocks_pointers;
    Block next = block_next();
    size_t c_size = getCurrSize();
    size_t merged_block_size = c_size;

    merged_blocks_pointers[1] = getPtr();

    if(!isLast() && !next.isBusy())
    {
        size_t n_curr_size = next.getCurrSize();
        size_t n_size = n_curr_size + BLOCK_STRUCT_SIZE;
        setCurrSize(c_size + n_size);
        merged_blocks_pointers[2] = next.getPtr();
        if (next.isLast())
            setLast(1);
        else 
        {
            Block nnext = next.block_next();
            nnext.setPrevSize(c_size + n_size);
        }
        
    }

    if (merge_only_next == false && merge_i == false && !isFirst())
    {
        Block prev = block_prev();
        std::vector<block*> pointers = block_merge_helper(prev);
        merged_blocks_pointers[0] = pointers[1];
    }

    return merged_blocks_pointers;
}

struct block* Block::block_next() 
{
    void* ptr = (char*)b + BLOCK_STRUCT_SIZE + getCurrSize();
    return (struct block*)ptr;
}

block* Block::block_prev() 
{
    return (block*)
        ((char *)b - BLOCK_STRUCT_SIZE - getPrevSize());
}

block* Block::block_split(size_t size)
{
    struct block *block2_struct = NULL;
    size_t size_curr;

    size_curr = getCurrSize();
    if (size_curr >= size + BLOCK_STRUCT_SIZE) 
    {
        size_curr -= size + BLOCK_STRUCT_SIZE;

        if (size_curr > 0)
        {
            setCurrSize(size);
            block2_struct = block_next();
            Block block2 = Block(block2_struct, true);
            block2.setCurrSize(size_curr);
            block2.setPrevSize(size);
            block2.setOffset(getOffset() + getCurrSize() + BLOCK_STRUCT_SIZE);
            if (isLast()) 
            {
                setLast(0);
                if (size_curr > 0)
                    block2.setLast(1);
            } 
            else {
                block2.setLast(0);
                Block block3 = block2.block_next();
                block3.setPrevSize(size_curr);
            }
        }
    }
    return block2_struct;
}

void* Block::block_to_payload()
{
    return (char *)b + BLOCK_STRUCT_SIZE;
}

Block Block::payload_to_block(void* ptr)
{
    block* block_ptr = (block*)((char*)ptr - BLOCK_STRUCT_SIZE);
    return (Block)block_ptr;
}

void Block::return_memory()
{
    Kernel k;
    k.mrelease(b, getCurrSize());
}

bool Block::checkBlock() 
{
    Block next = block_next();
    Block prev = block_prev();

    bool p_size = true;
    bool c_size = true;
    bool o = true;

    // check previous size
    if (!isFirst() && prev.getCurrSize() != getPrevSize())
        p_size = false;

    //check curr size
    if (!isLast() && getCurrSize() != next.getPrevSize())
        c_size = false;

    //check offset
    if (isFirst() && getOffset() != BLOCK_STRUCT_SIZE)
        o = false;
    else if (!isFirst() && getOffset() != prev.getOffset() + BLOCK_STRUCT_SIZE + prev.getCurrSize())
        o = false;

    bool res = p_size && c_size && o;
    cout << "prev_size -> " 
    << p_size << " | curr_size -> " << c_size << " | offset -> " << o 
    << " | ##res## -> " << res << " | block key " << getCurrSize() << endl;

    return res;
}

void Block::setOffset(size_t size) {b->offset = size;}
size_t Block::getOffset() {return b->offset;}
size_t Block::getCurrSize() {return b->currSize;}
void Block::setCurrSize(size_t size) {b->currSize = size;}
size_t Block::getPrevSize() {return b->prevSize;}
void Block::setPrevSize(size_t size) {b->prevSize = size;}
bool Block::isBusy() {return b->busy;}
void Block::setBusy(bool busyArg) {b->busy = busyArg;}
bool Block::isFirst() {return b->first;}
void Block::setFirst(bool firstArg) {b->first = firstArg;}
bool Block::isLast() {return b->last;}
void Block::setLast(bool lastArg) {b->last = lastArg;}
block* Block::getPtr() {return b;}
void Block::setMergeOnlyNext(bool mer) {merge_only_next = mer;};
