/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * Network send thread
 */

#ifndef NETWORK_SEND
#define NETWORK_SEND

#include <stdlib.h>

#include "network.h"

void *send_thread(void *);
void send_message(message *);

#endif