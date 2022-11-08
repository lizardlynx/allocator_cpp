#include <iostream>
#include <vector>
#include <string>
#include <allocator.h>
#include <allocator_impl.h>
#include <tester.h>
#include <utils.h>

using namespace std;
size_t arena = 1073741824;

Allocator a = Allocator(arena, "arena_size");

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

    cout << "\n" << "buf alloc" << endl;
    void* ptr1 = buf_alloc(a, 20);
    cout << "ptr1 " << ptr1 << endl;
    while (i < 100) {
        size_t size = ((size_t)rand() % (SZ_MAX - SZ_MIN)) + SZ_MIN;
        void* ptr = buf_alloc(a, size);
        cout <<  i << " ptr1 " << ptr1 << endl;
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
        if (i == 396) {
            cout << "fefe" << endl;
        }
        int rand_num = 25 + (std::rand()%99);
        result rand_res = results[rand_num];
        if (rand_res.ptr == NULL) continue;
        size_t size = ((size_t)rand() % (SZ_MAX - SZ_MIN)) + SZ_MIN;
        size_t size_min = size < rand_res.size ? size : rand_res.size;
        unsigned int checksum = buf_checksum((unsigned char*)rand_res.ptr, size_min);
        a.mem_show_tree();
        // a.mem_show_tree();
        cout << " realloc " << rand_res.size << " to " << size << " size_min " << size_min << endl;
        cout << "!!!!!!!!!!!ptr1 " << rand_res.ptr << endl;
        void* ptr = a.mem_realloc(rand_res.ptr, size);
        // return false; 
        cout << "!!!!!!!!!!!ptr2 " << ptr << endl;
        if (ptr != NULL) {
            if (checksum != buf_checksum((unsigned char*)ptr, size_min)) {
                cout << "CheckSum Failed at " << rand_res.ptr << endl;
                return false;
            }
            results[rand_num].ptr = ptr;
            results[rand_num].size = size;
            results[rand_num].sum = buf_checksum((unsigned char*)ptr, size);
            cout << "after return" << results[rand_num].size << endl;
        }
        // if (i == 106) {
        //     a.mem_show();
        //     return false;
        // }
    }
    // a.mem_show_tree();

    // a.mem_realloc(results[4].ptr, 7328);
    // a.mem_show();

    //mem free check
    i = 0;
    while (i < 50)
    {
        cout << "\n" << i << endl;
        if (i == 49) {
            cout << "fefe" << endl;
        }
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
        a.mem_free(ptr);
        rand_res.ptr = NULL;
        cout << "free key " << rand_res.size << endl;
    }

    // cout << "freee ptr1" << endl;
    // a.mem_free(ptr1);
    // a.mem_free(results[0].ptr);

    a.mem_show();
    // a.mem_show_tree();

    // a.checkBlocks();
    
    return true;
};
