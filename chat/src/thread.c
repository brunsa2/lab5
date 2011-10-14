/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 12 October 2011
 *
 * Thread convenience functions, wraps around Pthreads
 */

#include "thread.h"

void start_thread(pthread_t *thread, void *(* thread_function)(void *),
                         void *argument) {
    int thread_error_code;
    thread_error_code = pthread_create(thread, NULL, thread_function, argument);
    if(thread_error_code != 0) {
        fatal_shutdown("Error starting thread");
    }
}

void *join_thread(pthread_t *thread) {
    int thread_error_code;
    void *thread_return_value;
    thread_error_code = pthread_join(*thread, &thread_return_value);
    if(thread_error_code != 0) {
        fatal_shutdown("Error joining thread");
    }
    return thread_return_value;
}