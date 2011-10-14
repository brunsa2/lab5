/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 13 October 2011
 *
 * Keyboard buffer system
 */

#ifndef KEYBOARD
#define KEYBOARD

#include <stdio.h>
#include <stdbool.h>

#include "thread.h"

void *kb_thread(void *);
bool has_key(void);

#endif