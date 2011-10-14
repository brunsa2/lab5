/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 12 October 2011
 *
 * Thread convenience functions, wraps around Pthreads
 */

#include "thread.h"

/* TODO: Look further at thread priority */

void start_thread(pthread_t *thread, void *(* thread_function)(void *),
                         void *argument) {
    int thread_error_code;
    pthread_attr_t attributes;
    
    thread_error_code = pthread_attr_init(&attributes);
    if(thread_error_code != 0) {
        fatal_shutdown("Error initializing thread attributes");
    }
    
    thread_error_code = pthread_attr_setdetachstate(&attributes,
            PTHREAD_CREATE_DETACHED);
    if(thread_error_code != 0) {
        fatal_shutdown("Error setting thread detach state attribute");
    }
    
    thread_error_code = pthread_create(thread, &attributes, thread_function,
            argument);
    if(thread_error_code != 0) {
        fatal_shutdown("Error starting thread");
    }
    
    thread_error_code = pthread_attr_destroy(&attributes);
    if(thread_error_code != 0) {
        fatal_shutdown("Error destroying attributes");
    }
}