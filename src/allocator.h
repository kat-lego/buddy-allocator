// allocator.h
#pragma once
#ifndef OS_CUSTOM_ALLOCATOR_H
#define OS_CUSTOM_ALLOCATOR_H

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

// maximum block size in bytes: 2^20 = 1 MiB
#define MAX_EXP 20
#define MAX_SIZE 1<<MAX_EXP

// block struct
struct block {
    size_t size;                          // size in bytes
    bool free;                            // whether block is available for allocation
    void* data;                           // pointer to actual data
    struct block* next;                   // pointer to next block in list
    struct block* parent;                 // pointer to parent
   	int childen;						  // number of children
 };

// start of list
struct block* head;

// functions to be implemented in mm_alloc.c
void* custom_malloc(size_t size);
void* custom_realloc(void* ptr, size_t size);
void  custom_free(void* ptr);

//util fuctions
void create_block(struct block* newblock,struct block* parent,size_t size);
void split(struct block* blk, size_t size);
void splitHandler(struct block* parent, size_t size);

bool merge(struct block* parent);
void mergeHelper(struct block* parent, struct block* child);

// debug functions
void print_block(struct block* b);
void print_list();

#endif
