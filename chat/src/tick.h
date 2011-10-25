/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * Tick counting thread
 */

#ifndef TICK
#define TICK

#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "controller.h"

void *tick_thread(void *);
int get_elapsed_ticks(void);

#endif