#include <iostream>
#include <vector>
#include <string>
#include <allocator.h>
#include <tester.h>
#include <utils.h>
#include <tree.h>

using namespace std;

//
//1. release kernel memory +
// kernel calls based on os +-
// page type memory check +
//2. notify kernel about free page +?
//3. automatic tester
//4. add tree +
// do we need to align size when allocating memory for the first time?
// перевіряємо, що код для дерева не отримує та не звільняє динамічну пам’ять
// можливість створювати арени з одним блоком більшим за максимальний розмір блоку в default арен

int main()
{
    Tester t;
    t.test();
    // Allocator a = Allocator(4000, "arena_size");
    // cout << "\n------1-------" << endl;
    // void* ptr = buf_alloc(a, 45);
    // void* ptr2 = buf_alloc(a, 44);
    // // void* ptr3 = buf_alloc(a, 3816);
    
    
    // a.mem_show_tree();
    //  a.mem_show();
    // // cout << *(char**)ptr << endl;

    // cout << "\n------2-------" << endl;
    // a.mem_free(ptr);
    // a.mem_show();
    // a.mem_show_tree();

    // cout << "\n------3-------" << endl;
    // a.mem_free(ptr2);
    
    // a.mem_show();

    // // cout << "\n------4-------" << endl;
    // // a.mem_free(ptr3);
    
    // // a.mem_show();

    
    // // void* ptr3 = buf_alloc(a, 600);
    // // void* ptr4 = buf_alloc(a, 3);
    // // // // a.mem_free(ptr);
    // // a.mem_show();

    // cout << "\n------5-------" << endl;
    // a.mem_realloc(ptr2, 300);
    // // a.mem_alloc(48);
    // a.mem_show();

    // Tester t;
    // t.test();
}
