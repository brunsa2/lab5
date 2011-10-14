/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * Main controller thread
 */

#include <stdio.h>

#include "controller.h"

void *ctrl_thread(void *argument) {
    int x;
    for(x = 0; x < 5; x++) {
        printf("Controller\n");
        usleep(50000);
    }
    
    return NULL;
}