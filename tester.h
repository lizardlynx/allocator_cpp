#pragma once
#include <iostream>
#include <allocator.h>

struct result
{
    void* ptr;
    size_t size;
    int sum;
};

class Tester
{
private:
    result* results = new result[1000];
    unsigned int buf_checksum(unsigned char *c, size_t size);
public:
    bool test();
};



