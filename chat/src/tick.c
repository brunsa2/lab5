/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * Tick counting thread
 */

#include <stdio.h>

#include "tick.h"

void *tick_thread(void *argument) {
    int x;
    for(x = 0; x < 5; x++) {
        printf("Tick\n");
        usleep(50000);
    }
    
    return NULL;
}