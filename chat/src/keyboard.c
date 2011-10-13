/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 13 October 2011
 *
 * Keyboard buffer system
 */

#include <stdio.h>

#include "keyboard.h"

void *kb_thread(void *argument) {
    int x;
    for(x = 0; x < 10; x++) {
        printf("Keyboard\n");
        usleep(100000);
    }
    
    return NULL;
}