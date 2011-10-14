/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * Network startup thread
 */

#include "network.h"

void *network_thread(void *argument) {
    thread_id send_thread_id, receive_thread_id;
    
    start_thread(&send_thread_id, send_thread, NULL);
    start_thread(&receive_thread_id, receive_thread, NULL);
    
    return NULL;
}