/*
 * Jeff Stubler & Matt Edwards
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 18 October 2011
 *
 * User interface thread
 */

#ifndef UI
#define UI

#include <stdlib.h>
#include <ncurses.h>
#include <string.h> 

void *ui_thread(void *);
void refresh_all();

#endif
