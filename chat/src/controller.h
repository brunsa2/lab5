/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * Main controller thread
 */

#ifndef CONTROLLER
#define CONTROLLER

#include <stdio.h>

#include "tick.h"
#include "keyboard.h"

void ctrl_thread(void);
void clock_tick(void);

#endif