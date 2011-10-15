/*
 * Jeff Stubler
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 14 October 2011
 *
 * Chat daemon control
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static void print_usage_and_exit_with_code(int exit_status) {
    fprintf(stderr, "Usage: chatctl command\n\n"
            "start\t\tStart chatd\n"
            "stop\t\tStop chatd\n"
            "restart\t\tRestard chatd\n"
            "force-stop\tForce restart chatd\n"
            "force-restart\tForce restart chatd\n\n");
    exit(exit_status);
}

static void start_chatd(void) {
    printf("Starting chatd...\n");
    execl("./chatd", "./chatd", (char *) NULL);
    printf("Could not start chatd");
    exit(EXIT_FAILURE);
}

static void stop_chatd(void) {
    printf("Stopping chatd\n");
}

static void force_stop_chatd(void) {
    printf("Force stopping chatd\n");
}

int main(int argc, char **argv) {    
    if(argc > 1 &&
            (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        print_usage_and_exit_with_code(EXIT_SUCCESS);
    }
    
    if(strcmp(argv[0], "chatd-start") == 0 ||
            (argc > 1 && strcmp(argv[1], "start") == 0)) {
        start_chatd();
        return 0;
    }
    
    if(strcmp(argv[0], "chatd-stop") == 0 ||
       (argc > 1 && strcmp(argv[1], "stop") == 0)) {
        stop_chatd();
        return 0;
    }
    
    if(strcmp(argv[0], "chatd-restart") == 0 ||
       (argc > 1 && strcmp(argv[1], "restart") == 0)) {
        stop_chatd();
        start_chatd();
        return 0;
    }
    
    if(strcmp(argv[0], "chatd-force-stop") == 0 ||
       (argc > 1 && strcmp(argv[1], "force-stop") == 0)) {
        force_stop_chatd();
        return 0;
    }
    
    if(strcmp(argv[0], "chatd-force-restart") == 0 ||
       (argc > 1 && strcmp(argv[1], "force-restart") == 0)) {
        force_stop_chatd();
        start_chatd();
        return 0;
    }
    
    print_usage_and_exit_with_code(EXIT_FAILURE);
}