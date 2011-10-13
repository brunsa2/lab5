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

static void print_usage(int exit_status) {
    fprintf(stderr, "Usage: chat [OPTION]...\n"
            "Talk among other users connected to a chat server.\n\n"
            "-u\tSet username to be displayed to chat members\n"
            "-s\tSpecify host to connect to that is running a chat server\n"
            "-p\tSpecify port to connect to server on\n");
    exit(exit_status);
}

int main(int argc, char **argv) {
    int option;
    
    if(strcmp(argv[1], "--help") == 0) {
        print_usage(EXIT_SUCCESS);
    }
    
    while((option = getopt(argc, argv, ":p:s:u:")) != -1) {        
        switch(option) {
            case 'p':
                login.port = (char *) malloc(strlen(optarg));
                strcpy(login.port, optarg);
                break;
            case 's':
                login.server = (char *) malloc(strlen(optarg));
                strcpy(login.server, optarg);
                break;
            case 'u':
                login.username = (char *) malloc(strlen(optarg));
                strcpy(login.username, optarg);
                break;
            case ':':
                fprintf(stderr, "Missing argument for %c option\n\n", optopt);
                print_usage(EXIT_FAILURE);
            case '?':
                fprintf(stderr, "Unrecognized flag %c\n\n", optopt);
                print_usage(EXIT_FAILURE);
            default:
                fatal_shutdown("Unexpected response from option parser");
        }
    }
    
    /* TODO: Check for null for any login block information and set it to a
     * default value */
    
    return 0;
}