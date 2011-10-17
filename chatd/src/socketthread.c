/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 15 October 2011
 *
 * Main thread for each socket
 */

#include "socketthread.h"

void *socket_thread(void *thread) {
    syslog(LOG_INFO, "This thread will be using socket %d",
            ((thread_data *) thread)->socket);
    return NULL;
}