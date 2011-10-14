/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * Network startup thread
 */

#include <stdio.h>

#include "thread.h"
#include "network.h"
#include "send.h"
#include "receive.h"

void *network_thread(void *argument) {
    thread_id send_thread_id, receive_thread_id;
    int x;
    
    start_thread(&send_thread_id, send_thread, NULL);
    start_thread(&receive_thread_id, receive_thread, NULL);
    
    for(x = 0; x < 5; x++) {
        printf("Network\n");
        usleep(50000);
    }
    
    return NULL;
}