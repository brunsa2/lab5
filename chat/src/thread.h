/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 12 October 2011
 *
 * Thread convenience functions, wraps around Pthreads
 */

#ifndef THREADS
#define THREADS

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define fatal_shutdown(message) fprintf(stderr, "%s\n", message); \
        exit(EXIT_FAILURE);

typedef pthread_t thread_id;

void start_thread(pthread_t *, void *(* thread_function)(void *), void *);
    
#endif
