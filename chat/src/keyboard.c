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

#define adjusted_head(head) (head == 0 ? 16 : head)

//extern ncurses_initialized;
mutex_id kb_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

static char kb_buffer[KB_BUFFER_SIZE];
static int kb_buffer_head = KB_FIRST_HEAD, kb_buffer_tail = KB_FIRST_TAIL;

void *kb_thread(void *argument) {
    char next_character;
    //while(!ncurses_initialized);
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

bool kb_has_key(void) {
    lock(&kb_buffer_mutex);
    if(adjusted_head(kb_buffer_head) - kb_buffer_tail == 1) {
        unlock(&kb_buffer_mutex);
        return false;
    } else {
        unlock(&kb_buffer_mutex);
        return true;
    }
}

char kb_get_key(void) {
    char next_key;
    lock(&kb_buffer_mutex);
    if(!kb_has_key()) {
        unlock(&kb_buffer_mutex);
        return '\0';
    } else {
        next_key = kb_buffer[kb_buffer_tail];
        kb_buffer_tail = (kb_buffer_head + 1) & KB_BUFFER_SIZE;
        unlock(&kb_buffer_mutex);
        return next_key;
    }
}