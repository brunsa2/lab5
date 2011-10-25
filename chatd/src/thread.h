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
#include <string.h>

typedef pthread_t thread_id;
typedef pthread_mutex_t mutex_id;

void start_thread(pthread_t *, void *(* thread_function)(void *), void *);
void start_joinable_thread(pthread_t *, void *(* thread_function)(void *),
        void *);
void join_thread(pthread_t *);
void initialize_mutex(mutex_id *);
void lock(mutex_id *);
void unlock(mutex_id *);
void destroy_mutex(mutex_id *);

#endif