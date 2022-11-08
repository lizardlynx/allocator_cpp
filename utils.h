#pragma once
#include <iostream>
#include <allocator.h>


void buf_fill(char *c, size_t size);

void* buf_alloc(Allocator allocator, size_t size);
