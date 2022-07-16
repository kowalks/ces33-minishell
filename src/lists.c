#include <stdlib.h>
#include "lists.h"

typedef struct node_t {
    void* value;
    struct node_t* next;
} node_t;

typedef struct ll {
    node_t* head;
    node_t* tail;
    int size;
} ll;

void push(ll* list, void* value) {
    node_t* node = malloc(sizeof(node));
    node->value = value;
    node->next = NULL;
    if (list->head == NULL)
        list->head = node;
    else
        list->tail->next = node;
    list->tail = node;
    list->size++;
}

void* pop(ll* list) {
    if (list->head == NULL)
        return NULL;
    node_t* node = list->head;
    list->head = list->head->next;
    void* value = node->value;
    free(node);
    list->size--;
    return value;
}

void* front(ll* list) {
    if (list->head == NULL)
        return NULL;
    return list->head->value;
}

int size(ll* list) {
    return list->size;
}

ll* linked_list() {
    ll *list = malloc(sizeof(ll));
    list->head = list->tail = NULL;
    list->size = 0;
    return list;
}