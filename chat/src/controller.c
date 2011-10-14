/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * Main controller thread
 */

#include "controller.h"

void ctrl_thread(void) {
    int x;
    for(x = 0; x < 1000; x++) {
        if(has_key()) {
            printf("Has key\n");
        }
        usleep(10000);
    }
}

void clock_tick(void) {
    
}