/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 13 October 2011
 *
 * Keyboard buffer system
 */

#include "keyboard.h"

#define KB_BUFFER_SIZE 16
#define KB_FIRST_TAIL 0
#define KB_FIRST_HEAD 1

#define adjusted_head(head) (head == 0 ? KB_BUFFER_SIZE : head)

extern mutex_id kb_buffer_mutex;

static char kb_buffer[KB_BUFFER_SIZE];
static int kb_buffer_head = KB_FIRST_HEAD, kb_buffer_tail = KB_FIRST_TAIL;

static void print_buffer(void) {
    int x = 0;
    printf("-----------\n");
    for(; x < KB_BUFFER_SIZE; x++) {
        printf("%c", kb_buffer[x]);
        if(x == kb_buffer_head) {
            printf(" <--H");
        }
        if(x == kb_buffer_tail) {
            printf(" <--T");
        }
        printf("\n");
    }
    printf("-----------\n");
}

void *kb_thread(void *argument) {
    char next_character;
    while(true) {
        next_character = getchar();
        lock(&kb_buffer_mutex);
        while(kb_buffer_head == kb_buffer_tail);
        kb_buffer[kb_buffer_head] = next_character;
        kb_buffer_head = (kb_buffer_head + 1) % KB_BUFFER_SIZE;
        unlock(&kb_buffer_mutex);
    }
    
    return NULL;
}

bool has_key(void) {
    lock(&kb_buffer_mutex);
    if(adjusted_head(kb_buffer_head) - kb_buffer_tail == 1) {
        unlock(&kb_buffer_mutex);
        return false;
    } else {
        unlock(&kb_buffer_mutex);
        return true;
    }
}