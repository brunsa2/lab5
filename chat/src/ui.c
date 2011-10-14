/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * User interface thread
 */

#include <stdio.h>

#include "ui.h"

void *ui_thread(void *argument) {
    int x;
    for(x = 0; x < 5; x++) {
        printf("User interface\n");
        usleep(50000);
    }
    
    return NULL;
}