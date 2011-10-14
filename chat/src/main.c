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

#include "thread.h"
#include "keyboard.h"
#include "controller.h"

#define fatal_shutdown(message) fprintf(stderr, "%s\n", message); \
        exit(EXIT_FAILURE);

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

static void print_usage_and_exit_with_code(int exit_status) {
    fprintf(stderr, "Usage: chat [OPTION]... host\n"
            "Talk among other users connected to a chat server.\n\n"
            "-h, --help\tDisplay this help and exit\n"
            "-p\t\tSpecify port to connect to server on\n"
            "-u\t\tSet username to be displayed to chat members\n");
    exit(exit_status);
}

int main(int argc, char **argv) {
    int option, thread_error_code;
    thread_id kb_thread_id, ctrl_thread_id;
    void *return_value;
    
    if(argc > 1 && strcmp(argv[1], "--help") == 0) {
        print_usage_and_exit_with_code(EXIT_SUCCESS);
    }
    
    while((option = getopt(argc, argv, ":hp:u:")) != -1) {        
        switch(option) {
            case 'h':
                print_usage_and_exit_with_code(EXIT_SUCCESS);
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
    
    start_thread(&kb_thread_id, kb_thread, NULL);
    start_thread(&ctrl_thread_id, ctrl_thread, NULL);
    
    int x;
    for(x = 0; x < 5; x++) {
        usleep(50000);
        printf("Main\n");
    }
    
    join_thread(&kb_thread_id);
    join_thread(&ctrl_thread_id);
    
    return 0;
}