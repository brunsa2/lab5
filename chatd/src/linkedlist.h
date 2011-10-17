/*
 * Jeff Stubler
 * CS 3841
 * A Linked List in C
 * 21 September 2011
 *
 * Defines data structures and functions needed for the doubly-linked list.
 */

#include <stdbool.h>

#ifndef LINKED_LIST
#define LINKED_LIST

/* Structure for each item in the list */
struct listNode {
    struct listNode *previous, *next;
    int dataSize;
    void *data;
};

/* Structure defining the actual list */
struct linkedList {
    int size;
    struct listNode *head, *tail;
};

/* Structure for iterating through list */
struct linkedListIterator {
    struct listNode *current;
};

/* Initialize linked list structure
  *
  * Takes: pointer to linked list
  * Gives: initialized empty linked list
 */
void ll_init(struct linkedList *);

/* Clears linked list structure including freeing memory
  *
  * Takes: pointer to linked list
  * Gives: cleared linked list and freed memory
 */
void ll_clear(struct linkedList *);

/* Adds data of specified size to end of linked list
  *
  * Takes: pointer to linked list, pointer to data, and size of data in bytes
  * Gives: true if success, false if failure
 */
bool ll_add(struct linkedList *, void *, int);

/* Adds data of specified size to specified position in linked list (index is position where new item appears)
 *
 * Takes: pointer to linked list, pointer to data, size of data in bytes, index of item in list
 * Gives: true if success, false if failure
 */
bool ll_addIndex(struct linkedList *, void *, int, int);

/* Removes the list item at given index
 *
 * Takes: pointer to linked list, index to remove
 * Gives: true if successful, false otherwise
 */
bool ll_remove(struct linkedList *, int);

/* Retrieves data from the linked list
  *
  * Takes: pointer to linked list, index
  * Gives: null if failure, or pointer to data at specified index
 */
void *ll_get(struct linkedList *, int);

/* Gets an iterator for the linked list
 *
 * Takes: pointer to linked list
 * Gives: pointer to linked list iterator
 */
struct linkedListIterator *ll_getIterator(struct linkedList *); 

/* Checks if there's another list item to iterate over
 *
 * Takes: pointer to linked list iterator
 * Gives: true if there is another list item to iterate over
 */
bool ll_hasNext(struct linkedListIterator *);

/* Returns the next item in the linked list from the iterator
 *
 * Takes: pointer to the linked list iterator
 * Gives: pointer to the next data in the linked list
 */
void *ll_next(struct linkedListIterator *);

/* Returns the size of the linked list
 *
 * Takes: pointer to linked list
 * Gives: size as number of items on linked list
 */
int ll_size(struct linkedList *);

#endif