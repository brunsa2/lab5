/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 12 October 2011
 *
 * Main entry point for client, gathers arguments, starts threads, and acts as
 * main controller for system
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "keyboard.h"

/* Variables for parsing command line arguments */
extern int optind, opterr, optopt;
extern char *optarg;

/* Data for logging into server to be handled by send/receive system */
typedef struct {
    char *username;
    char *server;
    char *port;
} login_info;

login_info login;

static void fatal_shutdown(char *message) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

static void print_usage_and_exit_with_code(int exit_status) {
    fprintf(stderr, "Usage: chat [OPTION]... host\n"
            "Talk among other users connected to a chat server.\n\n"
            "-u\tSet username to be displayed to chat members\n"
            "-p\tSpecify port to connect to server on\n");
    exit(exit_status);
}

int main(int argc, char **argv) {
    int option, thread_error_code;
    pthread_t kb_thread_id;
    void *return_value;
    
    if(argc > 1 && strcmp(argv[1], "--help") == 0) {
        print_usage_and_exit_with_code(EXIT_SUCCESS);
    }
    
    while((option = getopt(argc, argv, ":p:u:")) != -1) {        
        switch(option) {
            case 'p':
                login.port = (char *) malloc(strlen(optarg));
                strcpy(login.port, optarg);
                break;
            case 'u':
                login.username = (char *) malloc(strlen(optarg));
                strcpy(login.username, optarg);
                break;
            case ':':
                fprintf(stderr, "Missing argument for %c option\n\n", optopt);
                print_usage_and_exit_with_code(EXIT_FAILURE);
            case '?':
                fprintf(stderr, "Unrecognized flag %c\n\n", optopt);
                print_usage_and_exit_with_code(EXIT_FAILURE);
            default:
                fatal_shutdown("Unexpected response from option parser");
        }
    }
    
    if(optind < argc) {
        login.server = (char *) malloc(strlen(argv[optind]));
        strcpy(login.server, argv[optind]);
    } else {
        fprintf(stderr, "Missing host name\n\n");
        print_usage_and_exit_with_code(EXIT_FAILURE);
    }
    
    /* TODO: Check for null for any login block information and set it to a
     * default value */
    
    printf("%s@%s:%s\n", login.username, login.server, login.port);
    
    thread_error_code = pthread_create(&kb_thread_id, NULL, kb_thread, NULL);
    if(thread_error_code != 0) {
        fatal_shutdown("Error starting keyboard thread");
    }
    
    int x;
    for(x = 0; x < 10; x++) {
        usleep(100000);
        printf("Main\n");
    }
    
    thread_error_code = pthread_join(kb_thread_id, &return_value);
    if(thread_error_code != 0) {
        fatal_shutdown("Error joinging keyboard thread");
    }
    
    return 0;
}