#include <iostream>
#include <vector>
#include <string>
#include <allocator.h>
#include <allocator_impl.h>
#include <tester.h>
#include <utils.h>

using namespace std;
size_t arena = 1073741824;

Allocator* a = new Allocator(arena, "arena_size");

unsigned int Tester::buf_checksum(unsigned char *c, size_t size)
{
    unsigned int checksum = 0;

    while (size--)
        checksum = (checksum << 5) ^ (checksum >> 3) ^ *c++;
    return checksum;
}

bool Tester::test()
{
    int i = 0;
    const size_t SZ_MAX = 10000;
    const size_t SZ_MIN = 10;

    while (i < 100) {
        size_t size = ((size_t)rand() % (SZ_MAX - SZ_MIN)) + SZ_MIN;
        void* ptr = buf_alloc(a, size);
        result r;
        r.size = size;
        r.ptr = ptr;
        r.sum = buf_checksum((unsigned char*)ptr, size);
        results[i] = r;
        i++;
    }
    
    //check mem realoc
    while (i < 200)
    {
        cout << i << endl;
        i++;
        // if (i == 106) {
        //     cout << "for debug" << endl;
        // }
        int rand_num = 25 + (std::rand()%99);
        result rand_res = results[rand_num];
        if (rand_res.ptr == NULL) continue;
        size_t size = ((size_t)rand() % (SZ_MAX - SZ_MIN)) + SZ_MIN;
        size_t size_min = size < rand_res.size ? size : rand_res.size;
        unsigned int checksum = buf_checksum((unsigned char*)rand_res.ptr, size_min);
        void* ptr = a->mem_realloc(rand_res.ptr, size);
        if (ptr != NULL) {
            if (checksum != buf_checksum((unsigned char*)ptr, size_min)) {
                cout << "CheckSum Failed at " << rand_res.ptr << endl;
                return false;
            }
            results[rand_num].ptr = ptr;
            results[rand_num].size = size;
            results[rand_num].sum = buf_checksum((unsigned char*)ptr, size);
        }
    }

    //mem free check
    i = 0;
    while (i < 50)
    {
        cout << "\n" << i << endl;
        // if (i == 49) {
        //     cout << "for debug" << endl;
        // }
        int rand_num = i;
        result rand_res = results[rand_num];
        i++;
        if (rand_res.ptr == NULL) continue;
        if (rand_res.sum != buf_checksum((unsigned char*)rand_res.ptr, rand_res.size))
        {
            cout << "CheckSum Failed at " << rand_res.ptr << endl;
            return false;
        }
        void * ptr = rand_res.ptr;
        a->mem_free(ptr);
        rand_res.ptr = NULL;
    }

    a->mem_show();
    // a->mem_show_tree();
    
    return true;
};
