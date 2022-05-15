/**
 * Generic linked list data structure
 * 
 * This header file declares a simple FIFO linked list with
 * generic data types. It is supposed to accept push_back,
 * pop_front, front, size and constructor operations.
 * 
 * Author: Guilherme Kowalczuk
 */ 


#ifndef __LISTS_H__
#define __LISTS_H__

typedef struct ll ll;

// Push an element to the end of the linked list
void push(ll* list, void* value);

// Pop an element from the beggining of the linked list
void* pop(ll* list);

// Return the first element from the linked list
void* front(ll* list);

// Return the number of elements
int size(ll* list);

// Constructor method for with corresponding malloc functions
ll* linked_list();

#endif