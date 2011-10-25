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
    int x = 0;
    for(x = 0; x < 5; x++) {
        sleep(10);
        printf("Alive\n");
    }
}

void clock_tick(void) {
    
}

void string_entered(void) {
    
}

void network_ready(void) {
    
}

void message_arrived(void) {
    
}