/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 12 October 2011
 *
 * Thread convenience functions, wraps around Pthreads
 */

#include "thread.h"

#define fatal_shutdown(message) fprintf(stderr, "%s\n", message); \
exit(EXIT_FAILURE);

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

void start_joinable_thread(pthread_t *thread, void *(* thread_function)(void *),
                           void *argument) {
    int thread_error_code;
    
    thread_error_code = pthread_create(thread, NULL, thread_function, argument);
    if(thread_error_code != 0) {
        fatal_shutdown("Error starting thread");
    }
}

void join_thread(pthread_t *thread) {
    pthread_t joined_thread;
    memcpy(&joined_thread, thread, sizeof(thread));
    pthread_join(joined_thread, NULL);
}

void initialize_mutex(mutex_id *mutex) {
    int mutex_error_code;
    mutex_error_code = pthread_mutex_init(mutex, NULL);
    if(mutex_error_code != 0) {
        fatal_shutdown("Error initializing mutex");
    }
}

void lock(mutex_id *mutex) {
    pthread_mutex_lock(mutex);
}

void unlock(mutex_id *mutex) {
    pthread_mutex_unlock(mutex);
}

void destroy_mutex(mutex_id *mutex) {
    int mutex_error_code;
    unlock(mutex);
    mutex_error_code = pthread_mutex_destroy(mutex);
    if(mutex_error_code != 0) {
        fatal_shutdown("Error destroying mutex");
    }
}