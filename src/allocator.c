// allocator.c
#define _DEFAULT_SOURCE
#include "allocator.h"
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>

/** Allocate a block of at least the requested size **/
void* custom_malloc(size_t size) {
    // TODO

    //return null if size requested is 0 or the size requested cannot fit
    if(size == 0 || size>=MAX_SIZE)return NULL;

     //if there are no blocks create a new block with max block size;
    if(head == NULL){      
        head = (struct block*) sbrk(MAX_SIZE);
        create_block(head,NULL, MAX_SIZE); 
    }
    size+= sizeof(struct block); // account for the storage of the block

    //find the smallest block that can fit the block and requested size
    struct block* curr_block = head; 
    struct block* min_fit = NULL;
    while(curr_block){
        if(curr_block->free && curr_block->size>=size && ( min_fit==NULL || min_fit->size > curr_block->size) ){
            min_fit = curr_block;
        }
        curr_block = curr_block->next;
    }

    //if there is not enough space
    if(!min_fit){
        return NULL;
    }
    min_fit->free = false; // allocate min_fit
    split(min_fit, size);

    return min_fit->data;
}

/** Mark a data block as free and merge free buddy blocks **/
void custom_free(void* ptr) {

    //find block storing ptr
    struct block* curr = head;
    while(curr && curr->data!=ptr){
        curr = curr->next;
    }

    //do nothing if ptr is an invalid ptr
    if(!curr){
        return;
    }

    //free and merge
    curr->free = true;
    while(curr && curr->free){
        curr = (merge(curr))? curr->parent: NULL;
    }

}

/** Change the memory allocation of the data to have at least the requested size **/
void* custom_realloc(void* ptr, size_t size) {
    //find block storing ptr
    struct block* curr = head;
    while(curr && curr->data!=ptr){
        curr = curr->next;
    }
    //if ptr is null or invalid return null

    if(!curr){
        return NULL;
    }

    //if the block can fit the data and the matadata then use the same block
    if(curr->size >= size+sizeof(struct block*)){
        split(curr, size+sizeof(struct block*));
        return curr;
    }

    //allocate at a new block
    void *new_ptr = custom_malloc(size);

    //free the old block and merge
    custom_free(ptr);

    // if no space was found try again having freed and merged some blocks
    if(new_ptr == NULL){
        new_ptr = custom_malloc(size);
    }

    // if trying again was successful
    if(new_ptr != NULL){
        memcpy(new_ptr, curr->data, size-sizeof(struct block*));
    }

    return new_ptr;
}

/*------------------------------------*\
|            UTIL FUNCTIONS           |
\*------------------------------------*/
void create_block(struct block* newblock, struct block* parent,size_t size){

    newblock->size = size;
    newblock->free = true;
    newblock->data = (void*)newblock+sizeof(struct block); // make sure poiter types are comparable
    newblock->next = NULL;
    newblock->parent = parent;
    newblock->childen = 0;

}

void split(struct block* blk, size_t size){
    //down size the smallest block that can fit the block and requested size
    size_t half_size = blk->size>>1; // me an intellectual dividing by 2 
    while( half_size >= size){
        splitHandler(blk, half_size);
        half_size >>= 1; // me an intellectual dividing by 2
    }
}

void splitHandler(struct block* parent, size_t size){
    struct block* b =(struct block*) ((void*) parent+size);
    create_block(b,parent,size);
    b->next = parent->next;

    parent->size = size;
    parent->next = b;
    parent->childen++;
}

/**
 * Merge root with all blocks created from root
 * return true if all blocks created from root where merged and false otherwise 
 */
bool merge(struct block* root){
    while(root->childen>0 && root->next->free && root->next->childen==0)
        mergeHelper(root, root->next);

    if(root->childen == 0)return true;
    else return false;
}

void mergeHelper(struct block* parent, struct block* child){

    parent->size<<=1;
    parent->next = child->next;
    parent->childen--;
    
}


/*------------------------------------*\
|            DEBUG FUNCTIONS           |
\*------------------------------------*/

/** Prints the metadata of a block **/
void print_block(struct block* b) {
    if(!b) {
        printf("NULL block\n");
    }
    else {
        printf("Strt = %p\n",b);
        printf("Size = %lu\n",b->size);
        printf("Free = %s\n",(b->free)?"true":"false");
        printf("Data = %p\n",b->data);
        printf("Next = %p\n",b->next);
        printf("parent = %p\n",b->parent);
        printf("childen = %d\n",b->childen);    
        printf("\n");
    }
}

/** Prints the metadata of all blocks **/
void print_list() {
    struct block* curr = head;
    printf("--HEAP--\n");
    if(!head) printf("EMPTY\n");
    while(curr) {
        print_block(curr);
        curr = curr->next;
    }
    printf("--END--\n");
}
