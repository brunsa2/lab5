/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 12 October 2011
 *
 * Definitions and prototypes for chat daemon
 */

#ifndef CHATD
#define CHATD

#include <stdio.h>
#include <syslog.h>
#include <stdbool.h>

#include "daemon.h"

void signal_handler(int);

#endif