/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * Tick counting thread
 */

#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "tick.h"
#include "controller.h"

#define MICROSECONDS_BETWEEN_CLOCK_UPDATE 1000000

static start_ticks, elapsed_ticks;

void *tick_thread(void *argument) {
    start_ticks = time(NULL);
    
    while(true) {
        elapsed_ticks = time(NULL) - start_ticks;
        usleep(MICROSECONDS_BETWEEN_CLOCK_UPDATE);
        clock_tick();
    }
    
    return NULL;
}

int get_elapsed_ticks(void) {
    return elapsed_ticks;
}