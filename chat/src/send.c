/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * Network send thread
 */

#include <stdio.h>

#include "send.h"

void *send_thread(void *argument) {
    int x;
    for(x = 0; x < 5; x++) {
        printf("Send\n");
        usleep(50000);
    }
    
    return NULL;
}