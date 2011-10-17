/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * Network receive thread
 */

#ifndef NETWORK_RECEIVE
#define NETWORK_RECEIVE

#include <stdlib.h>

#include "network.h"

void *receive_thread(void *);
message *get_message(void);

#endif