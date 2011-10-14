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
#include "tick.h"

void ctrl_thread(void) {
    int x;
    for(x = 0; x < 10; x++) {
        usleep(1000000);
    }
}

void clock_tick(void) {
    
}