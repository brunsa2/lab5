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
#include <pthread.h>

#include "thread.h"

void *kb_thread(void *);
bool kb_has_key(void);
char kb_get_key(void);

#endif