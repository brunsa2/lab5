/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 12 October 2011
 *
 * Daemonizing function to start server running
 */

#ifndef DAEMON
#define DAEMON

#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>

#define MAX_FILE_DESCRIPTOR_TO_CLOSE 8192

int become_daemon(void);

#endif