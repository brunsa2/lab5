/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * Network receive thread
 */

#include <stdio.h>

#include "receive.h"

void *receive_thread(void *argument) {
    int x;
    for(x = 0; x < 5; x++) {
        printf("Receive\n");
        usleep(50000);
    }
    
    return NULL;
}