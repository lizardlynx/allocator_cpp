#pragma once

#if defined(_WIN32)
#define OS 1
#include <windows.h>
#elif defined(__unix__)
#define OS 2
#endif

// check if has pages variable
//#define HAS_PAGES_OS

// макроси для розміру сторінки, розміру default арени.
#include <unistd.h>

#ifndef _SC_PAGE_SIZE
#define ALLOCATOR_PAGE_SIZE 4096;
#define PAGE_TYPE_OS 0;
#else 
#define ALLOCATOR_PAGE_SIZE sysconf(_SC_PAGE_SIZE)
#define PAGE_TYPE_OS 1
#endif

#define ALLOCATOR_ARENA_PAGES 16

#if PAGE_TYPE_OS == 1
#define ARENA_SIZE (ALLOCATOR_PAGE_SIZE * ALLOCATOR_ARENA_PAGES)
#else
#define ARENA_SIZE 4096*16
#endif
