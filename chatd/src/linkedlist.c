/*
 * Jeff Stubler
 * CS 3841
 * A Linked List in C
 * 21 September 2011
 *
 * Defines implementations of functions needed to initialize, modify, and
 * traverse a doubly-linked list.
 */

#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "linkedlist.h"

/* Initialize linked list structure
  *
  * Takes: pointer to linked list
  * Gives: initialized empty linked list
 */
void ll_init(struct linkedList *linked_list) {
    if(!linked_list) {
        return;
    }
    
    linked_list->head = NULL;
    linked_list->tail = NULL;
    linked_list->size = 0;
}

/* Clears linked list structure including freeing memory
  *
  * Takes: pointer to linked list
  * Gives: cleared linked list and freed memory
 */
void ll_clear(struct linkedList *linked_list) {
    if(!(linked_list && linked_list->head && linked_list->tail)) {
        return;
    }
    
    struct listNode *current = linked_list->tail;
    while(current->next) {
        free(current->data);
        current = current->next;
        free(current->previous);
    }
    free(current->data);
    free(current);
    linked_list->head = NULL;
    linked_list->tail = NULL;
    linked_list->size = 0;
}

/* Adds data of specified size to end of linked list
  *
  * Takes: pointer to linked list, pointer to data, and size of data in bytes
  * Gives: true if success, false if failure
 */
bool ll_add(struct linkedList *linked_list, void *object, int size) {
    struct listNode *new_node;
    
    if(!(linked_list && object)) {
        return false;
    }
    
    new_node = (struct listNode *) malloc(sizeof(struct listNode));
    if(new_node == NULL) {
        return false;
    }
    
    new_node->data = (void *) malloc(size);
    if(new_node->data == NULL) {
        free(new_node);
        return false;
    }
    
    memcpy(new_node->data, object, size);
    new_node->dataSize = size;
    new_node->next = NULL;
    new_node->previous = linked_list->tail;
    if(linked_list->tail) {
        linked_list->tail->next = new_node;
    }
    if(!linked_list->head) {
        linked_list->head = new_node;
    }
    linked_list->tail = new_node;
    linked_list->size++;
    
    return true;
}

/* Adds data of specified size to specified position in linked list (index is position where new item appears)
 *
 * Takes: pointer to linked list, pointer to data, size of data in bytes, index of item in list
 * Gives: true if success, false if failure
 */

bool ll_addIndex(struct linkedList *linked_list, void *object, int size, int index) {
    int traversingIndex;
    struct listNode *node_after_new_node, *new_node;
    
    if(!(linked_list && object && index >= 0 && index < ll_size(linked_list))) {
        return false;
    }
    
    node_after_new_node = linked_list->head;
    for(traversingIndex = 0; traversingIndex < index; traversingIndex++) {
        node_after_new_node = node_after_new_node->next;
    }
    
    new_node = (struct listNode *) malloc(sizeof(struct listNode));
    if(new_node == NULL) {
        return false;
    }
    
    new_node->data = (void *) malloc(size);
    if(new_node->data == NULL) {
        free(new_node);
        return false;
    }
    
    memcpy(new_node->data, object, size);
    new_node->dataSize = size;
    new_node->previous = node_after_new_node->previous;
    new_node->next = node_after_new_node;
    if(node_after_new_node->previous) {
        node_after_new_node->previous->next = new_node;
    }
    node_after_new_node->previous = new_node;
    if(index == 0) {
        linked_list->head = new_node;
    }
    linked_list->size++;
    
    return true;
}

/* Removes the list item at given index
 *
 * Takes: pointer to linked list, index to remove
 * Gives: true if successful, false otherwise
 */

bool ll_remove(struct linkedList *linked_list, int index) {
    int traversingIndex;
    struct listNode *node_to_delete;
    
    if(!(linked_list && index >= 0 && index < ll_size(linked_list))) {
        return false;
    }
    
    node_to_delete = linked_list->head;
    for(traversingIndex = 0; traversingIndex < index; traversingIndex++) {
        node_to_delete = node_to_delete->next;
    }
    
    if(node_to_delete->next) {
        node_to_delete->next->previous = node_to_delete->previous;
    }
    if(node_to_delete->previous) {
        node_to_delete->previous->next = node_to_delete->next;
    }
    if(index == 0) {
        linked_list->head = node_to_delete->next;
    }
    if(index == ll_size(linked_list) - 1) {
        linked_list->tail = node_to_delete->previous;
    }
    linked_list->size--;
    
    return true;
}

/* Retrieves data from the linked list
  *
  * Takes: pointer to linked list, index
  * Gives: null if failure, or pointer to data at specified index
 */
void *ll_get(struct linkedList *linked_list, int index) {
    int node_index;
    struct listNode *current;
    
    if(!linked_list) {
        return NULL;
    }
    if(index < 0 || index >= linked_list->size) {
        return NULL;
    }
    if(!linked_list->head) {
        return NULL;
    }
    
    current = linked_list->head;
    for(node_index = 0; node_index < index; node_index++) {
        if(!current->next) {
            return NULL;
        }
        current = current->next;
    }
    
    return current->data;
}

/* Gets an iterator for the linked list
 *
 * Takes: pointer to linked list
 * Gives: pointer to linked list iterator
 */

struct linkedListIterator *ll_getIterator(struct linkedList *linked_list) {
    struct linkedListIterator *linked_list_iterator;
    
    if(!linked_list) {
        return NULL;
    }
    if(!(linked_list->head && linked_list->tail)) {
        return NULL;
    }
    
    linked_list_iterator = (struct linkedListIterator *) malloc(sizeof(struct linkedListIterator));
    if(linked_list_iterator == NULL) {
        return false;
    }
    
    linked_list_iterator->current = linked_list->head;
    return linked_list_iterator;
}

/* Checks if there's another list item to iterate over
 *
 * Takes: pointer to linked list iterator
 * Gives: true if there is another list item to iterate over
 */

bool ll_hasNext(struct linkedListIterator *linked_list_iterator) {
    if(!(linked_list_iterator && linked_list_iterator->current)) {
        return false;
    }
    if(linked_list_iterator->current) {
        return true;
    }
}

/* Returns the next item in the linked list from the iterator
 *
 * Takes: pointer to the linked list iterator
 * Gives: pointer to the next data in the linked list
 */

void *ll_next(struct linkedListIterator *linked_list_iterator) {
    void *data;
    
    if(!(linked_list_iterator && linked_list_iterator->current)) {
        return NULL;
    }
    
    data = linked_list_iterator->current->data;
    linked_list_iterator->current = linked_list_iterator->current->next;
    return data;
}

/* Returns the size of the linked list
 *
 * Takes: pointer to linked list
 * Gives: size as number of items on linked list
 */

int ll_size(struct linkedList *linked_list) {
    if(!linked_list) {
        return 0;
    }
    if(!(linked_list->head && linked_list->tail)) {
        return 0;
    }
    
    return linked_list->size;
}