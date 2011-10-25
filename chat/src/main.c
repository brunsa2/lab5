/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 12 October 2011
 *
 * Main entry point for client, gathers arguments, starts threads, and acts as
 * main controller for system
 */

#include "main.h"

#define fatal_shutdown(message) fprintf(stderr, "%s\n", message); \
        exit(EXIT_FAILURE);

/* Variables for parsing command line arguments */
extern int optind, opterr, optopt;
extern char *optarg;
extern int network_error_code;

login_info login;

static void print_usage_and_exit_with_code(int exit_status) {
    fprintf(stderr, "Usage: chat [OPTION]... host\n"
            "Talk among other users connected to a chat server.\n\n"
            /*"-a\t\tJoin anonymously without greeting message\n"*/
            "-h, --help\tDisplay this help and exit\n"
            "-p\t\tSpecify port to connect to server on\n"
            "-u\t\tSet username to be displayed to chat members\n\n");
    exit(exit_status);
}

int main(int argc, char **argv) {
    int option, thread_error_code;
    thread_id kb_thread_id, tick_thread_id, ui_thread_id, network_thread_id;
    void *return_value;
    
    if(argc > 1 && strcmp(argv[1], "--help") == 0) {
        print_usage_and_exit_with_code(EXIT_SUCCESS);
    }
    
    while((option = getopt(argc, argv, ":ahp:u:")) != -1) {        
        switch(option) {
            case 'a':
                login.anonymous = true;
            case 'h':
                print_usage_and_exit_with_code(EXIT_SUCCESS);
            case 'p':
                if(login.port != NULL) {
                    free(login.port);
                }
                login.port = (char *) malloc(strlen(optarg));
                strcpy(login.port, optarg);
                break;
            case 'u':
                if(login.username != NULL) {
                    free(login.username);
                }
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
    start_thread(&tick_thread_id, tick_thread, NULL);
    start_thread(&ui_thread_id, ui_thread, NULL);
    printf("Connecting...\n");
    start_thread(&network_thread_id, network_thread, NULL);
    
    sleep(2);
    if(network_error_code != 0) {
        printf("Could not connect\nError code %d\n", network_error_code);
        exit(EXIT_FAILURE);
    }
    
    ctrl_thread();
    
    free(login.username);
    free(login.server);
    free(login.port);
    
    return 0;
}